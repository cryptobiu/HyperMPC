
#pragma once

#include <semaphore.h>

#include "ProtocolParty.h"
#include "GF2_8LookupTable.h"
#include "comm_client_cb_api.h"
#include "ac_protocol.h"

typedef GF2_8LookupTable GF28LT;

class psmpc_ac_gf28lt : private ProtocolParty<GF28LT>, public ac_protocol
{
    typedef enum
    {
        ps_nil=0,
        ps_rsfi1,
        ps_rsfi2,
        ps_prep1,
        ps_prep2,
        ps_inprp,
        ps_inadj1,
        ps_inadj2,
        ps_outpt,
        ps_done
    } party_state_t;

    typedef struct __party_t
    {
        party_state_t m_current_state;
        std::vector<u_int8_t> m_data;
        bool m_connected;
        size_t m_id;
        std::vector<GF28LT> m_aux;
        size_t rnd_data_sent, rnd_data_rcvd, rnd_data_2send, rnd_data_2recv;

        __party_t () : m_current_state(ps_nil), m_connected(false)
        			 , m_id((size_t)-1)
                     , rnd_data_sent(0), rnd_data_rcvd(0)
                     , rnd_data_2send(0), rnd_data_2recv(0)
        {}
    }party_t;

    ///common structs
    vector<party_t> m_parties_state;
    int m_no_buckets;

    bool party_run_around(const size_t party_id);
    bool on_round_send_and_recv(party_t &peer);

    bool all_on_the_same_page(party_state_t & current_state);
    bool rsfi1_2_rsfi2();
    bool rsfi2_2_prep1();
    bool prep1_2_prep2();
    bool prep2_2_inprp();
    bool inprp_2_inadj1();
    bool inadj1_2_inadj2();
    bool inadj2_2_outpt();
    bool outpt_2_done();

    void generate_random_double_shares();

    void print_data() const;

    bool send_aux(party_t &peer);
    bool recv_aux(party_t &peer, const size_t required_elements);
    void do_send_and_recv(const vector< vector< byte > > & _2send, vector< vector< byte > > & _2recv);

    std::string m_output;

public:
    psmpc_ac_gf28lt(int argc, char* argv [], comm_client::cc_args_t * args);
    virtual ~psmpc_ac_gf28lt();

protected:
    virtual void handle_party_conn(const size_t party_id, const bool connected);
    virtual void handle_party_msg(const size_t party_id, std::vector< u_int8_t > & msg);

    virtual int pre_run();
    virtual bool run_around();
    virtual bool round_up();
    virtual int post_run();

    int output_phase_comm(const size_t peer_id, size_t &to_send, size_t &to_recv);
};

