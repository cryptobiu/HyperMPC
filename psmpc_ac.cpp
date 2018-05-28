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
    {
        m_parties_state[i].m_id = i;
    }
    m_no_random = circuit.getNrOfInputGates();
    m_no_buckets = (m_no_random / (N-2*T))+1;

    generate_random_double_shares();
    m_parties_state[m_id].m_current_state = ps_rsfi1;
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
            m_parties_state[i].m_aux1.push_back(y1[i]);
    }
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
        case ps_input_preparation:
            return on_input_preparation(peer);
        case ps_input_adjusment:
            return on_input_adjusment(peer);
        case ps_computation:
            return on_computation(peer);
        case ps_output:
            return on_output(peer);
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
            return prep2_2_input_preparation();
        case ps_input_preparation:
            return input_preparation_2_input_adjustment();
        case ps_input_adjusment:
            return input_adjustment_2_computation();
        case ps_computation:
            return computation_2_output();
        case ps_output:
            return output_2_done();
        case ps_done:
            return true;
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

bool psmpc_ac::send_aux1(party_t &peer)
{
    size_t elem_size = field->getElementSizeInBytes();
    u_int8_t buffer[4096];
    size_t offset = 0;
    for (std::vector<GF28LT>::iterator j = peer.m_aux1.begin(); j != peer.m_aux1.end(); ++j)
    {
        field->elementToBytes(buffer + offset, *j);
        offset += elem_size;

        if ((offset + elem_size) > 4096 || peer.m_aux1.end() == (j + 1))
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

bool psmpc_ac::recv_aux1(party_t & peer, const size_t required_elements)
{
    size_t elem_size = field->getElementSizeInBytes();

    for(size_t i = 0; i < required_elements; ++i)
    {
        if(peer.m_data.size() < elem_size)
            return false;

        GF28LT x = field->bytesToElement(peer.m_data.data());
        peer.m_aux1.push_back(x);
        peer.m_data.erase(peer.m_data.begin(), peer.m_data.begin() + elem_size);
    }

    return true;
}

bool psmpc_ac::on_rsfi1(party_t & peer)
{

    size_t elem_size = field->getElementSizeInBytes();

    if(!peer.rsfi1_sent)
    {
        if (!send_aux1(peer))
        {
            LC.error("%s: failed sending data to party %lu; Perfect Secure failed.", __FUNCTION__, peer.m_id);
            return (m_run_flag = false);
        }
        peer.m_aux1.clear();
        peer.rsfi1_sent = true;
    }

    if(!peer.rsfi1_rcvd)
    {
        size_t required_elements = m_no_buckets - peer.m_aux1.size();

        if (!recv_aux1(peer, required_elements))
            return false;
        else
            peer.rsfi1_rcvd = true;
    }

    return true;
}

bool psmpc_ac::rsfi1_2_rsfi2()
{
    vector <GF28LT> x1(N), y1(N);
    for(int k=0; k < m_no_buckets; k++)
    {
        // generate random degree-T polynomial
        for (int i = 0; i < N; i++)
            x1[i] = m_parties_state[i].m_aux1[k];

        for (int i = 0; i < N; i++)
            m_parties_state[i].m_aux1.clear();

        matrix_him.MatrixMult(x1, y1);

        // these shall be checked
        int robin = 0;
        for (int i = 0; i < 2 * T; i++)
        {
            m_parties_state[robin].m_aux1.push_back(y1[i]);
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
    if (!peer.rsfi2_sent)
    {
        if (!send_aux1(peer))
        {
            LC.error("%s: failed sending data to party %lu; Perfect Secure failed.", __FUNCTION__, peer.m_id);
            return (m_run_flag = false);
        }
        peer.m_aux1.clear();
        peer.rsfi2_sent = true;
    }

    if(!peer.rsfi2_rcvd)
    {
        size_t required_elements = m_no_buckets - peer.m_aux1.size();

        if (!recv_aux1(peer, required_elements))
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
            x1[i] = m_parties_state[i].m_aux1[k];


        vector<GF28LT> x_until_d(N);
        for(int i=0; i<T; i++)
            x_until_d[i] = x1[i];

        for(int i=T; i<N; i++)
            x_until_d[i] = *(field->GetZero());

        // Check that x1 is t-consistent and x2 is 2t-consistent and secret is the same

        if(!checkConsistency(x1,T))
            return false;
    }

    for(size_t i = 0; i < m_parties_state.size(); ++i)
        m_parties_state[i].m_current_state = ps_prep1;

    sharingBufTElements.resize(m_no_buckets*(N-2*T)); // my shares of the double-sharings
    sharingBuf2TElements.resize(m_no_buckets*(N-2*T)); // my shares of the double-sharings

    /**
     *  generate double sharings.
     *  first degree t.
     *  subsequent: degree 2t with same secret.
     */

    for(int k=0; k < m_no_buckets; k++)
    {
        // generate random degree-T polynomial
        for(int i = 0; i < T+1; i++)
        {
            // A random field element, uniform distribution
            x1[i] = field->Random();

        }

        x2[0] = x1[0];

        for(int i = 1; i < 2*T+1; i++)
        {
            // otherwise random
            x2[i] = field->Random();
        }

        matrix_vand.MatrixMult(x1, y1, T+1); // eval poly at alpha-positions
        matrix_vand.MatrixMult(x2, y2, 2*T+1); // eval poly at alpha-positions

        // prepare shares to be sent
        for(int i=0; i < N; i++)
        {
            //cout << "y1[ " <<i<< "]" <<y1[i] << endl;
            m_parties_state[i].m_aux1[2*k] = y1[i];
            m_parties_state[i].m_aux1[2*k+1] = y2[i];
        }

    }//end print one

    return true;
}

bool psmpc_ac::on_prep1(party_t &peer)
{
    if (!peer.prep1_sent)
    {
        if (!send_aux1(peer))
        {
            LC.error("%s: failed sending data to party %lu; Perfect Secure failed.", __FUNCTION__, peer.m_id);
            return (m_run_flag = false);
        }
        peer.m_aux1.clear();
        peer.prep1_sent = true;
    }

    if(!peer.prep1_rcvd)
    {
        size_t required_elements = m_no_buckets - peer.m_aux1.size();

        if (!recv_aux1(peer, required_elements))
            return false;
        else
            peer.prep1_rcvd = true;
    }

    return true;
}

bool psmpc_ac::prep1_2_prep2()
{
    size_t elem_size = field->getElementSizeInBytes();
    vector<GF28LT> x1(N),x2(N),y1(N),y2(N);
    int robin = 0;

    // x1 : used for the N degree-t sharings
    // x2 : used for the N degree-2t sharings
    for(int k=0; k < m_no_buckets; k++)
    {
        // generate random degree-T polynomial
        for (int i = 0; i < N; i++)
        {
            x1[i] = m_parties_state[i].m_aux1[2 * k];
            x2[i] = m_parties_state[i].m_aux1[2 * k + 1];
        }

        matrix_him.MatrixMult(x1, y1);
        matrix_him.MatrixMult(x2, y2);
        // these shall be checked
        for (int i = 0; i < 2 * T; i++)
        {
            m_parties_state[robin].m_aux1.push_back(y1[i]);
            m_parties_state[robin].m_aux1.push_back(y2[i]);
            robin = (robin+1) % N; // next robin
        }

        // Y1 : the degree-t shares of my poly
        // Y2 : the degree 2t shares of my poly
        for (int i = 2 * T; i < N; i++) {

            sharingBufTElements[k*(N-2*T) + i - 2*T] = y1[i];
            sharingBuf2TElements[k*(N-2*T) + i - 2*T] =  y2[i];
        }
        x2[0] = *(field->GetZero());
        x1[0] = *(field->GetZero());
    }
}


bool psmpc_ac::on_prep2(party_t &peer)
{
    if (!peer.prep2_sent)
    {
        if (!send_aux1(peer))
        {
            LC.error("%s: failed sending data to party %lu; Perfect Secure failed.", __FUNCTION__, peer.m_id);
            return (m_run_flag = false);
        }
        peer.m_aux1.clear();
        peer.prep2_sent = true;
    }

    if(!peer.prep2_rcvd)
    {
        size_t required_elements = m_no_buckets - peer.m_aux1.size();

        if (!recv_aux1(peer, required_elements))
            return false;
        else
            peer.prep2_rcvd = true;
    }

    return true;
}
