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

class psmpc_ac : public ProtocolParty<GF28LT>, public ac_protocol
{
    typedef enum {
        ps_nil=0,
        ps_rsfi1,
        ps_rsfi2,
        ps_prep1,
        ps_prep2,
        ps_input_preparation,
        ps_input_adjusment,
        ps_computation,
        ps_output,
        ps_done
    } party_state_t;

    typedef struct __party_t
    {
        party_state_t m_current_state;
        std::vector<u_int8_t> m_data;
        bool m_connected;
        size_t m_id;

        std::vector<GF28LT> m_aux1;

        //rsfi1
        bool rsfi1_sent, rsfi1_rcvd;
        //rsfi2
        bool rsfi2_sent, rsfi2_rcvd;
        //prep1
        bool prep1_sent, prep1_rcvd;
        //prep2
        bool prep2_sent, prep2_rcvd;

        __party_t () : m_current_state(ps_nil), m_connected(false)
                     , rsfi1_sent(false), rsfi1_rcvd(false)
                     , rsfi2_sent(false), rsfi2_rcvd(false)
                     , prep1_sent(false), prep1_rcvd(false)
                     , prep2_sent(false), prep2_rcvd(false)
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
    bool on_input_preparation(party_t & peer);
    bool on_input_adjusment(party_t & peer);
    bool on_computation(party_t & peer);
    bool on_output(party_t & peer);

    bool all_on_the_same_page(party_state_t & current_state);
    bool rsfi1_2_rsfi2();
    bool rsfi2_2_prep1();
    bool prep1_2_prep2();
    bool prep2_2_input_preparation();
    bool input_preparation_2_input_adjustment();
    bool input_adjustment_2_computation();
    bool computation_2_output();
    bool output_2_done();

    int get_no_buckets() const;
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
