#include "Communication.h"
#include <thread>
#include <mutex>
#include <thread>
#include <mutex>
#include <condition_variable>


bool Communication::m_instanceFlag = false;
Communication* Communication::m_single = NULL;

using namespace std;


void ConnectHandler(const char *topicName, MQTTClient_message *&message, const string &str) {
    int elem = stoi(str);
    bool flag1 = true;
    for(int i=0; i< Communication::getInstance()->vecConn.size(); i++)
    {
        if(Communication::getInstance()->vecConn[i] == elem)
        {
            flag1 = false;
            break;
        }
    }
    if (flag1 == true) {

        if(flag_print) {
            printf("Message arrived\n");
            printf("   topics: %s\n", topicName);

            printf("   message: ");

            cout << elem << '\n';
        }

        Communication::getInstance()->vecConn.push_back(elem);
    }

}

/**
 * the function handle when message arrive
 */
/*
int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message) {
    string topic(topicName);
    string str_message = "";
    int i;
    char *payloadptr;
    string s = to_string(Communication::getInstance()->PARTYID);
    int len_of_message;

    payloadptr = (char *) message->payload;
    string str(payloadptr);

    string temptemp = str;


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

    len_of_message = message->payloadlen - (str.size() + 1);

    string prefix = "";
    string suffix = "";
    for (int j = 0; j < temptemp.size(); ++j) {
        if (temptemp[j] == '$'){
            prefix = temptemp.substr(0,j);
            suffix = temptemp.substr(j+1, len_of_message);
            if (flag_print) {
                cout << "prefix: " << prefix << "\nsuffix: " << suffix << endl; }
            break;
        }
    }

    str_message = suffix;

    if(topic.find("roundfunction8") != std::string::npos)
    {
        Communication::getInstance()->rfVectors[7][pid - 1] = str_message;
        Communication::getInstance()->counters[7]++;
    }
    else if (topic.find("roundfunction1") != std::string::npos)
    {
        Communication::getInstance()->rfVectors[0][pid - 1] = str_message;
        Communication::getInstance()->counters[0]++;
    }
    else if(topic.find("roundfunction7") != std::string::npos)
    {
        Communication::getInstance()->rfVectors[6][pid - 1] = str_message;
        Communication::getInstance()->counters[6]++;
    }
    else if (topic.find("roundfunction6") != std::string::npos) {
        Communication::getInstance()->rfVectors[5][pid - 1] = str_message;
        Communication::getInstance()->counters[5]++;
    }
    else if (topic.find("roundfunction5") != std::string::npos) {
        Communication::getInstance()->rfVectors[4][pid - 1] = str_message;
        Communication::getInstance()->counters[4]++;
    } else if (topic.find("roundfunction4") != std::string::npos) {
        Communication::getInstance()->rfVectors[3][pid - 1] = str_message;
        Communication::getInstance()->counters[3]++;
    } else if (topic.find("roundfunction3") != std::string::npos) {
        Communication::getInstance()->rfVectors[2][pid - 1] = str_message;
        Communication::getInstance()->counters[2]++;
    } else {
        // only when all x es recived we can calculate every x
        Communication::getInstance()->vecRF2[pid - 1] = str_message;
        Communication::getInstance()->countRF2++;
    }


    if(flag_print) {
        printf("Message arrived\n");
        printf("     topics: %s\n", topicName);
        printf("   message: ");

        cout << str_message;
        putchar('\n'); }
    // clean the pointer of the message
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

*/





////////////////////////BYTES/////////////////
int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message) {
    string topic(topicName);
    string str_message = "";
    int i;


    // loop until client_id
    if (topic == "CONNECT") {



        string s = to_string(Communication::getInstance()->PARTYID);
        char *payloadptr;
        payloadptr = (char *) message->payload;
        string str(payloadptr);

        str = strtok(payloadptr, "$");

        if(str==s) return 1;

        ConnectHandler(topicName, message, str);
        MQTTClient_freeMessage(&message);
        MQTTClient_free(topicName);
        return 1;
    }

    byte *payloadptr;
    //string s = to_string(Communication::getInstance()->PARTYID);


    payloadptr = (byte *) message->payload;


    uint32_t * getParty = (uint32_t*)payloadptr;

    int pid = getParty[0];
    string pidStr = to_string(pid);

    if (pid == Communication::getInstance()->PARTYID) return 1;




    //copy the message with out the leading party number of size int
    vector<byte> recMesg(message->payloadlen - 4);

    memcpy(recMesg.data(), payloadptr+4, recMesg.size());

    if(topic.find("roundfunction8") != std::string::npos)
    {
        Communication::getInstance()->rfVectors[7][pid - 1] = recMesg;
        Communication::getInstance()->counters[7]++;
    }
    else if (topic.find("roundfunction1") != std::string::npos)
    {
        Communication::getInstance()->rfVectors[0][pid - 1] = recMesg;
        Communication::getInstance()->counters[0]++;
    }
    else if(topic.find("roundfunction7") != std::string::npos)
    {
        Communication::getInstance()->rfVectors[6][pid - 1] = recMesg;
        Communication::getInstance()->counters[6]++;
    }
    else if (topic.find("roundfunction6") != std::string::npos) {
        Communication::getInstance()->rfVectors[5][pid - 1] = recMesg;
        Communication::getInstance()->counters[5]++;
    }
    else if (topic.find("roundfunction5") != std::string::npos) {
        Communication::getInstance()->rfVectors[4][pid - 1] = recMesg;
        Communication::getInstance()->counters[4]++;
    } else if (topic.find("roundfunction4") != std::string::npos) {
        Communication::getInstance()->rfVectors[3][pid - 1] = recMesg;
        Communication::getInstance()->counters[3]++;
    } else if (topic.find("roundfunction3") != std::string::npos) {
        Communication::getInstance()->rfVectors[2][pid - 1] = recMesg;
        Communication::getInstance()->counters[2]++;
    } else {
        // only when all x es recived we can calculate every x
        Communication::getInstance()->rfVectors[1][pid - 1] = recMesg;
        Communication::getInstance()->counters[1]++;
    }


    if(flag_print) {
        printf("Message arrived\n");
        printf("     topics: %s\n", topicName);
        printf("   message: ");

        cout << str_message;
        putchar('\n'); }
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

Communication::Communication(int n, int id, string ADDRESS) {
    PARTYID = id;
    N = n;
    T = N/3 - 1;
    if(N%3 > 0)
    {
        T++;
    }
    rfVectors.resize(8);
    counters.resize(8);

    for(int i=0; i<8; i++){
        counters[i] = 0;
        rfVectors[i].resize(N);
    }


    vecRF1.resize(N);
    vecRF2.resize(N);
    vecRF3.resize(N);
    vecRF4.resize(N);
    vecRF5.resize(N);
    vecRF6.resize(N);
    vecRF7.resize(N);
    vecRF8.resize(N);
    countRF1 = 0;
    countRF2 = 0;
    countRF3 = 0;
    countRF4 = 0;
    countRF5 = 0;
    countRF6 = 0;
    countRF7 = 0;
    countRF8 = 0;

    counterEven = 0;
    counterOdd = 0;
    vecRFEven.resize(N);
    vecRFOdd.resize(N);



    // start intialize the connection to server
    // messages
    m_pubmsg = MQTTClient_message_initializer;

    // create client object
    const char* c ="party" + id;
    MQTTClient_create(&m_client, ADDRESS.c_str(), c,
                      MQTTCLIENT_PERSISTENCE_NONE, NULL);

    m_conn_opts.keepAliveInterval = 50000;
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
    topic1 = "SHARE_Ps_VECTOR";
    topic2 = "roundfunction3"+to_string(id);
    topic3 = "CONNECT";
    topic4 = "roundfunction4"+to_string(id);
    topic5 = "roundfunction5"+to_string(id);
    topic6 = "roundfunction6"+to_string(id);
    topic7 = "roundfunction7"+to_string(id);
    topic8 = "roundfunction1"+to_string(id);
    topic9 = "roundfunction8"+to_string(id);
    topics[0] = (char*)topic1.c_str();
    topics[1] = (char*)topic2.c_str();
    topics[2] = (char*)topic3.c_str();
    topics[3] = (char*)topic4.c_str();
    topics[4] = (char*)topic5.c_str();
    topics[5] = (char*)topic6.c_str();
    topics[6] = (char*)topic7.c_str();
    topics[7] = (char*)topic8.c_str();
    topics[8] = (char*)topic9.c_str();
    int QQS_ARR[9] = {1, 1, 1, 1, 1, 1, 1, 1, 1};

    // update the topics
    MQTTClient_subscribeMany(m_client,  9, topics, QQS_ARR);
}

/**
 * return the Communication
 */
Communication* Communication::getInstance(int numOfParties, int id, string ADDRESS)
{
    if(!m_instanceFlag)
    {
        m_instanceFlag = true;
        m_single = new Communication(numOfParties, id, ADDRESS);
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

void Communication::ConnectionToServer(const string &s) {

    string strMessage = s + "$" + s;

    string myTopicForMessage = "CONNECT";

    // update the details of message
    m_pubmsg.payload =(void*) strMessage.c_str();
    m_pubmsg.payloadlen = strMessage.size();
    m_pubmsg.qos = 1;
    m_pubmsg.retained = 0;
    deliveredtoken = 0;

    // publish the message to all parties
    MQTTClient_publishMessage(m_client, myTopicForMessage.c_str(), &m_pubmsg, &m_token);

    // wait until all parties connect
    while(vecConn.size() < N-1)
    {
        MQTTClient_publishMessage(m_client, myTopicForMessage.c_str(), &m_pubmsg, &m_token);
    }

    MQTTClient_publishMessage(m_client, myTopicForMessage.c_str(), &m_pubmsg, &m_token);

    if(flag_print) {
        cout << "Let's start!" << '\n'; }
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
    this->m_pubmsg.qos = 0;
    this->m_pubmsg.retained = 0;
    this->deliveredtoken = 0;

    // publish the message to all parties
    MQTTClient_publishMessage(this->m_client, myTopicForMessage.c_str(), &this->m_pubmsg, &this->m_token);

    // waiting until the message send
    //while (this->deliveredtoken != this->m_token) {};
}


/**
* the function update the details of message and send it.
* @param myTopicForMessage
* @param myMessage
*/
void Communication::sendBytes(const string &myTopicForMessage, byte *msg, int size) {
    // update the details of message
    this->m_pubmsg.payload = (void *) msg;
    this->m_pubmsg.payloadlen = size;
    this->m_pubmsg.qos = 0;
    this->m_pubmsg.retained = 0;
    this->deliveredtoken = 0;

    // publish the message to all parties
    MQTTClient_publishMessage(this->m_client, myTopicForMessage.c_str(), &this->m_pubmsg, &this->m_token);

}



void Communication::roundfunctionI(vector<vector<byte>> &sendBufs, vector<vector<byte>> &recBufs, int roundFunctionId) {
    string myTopicForMessage;


    for(int i=0; i<N; i++)
    {
        myTopicForMessage = "roundfunction" + to_string(roundFunctionId) + to_string(i+1);

        // add id party to the message
        ((int*)sendBufs[i].data())[0] = PARTYID;
        sendBytes(myTopicForMessage, sendBufs[i].data(), sendBufs[i].size() );
    }

    while(counters[roundFunctionId-1] < N - 1) {}

    //there is no farther use of the rfVectors so we can move the data
    recBufs = rfVectors[roundFunctionId-1];


    //get the vector without the leading party id.
    vector<byte> sendCut(sendBufs[PARTYID-1].size() - 4);

    memcpy(sendCut.data(), sendBufs[PARTYID-1].data()+4, sendCut.size());

    recBufs[PARTYID-1] = move(sendCut);

    counters[roundFunctionId-1] = 0;

}


void Communication::roundfunction2(vector<byte> &myMessage, vector<vector<byte>> &recBufs) {
    recBufs[PARTYID-1] = myMessage;
    string myTopicForMessage = "SHARE_Ps_VECTOR";

    // add id party to the message
    ((int*)myMessage.data())[0] = PARTYID;
    sendBytes(myTopicForMessage, myMessage.data(), myMessage.size() );


    while (counters[1] < N - 1) {}


    //there is no farther use of the rfVectors so we can move the data
    recBufs = move(rfVectors[1]);

    //get the vector without the leading party id.
    vector<byte> sendCut(myMessage.size() - 4);

    memcpy(sendCut.data(), myMessage.data()+4, sendCut.size());

    recBufs[PARTYID-1] = move(sendCut);

    counters[1] = 0;

}

void Communication::roundfunction3(vector<string> &buffers, vector<string> &recBufs) {// buffers[i] = the buffer of party i+1
    // buffers[0] = party 1
    recBufs[PARTYID-1] = buffers[PARTYID-1];

    string myMessage, myTopicForMessage;
    string s= to_string(PARTYID);
    for(int i=0; i<buffers.size(); i++)
    {
        myMessage = s + "$" + buffers[i];

        myTopicForMessage = "roundfunction3" + to_string(i + 1);

        m_pubmsg.payload = (void *) myMessage.c_str();

        m_pubmsg.payloadlen = myMessage.size();
        m_pubmsg.qos = 0;
        m_pubmsg.retained = 0;
        deliveredtoken = 0;

        MQTTClient_publishMessage(m_client, myTopicForMessage.c_str(), &m_pubmsg, &m_token);

        //while (deliveredtoken != m_token) {};
    }
    while (countRF3 < N - 1) {}

    for(int i=0; i<N; i++)
    {
        if(i != PARTYID-1) {
            recBufs[i] =  vecRF3[i];
        }
    }
}

void Communication::roundfunction4(vector<string> &sendBufs, vector<string> &recBufs) {
    string s = to_string(PARTYID);
    string myTopicForMessage;

    recBufs[PARTYID-1] = sendBufs[PARTYID-1];

    for(int i=0; i<N; i++)
    {
        myTopicForMessage = "roundfunction4" + to_string(i+1);
        // add id party to the message
        string myMessage = "";
        myMessage = s + "$" + sendBufs[i];
        send(myTopicForMessage, myMessage);
    }

    while(countRF4 < N - 1) {}

    for(int i=0; i<N; i++)
    {
        if(i != PARTYID-1) {
            recBufs[i] = vecRF4[i];
        }
    }
    countRF4 = 0;
}

void Communication::roundfunction5(vector<string> &sendBufs, vector<string> &recBufs) {
    string s = to_string(PARTYID);
    string myTopicForMessage;

    recBufs[PARTYID-1] = sendBufs[PARTYID-1];

    for(int i=0; i<N; i++)
    {
        myTopicForMessage = "roundfunction5" + to_string(i+1);
        // add id party to the message
        string myMessage = s + "$" + sendBufs[i];
        send(myTopicForMessage, myMessage);
    }

    while(countRF5 < N - 1) {}

    for(int i=0; i<N; i++)
    {
        if(i != PARTYID-1) {
            recBufs[i] = vecRF5[i];
        }
    }
    countRF5 = 0;
}

void Communication::roundfunction6(vector<string> &sendBufs, vector<string> &recBufs) {
    string s = to_string(PARTYID);
    string myTopicForMessage;

    recBufs[PARTYID-1] = sendBufs[PARTYID-1];

    for(int i=0; i<N; i++)
    {
        myTopicForMessage = "roundfunction6" + to_string(i+1);
        // add id party to the message
        string myMessage = s + "$" + sendBufs[i];
        send(myTopicForMessage, myMessage);
    }

    while(countRF6 < N - 1) {}

    for(int i=0; i<N; i++)
    {
        if(i != PARTYID-1) {
            recBufs[i] = vecRF6[i];
        }
    }

    countRF6 = 0;
}

void Communication::roundfunction7(vector<string> &sendBufs, vector<string> &recBufs) {
    string s = to_string(PARTYID);
    string myTopicForMessage;

    recBufs[PARTYID-1] = sendBufs[PARTYID-1];

    for(int i=0; i<N; i++)
    {
        myTopicForMessage = "roundfunction7" + to_string(i+1);
        // add id party to the message
        string myMessage = s + "$" + sendBufs[i];
        send(myTopicForMessage, myMessage);
    }

    while(countRF7 < N - 1) {}

    for(int i=0; i<N; i++)
    {
        if(i != PARTYID-1) {
            recBufs[i] = vecRF7[i];
        }
    }
    countRF7 = 0;
}

void Communication::roundfunction8(vector<string> &sendBufs, vector<string> &recBufs) {

    string s = to_string(PARTYID);
    string myTopicForMessage;

    recBufs[PARTYID-1] = sendBufs[PARTYID-1];

    for(int i=0; i<N; i++)
    {
        myTopicForMessage = "roundfunction8" + to_string(i+1);
        // add id party to the message
        string myMessage = s + "$" + sendBufs[i];
        send(myTopicForMessage, myMessage);
    }

    while(countRF8 < N - 1) {}

    for(int i=0; i<N; i++)
    {
        if(i != PARTYID-1) {
            recBufs[i] = vecRF8[i];
        }
    }

    countRF8 = 0;
}


Communication::~Communication() {
    MQTTClient_disconnect(m_client, 10000);
    MQTTClient_destroy(&m_client);
    delete[] topics;
}