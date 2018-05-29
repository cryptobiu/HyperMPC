//
// Created by liork on 5/27/18.
//

#include <log4cpp/Category.hh>

#include "psmpc_ac.h"
#include "comm_client.h"

#define LC log4cpp::Category::getInstance(m_logcat)

psmpc_ac::psmpc_ac(int argc, char* argv [], const char * logcat)
: ProtocolParty<GF28LT>(argc, argv), ac_protocol(logcat), m_no_buckets(0), m_no_random(0)
{

}

psmpc_ac::~psmpc_ac() {}

void psmpc_ac::handle_party_conn(const size_t party_id, const bool connected)
{
    party_t & peer(m_parties_state[party_id]);
    if (connected)
    {
        if(!peer.m_connected)
        {
            LC.debug("%s: party %lu is now connected.", __FUNCTION__, party_id);
            peer.m_connected = true;
        }
        else
            LC.warn("%s: party %lu unexpectedly again connected.", __FUNCTION__, party_id);
    }
    else
    {
        if(peer.m_current_state != ps_done)
        {
            LC.error("%s: party id %lu premature disconnection; Perfect Secure failed.", __FUNCTION__, party_id);
            m_run_flag = false;
        }
        else
        {
            LC.debug("%s: party %lu is now disconnected.", __FUNCTION__, party_id);
        }
    }
}

void psmpc_ac::handle_party_msg(const size_t party_id, std::vector<u_int8_t> &msg)
{
    party_t & peer(m_parties_state[party_id]);
    peer.m_data.insert(peer.m_data.end(), msg.data(), msg.data() + msg.size());
}

int psmpc_ac::pre_run()
{
    m_parties_state.resize(m_parties);

    for(size_t i = 0; i < m_parties; ++i)
        m_parties_state[i].m_id = i;

    m_no_random = circuit.getNrOfInputGates();
    m_no_buckets = (m_no_random / (N-2*T))+1;

    generate_random_double_shares();
    m_parties_state[m_id].m_current_state = ps_rsfi1;
    return 0;
}

int psmpc_ac::post_run()
{
    return 0;
}

void psmpc_ac::generate_random_double_shares()
{
    /**
     *  generate double sharings.
     *  first degree t.
     *  subsequent: degree 2t with same secret.
     */
    vector<GF28LT> x1(N, 0),y1(N, 0);
    for(int k=0; k < m_no_buckets; k++)
    {
        // generate random degree-T polynomial
        for(int i = 0; i < T+1; i++)
            x1[i] = field->Random();

        matrix_vand.MatrixMult(x1, y1, T+1); // eval poly at alpha-positions

        // prepare shares to be sent
        for(int i=0; i < N; i++)
            m_parties_state[i].m_aux.push_back(y1[i]);
    }
}

void psmpc_ac::set_circuit_name(const std::string &circuit_name)
{
    m_circuit_name = circuit_name;
}

int psmpc_ac::output_phase_comm(const size_t peer_id, size_t &to_send, size_t &to_recv)
{
    to_send = to_recv = 0;

    for(int k=M-numOfOutputGates; k < M; k++)
    {
        if(circuit.getGates()[k].gateType == OUTPUT)
        {
            if(circuit.getGates()[k].party == peer_id)
                to_send++;
            else if(circuit.getGates()[k].party == m_id)
                to_recv++;
        }
        else
        {
            LC.error("%s: gate %d shold be an output gate", __FUNCTION__, k);
            return -1;
        }
    }

    return 0;
}

bool psmpc_ac::run_around()
{
    bool round_ready = true;
    for(size_t pid = 0; pid < m_parties; ++pid)
    {
        if(pid == m_id) continue;
        round_ready = round_ready && party_run_around(pid);//(ps_round_up == m_party_states[pid].state);
    }
    return round_ready;
}

bool psmpc_ac::party_run_around(const size_t party_id)
{
    party_t & peer(m_parties_state[party_id]);
    switch(peer.m_current_state)
    {
        case ps_nil:
            if (!peer.m_connected)
                return false;
            else
                peer.m_current_state = ps_rsfi1;
            /* no break */
        case ps_rsfi1:
            return on_rsfi1(peer);
        case ps_rsfi2:
            return on_rsfi2(peer);
        case ps_prep1:
            return on_prep1(peer);
        case ps_prep2:
            return on_prep2(peer);
        case ps_inprp:
            return on_inprp(peer);
        case ps_inadj1:
            return on_inadj1(peer);
        case ps_inadj2:
            return on_inadj2(peer);
        case ps_cmptn:
            return on_cmptn(peer);
        case ps_outpt:
            return on_outpt(peer);
        case ps_done:
            return true;
        default:
            LC.error("%s: invalid party state value %u.", __FUNCTION__, peer.m_current_state);
            exit(__LINE__);
    }

    return false;
}

bool psmpc_ac::round_up()
{
    party_state_t current_state = ps_nil;
    if(!all_on_the_same_page(current_state))
        return false;
    else
        LC.debug("%s: All parties are on the same state %u", __FUNCTION__, current_state);

    switch(current_state)
    {
        case ps_nil:
            LC.error("%s: Invalid state nil", __FUNCTION__);
            return false;
        case ps_rsfi1:
            return rsfi1_2_rsfi2();
        case ps_rsfi2:
            return rsfi2_2_prep1();
        case ps_prep1:
            return prep1_2_prep2();
        case ps_prep2:
            return prep2_2_inprp();
        case ps_inprp:
            return inprp_2_inadj1();
        case ps_inadj1:
            return inadj1_2_inadj2();
        case ps_inadj2:
            return inadj2_2_cmptn();
        case ps_cmptn:
            return cmptn_2_outpt();
        case ps_outpt:
            return outpt_2_done();
        case ps_done:
            return (m_run_flag = false);
    }

    return false;
}

bool psmpc_ac::all_on_the_same_page(party_state_t & current_state)
{
    current_state = m_parties_state[m_id].m_current_state;

    for (size_t i = 0; i < m_parties_state.size(); ++i)
    {
        if (i == m_id) continue;

        if(m_parties_state[i].m_current_state != current_state) return false;
    }

    return true;
}

bool psmpc_ac::send_aux(party_t &peer)
{
    size_t elem_size = field->getElementSizeInBytes();
    u_int8_t buffer[4096];
    size_t offset = 0;
    for (std::vector<GF28LT>::iterator j = peer.m_aux.begin(); j != peer.m_aux.end(); ++j)
    {
        field->elementToBytes(buffer + offset, *j);
        offset += elem_size;

        if ((offset + elem_size) > 4096 || peer.m_aux.end() == (j + 1))
        {
            if (0 != m_cc->send(peer.m_id, buffer, offset))
            {
                return false;
            }
            offset = 0;
        }
    }
    return true;
}

bool psmpc_ac::recv_aux(party_t &peer, const size_t required_elements)
{
    size_t elem_size = field->getElementSizeInBytes();

    for(size_t i = 0; i < required_elements; ++i)
    {
        if(peer.m_data.size() < elem_size)
            return false;

        GF28LT x = field->bytesToElement(peer.m_data.data());
        peer.m_aux.push_back(x);
        peer.m_data.erase(peer.m_data.begin(), peer.m_data.begin() + elem_size);
    }

    return true;
}

bool psmpc_ac::on_rsfi1(party_t & peer)
{
    static const size_t num_of_elems = peer.m_aux.size();

    if(!peer.rsfi1_sent)
    {
        if (!send_aux(peer))
        {
            LC.error("%s: failed sending data to party %lu; Perfect Secure failed.", __FUNCTION__, peer.m_id);
            return (m_run_flag = false);
        }
        peer.m_aux.clear();
        peer.rsfi1_sent = true;
        LC.info("%s: %lu elements sent to peer %lu.", __FUNCTION__, num_of_elems, peer.m_id);
    }

    if(!peer.rsfi1_rcvd)
    {
        size_t required_elements = num_of_elems - peer.m_aux.size();

        if (!recv_aux(peer, required_elements))
            return false;
        else
        {
            peer.rsfi1_rcvd = true;
            LC.info("%s: %lu elements received from peer %lu.", __FUNCTION__, num_of_elems, peer.m_id);
        }
    }

    return true;
}

bool psmpc_ac::rsfi1_2_rsfi2()
{
    vector <GF28LT> x1(N), y1(N);

    sharingBufInputsTElements.resize((size_t) m_no_buckets * (N - 2 * T));
    for(int k=0; k < m_no_buckets; k++)
    {

        // generate random degree-T polynomial
        for (int i = 0; i < N; i++)
            x1[i] = m_parties_state[i].m_aux[k];

        for (int i = 0; i < N; i++)
            m_parties_state[i].m_aux.clear();

        matrix_him.MatrixMult(x1, y1);

        // these shall be checked
        int robin = 0;
        for (int i = 0; i < 2 * T; i++)
        {
            m_parties_state[robin].m_aux.push_back(y1[i]);
            robin = (robin+1) % N; // next robin
        }

        // Y1 : the degree-t shares of my poly
        // Y2 : the degree 2t shares of my poly
        for (int i = 2 * T; i < N; i++)
            sharingBufInputsTElements[k*(N-2*T) + i - 2*T] = y1[i];
    }

    for(size_t i = 0; i < m_parties_state.size(); ++i)
        m_parties_state[i].m_current_state = ps_rsfi2;

    return true;
}

bool psmpc_ac::on_rsfi2(party_t & peer)
{
    static const size_t num_of_elems = peer.m_aux.size();

    if (!peer.rsfi2_sent)
    {
        if (!send_aux(peer))
        {
            LC.error("%s: failed sending data to party %lu; Perfect Secure failed.", __FUNCTION__, peer.m_id);
            return (m_run_flag = false);
        }
        peer.m_aux.clear();
        peer.rsfi2_sent = true;
    }

    if(!peer.rsfi2_rcvd)
    {
        size_t required_elements = num_of_elems - peer.m_aux.size();

        if (!recv_aux(peer, required_elements))
            return false;
        else
            peer.rsfi2_rcvd = true;
    }

    return true;
}

bool psmpc_ac::rsfi2_2_prep1()
{
    int count = m_no_buckets * (2*T) / N;
    if(m_no_buckets * (2*T)%N > m_partyId)
        count++;

    vector<GF28LT> x1(N),x2(N),y1(N),y2(N);

    for(int k=0; k < count; k++)
    {
        for (int i = 0; i < N; i++)
            x1[i] = m_parties_state[i].m_aux[k];


//        vector<GF28LT> x_until_d(N);
//        for(int i=0; i<T; i++)
//            x_until_d[i] = x1[i];
//
//        for(int i=T; i<N; i++)
//            x_until_d[i] = *(field->GetZero());

        // Check that x1 is t-consistent and x2 is 2t-consistent and secret is the same

        if(!checkConsistency(x1,T))
            return false;
    }

    for (int i = 0; i < N; i++)
        m_parties_state[i].m_aux.clear();

    for(size_t i = 0; i < m_parties_state.size(); ++i)
        m_parties_state[i].m_current_state = ps_prep1;

    sharingBufTElements.resize(m_no_buckets*(N-2*T)); // my shares of the double-sharings
    sharingBuf2TElements.resize(m_no_buckets*(N-2*T)); // my shares of the double-sharings

    /**
     *  generate double sharings.
     *  first degree t.
     *  subsequent: degree 2t with same secret.
     */

    for (int i = 0; i < N; i++)
        m_parties_state[i].m_aux.resize((size_t) 2 * m_no_buckets);

    for(int k=0; k < m_no_buckets; k++)
    {
        // generate random degree-T polynomial
        for(int i = 0; i < T+1; i++)
            x1[i] = field->Random();

        x2[0] = x1[0];

        for(int i = 1; i < 2*T+1; i++)
            x2[i] = field->Random();


        matrix_vand.MatrixMult(x1, y1, T+1); // eval poly at alpha-positions
        matrix_vand.MatrixMult(x2, y2, 2*T+1); // eval poly at alpha-positions

        // prepare shares to be sent
        for(int i=0; i < N; i++)
        {
            m_parties_state[i].m_aux[2*k] = y1[i];
            m_parties_state[i].m_aux[2*k+1] = y2[i];
        }

    }

    return true;
}

bool psmpc_ac::on_prep1(party_t &peer)
{
    static const size_t num_of_elems = peer.m_aux.size();

    if (!peer.prep1_sent)
    {
        if (!send_aux(peer))
        {
            LC.error("%s: failed sending data to party %lu; Perfect Secure failed.", __FUNCTION__, peer.m_id);
            return (m_run_flag = false);
        }
        peer.m_aux.clear();
        peer.prep1_sent = true;
    }

    if(!peer.prep1_rcvd)
    {
        size_t required_elements = num_of_elems - peer.m_aux.size();

        if (!recv_aux(peer, required_elements))
            return false;
        else
            peer.prep1_rcvd = true;
    }

    return true;
}

bool psmpc_ac::prep1_2_prep2()
{
    vector<GF28LT> x1(N),x2(N),y1(N),y2(N);
    int robin = 0;

    // x1 : used for the N degree-t sharings
    // x2 : used for the N degree-2t sharings
    for(int k=0; k < m_no_buckets; k++)
    {
        // generate random degree-T polynomial
        for (int i = 0; i < N; i++)
        {
            x1[i] = m_parties_state[i].m_aux[2 * k];
            x2[i] = m_parties_state[i].m_aux[2 * k + 1];
        }

        matrix_him.MatrixMult(x1, y1);
        matrix_him.MatrixMult(x2, y2);
        // these shall be checked
        for (int i = 0; i < 2 * T; i++)
        {
            m_parties_state[robin].m_aux.push_back(y1[i]);
            m_parties_state[robin].m_aux.push_back(y2[i]);
            robin = (robin+1) % N; // next robin
        }

        // Y1 : the degree-t shares of my poly
        // Y2 : the degree 2t shares of my poly
        for (int i = 2 * T; i < N; i++) {

            sharingBufTElements[k*(N-2*T) + i - 2*T] = y1[i];
            sharingBuf2TElements[k*(N-2*T) + i - 2*T] =  y2[i];
        }

        x1[0] = *(field->GetZero());
        x2[0] = *(field->GetZero());
    }

    for(size_t i = 0; i < m_parties_state.size(); ++i)
        m_parties_state[i].m_current_state = ps_prep2;

    return true;
}


bool psmpc_ac::on_prep2(party_t &peer)
{
    static const size_t num_of_elems = peer.m_aux.size();

    if (!peer.prep2_sent)
    {
        if (!send_aux(peer))
        {
            LC.error("%s: failed sending data to party %lu; Perfect Secure failed.", __FUNCTION__, peer.m_id);
            return (m_run_flag = false);
        }
        peer.m_aux.clear();
        peer.prep2_sent = true;
    }

    if(!peer.prep2_rcvd)
    {
        size_t required_elements = num_of_elems - peer.m_aux.size();

        if (!recv_aux(peer, required_elements))
            return false;
        else
            peer.prep2_rcvd = true;
    }

    return true;
}

bool psmpc_ac::prep2_2_inprp()
{
    int count = m_no_buckets * (2*T) / N;
    if(m_no_buckets * (2*T)%N > m_partyId)
        count++;

    vector<GF28LT> x1(N),x2(N),y1(N),y2(N);


    for(int k=0; k < count; k++)
    {
        for (int i = 0; i < N; i++)
        {

            x1[i] = m_parties_state[i].m_aux[2 * k];
            x2[i] = m_parties_state[i].m_aux[2 * k + 1];
        }

        vector<GF28LT> x_until_d(N);
        for(int i=0; i<T; i++)
            x_until_d[i] = x1[i];
        for(int i=T; i<N; i++)
            x_until_d[i] = *(field->GetZero());

        // Check that x1 is t-consistent and x2 is 2t-consistent and secret is the same
        if(!checkConsistency(x1,T) || !checkConsistency(x2,2*T) || (interpolate(x1)) != (interpolate(x2)))
            return false;
    }

    for(size_t i = 0; i < m_parties_state.size(); ++i)
        m_parties_state[i].m_current_state = ps_inprp;

    for(size_t i =0; i < m_parties;++i)
        m_parties_state[i].m_aux.clear();

    for(int k = 0; k < numOfInputGates; k++)//these are only input gates
    {
        gateShareArr[circuit.getGates()[k].output] = sharingBufInputsTElements[k];
        int i = (circuit.getGates())[k].party; // the number of party which has the input
        // reconstruct sharing towards input party
        m_parties_state[i].m_aux.push_back(gateShareArr[circuit.getGates()[k].output]);
    }

    return true;
}

bool psmpc_ac::on_inprp(psmpc_ac::party_t &peer)
{
    static const size_t num_of_elems = peer.m_aux.size();

    if (!peer.inprp_sent)
    {
        if (!send_aux(peer))
        {
            LC.error("%s: failed sending data to party %lu; Perfect Secure failed.", __FUNCTION__, peer.m_id);
            return (m_run_flag = false);
        }
        peer.m_aux.clear();
        peer.inprp_sent = true;
    }

    if(!peer.inprp_rcvd)
    {
        size_t required_elements = num_of_elems - peer.m_aux.size();

        if (!recv_aux(peer, required_elements))
            return false;
        else
            peer.inprp_rcvd = true;
    }

    return true;
}

bool psmpc_ac::inprp_2_inadj1()
{
    vector<GF28LT> x1(N);
    int counter = 0;
    GF28LT secret;
    // reconstruct my random input values
    for(int k = 0; k < numOfInputGates; k++)
    {
        if (circuit.getGates()[k].party == m_partyId)
        {

            for (int i = 0; i < N; i++)
                x1[i] = m_parties_state[i].m_aux[counter];

            counter++;
            if (!checkConsistency(x1, T))
            {
                LC.error("%s: cheat check failed.", __FUNCTION__);
                return (m_run_flag = false);
            }
            // the (random) secret
            secret = interpolate(x1);
            gateValueArr[k] = secret;
        }
    }

    for(size_t i = 0; i < m_parties_state.size(); ++i)
        m_parties_state[i].m_current_state = ps_inadj1;

    // read the inputs of the party
    vector<int> sizes(N);
    vector<GF28LT> diffElements;
    int input, index = 0;

    for (int k = 0; k < numOfInputGates; k++)
    {
        if(circuit.getGates()[k].gateType == INPUT)
        {
            sizes[circuit.getGates()[k].party]++;
            if (circuit.getGates()[k].party == m_partyId)
            {
                input = myInputs[index];
                index++;

                // the value is gateValue[k], but should be input.
                GF28LT myinput = field->GetElement(input);

                GF28LT different = myinput - gateValueArr[k];
                diffElements.push_back(different);
            }
        }
    }

    for(size_t i = 0; i < m_parties; ++i)
        m_parties_state[i].m_aux = diffElements;

    return true;
}

bool psmpc_ac::on_inadj1(psmpc_ac::party_t &peer)
{
    static const size_t num_of_elems = peer.m_aux.size();

    if (!peer.inadj1_sent)
    {
        if (!send_aux(peer))
        {
            LC.error("%s: failed sending data to party %lu; Perfect Secure failed.", __FUNCTION__, peer.m_id);
            return (m_run_flag = false);
        }
        peer.m_aux.clear();
        peer.inadj1_sent = true;
    }

    if(!peer.inadj1_rcvd)
    {
        size_t required_elements = num_of_elems - peer.m_aux.size();

        if (!recv_aux(peer, required_elements))
            return false;
        else
            peer.inadj1_rcvd = true;
    }

    return true;
}

bool psmpc_ac::inadj1_2_inadj2()
{
    // calculate total number of values which received
    int count = m_parties * m_parties_state[m_id].m_aux.size();

    vector<GF28LT> valBufs;
    valBufs.reserve(count);
    for(size_t i = 0; i < m_parties; ++i)
    {
        valBufs.insert(valBufs.end(), m_parties_state[i].m_aux.begin(), m_parties_state[i].m_aux.end());
        m_parties_state[i].m_aux.clear();
    }

    for(size_t i = 0; i < m_parties_state.size(); ++i)
        m_parties_state[i].m_current_state = ps_inadj2;

    vector<GF28LT> x1(N);
    vector<GF28LT> y1(N);

    m_no_buckets = count / (N - T) + 1;
    int index = 0;

    for(size_t k = 0; k < m_no_buckets; k++)
    {
        for(size_t i = 0; i < N; i++)
        {
            if((i < N-T) && (k*(N-T)+i < count))
            {
                x1[i]= valBufs[index++];
            }
            else
            {
                // padding zero
                x1[i] = *(field->GetZero());
            }
        }

        // x1 contains (up to) N-T values from ValBuf
        matrix_him.MatrixMult(x1, y1); // no cheating: all parties have same y1

        for(size_t i = 0; i < N; i++)
            m_parties_state[i].m_aux.push_back(y1[i]);

        for(size_t i = 0; i < N; i++)
        {
            x1[i] = *(field->GetZero());
            y1[i] = *(field->GetZero());
        }
    }

    return true;
}

bool psmpc_ac::on_inadj2(party_t &peer)
{
    static const size_t num_of_elems = peer.m_aux.size();

    if (!peer.inadj2_sent)
    {
        if (!send_aux(peer))
        {
            LC.error("%s: failed sending data to party %lu; Perfect Secure failed.", __FUNCTION__, peer.m_id);
            return (m_run_flag = false);
        }
        peer.m_aux.clear();
        peer.inadj2_sent = true;
    }

    if(!peer.inadj2_rcvd)
    {
        size_t required_elements = num_of_elems - peer.m_aux.size();

        if (!recv_aux(peer, required_elements))
            return false;
        else
            peer.inadj2_rcvd = true;
    }

    return true;
}

bool psmpc_ac::inadj2_2_cmptn()
{
    GF28LT temp1;

    for(size_t k=0; k < m_no_buckets; k++)
    {
        temp1 = m_parties_state[0].m_aux[k];

        for (size_t i = 1; i < N; i++)
        {
            if(temp1 != m_parties_state[i].m_aux[k])
            {
                LC.error("%s: elem: %lu mismatch of party %lu", __FUNCTION__, k, i);
                return false;
            }

        }
    }


    GF28LT db;
    vector<int> counters(N, 0);

    for (size_t k = 0; k < numOfInputGates; k++)
    {
        if(circuit.getGates()[k].gateType == INPUT)
        {
            db = m_parties_state[circuit.getGates()[k].party].m_aux[counters[circuit.getGates()[k].party]];
            counters[circuit.getGates()[k].party] += 1;
            gateShareArr[circuit.getGates()[k].output] = gateShareArr[circuit.getGates()[k].output] + db; // adjustment
        }
    }

    for(size_t i =0; i < m_parties;++i)
        m_parties_state[i].m_aux.clear();

    for(size_t i = 0; i < m_parties_state.size(); ++i)
        m_parties_state[i].m_current_state = ps_cmptn;

    return true;

}

bool psmpc_ac::on_cmptn(psmpc_ac::__party_t&)
{
    return true;
}

bool psmpc_ac::cmptn_2_outpt()
{
    computationPhase(m);

    for(size_t i = 0; i < m_parties_state.size(); ++i)
       m_parties_state[i].m_current_state = ps_outpt;

    for(int k=M-numOfOutputGates; k < M; k++)
    {
        const TGate & a_gate(circuit.getGates()[k]);
        if(OUTPUT == a_gate.gateType)
        {

            // send to party (which need this gate) your share for this gate
            //m_parties_state[circuit.getGates()[k].party].m_aux.push_back(gateShareArr[circuit.getGates()[k].input1]);
            m_parties_state[a_gate.party].m_aux.push_back(a_gate.input1);
        }
        else
        {
            LC.error("%s: gate %lu should be output; Perfect Secure failed.", __FUNCTION__, k);
            return (m_run_flag = false);
        }
    }

    return true;
}

bool psmpc_ac::on_outpt(party_t &peer)
{
    size_t to_send = 0, to_recv = 0;

    if(0 != output_phase_comm(peer.m_id, to_send, to_recv))
    {
        LC.error("%s: failed retrieve outputs comm requirements to party %lu; Perfect Secure failed."
                 , __FUNCTION__, peer.m_id);
        return (m_run_flag = false);
    }

    if (peer.outpt_sent < to_send)
    {
        if (!send_aux(peer))
        {
            LC.error("%s: failed sending data to party %lu; Perfect Secure failed.", __FUNCTION__, peer.m_id);
            return (m_run_flag = false);
        }
        peer.m_aux.clear();
        peer.outpt_sent = to_send;
        LC.debug("%s: Successfully sent %lu elements to peer %lu", __FUNCTION__, to_send, peer.m_id);
    }

    if(peer.outpt_rcvd < to_recv)
    {
        size_t required_elements = to_recv - peer.m_aux.size();

        if (!recv_aux(peer, required_elements))
            return false;
        else
        {
            peer.outpt_rcvd = to_recv;
            LC.debug("%s: Successfully received %lu elements to peer %lu", __FUNCTION__, to_recv, peer.m_id);
        }
    }

    return true;
}


bool psmpc_ac::outpt_2_done()
{
    int counter = 0;
    vector<GF28LT> x1(N);

    for(int k=M-numOfOutputGates ; k < M; k++)
    {
        if(circuit.getGates()[k].gateType == OUTPUT && circuit.getGates()[k].party == m_partyId)
        {
            for(int i=0; i < N; i++)
                x1[i] = m_parties_state[i].m_aux[counter];

            counter++;

            // my output: reconstruct received shares
            if (!checkConsistency(x1, T))
            {
                LC.error("%s: cheat check failed.", __FUNCTION__);
                return (m_run_flag = false);
            }
        }
    }

    for(size_t i = 0; i < m_parties_state.size(); ++i)
        m_parties_state[i].m_current_state = ps_done;

    LC.notice("%s: Protocol done; success.",__FUNCTION__);
    sleep(1);
    return (m_run_flag = false);
}