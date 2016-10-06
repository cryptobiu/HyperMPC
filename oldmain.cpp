//
//#include "stdlib.h"
//#include "../../workspace/paho/src/MQTTClient.h"
//#include "TParty.h"
//#include <stdio.h>
//#include <iostream>
//#include <string>
//#include <iostream>
//#include <vector>
//#include <map>
//#include <iostream>
//#include <cassert>
//#include <cstring>
//
//using namespace std;
//
//#define ADDRESS     "tcp://localhost:1883"
//#define COUNT_TOPIC 3
//
//#define PAYLOAD     "Hello World!"
//#define QOS         1
//#define TIMEOUT     10000L
//
//int PARTYID;
//bool send_result = false;
//bool flag_is_first = true;
//bool finish_sending_x_part = false;
//volatile MQTTClient_deliveryToken deliveredtoken;
//
//int countXRecieve =0;
//int N;
//map<int, string> map_check;
//map<int, string> map_vecRecX;
//vector<int> vecConn;
//vector<TFieldElement> vecRec;
//vector<TFieldElement*> vecResult;
//vector<TFieldElement*> vecRecX;
//bool flag_send_vecResult = false;
//bool flag_is_happy = true;
//
//string Ps;
//bool flagPs = true;
//
//void
//Broadcaster(string &myMessage, int is_Ps, const MQTTClient &m_client, char *const *topic, string &myTopicForMessage,
//            MQTTClient_message &m_pubmsg, MQTTClient_deliveryToken &m_token, string &s);
//
//void ConnectionToServer(const MQTTClient &m_client, const string &s, string &myTopicForMessage, MQTTClient_message &m_pubmsg,
//                        MQTTClient_deliveryToken &m_token);
//
//void ConnectHandler(const char *topicName, MQTTClient_message *&message, const string &str);
//
//void SendXVectorToAllParties(string &myMessage, MQTTClient const &m_client, char *const *topic,
//                             string &myTopicForMessage, MQTTClient_message &m_pubmsg,
//                             MQTTClient_deliveryToken &m_token, string &s);
//
//void SendYVectorResult(string &myMessage, MQTTClient const &m_client, char *const *topic, string &myTopicForMessage,
//                       MQTTClient_message &m_pubmsg, MQTTClient_deliveryToken &m_token, const string &s, vector<string> &buffers);
//
//void SendTheResult(string &myMessage, MQTTClient const &m_client, string &myTopicForMessage, MQTTClient_message &m_pubmsg,
//           MQTTClient_deliveryToken &m_token, const string &s, const vector<string> &buffers);
//
//void delivered(void *context, MQTTClient_deliveryToken dt)
//{
//    deliveredtoken = dt;
//}
//
//
//void split(const string &s, char delim, vector<string> &elems) {
//    stringstream ss;
//    ss.str(s);
//    string item;
//    while (getline(ss, item, delim)) {
//        elems.push_back(item);
//    }
//}
//
//
//vector<string> split(const string &s, char delim) {
//    vector<string> elems;
//    split(s, delim, elems);
//    return elems;
//}
//
//string convertToString(vector<TFieldElement*> t)
//{
//    string str = "";
//    for (int i=0; i < N - 1; i++)
//    {
//        str += t[i]->toString() + "*";
//    }
//    str += t[N-1]->toString();
//    return str;
//}
//
//void addElement(string str)
//{
//    // check if we can remove the pointer
//    TFieldElement* n = new TFieldElement(str);
//    vecRec.push_back(*n);
//}
//
//string getCurrectMessage(string str) {
//    vector<string> arr;
//
//    char tab[1024];
//    strcpy(tab, str.c_str());
//    string str1;
//    string nstr;
//    for (int i=0; i < str.size(); i++)
//    {
//        if (tab[i] == '*') {
//            nstr = str1;
//            arr.push_back(nstr);
//            str1 = "";
//        }
//        else {
//            str1 += str[i];
//        }
//    }
//    nstr = str1;
//    arr.push_back(nstr);
//    nstr = arr[PARTYID - 1];
//    return nstr;
//}
//
//
//string getCurrectMessageById(string str, int i) {
//    vector<string> arr;
//
//    char tab[1024];
//    strcpy(tab, str.c_str());
//    string str1;
//    string nstr;
//    for (int i=0; i < str.size(); i++)
//    {
//        if (tab[i] == '*') {
//            nstr = str1;
//            arr.push_back(nstr);
//            str1 = "";
//        }
//        else {
//            str1 += str[i];
//        }
//    }
//    nstr = str1;
//    arr.push_back(nstr);
//    nstr = arr[i - 1];
//    return nstr;
//}
//
//string getXVector(string str, int pid)
//{
//   // vector<TFieldElement*> vecRecX
//    vector<string> arr = {};
//    arr = split(str, '*');
//    string str1;
//    string str2 ="";
//    TFieldElement* n;
//    for (int i = 0; i < arr.size() ; i++) {
//        cout << i << ".." << arr[i] << '\n';
//        str1 = arr[i];
//        str2 += str1;
//        n = new TFieldElement(str1);
//        vecRecX.push_back(n);
//        cout << "pid L             " << pid << '\n';
//    }
//    // padding zero
//    for(int i = arr.size(); i < N ; i++)
//    {
//        n = new TFieldElement("[]");
//        vecRecX.push_back(n);
//    }
//    map_vecRecX[pid] = str2;
//    return str2;
//}
//
//void calculate()
//{
//    HIM matrix(N,N);
//    matrix.InitHIM();
////    vecRecX[1];
////    vecRecX[2];
//    vecResult = matrix.MatrixMult(vecRecX);
//
//    flag_send_vecResult = true;
//
//}
//
///**
// * the function handle when message arrive
// */
//int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
//{
//    string topic(topicName);
//    string str_message = "";
//    int i;
//    char* payloadptr;
//    string s = to_string(PARTYID);
//    int len_of_message;
//
//
//
//    payloadptr = (char*)message->payload;
//    string str(payloadptr);
//
//    str = strtok(payloadptr, "$");
//
//    int pid = atoi(str.c_str());
//
//
//    if(str == s) return 1;
//
//    // loop until client_id
//    if(topic == "CONNECT")
//    {
//        ConnectHandler(topicName, message, str);
//        MQTTClient_freeMessage(&message);
//        MQTTClient_free(topicName);
//        return 1;
//    }
//
//    int count = 0;
//    int div, mod;
//    div = PARTYID / 10;
//    mod = PARTYID % 10;
//    if(mod == 0)
//    {
//        count = div;
//    } else {
//        count = div + 1;
//    }
//    for(i=0; i<count+1;i++){
//        *payloadptr++;
//    }
//    len_of_message= message->payloadlen - (count+1);
//    for(i=0; i<len_of_message; i++)
//    {
//        str_message += (*payloadptr++);
//    }
//
//    if(flagPs){
//        Ps = str_message;
//        flagPs = false;
//
//    } else {
//        if (map_check.count(PARTYID) == 0) {
//            map_check[PARTYID] = str_message;
//        } else {
//            cout << '\n';
//            cout << '\n';
//            cout << "this party already send a message" << '\n';
//        }
//    }
//
//    //   if(topic == "SHARE_Yjk_VECTOR") {
//    if(topic.find("SHARE_Yjk_VECTOR") != std::string::npos) {
//        //	str_message = getCurrectMessage(str_message);
//        // cout << "you right" << '\n';
//        addElement(str_message);
//    } else {
//        // only when all x es recived we can calculate every x
//        countXRecieve++;
//
//        str_message = getXVector(str_message, pid);
//        // add x (=str_message) to vector
//        // it happend in getXVector.
//        // vecRECx Tfieldelement
//
//        calculate();
//        if(countXRecieve == N-1) {
//            finish_sending_x_part = true;
//            cout << "f f f f f f f finish" << '\n';
//        }
//    }
//
//    printf("Message arrived\n");
//    printf("     topic: %s\n", topicName);
//    printf("   message: ");
//
//    cout << str_message;
//    putchar('\n');
//    // clean the pointer of the message
//    MQTTClient_freeMessage(&message);
//    MQTTClient_free(topicName);
//
//    return 1;
//}
//
//void ConnectHandler(const char *topicName, MQTTClient_message *&message, const string &str) {
//    int elem = stoi(str);
//    bool flag = true;
//    for(int i=0; i<vecConn.size(); i++)
//    {
//        if(vecConn[i] == elem)
//        {
//            flag = false;
//            break;
//        }
//    }
//    if (flag == true) {
//
//        printf("Message arrived\n");
//        printf("   topic: %s\n", topicName);
//
//        printf("   message: ");
//
//        cout << elem << '\n';
//        vecConn.push_back(elem);
//    }
//
//}
//
//void connlost(void *context, char *cause)
//{
//    printf("\nConnection lost\n");
//    printf("     cause: %s\n", cause);
//}
////
////void old_func(char** carray, size_t size)
////{
////    for(size_t i = 0; i < size; ++i)
////        std::cout << carray[i] << '\n';
////}
//
//int communication_broadcast(int party_id, string myMessage, int is_Ps)
//{
//    MQTTClient m_client;
//    static MQTTClient_connectOptions m_conn_opts = MQTTClient_connectOptions_initializer;
//    int m_rc;
//    int m_ch;
//
//    int len;
//    string myTopicForMessage="";
//
//    // messages
//    MQTTClient_message m_pubmsg = MQTTClient_message_initializer;
//    MQTTClient_deliveryToken m_token;
//
//    PARTYID = party_id;
//
//    // create client object
//
//    const char* c ="party" +PARTYID;
//    MQTTClient_create(&m_client, ADDRESS, c,
//                      MQTTCLIENT_PERSISTENCE_NONE, NULL);
//
//    m_conn_opts.keepAliveInterval = 60;
//    m_conn_opts.cleansession = 1;
//
//
//    // msgarrvd - handle function
//    MQTTClient_setCallbacks(m_client, NULL, connlost, msgarrvd, delivered);
//
//    // try to connect to server
//    if ((m_rc = MQTTClient_connect(m_client, &m_conn_opts)) != MQTTCLIENT_SUCCESS)
//    {
//        printf("Failed to connect, return code %d\n", m_rc);
//        exit(EXIT_FAILURE);
//    }
//
//    //free topic
//    // create topics
//    string s1 = "SHARE_Ps_VECTOR";
//    string s2 = "SHARE_Yjk_VECTOR"+to_string(PARTYID);
//    string s3 = "CONNECT";
//    char** topic = new char*[3];
//    topic[0] = (char*)s1.c_str();
//    topic[1] = (char*)s2.c_str();
//    topic[2] = (char*)s3.c_str();
//    int QQS_ARR[3] = {1, 1, 1};
//
//    // update the topics
//    MQTTClient_subscribeMany(m_client,  3, topic, QQS_ARR);
//
//    string s = to_string(PARTYID);
//
//    ConnectionToServer(m_client, s, myTopicForMessage, m_pubmsg, m_token);
//
//    Broadcaster(myMessage, is_Ps, m_client, topic, myTopicForMessage, m_pubmsg, m_token, s);
//
//    MQTTClient_disconnect(m_client, 10000);
//    MQTTClient_destroy(&m_client);
//    return m_rc;
//}
//
//void ConnectionToServer(const MQTTClient &m_client, const string &s, string &myTopicForMessage, MQTTClient_message &m_pubmsg,
//                        MQTTClient_deliveryToken &m_token) {// connect
//
//    string strMessage = s + "$" + s;
//
//    myTopicForMessage = "CONNECT";
//
//    // update the details of message
//    m_pubmsg.payload =(void*) strMessage.c_str();
//    m_pubmsg.payloadlen = strMessage.size();
//    m_pubmsg.qos = QOS;
//    m_pubmsg.retained = 0;
//    deliveredtoken = 0;
//
//
//    // publish the message to all parties
//    MQTTClient_publishMessage(m_client, myTopicForMessage.c_str(), &m_pubmsg, &m_token);
//
//    // wait until all parties connect
//
//    while(vecConn.size() < N-1){
//        MQTTClient_publishMessage(m_client, myTopicForMessage.c_str(), &m_pubmsg, &m_token);
//    }
//
//    MQTTClient_publishMessage(m_client, myTopicForMessage.c_str(), &m_pubmsg, &m_token);
//
//   // vecConn.clear();
//
//    cout << "Let's start!" << '\n';
//}
//
//void
//Broadcaster(string &myMessage, int is_Ps, const MQTTClient &m_client, char *const *topic, string &myTopicForMessage,
//            MQTTClient_message &m_pubmsg, MQTTClient_deliveryToken &m_token, string &s) {
//
//    vector<string> buffers;
//    string temp = myMessage;
// //   if(is_Ps == 0) {
//        SendXVectorToAllParties(myMessage, m_client, topic, myTopicForMessage, m_pubmsg, m_token, s);
//        getXVector(temp, PARTYID);
//        calculate();
//        SendYVectorResult(myMessage, m_client, topic, myTopicForMessage, m_pubmsg, m_token, s, buffers);
//
//        SendTheResult(myMessage, m_client, myTopicForMessage, m_pubmsg, m_token, s,
//              buffers);
//        while (vecRec.size() < N - 1) {}
// //   }
//
//    while (finish_sending_x_part == false) {}
//
//    for(int i=1; i<N+1; i++) {
//       // vector<TFieldElement*> tempvec = map_vecRecX[i];
//        cout << "pppppppppppppppppppppppppppp             "<<map_vecRecX[i] << '\n';
//    }
//
//    //TFieldElement tempelem =
//
// //   if(is_Ps == 1) {
//        // waiting until finish calculate
//        SendYVectorResult(myMessage, m_client, topic, myTopicForMessage, m_pubmsg, m_token, s, buffers);
//        cout << "size" <<vecRec.size() << '\n';
//
//        SendTheResult(myMessage, m_client, myTopicForMessage, m_pubmsg, m_token, s,
//              buffers);
//        while (vecRec.size() < N - 2 || vecRec.size() == 0) {}
//        cout << "size" <<vecRec.size() << '\n';
// //   }
//
//    cout << "finish broadcast" << '\n';
//
//    if(vecRec.size() == 0)
//    {
//        cout << "there is no result" << '\n';
//    }
//    else {
//
//        for (int i = 0; i < vecRec.size() - 1; i++) {
//            if (vecRec[i].toString() != vecRec[i + 1].toString()) {
//                flag_is_happy = false;
//            }
//        }
//    }
//    cout << "flag_is_happy" << flag_is_happy << '\n';
//
//}
//
//void SendYVectorResult(string &myMessage, MQTTClient const &m_client, char *const *topic, string &myTopicForMessage,
//                       MQTTClient_message &m_pubmsg, MQTTClient_deliveryToken &m_token, const string &s, vector<string> &buffers) {
//
//    while (flag_send_vecResult == false) {}
//
//    myMessage = convertToString(vecResult);
//
//    string temp = myMessage;
//
//  //  string strstr;
//    for(int i=1; i<N+1; i++) {
//   //     cout << "             temp : " << temp << '\n';
//        string strstr = getCurrectMessageById(temp, i);
//        buffers.push_back(strstr);
//
//    }
//
//
//}
//
//void SendTheResult(string &myMessage, MQTTClient const &m_client, string &myTopicForMessage, MQTTClient_message &m_pubmsg,
//           MQTTClient_deliveryToken &m_token, const string &s, const vector<string> &buffers) {// buffers[i] = the buffer of party i+1
//    // buffers[0] = party 1
//    for(int i=0; i<buffers.size(); i++)
//    {
//        myMessage = s + "$" + buffers[i];
//
//        myTopicForMessage = "SHARE_Yjk_VECTOR" + to_string(i+1);
//
//        //    cout << "my topic     " << myTopicForMessage << '\n';
//        m_pubmsg.payload = (void *) myMessage.c_str();
//
//        m_pubmsg.payloadlen = myMessage.size();
//        m_pubmsg.qos = QOS;
//        m_pubmsg.retained = 0;
//        deliveredtoken = 0;
//
//        MQTTClient_publishMessage(m_client, myTopicForMessage.c_str(), &m_pubmsg, &m_token);
//
//        while (deliveredtoken != m_token) {};
//        cout << "****** " << buffers[i] << '\n';
//    }
//}
//
//void SendXVectorToAllParties(string &myMessage, MQTTClient const &m_client, char *const *topic,
//                             string &myTopicForMessage, MQTTClient_message &m_pubmsg,
//                             MQTTClient_deliveryToken &m_token, string &s) {
//    myTopicForMessage = topic[0];
//    flag_is_first = false;
//    // add id party to the message
//    myMessage = s + "$" + myMessage;
//
//    // update the details of message
//    m_pubmsg.payload = (void *) myMessage.c_str();
//    m_pubmsg.payloadlen = myMessage.size();
//    m_pubmsg.qos = QOS;
//    m_pubmsg.retained = 0;
//    deliveredtoken = 0;
//
//    // publish the message to all parties
//    MQTTClient_publishMessage(m_client, myTopicForMessage.c_str(), &m_pubmsg, &m_token);
//
//    // waiting until the message send
//    while (deliveredtoken != m_token) {};
//}
//
///*
// * argv[1] = party id
// * argv[2] = number of parties
// * argv[3] = Ps = 0 / other = 1
// */
//int main(int argc, char* argv[])
//{
//    if(argc != 4)
//    {
//        cout << "error";
//        return 0;
//    }
//
//    N = atoi(argv[2]);
//    int partypid = atoi(argv[1]);
//    int is_Ps = atoi(argv[3]);
//
//    TParty t(partypid);
//
//    GF2X irreduciblePolynomial = BuildSparseIrred_GF2X(8);
//    GF2E::init(irreduciblePolynomial);
//
//    GF2X p1, p2, p3, p4;
//    SetCoeff(p1, 5);
//    SetCoeff(p1, 2);
//    SetCoeff(p2, 3);
//    SetCoeff(p3, 1);
//    SetCoeff(p4, 3);
//    SetCoeff(p4, 1);
//
//
//    cout << p1 << '\n';
//    cout << p2 << '\n';
//
//    TFieldElement e1(p1);
//
//    TFieldElement e2(p2);
//
//    TFieldElement e3(p3);
//
//    TFieldElement e4(p4);
//
//    string x = e1.toString() +"*" +e2.toString();
//
//   // string x = e1.toString() +"*" +e2.toString() + "*"  +e3.toString() + "*" + e4.toString();
//    cout << x << '\n';
//
//    // start broadcast
//    communication_broadcast(partypid, x, is_Ps);
//
//
//}