#ifndef CONTEXT_H_
#define CONTEXT_H_

#include "stdlib.h"
#include "../../workspace/paho/src/MQTTClient.h"
#include "VDM.h"
#include "TGate.h"
#include "ArithmeticCircuit.h"
#include <atomic>

#define flag_print false
/**
 * We implement a synchronous protocol, but let it run over an asynchronous network.
 * At first glance, this is insane. We can derive what security properties are still achievable and which are not.
 * no need to waste CPU cycles for objectives which are not achievable, anyway.
 * Assumption: In each round, every party always waits for all messages.
 * Direct consequence: The adversary can make parties wait forever.
 * Indirect consequence: When an (honest) party detects cheating, it can just stop sending further messages.
 * This makes other parties not terminate, but the (apparently present) adversary could have chosen this option anyway.
 * Note: One could also work with a timeout, but it does not change the above indirect consequence.
 */


/**
 * This method handle when new party send connect message.
 */
extern void ConnectHandler(const char *topicName, MQTTClient_message *&message, const string &str);

/**
 * This is a callback method.
 * message The MQTTClient_message structure for the received message.
 * This structure contains the message payload and attributes.
 * This method add the messages to the currect vector of messages according the topic of the message.
 */
extern int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message);

/**
 * This is a callback method.
 * calling when the connection lost.
 */
extern void connlost(void *context, char *cause);

/**
 * This is a callback method.
 * calling when the message publish.
 */
extern void delivered(void *context, MQTTClient_deliveryToken dt);

class Communication {

private:
    static bool m_instanceFlag;
    static Communication* m_single;
    Communication(int n, int id, string ADDRESS);

public:

    /**
     * This members should be public because that the callback methods update them.
     */
    int PARTYID, N, T;
    std::atomic<int> countRF1, countRF2, countRF3, countRF4,
            countRF5, countRF6, countRF7, countRF8;
    vector<string>  vecRF1, vecRF2, vecRF3, vecRF4,
            vecRF5, vecRF6, vecRF7, vecRF8;
    vector<int> vecConn;
    string ADDRESS;

    // MQTTClient members
    int m_rc;
    char** topics = new char*[9];
    MQTTClient_message m_pubmsg;
    MQTTClient_deliveryToken m_token;
    string topic1, topic2, topic3, topic4, topic5, topic6, topic7, topic8, topic9;
    volatile MQTTClient_deliveryToken deliveredtoken;
    MQTTClient_connectOptions m_conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient m_client;

    /**
     * This method create an object of Communication and return it.
     */
    static Communication* getInstance(int numOfParties, int id, string ADDRESS);

    /**
     * This method return an object of Communication.
     */
    static Communication* getInstance();

    /**
     * This method send message "connect" to the broker.
     * All parties wait until there is N "connect" messages before starting the protocol.
     */
    void ConnectionToServer(const string &s);

    /**
     * In an implementation, it is important to send few long messages rather than millions of short messages,
     * even when sent in parallel (in separate threads).
     * In this specification, we always collect all messages of a round into a buffer,
     * and then send the buffer as a whole.
     * This approach of course requires rigorous book-keeping.
     *
     * We communicate in rounds.
     * There is a RoundFunction, which takes a communication object as input and
     * gives a communication object as output. The communication object is an array of length N of buffers,
     * where the i-th buffer in the array is for/from party Pi.
     * For ease of notation, a party can send a buffer to itself.
     * it is then just copied from the send buffer to the receive buffer.
     *
     * Optimization : In this specification, we prefare to separate to 8 different functions, rather one function.
     */
    void roundfunction1(vector<string> &sendBufs, vector<string> &recBufs);
    void roundfunction2(string &myMessage, vector<string> &recBufs);
    void roundfunction3(vector<string> &buffers, vector<string> &recBufs);
    void roundfunction4(vector<string> &sendBufs, vector<string> &recBufs);
    void roundfunction5(vector<string> &sendBufs, vector<string> &recBufs);
    void roundfunction6(vector<string> &sendBufs, vector<string> &recBufs);
    void roundfunction7(vector<string> &sendBufs, vector<string> &recBufs);
    void roundfunction8(vector<string> &sendBufs, vector<string> &recBufs);

    /**
     * This method send the message with the currect topic.
     */
    void send(const string &myTopicForMessage, const string &myMessage);

    virtual ~Communication();
};


#endif /* CONTEXT_H_ */