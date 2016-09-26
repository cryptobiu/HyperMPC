#include "Communication.h"
#include <thread>
bool Communication::m_instanceFlag = false;
Communication* Communication::m_single = NULL;
#define ADDRESS "tcp://localhost:1883"
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;


void ConnectHandler(const char *topicName, MQTTClient_message *&message, const string &str) {
    int elem = stoi(str);
    bool flag = true;
    for(int i=0; i< Communication::getInstance()->vecConn.size(); i++)
    {
        if(Communication::getInstance()->vecConn[i] == elem)
        {
            flag = false;
            break;
        }
    }
    if (flag == true) {

        printf("Message arrived\n");
        printf("   topic: %s\n", topicName);

        printf("   message: ");

        cout << elem << endl;
        Communication::getInstance()->vecConn.push_back(elem);
    }

}

/**
 * the function handle when message arrive
 */
int receive(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
    string topic(topicName);
    string str_message = "";
    int i;
    char *payloadptr;
    string s = to_string(Communication::getInstance()->PARTYID);
    int len_of_message;

    payloadptr = (char *) message->payload;
    string str(payloadptr);

    str = strtok(payloadptr, "$");

    int pid = atoi(str.c_str());

    if (str == s) return 1;

    // loop until client_id
    if (topic == "CONNECT") {
        ConnectHandler(topicName, message, str);
        MQTTClient_freeMessage(&message);
        MQTTClient_free(topicName);
        return 1;
    }

    int count = 0;
    int div, mod;
    div = Communication::getInstance()->PARTYID / 10;
    mod = Communication::getInstance()->PARTYID % 10;
    if (mod == 0) {
        count = div;
    } else {
        count = div + 1;
    }
    for (i = 0; i < count + 1; i++) {
        *payloadptr++;
    }
    len_of_message = message->payloadlen - (count + 1);
    for (i = 0; i < len_of_message; i++) {
        str_message += (*payloadptr++);
    }
}


/**
 * the function handle when message arrive
 */
int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message) {

  //  cout << " thread id" << std::this_thread::get_id() << endl;

    string topic(topicName);
    string str_message = "";
    int i;
    char *payloadptr;
    string s = to_string(Communication::getInstance()->PARTYID);
    int len_of_message;

    payloadptr = (char *) message->payload;
    string str(payloadptr);

    str = strtok(payloadptr, "$");

    int pid = atoi(str.c_str());

    if (str == s) return 1;

    // loop until client_id
    if (topic == "CONNECT") {
        ConnectHandler(topicName, message, str);
        MQTTClient_freeMessage(&message);
        MQTTClient_free(topicName);
        return 1;
    }

    int count = 0;
    int div, mod;
    div = Communication::getInstance()->PARTYID / 10;
    mod = Communication::getInstance()->PARTYID % 10;
    if (mod == 0) {
        count = div;
    } else {
        count = div + 1;
    }
    for (i = 0; i < count + 1; i++) {
        *payloadptr++;
    }
    len_of_message = message->payloadlen - (count + 1);
    for (i = 0; i < len_of_message; i++) {
        str_message += (*payloadptr++);
    }


    if(topic.find("nposLastsend") != std::string::npos)
    {
        Communication::getInstance()->vecLast[pid - 1] = str_message;
        Communication::getInstance()->countLast++;
    }
    else if (topic.find("sendGateShareArr") != std::string::npos) {
        Communication::getInstance()->vecGateShareArr[pid - 1] = str_message;
        Communication::getInstance()->countGateShareArr++;
    }
    else if (topic.find("sendDoubleShare") != std::string::npos) {
        Communication::getInstance()->vecDoubleShare[pid - 1] = str_message;
        Communication::getInstance()->countDoubleShare++;
    } else if (topic.find("sendPartOfPoly") != std::string::npos) {
        Communication::getInstance()->vecSendPartOfPoly[pid - 1] = str_message;
        Communication::getInstance()->countPartOfPoly++;
    } else if (topic.find("SHARE_Yjk_VECTOR") != std::string::npos) {
        Communication::getInstance()->vecRecForCheck[pid - 1] = str_message;
        Communication::getInstance()->countYRecieve++;
    } else {
        // only when all x es recived we can calculate every x
        Communication::getInstance()->vec[pid - 1] = str_message;
        Communication::getInstance()->countXRecieve++;
    }

    printf("Message arrived\n");
    printf("     topic: %s\n", topicName);
    printf("   message: ");

    cout << str_message;
    putchar('\n');
    // clean the pointer of the message
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);

    return 1;
}

void connlost(void *context, char *cause)
{
    printf("\nConnection lost\n");
    printf("     cause: %s\n", cause);
}

void delivered(void *context, MQTTClient_deliveryToken dt)
{
    Communication::getInstance()->deliveredtoken = dt;
}


Communication::Communication(int n, int id) {
    N = n;
    T = N/3 -1;
    vec.resize(N);
    PARTYID = id;
    vecSendPartOfPoly.resize(N);
    vecDoubleShare.resize(N);
    vecGateShareArr.resize(N);
    vecLast.resize(N);
    countLast = 0;
    countPartOfPoly = 0;
    countDoubleShare = 0;
    // start intialize the connection to server

    // messages
    m_pubmsg = MQTTClient_message_initializer;

    // create client object
    const char* c ="party" + id;
    MQTTClient_create(&m_client, ADDRESS, c,
                      MQTTCLIENT_PERSISTENCE_NONE, NULL);

    m_conn_opts.keepAliveInterval = 120;
    m_conn_opts.cleansession = 1;


    // msgarrvd - handle function
    MQTTClient_setCallbacks(m_client, NULL, connlost, msgarrvd, delivered);

    // try to connect to server
    if ((m_rc = MQTTClient_connect(m_client, &m_conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", m_rc);
        //   exit(EXIT_FAILURE);
    }

    // create topics
    string s1 = "SHARE_Ps_VECTOR";
    string s2 = "SHARE_Yjk_VECTOR"+to_string(id);
    s3 = "CONNECT";
    string s4 = "sendPartOfPoly"+to_string(id);
    string s5 = "sendDoubleShare"+to_string(id);
    string s6 = "sendGateShareArr"+to_string(id);
    string s7 = "Lastsend"+to_string(id);

    topic[0] = (char*)s1.c_str();
    topic[1] = (char*)s2.c_str();
    topic[2] = (char*)s3.c_str();
    topic[3] = (char*)s4.c_str();
    topic[4] = (char*)s5.c_str();
    topic[5] = (char*)s6.c_str();
    topic[6] = (char*)s7.c_str();
    int QQS_ARR[7] = {1, 1, 1, 1, 1, 1, 1};

    // update the topics
    MQTTClient_subscribeMany(m_client,  7, topic, QQS_ARR);
}

/**
 * return the Communication
 */
Communication* Communication::getInstance(int numOfParties, int id)
{
    if(!m_instanceFlag)
    {
        m_instanceFlag = true;
        m_single = new Communication(numOfParties, id);
        return m_single;
    }
    else
    {
        return m_single;
    }
}

Communication* Communication::getInstance()
{
    return m_single;
}

void Communication::ConnectionToServer(const MQTTClient &m_client, const string &s, string &myTopicForMessage, MQTTClient_message &m_pubmsg,
                        MQTTClient_deliveryToken &m_token) {

    string strMessage = s + "$" + s;

    myTopicForMessage = "CONNECT";

    // update the details of message
    m_pubmsg.payload =(void*) strMessage.c_str();
    m_pubmsg.payloadlen = strMessage.size();
    m_pubmsg.qos = 1;
    m_pubmsg.retained = 0;
    deliveredtoken = 0;

    // publish the message to all parties
    MQTTClient_publishMessage(m_client, myTopicForMessage.c_str(), &m_pubmsg, &m_token);

    // wait until all parties connect
    while(vecConn.size() < N-1){
        MQTTClient_publishMessage(m_client, myTopicForMessage.c_str(), &m_pubmsg, &m_token);
    }

    MQTTClient_publishMessage(m_client, myTopicForMessage.c_str(), &m_pubmsg, &m_token);

    cout << "Let's start!" << endl;
}

void Communication::SendTheResult(string &myMessage, MQTTClient const &m_client, string &myTopicForMessage,
                   MQTTClient_message &m_pubmsg,
                   MQTTClient_deliveryToken &m_token, const string &s, const vector<string> &buffers) {// buffers[i] = the buffer of party i+1
    // buffers[0] = party 1

    for(int i=0; i<buffers.size(); i++)
    {
        myMessage = s + "$" + buffers[i];

        myTopicForMessage = "SHARE_Yjk_VECTOR" + to_string(i + 1);

        m_pubmsg.payload = (void *) myMessage.c_str();

        m_pubmsg.payloadlen = myMessage.size();
        m_pubmsg.qos = 1;
        m_pubmsg.retained = 0;
        deliveredtoken = 0;

        MQTTClient_publishMessage(m_client, myTopicForMessage.c_str(), &m_pubmsg, &m_token);

        while (deliveredtoken != m_token) {};
    }
    while (countYRecieve < N - 1) {}
}


void Communication::SendXVectorToAllParties(string &myMessage, MQTTClient const &m_client, char *const *topic,
                             string &myTopicForMessage, MQTTClient_message &m_pubmsg,
                             MQTTClient_deliveryToken &m_token, string &s) {
    myTopicForMessage = "SHARE_Ps_VECTOR";

    // add id party to the message
    myMessage = s + "$" + myMessage;

    // update the details of message
    m_pubmsg.payload = (void *) myMessage.c_str();
    m_pubmsg.payloadlen = myMessage.size();
    m_pubmsg.qos = 1;
    m_pubmsg.retained = 0;
    deliveredtoken = 0;

    // publish the message to all parties
    MQTTClient_publishMessage(m_client, myTopicForMessage.c_str(), &m_pubmsg, &m_token);

    // waiting until the message send
    while (deliveredtoken != m_token) {};

    while (countXRecieve < N - 1) {}
}

Communication::~Communication() {
    cout << "                                                      in  ~Communication()" << endl;
    MQTTClient_disconnect(m_client, 10000);
    cout << "                                                       after MQTTClient_disconnect " << endl;
    MQTTClient_destroy(&m_client);

    cout << "                                                               after destroy " << endl;
}

/**
 * the function update the details of message and send it.
 * @param myTopicForMessage
 * @param myMessage
 */
void Communication::send(const string &myTopicForMessage, const string &myMessage) {
    // update the details of message
    this->m_pubmsg.payload = (void *) myMessage.c_str();
    this->m_pubmsg.payloadlen = myMessage.size();
    this->m_pubmsg.qos = 1;
    this->m_pubmsg.retained = 0;
    this->deliveredtoken = 0;

    // publish the message to all parties
    MQTTClient_publishMessage(this->m_client, myTopicForMessage.c_str(), &this->m_pubmsg, &this->m_token);

    // waiting until the message send
    while (this->deliveredtoken != this->m_token) {};
}


void Communication::sendPartOfPoly(vector<string> &sendBufs,vector<string> &recBufs) {
    string s = to_string(PARTYID);
    string myTopicForMessage;

    recBufs[PARTYID-1] = sendBufs[PARTYID-1];

    for(int i=0; i<N; i++)
    {
        myTopicForMessage = "sendPartOfPoly" + to_string(i+1);
        // add id party to the message
        string myMessage = s + "$" + sendBufs[i];
        send(myTopicForMessage, myMessage);
        cout << "i publish my message to :    " << i+1 <<"   "<< myMessage <<endl;
    }

    while(countPartOfPoly < N - 1) {}

    for(int i=0; i<N; i++)
    {
        if(i != PARTYID-1) {
            recBufs[i] = vecSendPartOfPoly[i];
        }
    }
}

void Communication::sendDoubleShare(vector<string> &sendBufs,vector<string> &recBufs) {
    string s = to_string(PARTYID);
    string myTopicForMessage;

    recBufs[PARTYID-1] = sendBufs[PARTYID-1];

    for(int i=0; i<N; i++)
    {
        myTopicForMessage = "sendDoubleShare" + to_string(i+1);
        // add id party to the message
        string myMessage = s + "$" + sendBufs[i];
        send(myTopicForMessage, myMessage);
        cout << "i publish my message to :    " << i+1 <<"   "<< myMessage <<endl;
    }

    while(countDoubleShare < N - 1) {}

    for(int i=0; i<N; i++)
    {
        if(i != PARTYID-1) {
            recBufs[i] = vecDoubleShare[i];
        }
    }
}

void Communication::sendGateShareArr(vector<string> &sendBufs,vector<string> &recBufs) {
    string s = to_string(PARTYID);
    string myTopicForMessage;

    recBufs[PARTYID-1] = sendBufs[PARTYID-1];

    for(int i=0; i<N; i++)
    {
        myTopicForMessage = "sendGateShareArr" + to_string(i+1);
        // add id party to the message
        string myMessage = s + "$" + sendBufs[i];
        send(myTopicForMessage, myMessage);
        cout << "i publish my message to :    " << i+1 <<"   "<< myMessage <<endl;
    }

    while(countGateShareArr < N - 1) {}

    for(int i=0; i<N; i++)
    {
        if(i != PARTYID-1) {
            recBufs[i] = vecGateShareArr[i];
        }
    }
}

void Communication::Lastsend(vector<string> &sendBufs,vector<string> &recBufs) {
    string s = to_string(PARTYID);
    string myTopicForMessage;

    recBufs[PARTYID-1] = sendBufs[PARTYID-1];

    for(int i=0; i<N; i++)
    {
        myTopicForMessage = "Lastsend" + to_string(i+1);
        // add id party to the message
        string myMessage = s + "$" + sendBufs[i];
        send(myTopicForMessage, myMessage);
        cout << "i publish my message to :    " << i+1 <<"   "<< myMessage <<endl;
    }

    while(countLast < N - 1) {}

    for(int i=0; i<N; i++)
    {
        if(i != PARTYID-1) {
            recBufs[i] = vecLast[i];
        }
    }
}