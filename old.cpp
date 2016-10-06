///*
// * main.cpp
// *
// *  Created on: Aug 9, 2016
// *      Author: hila
// */
//
//#include <iostream>
//#include <NTL/GF2X.h>
//#include <NTL/mat_GF2.h>
//#include <stdint.h>
//#include<stdio.h>
//#include <NTL/GF2E.h>
//#include <bitset>
//#include "TFieldElement.h"
//#include "TField.h"
//#include "HIM.h"
//#include<NTL/GF2XFactoring.h>
//#include <sstream>
//#include "stdlib.h"
//
//#include "MQTTClient.h"
//#include <stdio.h>
//#include <iostream>
//#include <string>
//#include <iostream>
//#include <vector>
//#include <map>
//#include <iostream>
//#include <cassert>
//
//using namespace std;
//using namespace NTL;
////void testMatHIM();
////
////
//
//
//
//
//#define ADDRESS     "tcp://localhost:1883"
//#define COUNT_TOPIC 2
//
//#define PAYLOAD     "Hello World!"
//#define QOS         1
//#define TIMEOUT     10000L
//
//int PARTYID;
//bool flag_is_first = true;
//volatile MQTTClient_deliveryToken deliveredtoken;
////
////struct Message
////{
////   string data;
////   int sender_id;
////   string topic;
////};
////
////vector<Message> myMessages;
////std::map<std::string, int> m;
//// m["hello"] = 23;
//// // check if key is present
//// if (m.find("world") != m.end())
////   std::cout << "map contains key world!\n";
//// // retrieve
//// std::cout << m["hello"] << '\n';
//// std::map<std::string, int>::iterator i = m.find("hello");
//// assert(i != m.end());
//// std::cout << "Key: " << i->first << " Value: " << i->second << '\n';
////
//
//map<int, string> map_check;
//
//string Ps;
//bool flagPs = true;
//
//void delivered(void *context, MQTTClient_deliveryToken dt)
//{
//  //  printf("Message with token value %d delivery confirmed\n", dt);
//    deliveredtoken = dt;
//}
//
///**
// * the function handle when message arrive
// */
//int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
//{
//	string str_message = "";
//    int i;
//   // string p = "/0";
//    char* payloadptr;
//    //= (char*)p.c_str();
//    string s = to_string(PARTYID);
//    int len_of_message;
//
//    payloadptr = (char*)message->payload;
//    string str(payloadptr);
//
//    str = strtok(payloadptr, "**");
//
//    if(str == s) return 1;
//
//    // loop until client_id
//
//
//     int count = 0;
//     int div, mod;
//     div = PARTYID / 10;
//     mod = PARTYID % 10;
//     if(mod == 0)
//     {
//     	count = div;
//     } else {
//     	count = div + 1;
//     }
//     for(i=0; i<count+2;i++){
//     	*payloadptr++;
//     }
//    len_of_message= message->payloadlen - (count+2);
//     for(i=0; i<len_of_message; i++)
//     {
//         str_message += (*payloadptr++);
//     }
//
//    if(flagPs){
//    	Ps = str_message;
//    	flagPs = false;
//
//    } else {
//    	if (map_check.count(PARTYID) == 0) {
//    	map_check[PARTYID] = str_message;
//    	} else {
//    		cout << '\n';
//    		cout << '\n';
//    		cout << "this party already send a message" << '\n';
//    	}
//    }
//
//    printf("Message arrived\n");
//    printf("     topic: %s\n", topicName);
//    printf("   message: ");
//
//    cout << str_message;
//    putchar('\n');
//    // clean the pointer of the message
//   // message->payload = (char*)p.c_str();
//    MQTTClient_freeMessage(&message);
//    MQTTClient_free(topicName);
//
//    return 1;
//}
//
//void connlost(void *context, char *cause)
//{
//    printf("\nConnection lost\n");
//    printf("     cause: %s\n", cause);
//}
//
//void old_func(char** carray, size_t size)
//{
//    for(size_t i = 0; i < size; ++i)
//        std::cout << carray[i] << '\n';
//}
//
//int communication_broadcast(int party_id)
//{
//    MQTTClient m_client;
//    MQTTClient_connectOptions m_conn_opts = MQTTClient_connectOptions_initializer;
//    int m_rc;
//    int m_ch;
//
//    // messages
//
//    MQTTClient_message m_pubmsg = MQTTClient_message_initializer;
//    MQTTClient_deliveryToken m_token;
//
//    PARTYID = party_id;
//    // create client object
//
//    const char* c ="party" +PARTYID;
//    MQTTClient_create(&m_client, ADDRESS, c,
//        MQTTCLIENT_PERSISTENCE_NONE, NULL);
//    m_conn_opts.keepAliveInterval = 20;
//    m_conn_opts.cleansession = 1;
//
//    MQTTClient_setCallbacks(m_client, NULL, connlost, msgarrvd, delivered);
//
//    if ((m_rc = MQTTClient_connect(m_client, &m_conn_opts)) != MQTTCLIENT_SUCCESS)
//    {
//        printf("Failed to connect, return code %d\n", m_rc);
//        exit(EXIT_FAILURE);
//    }
//  //  printf("Subscribing to topic %s\nfor client %d using QoS%d\n\n"
//   //        "Press Q<EmqttAsyncCnter> to quit\n\n", TOPIC, PARTYID, QOS);
////    MQTTClient_subscribe(m_client, c, QOS);
//
//  //  string myTopic;
//  //  cout << "enter a topic" << '\n';
//
//   // vector<string> strings= {};
//
////    while(1) {
////   // 	client.subscribe(["/topic1", "/topic2", "/topic3"], [0,1,2]);
////    	getline(cin, myTopic);
////    	if(myTopic =="Q" || myTopic == "q") break;
////    	if(myTopic.size() > 0)
////    	{
////    		strings.push_back(myTopic);
////    	}
////    }
//
////
////    cout << strings.size() << '\n';
////
////     //print the vector
////    for (std::vector<string>::iterator it = strings.begin(); it != strings.end(); ++it)
////      std::cout << "**" << *it;
////    std::cout << '\n';
////
////    string myStr;
////     char* myCharArr;
////   char** arr = new char*[strings.size()];
////
////    int i = 0;
////    for (std::vector<string>::iterator it = strings.begin(); it != strings.end(); ++it) {
////    	myStr = *it;
////
////    	arr[i] = (char*)myStr.c_str();
////    	i++;
////    }
////
////    int arrQOS [strings.size()];
////    for(int i=0; i<strings.size(); i++){
////    	arrQOS[i] = 1;
////    }
//
//
//    //free topic
//    string s1 = "SHARE_Ps_VECTOR";
//    string s2 = "SHARE_Yjk_VECTOR";
//    char** topic = new char*[2];
//    topic[0] = (char*)s1.c_str();
//    topic[1] = (char*)s2.c_str();
//
//    int QQS_ARR[2] = {1, 1};
//    MQTTClient_subscribeMany(m_client, COUNT_TOPIC, topic, QQS_ARR);
//
//    int len;
//    string myMessage ="";
//    string myTopicForMessage="";
//    cout << "enter a message" << '\n';
//    while(1) {
//
////    	cin >> myMessage;
//    	myMessage = "";
//    	cout << "while"<<myMessage << '\n';
//    	getline(cin, myMessage);
//
//  //  	cout << "while"<<myMessage << '\n';
//
//    	if(myMessage == "") continue;
//
//    	if(myMessage =="Q" || myMessage == "q") break;
//
//    	len = myMessage.size();
//
//    	string s = to_string(PARTYID);
//
//    	myMessage = s + "**" + myMessage;
//
//
////    	cout << "                     myMessage               " << myMessage;
//
//    	if(flag_is_first)
//    	{
//    		myTopicForMessage = topic[0];
//    		flag_is_first = false;
//    	} else {
//    		myTopicForMessage = topic[1];
//    	}
//
//   //     cout << "the topic is" << myTopicForMessage << '\n';
//
//    	//m_pubmsg.payload = (void*)"";
//   // 	cout << "vv"<<m_pubmsg.payload  << '\n';
//        m_pubmsg.payload =(void*) myMessage.c_str();
//     //   cout << "ddd"<<m_pubmsg.payload  << '\n';
//        m_pubmsg.payloadlen = myMessage.size();
//        m_pubmsg.qos = QOS;
//        m_pubmsg.retained = 0;
//        deliveredtoken = 0;
//        MQTTClient_publishMessage(m_client, myTopicForMessage.c_str(), &m_pubmsg, &m_token);
////        printf("Waiting for publication of %s\n"
////                "on topic %s for client with ClientID: %d\n",
////                PAYLOAD, TOPIC, CLIENTID);
//     //   printf("Waiting for publication\n");
//
//    //    MQTTClient_receive()
//
//        while(deliveredtoken != m_token);
//
//       // cout << "publish !!" << '\n';
//        cout << "enter a message" << '\n';
//    }
//
//    MQTTClient_disconnect(m_client, 10000);
//    MQTTClient_destroy(&m_client);
//    return m_rc;
//}
//
//int main(int argc, char* argv[])
//{
//	if(argc != 2)
//	{
//		cout << "error";
//		return 0;
//	}
//	communication_broadcast(atoi(argv[1]));
//}
//
//
//
//
//
//
//
//
//
//
////
////
////
////
////
////
////
////
////
////
////
////
////int main()
////{
//////	GF2X irreduciblePolynomial = BuildSparseIrred_GF2X(8);
////	    //std::cout << "Leader irreducible Polynomial is " << irreduciblePolynomial << std::'\n';
////	//    GF2E::init(irreduciblePolynomial);
////
////	GF2X irreduciblePolynomial = BuildSparseIrred_GF2X(8);
////	cout << "Leader irreducible Polynomial is " << irreduciblePolynomial << '\n';
////	GF2E::init(irreduciblePolynomial);
////
////	GF2X p1 , p2, p3, p4, p5;
////	SetCoeff(p1, 0, 1);
////
////	SetCoeff(p2, 1, 1);
////	SetCoeff(p2, 0, 0);
////
////
////	cout << "num" << '\n';
////	int num;
////	cin >> num;
////	TParty* t1 = new TParty(num);
////	cout << "f";
////	//t1->f("l");
////
//////	while(1);
//////
//////	delete(t1);
//////
//////int b=0;
////////	for(int i=0; i<100000000000; i++)
////////	{
////////		b++;
////////	}
//////
//////	cout << "finish" << b << '\n';
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
//////	cout << "p1 : " << p1 << '\n';
//////	cout << "p2 : " << p2 << '\n';
//////
//////	div(p3, p1 ,p2);
//////	cout << "result by div : " << p3 << '\n';
//////
//////	InvMod(p3, p2, irreduciblePolynomial);
//////
//////	MulMod (p4, p1, p3, irreduciblePolynomial);
//////	cout << "result by inv and mul : " << p3 << '\n';
//////
//////	p5 = p1 / p2;
//////	cout << "result by operator / : " << p5 << '\n';
////
//////	p4 = p1 * p3;
//////	cout << "p4" << p3 << '\n';
//////	MulMod (p4, p1, p3, irreduciblePolynomial);
//////	cout << "p4" << p3 << '\n';
//////
//////	TFieldElement tf (p1);
//////	string p1s = tf.toString();
//////
//////	TFieldElement tf2 (p2);
//////	string p2s = tf2.toString();
//////
//////	cout << p1s << '\n';
//////	cout << p2s << '\n';
//////
//////	std::string some_string;
//////
//////	//cin >> some_string;
//////	getline(cin , some_string);
//////
//////	cout << "str" << some_string << '\n';
//////
//////	std::istringstream iss(some_string);
//////
//////
//////
//////
//////	//std::getline(iss, p4);
//////
//////	iss >> p4;
//////
//////
//////	cout << "p4" <<p4 << '\n';
//////
//////	p5 = p4 + p1;
//////
//////	cout << p5 << '\n';
//////
//////
//////
//////	TFieldElement tf3 (some_string);
//////	cout <<" f" << tf3.getElement() << '\n';
////
////
//////	testMatHIM();
//////
//////	GF2X p;
//////	SetCoeff(p, 8);
//////	SetCoeff(p, 4);
//////	SetCoeff(p, 3);
//////	SetCoeff(p, 1);
//////	SetCoeff(p, 0);
//////
//////	//cout << p.is_irreducible() <<'\n';
//////
//////	GF2E::init(p);
//////
//////	NTL::GF2X g1 (8);
//////	NTL::GF2X g2(8);
//////	NTL::GF2X g3;
//////	SetCoeff(g1,3,1);
//////	SetCoeff(g2,0,1);
//////	SetCoeff(g2,1,0);
//////	SetCoeff(g2,2,1);
//////	cout << g1 << g2 << g3;
//////	g3 = g1 / g2;
//////	cout << g1 << g2 << g3;
////}
////
////void testMatHIM()
////{
////	int n,m;
////	cout << "enter m" << '\n';
////	cin >> m;
////	cout << "enter n" << '\n';
////	cin >> n;
////	TField* t = TField::getInstance();
////	HIM* mat = new HIM(m,n);
////	TFieldElement** na = mat->CheckInitHIM();
////	cout << "HIM: " << '\n';
////	mat->Print();
////	TFieldElement* vec = new TFieldElement[m];
////
////	TFieldElement* result;
////
////	for (int i=0; i<m; i++)
////	{
////		vec[i].setPoly(NTL::GF2X(8));
////	}
//////	vec[0].setPoly(NTL::GF2X(1));
//////	vec[1].setPoly(NTL::GF2X(156));
////	SetCoeff(vec[0].getElement(),1,1);
////	SetCoeff(vec[1].getElement(),0,1);
////	cout << "vec:" << '\n';
////	for (int i=0; i<m; i++)
////	{
////		cout << vec[i].getElement() << '\n';
////	}
////	//vec[2].setPoly(NTL::GF2X(253));
//////	cout << "result of mult:" << '\n';
//////	result = mat->MatrixMult(vec);
//////	for (int i=0; i<m; i++)
//////	{
//////		cout << result[i].getElement() << '\n';
//////	}
//////	cout<< "finish";
////	// to do : calculate and print determinant
////}
////
////
