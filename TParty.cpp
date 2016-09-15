///*
// * Party.cpp
// *
// *  Created on: Aug 16, 2016
// *      Author: hila
// */
//
//#include "TParty.h"
//
//
//#define ADDRESS     "tcp://localhost:1883"
//#define COUNT_TOPIC 2
//
//#define PAYLOAD     "Hello World!"
//#define QOS         1
//#define TIMEOUT     10000L
//
//
//TParty::TParty(int num) {
//	m_is_happy = true;
//
//	int m_rc;
//
//	//reference to client
//	//MQTTClient& m_client = client;
//
//    MQTTClient_connectOptions m_conn_opts = MQTTClient_connectOptions_initializer;
//
//    int m_ch;
//
//    // messages
//
//    m_pubmsg = MQTTClient_message_initializer;
//    cout << "b1";
//    m_pid = num;
//    PARTYID = num;
//    // create client object
//
//    const char* c ="party" +PARTYID;
//    MQTTClient_create(&m_client, ADDRESS, c,
//        MQTTCLIENT_PERSISTENCE_NONE, NULL);
//    m_conn_opts.keepAliveInterval = 20;
//    m_conn_opts.cleansession = 1;
//    cout << "b2";
//
//    MQTTClient_setCallbacks(m_client, NULL, connlost, msgarrvd, delivered);
//
//
//
//    if ((m_rc = MQTTClient_connect(m_client, &m_conn_opts)) != MQTTCLIENT_SUCCESS)
//    {
//        printf("Failed to connect, return code %d\n", m_rc);
//        exit(EXIT_FAILURE);
//    }
//
//
//    //free topic
//    // ALL\SHAREkey..
//    string s1 = "SHARE_Ps_VECTOR";
//    string s2 = "SHARE_Yjk_VECTOR";
//    m_topic = new char*[2];
//    m_topic[0] = (char*)s1.c_str();
//    m_topic[1] = (char*)s2.c_str();
//    cout << "b3";
//
//    int QQS_ARR[2] = {1, 1};
//    MQTTClient_subscribeMany(m_client, COUNT_TOPIC, m_topic, QQS_ARR);
//
//   cout << "b4";
//
//}
//
///**
// * input:
// * n - size of alpha
// */
//TFieldElement TParty::Interpolate (TFieldElement* alpha, int n, TFieldElement* x)
//{
//	TFieldElement y;
//	TFieldElement* beta = new TFieldElement[1];
//	beta[0] = *(TField::getInstance()->GetZero());
//	HIM* matrix = new HIM(n, 1);
//	matrix->InitHIMByVectors(alpha, beta);
//	y = *(matrix->MatrixMult(x));
//	return y;
//}
//
//void TParty::f(string myMessage){
//
//	cout << m_pid;
//
//}
//
////TFieldElement** vector_of_x
//void TParty::sendVectorToParties(string	myMessage) {
//
//	    int m_rc;
//	    int len;
//
//	    cout << "12";
//	//    string	myMessage = "";
//	    string myTopicForMessage="";
////	    cout << "enter a message" << endl;
////
//
//	//    	cout << "while"<<myMessage << endl;
////	    cout << "11";
////	    //	getline(cin, myMessage);
////	    	cin >> myMessage;
//	    	 cout << "12";
//	    	if(myMessage == "") return;
//
//	//    	/if(myMessage =="Q" || myMessage == "q") return;
//
//	    	len = myMessage.size();
//	    	 cout << "13";
//	    	string s = to_string(m_pid);
//
//	    	myMessage = s + "**" + myMessage;
//	    	 cout << "13";
//	    	if(flag_is_first)
//	    	{
//	    		myTopicForMessage = m_topic[0];
//	    		flag_is_first = false;
//	    	} else {
//	    		myTopicForMessage = m_topic[1];
//	    	}
//	        m_pubmsg.payload =(void*) myMessage.c_str();
//	        m_pubmsg.payloadlen = myMessage.size();
//	        m_pubmsg.qos = QOS;
//	        m_pubmsg.retained = 0;
//	        deliveredtoken = 0;
//	        MQTTClient_publishMessage(m_client, myTopicForMessage.c_str(), &m_pubmsg, &m_token);
//
//	        cout << "l";
//	        while(deliveredtoken != m_token);
//
//	//        cout << "enter a message" << endl;
//	//    }
//
//
//
//
//
//
//}
//
//TParty::~TParty() {
////	disconnect(m_client);
////	cout << "dF" << endl;
//////
//////    MQTTClient_disconnect(m_client, 10000);
//////    MQTTClient_destroy(&m_client);
//	MQTTClient_disconnect(m_client, 10000);
//	MQTTClient_destroy(&m_client);
//	cout << "dF" << endl;
//
//}
//





/*
 * Party.cpp
 *
 *  Created on: Aug 16, 2016
 *      Author: hila
 */

#include "TParty.h"


#define ADDRESS     "tcp://localhost:1883"
#define COUNT_TOPIC 2

#define PAYLOAD     "Hello World!"
#define QOS         1
#define TIMEOUT     10000L


TParty::TParty(int num) {
	m_is_happy = true;
	m_pid = num;
}

/**
 * input:
 * n - size of alpha
 */
//TFieldElement TParty::Interpolate (vector<TFieldElement*> alpha, int n, vector<TFieldElement*> x)
//{
//	TFieldElement y;
//	vector<TFieldElement*> beta;
//	/////////////////////////////////
//	beta.resize(1);
//	beta[0] = (TField::getInstance()->GetZero());
//	HIM* matrix = new HIM(n, 1);
//	matrix->InitHIMByVectors(alpha, beta);
////	y = *(matrix->MatrixMult2(x));
//	return y;
//}

void TParty::f(string myMessage){

	cout << m_pid;

}

//TFieldElement** vector_of_x
void TParty::sendVectorToParties(string	myMessage) {

}

TParty::~TParty() {}

































