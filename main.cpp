
#include "stdlib.h"
#include "../../workspace/paho/src/MQTTClient.h"
#include "TParty.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <iostream>
#include <cassert>
#include <cstring>

using namespace std;

#define ADDRESS     "tcp://localhost:1883"
#define COUNT_TOPIC 3

#define PAYLOAD     "Hello World!"
#define QOS         1
#define TIMEOUT     10000L

int PARTYID;
//bool finish_sending_x_part = false;
volatile MQTTClient_deliveryToken deliveredtoken;

int countXRecieve =0;
int countYRecieve =0;
int N, T;
vector<int> vecConn;
vector<TFieldElement*> vecRecX;
vector<string> vec;
vector<string> vecRecForCheck;



void ConnectHandler(const char *topicName, MQTTClient_message *&message, const string &str) {
    int elem = stoi(str);
    bool flag = true;
    for(int i=0; i<vecConn.size(); i++)
    {
        if(vecConn[i] == elem)
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
        vecConn.push_back(elem);
    }

}
/**
 * the function handle when message arrive
 */
int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message) {
    string topic(topicName);
    string str_message = "";
    int i;
    char *payloadptr;
    string s = to_string(PARTYID);
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
    div = PARTYID / 10;
    mod = PARTYID % 10;
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
        vecRecForCheck[pid-1] = str_message;
        countYRecieve++;
    } else {
        // only when all x es recived we can calculate every x
        vec[pid-1] = str_message;
        countXRecieve++;
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
    deliveredtoken = dt;
}

void split(const string &s, char delim, vector<string> &elems) {
    stringstream ss;
    ss.str(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
}


vector<string> split(const string &s, char delim) {
    vector<string> elems;
    split(s, delim, elems);
    return elems;
}

void ConnectionToServer(const MQTTClient &m_client, const string &s, string &myTopicForMessage, MQTTClient_message &m_pubmsg,
                        MQTTClient_deliveryToken &m_token) {// connect

    string strMessage = s + "$" + s;

    myTopicForMessage = "CONNECT";

    // update the details of message
    m_pubmsg.payload =(void*) strMessage.c_str();
    m_pubmsg.payloadlen = strMessage.size();
    m_pubmsg.qos = QOS;
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

void SendD(string &myMessage, MQTTClient const &m_client, string &myTopicForMessage, MQTTClient_message &m_pubmsg,
           MQTTClient_deliveryToken &m_token, const string &s, const vector<string> &buffers) {// buffers[i] = the buffer of party i+1
    // buffers[0] = party 1
    for(int i=0; i<buffers.size(); i++)
    {
        myMessage = s + "$" + buffers[i];

        myTopicForMessage = "SHARE_Yjk_VECTOR" + to_string(i + 1);

        m_pubmsg.payload = (void *) myMessage.c_str();

        m_pubmsg.payloadlen = myMessage.size();
        m_pubmsg.qos = QOS;
        m_pubmsg.retained = 0;
        deliveredtoken = 0;

        MQTTClient_publishMessage(m_client, myTopicForMessage.c_str(), &m_pubmsg, &m_token);

        while (deliveredtoken != m_token) {};
    }
}

void SendXVectorToAllParties(string &myMessage, MQTTClient const &m_client, char *const *topic,
                             string &myTopicForMessage, MQTTClient_message &m_pubmsg,
                             MQTTClient_deliveryToken &m_token, string &s) {
    myTopicForMessage = topic[0];

    // add id party to the message
    myMessage = s + "$" + myMessage;

    // update the details of message
    m_pubmsg.payload = (void *) myMessage.c_str();
    m_pubmsg.payloadlen = myMessage.size();
    m_pubmsg.qos = QOS;
    m_pubmsg.retained = 0;
    deliveredtoken = 0;

    // publish the message to all parties
    MQTTClient_publishMessage(m_client, myTopicForMessage.c_str(), &m_pubmsg, &m_token);

    // waiting until the message send
    while (deliveredtoken != m_token) {};
}

void Broadcaster(string &myMessage, const MQTTClient &m_client, char *const *topic, string &myTopicForMessage,
                 MQTTClient_message &m_pubmsg, MQTTClient_deliveryToken &m_token, string &s) {

    string temp = myMessage;
    vec[PARTYID-1] = temp;
    SendXVectorToAllParties(myMessage, m_client, topic, myTopicForMessage, m_pubmsg, m_token, s);

    // Round Function
    while (countXRecieve < N-1) {}

    for(int i=0; i<vec.size(); i++)
    {

        cout << "this is: "<< i<< "  " << vec[i] << endl;
    }
}

void getXVector(string str, int pid)
{
    // vector<TFieldElement*> vecRecX
    vector<string> arr = {};
    arr = split(str, '*');
    string str1;
    string str2 ="";
    TFieldElement* n;
    for (int i = 0; i < arr.size() ; i++) {
        str1 = arr[i];
        str2 += str1;
        n = new TFieldElement(str1);
        vecRecX.push_back(n);
    }

}
void ConcatenateEverything(vector<string> &buffers, int &no_buckets)
{
    vector<TFieldElement*> X1;
    vector<TFieldElement*> Y1;

    X1.resize(N);
    Y1.resize(N);
    HIM matrix(N,N);
    matrix.InitHIM();

    for(int i=0; i<N; i++)
    {
        getXVector(vec[i],i+1);
    }

    // total number of values
    int count = vecRecX.size();

    // nr of buckets
    no_buckets = count / (N-T) + 1;
    TFieldElement* n;

    for(int k = 0; k < no_buckets; k++)
    {
        for(int i = 0; i < N; i++)
        {
            if((i < N-T) && (k*(N-T)+i < count))
            {
                X1[i]=vecRecX[i];
            }
                // padding zero
            else
            {
                n = new TFieldElement("[]");
                X1[i]=n;
            }
        }
        matrix.MatrixMult3(X1, Y1);
        cout << "Y1[i]" << endl;
        for(int i = 0; i < N; i++) {
            cout << i << endl;
            cout << Y1[i]->toString() << endl;
        }

        for(int i = 0; i < N; i++) {
            buffers[i] += Y1[i]->toString() + "*";
        }
        X1.clear();
        Y1.clear();
        X1.resize(N);
        Y1.resize(N);
    }

    // no cheating: all parties have same y1
    // ‘‘Reconstruct’’ values towards some party (‘‘reconstruct’’ with degree 0)

    // buffers[0] --> the buffer of party with num id is 1


    cout << "Result" << endl;
    for(int i = 0; i < N; i++) {
        cout << buffers[i] << endl;
    }

}

bool CheckIfHappyOrCry(int &no_buckets)
{
    while (countYRecieve < N-1) {}

    //  vector<string> arr = {};

//    for(int k=0; k < no_buckets ; k++) {
//        arr = split(vecRecForCheck[k], '*');
//
//    }
//
//    FOR k := 0 TO no_buckets-1 DO
//            BEGIN
//    Read(RecBufs2[0],x);
//    FOR i := 1 TO N-1 DO
//            BEGIN
//    Read(RecBufs2[i],y);
//    IF x <> y THEN Halt;
//

    string temp;
    for(int i=0; i<vecRecForCheck.size(); i++) {

        vector<string> arr = {};
        arr = split(vecRecForCheck[i], '*');
        if(arr.size() > 0) {
            temp = arr[0];
            for (int i = 1; i < arr.size() - 1; i++) {
                //cout << "this is: " << i << "  " << arr[i] << endl;
                if(temp != arr[i])
                {
                    return false;
                }
            }
        }
    }

    return true;

}

bool broadcast(int party_id, string myMessage ,MQTTClient &m_client, MQTTClient_message &m_pubmsg,
                  MQTTClient_deliveryToken &m_token, char** &topic, int &m_rc)
{
    int no_buckets;
    vector<string> buffers;
    buffers.resize(N);
    vecRecForCheck.resize(N);
    bool bit_is_happy;
    string myTopicForMessage="";

    string s = to_string(PARTYID);

    ConnectionToServer(m_client, s, myTopicForMessage, m_pubmsg, m_token);

    Broadcaster(myMessage, m_client, topic, myTopicForMessage, m_pubmsg, m_token, s);

    ConcatenateEverything(buffers, no_buckets);

    SendD(myMessage, m_client, myTopicForMessage, m_pubmsg, m_token, s, buffers);

    bit_is_happy = CheckIfHappyOrCry(no_buckets);


    return m_rc;
}

int main(int argc, char* argv[])
{

    if(argc != 3)
    {
        cout << "error";
        return 0;
    }

    N = atoi(argv[2]);
    T = N/3 -1;
    vec.resize(N);
    int partypid = atoi(argv[1]);
    //int is_Ps = atoi(argv[3]);

    TParty t(partypid);

    GF2X irreduciblePolynomial = BuildSparseIrred_GF2X(8);
    GF2E::init(irreduciblePolynomial);

    GF2X p1, p2, p3, p4, p5, p6, p7, p8;
    SetCoeff(p1, 5);
    SetCoeff(p1, 2);
    SetCoeff(p2, 3);
    SetCoeff(p3, 1);
    SetCoeff(p4, 3);
    SetCoeff(p4, 1);
    SetCoeff(p5, 1);
    SetCoeff(p5, 6);
    SetCoeff(p5, 4);
    SetCoeff(p6, 2);
    SetCoeff(p7, 3);
    SetCoeff(p7, 1);
    SetCoeff(p7, 8);
    SetCoeff(p8, 1);
    SetCoeff(p6, 1);
    SetCoeff(p7, 6);
    SetCoeff(p8, 4);

    cout << p1 << endl;
    cout << p2 << endl;
    cout << p3 << endl;
    cout << p4 << endl;
    cout << p5 << endl;
    cout << p6 << endl;
    cout << p7 << endl;
    cout << p8 << endl;

    TFieldElement e1(p1);

    TFieldElement e2(p2);

    TFieldElement e3(p3);

    TFieldElement e4(p4);

    TFieldElement e5(p5);

    TFieldElement e6(p5);

    TFieldElement e7(p5);

    TFieldElement e8(p5);

    TFieldElement e9(p5);

    string x = e2.toString() +"*" +e1.toString()+ "*" + e4.toString();
    //string x = e7.toString() +"*" +e2.toString() + "*" + e4.toString() + "*" + e8.toString();
    // string x =  e4.toString() + "*" + e5.toString();
    //string x = e1.toString() +"*" +e8.toString() + "*"  +e3.toString() + "*" + e7.toString() + "*" + e6.toString();
    //  string x = e1.toString() +"*" +e2.toString() + "*"  +e3.toString() + "*" + e4.toString() + "*" + e5.toString();
    // string x = e1.toString();

    //string x = e8.toString()+"*" +e2.toString();

    cout << x << endl;

    // ^^^^^^^^^^^^^^6

    PARTYID = atoi(argv[1]);

    ////

    MQTTClient m_client;
    static MQTTClient_connectOptions m_conn_opts = MQTTClient_connectOptions_initializer;
    int m_rc;
    int m_ch;
    char** topic = new char*[3];
    int len;
    MQTTClient_message m_pubmsg;
    MQTTClient_deliveryToken m_token;

    // messages
    m_pubmsg = MQTTClient_message_initializer;

    // create client object

    const char* c ="party" +PARTYID;
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
    string s2 = "SHARE_Yjk_VECTOR"+to_string(PARTYID);
    string s3 = "CONNECT";

    topic[0] = (char*)s1.c_str();
    topic[1] = (char*)s2.c_str();
    topic[2] = (char*)s3.c_str();
    int QQS_ARR[3] = {1, 1, 1};



    // update the topics
    MQTTClient_subscribeMany(m_client,  3, topic, QQS_ARR);


    //while(1){}
    // start broadcast
    bool flag;

    flag = broadcast(partypid, x, m_client, m_pubmsg, m_token, topic, m_rc);

    // cheating - halt
    if(flag == false) {

        cout << "cheating!!!" << endl;
        MQTTClient_disconnect(m_client, 10000);
        MQTTClient_destroy(&m_client);
    }
    else{
        cout << "no cheating!!!" << endl;
    }

}