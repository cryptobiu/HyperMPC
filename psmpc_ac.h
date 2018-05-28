//
// Created by liork on 5/27/18.
//

#ifndef SECRET_SHARING_PSMPC_AC_H
#define SECRET_SHARING_PSMPC_AC_H


#include "ProtocolParty.h"
#include "GF2_8LookupTable.h"
#include "comm_client_cb_api.h"
#include "ac_protocol.h"

typedef GF2_8LookupTable GF28LT;

class psmpc_ac : private ProtocolParty<GF28LT>, public ac_protocol
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
        ps_cmptn,
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
//        string m_inadj_diff;

        //rsfi1
        bool rsfi1_sent, rsfi1_rcvd;
        //rsfi2
        bool rsfi2_sent, rsfi2_rcvd;
        //prep1
        bool prep1_sent, prep1_rcvd;
        //prep2
        bool prep2_sent, prep2_rcvd;
        //inprp
        bool inprp_sent, inprp_rcvd;
        //inadj1
        bool inadj1_sent, inadj1_rcvd;
        //inadj2
        bool inadj2_sent, inadj2_rcvd;
        //outpt
        bool outpt_sent, outpt_rcvd;

        __party_t () : m_current_state(ps_nil), m_connected(false)
                     , rsfi1_sent(false), rsfi1_rcvd(false)
                     , rsfi2_sent(false), rsfi2_rcvd(false)
                     , prep1_sent(false), prep1_rcvd(false)
                     , prep2_sent(false), prep2_rcvd(false)
                     , inprp_sent(false), inprp_rcvd(false)
                     , inadj1_sent(false), inadj1_rcvd(false)
                     , inadj2_sent(false), inadj2_rcvd(false)
                     , outpt_sent(false), outpt_rcvd(false)
        {}
    }party_t;

    ///common structs
    vector<party_t> m_parties_state;
    int m_no_buckets, m_no_random;

    bool party_run_around(const size_t party_id);
    bool on_rsfi1(party_t & peer);
    bool on_rsfi2(party_t & peer);
    bool on_prep1(party_t & peer);
    bool on_prep2(party_t & peer);
    bool on_inprp(party_t & peer);
    bool on_inadj1(party_t & peer);
    bool on_inadj2(party_t & peer);
    bool on_cmptn(party_t & peer);
    bool on_outpt(party_t & peer);

    bool all_on_the_same_page(party_state_t & current_state);
    bool rsfi1_2_rsfi2();
    bool rsfi2_2_prep1();
    bool prep1_2_prep2();
    bool prep2_2_inprp();
    bool inprp_2_inadj1();
    bool inadj1_2_inadj2();
    bool inadj2_2_cmptn();
    bool cmptn_2_outpt();
    bool outpt_2_done();

    void generate_random_double_shares();

    bool send_aux1(party_t &peer);
    bool recv_aux1(party_t & peer, const size_t required_elements);


public:
    psmpc_ac(int argc, char* argv [], const char * logcat);
    virtual ~psmpc_ac();

protected:
    virtual void handle_party_conn(const size_t party_id, const bool connected);
    virtual void handle_party_msg(const size_t party_id, std::vector< u_int8_t > & msg);

    virtual int pre_run();
    virtual bool run_around();
    virtual bool round_up();
    virtual int post_run();
};


#endif //SECRET_SHARING_PSMPC_AC_H
