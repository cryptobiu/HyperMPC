//
//#include "ProtocolBroadcast.h"
//
//using namespace std;
//
////#define ADDRESS     "tcp://localhost:1883"
////#define COUNT_TOPIC 2
////
////#define PAYLOAD     "Hello World!"
////#define QOS         1
////#define TIMEOUT     10000L
//////
////int PARTYID;
//////bool flag_is_first = true;
////volatile MQTTClient_deliveryToken deliveredtoken;
////
//map<int, string> map_check;
//
//string Ps;
//bool flagPs = true;
//
//
//volatile MQTTClient_deliveryToken deliveredtoken;
//int PARTYID;
//bool flag_is_first = true;
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
//    		cout << endl;
//    		cout << endl;
//    		cout << "this party already send a message" << endl;
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
////
////void old_func(char** carray, size_t size)
////{
////    for(size_t i = 0; i < size; ++i)
////        std::cout << carray[i] << '\n';
////}
////
////int send_message(MQTTClient m_client, char** topic, MQTTClient_message m_pubmsg, MQTTClient_deliveryToken m_token, bool flag_is_first, string myMessage)
////{
////    int m_rc;
////    int len;
////
////    string myTopicForMessage="";
////    cout << "enter a message" << endl;
////
////    	myMessage = "";
//////    	cout << "while"<<myMessage << endl;
//////    	getline(cin, myMessage);
////
////    	if(myMessage == "") return 0;
////
//////    	/if(myMessage =="Q" || myMessage == "q") return;
////
////    	len = myMessage.size();
////
////    	string s = to_string(PARTYID);
////
////    	myMessage = s + "**" + myMessage;
////
////    	if(flag_is_first)
////    	{
////    		myTopicForMessage = topic[0];
////    		flag_is_first = false;
////    	} else {
////    		myTopicForMessage = topic[1];
////    	}
////        m_pubmsg.payload =(void*) myMessage.c_str();
////        m_pubmsg.payloadlen = myMessage.size();
////        m_pubmsg.qos = QOS;
////        m_pubmsg.retained = 0;
////        deliveredtoken = 0;
////        MQTTClient_publishMessage(m_client, myTopicForMessage.c_str(), &m_pubmsg, &m_token);
////
////        while(deliveredtoken != m_token);
////
//////        cout << "enter a message" << endl;
//////    }
////
////
////    return m_rc;
////}
////
////void disconnect(MQTTClient* m_client)
////{
//////	MQTTClient& m_client = client;
////    MQTTClient_disconnect(*m_client, 10000);
////    MQTTClient_destroy(m_client);
////}
////
////void connect(int party_id, MQTTClient client)
////{
////	int m_rc;
////
////	//reference to client
////	MQTTClient& m_client = client;
////
////    MQTTClient_connectOptions m_conn_opts = MQTTClient_connectOptions_initializer;
////
////    int m_ch;
////
////    // messages
////
////    MQTTClient_message m_pubmsg = MQTTClient_message_initializer;
////    MQTTClient_deliveryToken m_token;
////
////    PARTYID = party_id;
////    // create client object
////
////    const char* c ="party" +PARTYID;
////    MQTTClient_create(&m_client, ADDRESS, c,
////        MQTTCLIENT_PERSISTENCE_NONE, NULL);
////    m_conn_opts.keepAliveInterval = 20;
////    m_conn_opts.cleansession = 1;
////
////    MQTTClient_setCallbacks(m_client, NULL, connlost, msgarrvd, delivered);
////
////    if ((m_rc = MQTTClient_connect(m_client, &m_conn_opts)) != MQTTCLIENT_SUCCESS)
////    {
////        printf("Failed to connect, return code %d\n", m_rc);
////        exit(EXIT_FAILURE);
////    }
////
////    //free topic
////    // ALL\SHAREkey..
////    string s1 = "SHARE_Ps_VECTOR";
////    string s2 = "SHARE_Yjk_VECTOR";
////    char** topic = new char*[2];
////    topic[0] = (char*)s1.c_str();
////    topic[1] = (char*)s2.c_str();
////
////    int QQS_ARR[2] = {1, 1};
////    MQTTClient_subscribeMany(m_client, COUNT_TOPIC, topic, QQS_ARR);
////
////
////}
