//
// Created by liork on 5/27/18.
//

#include <log4cpp/Category.hh>

#include "comm_client_factory.h"
#include "comm_client.h"
#include "psmpc_ac_gf28lt.h"

#define LC log4cpp::Category::getInstance(m_logcat)

psmpc_ac_gf28lt::psmpc_ac_gf28lt(int argc, char* argv [],  comm_client::cc_args_t * args)
: ProtocolParty<GF28LT>(argc, argv, false), ac_protocol(comm_client_factory::cc_tcp_mesh, args), m_no_buckets(-1)
{

}

psmpc_ac_gf28lt::~psmpc_ac_gf28lt() {}

void psmpc_ac_gf28lt::handle_party_conn(const size_t party_id, const bool connected)
{
    party_t & peer(m_parties_state[party_id]);
    if (connected)
    {
        if(!peer.m_connected)
        {
            LC.notice("%s: party %lu is now connected.", __FUNCTION__, party_id);
            peer.m_connected = true;
        }
        else
            LC.warn("%s: party %lu unexpectedly again connected.", __FUNCTION__, party_id);
    }
    else
    {
    	if(ps_done != peer.m_current_state)
    	{
            LC.error("%s: party id %lu premature disconnection while in state %lu; Perfect Secure failed.", __FUNCTION__, party_id, (size_t)peer.m_current_state);
            m_run_flag = false;
    	}
        else
        {
            LC.notice("%s: party %lu is now disconnected.", __FUNCTION__, party_id);
        }
    }
}

void psmpc_ac_gf28lt::handle_party_msg(const size_t party_id, std::vector<u_int8_t> &msg)
{
    party_t & peer(m_parties_state[party_id]);
    peer.m_data.insert(peer.m_data.end(), msg.data(), msg.data() + msg.size());
}

int psmpc_ac_gf28lt::pre_run()
{
    m_parties_state.resize(m_parties);

    for(size_t i = 0; i < m_parties; ++i)
        m_parties_state[i].m_id = i;

    generate_random_double_shares();
    m_parties_state[m_id].m_current_state = ps_rsfi1;

    return 0;
}

int psmpc_ac_gf28lt::post_run()
{
    return 0;
}

void psmpc_ac_gf28lt::generate_random_double_shares()
{
    /**
     *  generate double sharings.
     *  first degree t.
     *  subsequent: degree 2t with same secret.
     */
    int no_random = circuit.getNrOfInputGates();
    m_no_buckets = (no_random / (N-2*T))+1;

    vector<GF28LT> x1(N, 0),y1(N, 0);
    for(size_t k=0; k < m_no_buckets; k++)
    {
        // generate random degree-T polynomial
        for(size_t i = 0; i < T+1; i++)
            x1[i] = field->Random();

        matrix_vand.MatrixMult(x1, y1, T+1); // eval poly at alpha-positions

        // prepare shares to be sent
        for(size_t i=0; i < N; i++)
        {
            m_parties_state[i].m_aux.push_back(y1[i]);
            m_parties_state[i].rnd_data_2recv++;
            m_parties_state[i].rnd_data_2send++;
        }
    }
}

int psmpc_ac_gf28lt::output_phase_comm(const size_t peer_id, size_t &to_send, size_t &to_recv)
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

bool psmpc_ac_gf28lt::run_around()
{
    bool round_ready = true;
    for(size_t pid = 0; pid < m_parties; ++pid)
    {
        if(pid == m_id) continue;
        round_ready = round_ready && party_run_around(pid);//(ps_round_up == m_party_states[pid].state);
    }
    return round_ready;
}

bool psmpc_ac_gf28lt::party_run_around(const size_t party_id)
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
        case ps_rsfi2:
        case ps_prep1:
        case ps_prep2:
        case ps_inprp:
        case ps_inadj1:
        case ps_inadj2:
        case ps_outpt:
        case ps_done:
            return on_round_send_and_recv(peer);
        default:
            LC.error("%s: invalid party state value %u.", __FUNCTION__, peer.m_current_state);
            exit(__LINE__);
    }

    return false;
}

bool psmpc_ac_gf28lt::round_up()
{
    party_state_t current_state = ps_nil;
    if(!all_on_the_same_page(current_state))
        return false;
    else
        LC.notice("%s: All parties are on the same state %u", __FUNCTION__, current_state);
    m_parties_state[m_id].rnd_data_2recv = m_parties_state[m_id].rnd_data_2send = 0;

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
            return inadj2_2_outpt();
        case ps_outpt:
            return outpt_2_done();
        case ps_done:
            LC.notice("%s: Protocol done; success.",__FUNCTION__);
            LC.notice("%s: Protocol output : %s" , __FUNCTION__, m_output.c_str());
            return (m_run_flag = false);
    }

    return false;
}

bool psmpc_ac_gf28lt::all_on_the_same_page(party_state_t & current_state)
{
    current_state = m_parties_state[m_id].m_current_state;

    for (size_t i = 0; i < m_parties; ++i)
    {
        if (i == m_id) continue;

        if(m_parties_state[i].m_current_state != current_state) return false;
    }

    return true;
}

bool psmpc_ac_gf28lt::send_aux(party_t &peer)
{
    size_t elem_size = field->getElementSizeInBytes();
    u_int8_t buffer[4096];

    size_t chunk_elements = 4096 / elem_size;
    if(0 == chunk_elements)
    {
    	LC.error("%s: chunk_elements = 4096 / elem_size(%lu) = 0", __FUNCTION__, elem_size);
    	return false;
    }

    if(peer.m_aux.size() < chunk_elements) chunk_elements = peer.m_aux.size();

    for(size_t i = 0; i < chunk_elements; ++i)
    {
    	field->elementToBytes(buffer + (i * elem_size), peer.m_aux[i]);
    }

    if (0 != m_cc->send(peer.m_id, buffer, chunk_elements * elem_size))
    {
    	LC.error("%s: comm client send() failed.", __FUNCTION__);
        return false;
    }

    peer.rnd_data_sent += chunk_elements;
    peer.m_aux.erase(peer.m_aux.begin(), peer.m_aux.begin() + chunk_elements);
    return true;
}

bool psmpc_ac_gf28lt::recv_aux(party_t &peer, const size_t required_elements)
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

bool psmpc_ac_gf28lt::on_round_send_and_recv(party_t &peer)
{
    LC.debug("%s: peer %lu current state %lu; 2snd %lu; 2rcv %lu;",
             __FUNCTION__, peer.m_id, peer.m_current_state, peer.rnd_data_2send, peer.rnd_data_2recv);

    if(peer.rnd_data_2send > peer.rnd_data_sent)
    {
    	if((peer.rnd_data_2send - peer.rnd_data_sent) != peer.m_aux.size())
    	{
            LC.error("%s: peer %lu send mismatch: 2send = %lu; sent = %lu; send buffer = %lu.",
            		__FUNCTION__, peer.m_id, peer.rnd_data_2send, peer.rnd_data_sent, peer.m_aux.size());
            return (m_run_flag = false);
    	}

    	if (!send_aux(peer))
        {
            LC.error("%s: failed sending data to party %lu; Perfect Secure failed.", __FUNCTION__, peer.m_id);
            return (m_run_flag = false);
        }

    	if(!peer.m_aux.empty())
        	return false;
    }

    if(peer.rnd_data_rcvd < peer.rnd_data_2recv)
    {
        size_t required_elements = peer.rnd_data_2recv - peer.m_aux.size();
        if (!recv_aux(peer, required_elements))
            return false;
        else
            peer.rnd_data_rcvd = peer.rnd_data_2recv;
    }

    peer.rnd_data_2send = peer.rnd_data_sent = 0;
    peer.rnd_data_2recv = peer.rnd_data_rcvd = 0;
    return true;
}

bool psmpc_ac_gf28lt::rsfi1_2_rsfi2()
{
    vector <GF28LT> x1(N), y1(N);
    int no_random = circuit.getNrOfInputGates();
    m_no_buckets = (no_random / (N-2*T))+1;

    sharingBufInputsTElements.resize((size_t) m_no_buckets * (N - 2 * T));
    int robin = 0;

    std::vector< std::vector< GF28LT > > aux_temp(m_parties);
    for (int i = 0; i < N; i++)
        aux_temp[i].swap(m_parties_state[i].m_aux);

    for(size_t i = 0; i < m_parties; ++i)
        m_parties_state[i].m_current_state = ps_rsfi2;

    for(int k=0; k < m_no_buckets; k++)
    {
        // generate random degree-T polynomial
        for (int i = 0; i < N; i++)
            x1[i] = aux_temp[i][k];

        matrix_him.MatrixMult(x1, y1);

        for (int i = 0; i < 2 * T; i++)
        {
            m_parties_state[robin].m_aux.push_back(y1[i]);
            m_parties_state[robin].rnd_data_2send++;
            robin = (robin+1) % N; // next robin
        }

        // Y1 : the degree-t shares of my poly
        // Y2 : the degree 2t shares of my poly
        for (int i = 2 * T; i < N; i++)
            sharingBufInputsTElements[k*(N-2*T) + i - 2*T] = y1[i];
    }

    for (size_t j = 0; j < N; j++)
        m_parties_state[j].rnd_data_2recv = m_parties_state[m_id].rnd_data_2send;

    return true;
}

bool psmpc_ac_gf28lt::rsfi2_2_prep1()
{
    //m_no_buckets used here with the same value calculated in psmpc_ac_gf28lt::rsfi1_2_rsfi2()

    int count = m_no_buckets * (2*T) / N;
    if(m_no_buckets * (2*T)%N > m_partyId)
        count++;

    vector<GF28LT> x1(N),x2(N),y1(N),y2(N);

    for(int k=0; k < count; k++)
    {
        for (int i = 0; i < N; i++)
            x1[i] = m_parties_state[i].m_aux[k];

        // Check that x1 is t-consistent and x2 is 2t-consistent and secret is the same
        if(!checkConsistency(x1,T))
        {
            LC.error("%s: cheat check failed.", __FUNCTION__);
            return (m_run_flag = false);
        }
    }

    for (int i = 0; i < N; i++) {
        m_parties_state[i].m_aux.clear();
    }
    
    for(size_t i = 0; i < m_parties; ++i)
        m_parties_state[i].m_current_state = ps_prep1;

    int no_random = circuit.getNrOfMultiplicationGates();
    m_no_buckets = (no_random / (N-2*T))+1;

    sharingBufTElements.resize(m_no_buckets*(N-2*T)); // my shares of the double-sharings
    sharingBuf2TElements.resize(m_no_buckets*(N-2*T)); // my shares of the double-sharings

    /**
     *  generate double sharings.
     *  first degree t.
     *  subsequent: degree 2t with same secret.
     */

    size_t round_data_size = (size_t) 2 * m_no_buckets;

    for (int i = 0; i < N; i++)
    {
        m_parties_state[i].m_aux.resize(round_data_size);
        m_parties_state[i].rnd_data_2send = m_parties_state[i].rnd_data_2recv = round_data_size;
    }

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

bool psmpc_ac_gf28lt::prep1_2_prep2()
{
    int no_random = circuit.getNrOfMultiplicationGates();
    m_no_buckets = (no_random / (N-2*T))+1;

    vector<GF28LT> x1(N),x2(N),y1(N),y2(N);
    int robin = 0;

    std::vector< std::vector< GF28LT > > aux_temp(m_parties);
    for(size_t i = 0; i < m_parties; ++i)
        aux_temp[i].swap(m_parties_state[i].m_aux);

    // x1 : used for the N degree-t sharings
    // x2 : used for the N degree-2t sharings
    for(int k=0; k < m_no_buckets; k++)
    {
        // generate random degree-T polynomial
        for (int i = 0; i < N; i++)
        {
            x1[i] = aux_temp[i][2 * k];
            x2[i] = aux_temp[i][2 * k + 1];
        }

        matrix_him.MatrixMult(x1, y1);
        matrix_him.MatrixMult(x2, y2);
        // these shall be checked
        for (int i = 0; i < 2 * T; i++)
        {
            m_parties_state[robin].m_aux.push_back(y1[i]);
            m_parties_state[robin].rnd_data_2send++;

            m_parties_state[robin].m_aux.push_back(y2[i]);
            m_parties_state[robin].rnd_data_2send++;

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

    for(size_t i = 0; i < m_parties; ++i)
    {
        m_parties_state[i].rnd_data_2recv = m_parties_state[m_id].rnd_data_2send;
        m_parties_state[i].m_current_state = ps_prep2;
    }

    return true;
}

bool psmpc_ac_gf28lt::prep2_2_inprp()
{
    int no_random = circuit.getNrOfMultiplicationGates();
    m_no_buckets = (no_random / (N-2*T))+1;
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
        {
            LC.error("%s: cheat check failed.", __FUNCTION__);
            return (m_run_flag = false);
        }
    }

    for(size_t i = 0; i < m_parties; ++i)
        m_parties_state[i].m_current_state = ps_inprp;

    for(size_t i =0; i < m_parties;++i)
        m_parties_state[i].m_aux.clear();

    for(int k = 0; k < numOfInputGates; k++)//these are only input gates
    {
        gateShareArr[circuit.getGates()[k].output] = sharingBufInputsTElements[k];
        int i = (circuit.getGates())[k].party; // the number of party which has the input
        // reconstruct sharing towards input party
        m_parties_state[i].m_aux.push_back(gateShareArr[circuit.getGates()[k].output]);
        m_parties_state[i].rnd_data_2send++;
    }

    for(size_t i = 0; i < m_parties; ++i)
        m_parties_state[i].rnd_data_2recv = m_parties_state[m_id].rnd_data_2send;

    return true;
}

bool psmpc_ac_gf28lt::inprp_2_inadj1()
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

    for(size_t i = 0; i < m_parties; ++i)
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
    {
        m_parties_state[i].m_aux = diffElements;
        m_parties_state[i].rnd_data_2recv = m_parties_state[i].rnd_data_2send = diffElements.size();
    }

    return true;
}

bool psmpc_ac_gf28lt::inadj1_2_inadj2()
{
    // calculate total number of values which received
    int count = m_parties * m_parties_state[m_id].m_aux.size();
    m_no_buckets = count / (N - T) + 1;

    vector<GF28LT> valBufs;
    valBufs.reserve(count);
    for(size_t i = 0; i < m_parties; ++i)
    {
        valBufs.insert(valBufs.end(), m_parties_state[i].m_aux.begin(), m_parties_state[i].m_aux.end());
        m_parties_state[i].m_aux.clear();
    }

    for(size_t i = 0; i < m_parties; ++i)
        m_parties_state[i].m_current_state = ps_inadj2;

    vector<GF28LT> x1(N);
    vector<GF28LT> y1(N);

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

    for(size_t i = 0; i < N; i++)
        m_parties_state[i].rnd_data_2send = m_parties_state[i].rnd_data_2recv = (size_t)m_no_buckets;

    return true;
}

bool psmpc_ac_gf28lt::inadj2_2_outpt()
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

    computationPhase(m);

    for(size_t i = 0; i < m_parties; ++i)
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

    for(size_t i = 0; i < m_parties; ++i)
    {
        party_t &peer(m_parties_state[i]);
        if(0 != output_phase_comm(peer.m_id, peer.rnd_data_2send, peer.rnd_data_2recv))
        {
            LC.error("%s: failed retrieve outputs comm requirements to party %lu; Perfect Secure failed."
                    , __FUNCTION__, peer.m_id);
            return (m_run_flag = false);
        }
    }

    return true;
}

bool psmpc_ac_gf28lt::outpt_2_done()
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
            else
                m_output = field->elementToString(interpolate(x1));
        }
    }

    for(size_t i = 0; i < m_parties; ++i)
        m_parties_state[i].m_current_state = ps_done;

    static const u_int8_t done_token[] = { 'd', 'o', 'n', 'e' };
    for(size_t i = 0; i < m_parties; ++i)
    {
    	m_parties_state[i].m_aux.assign(done_token, done_token + 4);
    	m_parties_state[i].rnd_data_2recv = m_parties_state[i].rnd_data_2send = 4;
    }

    return true;
}

void psmpc_ac_gf28lt::print_data() const
{
    for(size_t i = 0; i< m_parties; ++i)
    {
        LC.debug("%s party %lu aux size %lu", __FUNCTION__,i, m_parties_state[i].m_aux.size());
        for (size_t j = 0; j<m_parties_state[i].m_aux.size(); ++j)
        {
            string ___element = field->elementToString(m_parties_state[i].m_aux[j]);
            LC.debug("%s party %lu aux[%lu]=%s", __FUNCTION__, i, j, ___element.c_str());
        }
    }

}
