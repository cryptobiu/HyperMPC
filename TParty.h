///*
// * Party.h
// *
// *  Created on: Aug 16, 2016
// *      Author: hila
// */
//
//#ifndef PARTY_H_
//#define PARTY_H_
//#include "TField.h"
//#include "HIM.h"
//
//#include "stdio.h"
//#include "stdlib.h"
//#include <string>
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
//#include "ProtocolBroadcast.h"
//
//using namespace std;
//
//
//
//
//
//
//class TParty {
//private:
//	char** m_topic;
//	 MQTTClient_message m_pubmsg;
//	 MQTTClient_deliveryToken m_token;
//	 int m_pid;
//	MQTTClient m_client;
//	bool m_is_happy;
//public:
//	TParty(int num);
//	TFieldElement Interpolate(TFieldElement* alpha, int n, TFieldElement* x);
//	//void sendVectorToParties(TFieldElement** vector_of_x);
//	void sendVectorToParties(string	myMessage);
//	void f(string myMessage);
//	virtual ~TParty();
//};
//
//#endif /* PARTY_H_ */



/*
 * Party.h
 *
 *  Created on: Aug 16, 2016
 *      Author: hila
 */

#ifndef PARTY_H_
#define PARTY_H_
#include "TField.h"
#include "HIM.h"

#include "stdio.h"
#include "stdlib.h"
#include <string>
#include "../../workspace/paho/src/MQTTClient.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <iostream>
#include <cassert>

#include "ProtocolBroadcast.h"

using namespace std;






class TParty {
private:
	int m_pid;
	bool m_is_happy;
public:
	TParty(int num);
	TFieldElement Interpolate(TFieldElement* alpha, int n, TFieldElement* x);
	//void sendVectorToParties(TFieldElement** vector_of_x);
	void sendVectorToParties(string	myMessage);
	void f(string myMessage);
	virtual ~TParty();
};

#endif /* PARTY_H_ */
