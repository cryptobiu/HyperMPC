///*
// * ProtocolBroadcast.h
// *
// *  Created on: Aug 24, 2016
// *      Author: hila
// */
//
//#ifndef PROTOCOLBROADCAST_H_
//#define PROTOCOLBROADCAST_H_
//
//#include "stdio.h"
//#include "stdlib.h"
//#include "string.h"
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
//extern volatile MQTTClient_deliveryToken deliveredtoken;
//extern int PARTYID;
//extern bool flag_is_first;
//
//
//
//void delivered(void *context, MQTTClient_deliveryToken dt);
//
///**
// * the function handle when message arrive
// */
//int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message);
//
//void connlost(void *context, char *cause);
////
////void old_func(char** carray, size_t size);
////
////int send_message(MQTTClient m_client, char** topic, MQTTClient_message m_pubmsg, MQTTClient_deliveryToken m_token, bool flag_is_first, string myMessage);
////
////void disconnect(MQTTClient* client);
////
////void connect(int party_id, MQTTClient client);
//
//
//#endif /* PROTOCOLBROADCAST_H_ */
