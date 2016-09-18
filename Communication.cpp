#include "Communication.h"
#include <thread>
bool Communication::m_instanceFlag = false;
Communication* Communication::m_single = NULL;
#define ADDRESS "tcp://localhost:1883"

void ConnectHandler(const char *topicName, MQTTClient_message *&message, const string &str) {
    int elem = stoi(str);
    bool flag = true;
    for(int i=0; i< Communication::getInstance(0, 0)->vecConn.size(); i++)
    {
        if(Communication::getInstance(0, 0)->vecConn[i] == elem)
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
        Communication::getInstance(0, 0)->vecConn.push_back(elem);
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
    string s = to_string(Communication::getInstance(0, 0)->PARTYID);
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
    div = Communication::getInstance(0, 0)->PARTYID / 10;
    mod = Communication::getInstance(0, 0)->PARTYID % 10;
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

    if (topic.find("SHARE_Yjk_VECTOR") != std::string::npos) {
        Communication::getInstance(0, 0)->vecRecForCheck[pid - 1] = str_message;
        Communication::getInstance(0, 0)->countYRecieve++;
    } else {
        // only when all x es recived we can calculate every x
        Communication::getInstance(0, 0)->vec[pid - 1] = str_message;
        Communication::getInstance(0, 0)->countXRecieve++;
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
    Communication::getInstance(0, 0)->deliveredtoken = dt;
}


Communication::Communication(int n, int id) {
    N = n;
    T = N/3 -1;
    vec.resize(N);
    PARTYID = id;

    // start intialize the connection to server

    // messages
    m_pubmsg = MQTTClient_message_initializer;

    // create client object
    const char* c ="party" + id;
    MQTTClient_create(&m_client, ADDRESS, c,
                      MQTTCLIENT_PERSISTENCE_NONE, NULL);

    m_conn_opts.keepAliveInterval = 60;
    m_conn_opts.cleansession = 1;


    // msgarrvd - handle function
    MQTTClient_setCallbacks(m_client, NULL, connlost, msgarrvd, delivered);

    // try to connect to server
    if ((m_rc = MQTTClient_connect(m_client, &m_conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", m_rc);
        //   exit(EXIT_FAILURE);
    }

    //free topic
    // create topics
    string s1 = "SHARE_Ps_VECTOR";
    string s2 = "SHARE_Yjk_VECTOR"+to_string(id);
    s3 = "CONNECT";

    topic[0] = (char*)s1.c_str();
    topic[1] = (char*)s2.c_str();
    topic[2] = (char*)s3.c_str();
    int QQS_ARR[3] = {1, 1, 1};

    // update the topics
    MQTTClient_subscribeMany(m_client,  3, topic, QQS_ARR);
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
                        MQTTClient_deliveryToken &m_token) {// connect

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
}