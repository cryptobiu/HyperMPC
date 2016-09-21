#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#include "stdlib.h"
#include "../../workspace/paho/src/MQTTClient.h"
#include "TParty.h"
#include "VDM.h"
#include "TGate.h"
#include "ArythmeticCircuit.h"
#include "Communication.h"

using namespace std;

#define ADDRESS  "tcp://localhost:1883"


class Protocol {
private:
public:
    Protocol();
    void split(const string &s, char delim, vector<string> &elems);
    vector<string> split(const string &s, char delim);
    void Broadcaster(string &myMessage, const MQTTClient &m_client, char *const *topic, string &myTopicForMessage,
                     MQTTClient_message &m_pubmsg, MQTTClient_deliveryToken &m_token, string &s);
    void getXVector(string str, int pid);
    void ConcatenateEverything(vector<string> &buffers, int &no_buckets, HIM &matrix);
    bool CheckIfHappyOrCry(int &no_buckets);
    bool broadcast(int party_id, string myMessage ,MQTTClient &m_client, MQTTClient_message &m_pubmsg,
                   MQTTClient_deliveryToken &m_token, char** &topic, int &m_rc, HIM &him_matrix);
    string test();
    void InputAdjustment(vector<TFieldElement> &diff);
    void InitializationPhase(vector<TGate> &GateValueArr, vector<TFieldElement*> &GateShareArr, vector<bool> &GateDoneArr,
                             HIM &matrix_him,  VDM &matrix_vand);
    void publicReconstruction(vector<TFieldElement*> alpha);
    bool preparationPhase(VDM &matrix_vand, HIM &matrix_him, vector<string> &sharingBuf);
    bool inputPreparation(vector<string> &sharingBuf, vector<TFieldElement*> &gateShareArr, ArythmeticCircuit &circuit);
    void run();
};


#endif /* PROTOCOL_H_ */
