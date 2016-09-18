//
// Created by hila on 18/09/16.
//

#ifndef CONTEXT_H_
#define CONTEXT_H_

#include "stdlib.h"
#include "../../workspace/paho/src/MQTTClient.h"
#include "TParty.h"
#include "VDM.h"
#include "TGate.h"
#include "TCircuit.h"

extern void ConnectHandler(const char *topicName, MQTTClient_message *&message, const string &str);
extern int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message);
extern void connlost(void *context, char *cause);
extern void delivered(void *context, MQTTClient_deliveryToken dt);

class Communication {
private:
    static bool m_instanceFlag;
    static Communication *m_single;
    Communication(int n, int id);

public:
    static Communication *getInstance(int numOfParties, int id);
    static Communication* getInstance();

    int PARTYID, N, T, M, countXRecieve, countYRecieve;
    vector<int> vecConn;
    vector<TFieldElement*> vecRecX;
    vector<string> vec;
    vector<string> vecRecForCheck;
    volatile MQTTClient_deliveryToken deliveredtoken;
    MQTTClient_connectOptions m_conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient m_client;
    int m_rc;
    int m_ch;
    char** topic = new char*[3];
    int len;
    MQTTClient_message m_pubmsg;
    MQTTClient_deliveryToken m_token;
    string s3;

    void ConnectionToServer(const MQTTClient &m_client, const string &s, string &myTopicForMessage, MQTTClient_message &m_pubmsg,
                                     MQTTClient_deliveryToken &m_token);
    void SendTheResult(string &myMessage, MQTTClient const &m_client, string &myTopicForMessage,
                       MQTTClient_message &m_pubmsg,
                       MQTTClient_deliveryToken &m_token, const string &s, const vector<string> &buffers);
    void SendXVectorToAllParties(string &myMessage, MQTTClient const &m_client, char *const *topic,
                                 string &myTopicForMessage, MQTTClient_message &m_pubmsg,
                                 MQTTClient_deliveryToken &m_token, string &s);
};


#endif /* CONTEXT_H_ */
