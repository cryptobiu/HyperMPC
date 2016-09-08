
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
bool send_result = false;
bool flag_is_first = true;
volatile MQTTClient_deliveryToken deliveredtoken;

int N;
map<int, string> map_check;
vector<int> vecConn;
vector<TFieldElement> vecRec;
vector<TFieldElement> vecResult;
vector<TFieldElement> vecRecX;

string Ps;
bool flagPs = true;

void delivered(void *context, MQTTClient_deliveryToken dt)
{
    deliveredtoken = dt;
}


vector<string> split(const string &s, char delim, vector<string> &elems)
{
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim))
    {
        elems.push_back(item);
    }
    return elems;
}


vector<string> split(const string &s, char delim)
{
    vector<string> elems;
    split(s, delim, elems);
    return elems;
}

string convertToString(vector<TFieldElement> t)
{
	string str = "";
	for (int i=0; i < N - 1; i++)
	{
		str += t[i].toString() + "*";
	}
	str += t[N].toString();
}

void addElement(string str)
{
	// check if we can remove the pointer
	TFieldElement* n = new TFieldElement(str);
	vecRec.push_back(*n);
}

string getCurrectMessage(string str)
{
	vector<string> arr = {};
	arr = split(str, '*');
	string str1= arr[PARTYID];

	return str1;
}

string getXVector(string str)
{

	vector<string> arr = {};
	arr = split(str, '*');
	string str1;
	string str2 ="";
	TFieldElement* n;
	for (int i = 0; i < N ; i++) {
		cout << i << ".." << arr[i] << endl;
		str1 = arr[i];
		str2 += str1;
		n = new TFieldElement(str1);
		vecRecX.push_back(*n);
	}
	return str2;
}

void calculate()
{
	HIM matrix(N,N);

	matrix.InitHIM();

	vecResult = matrix.MatrixMult(vecRecX);

}

/**
 * the function handle when message arrive
 */
int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
    string topic(topicName);
	string str_message = "";
    int i;
    char* payloadptr;
    string s = to_string(PARTYID);
    int len_of_message;

    payloadptr = (char*)message->payload;
    string str(payloadptr);


    str = strtok(payloadptr, "$");


    if(str == s) return 1;
	cout << "sssss  "<< str<<endl;
    // loop until client_id



    if(topic == "CONNECT")
    {
    	cout << "sssss  "<< str<<endl;
    	cout << "connect" << endl;
    	int elem = std::stoi(str);
    	cout <<"elem"<< elem << endl;
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
    		cout << "1" << endl;
    		cout << "ppppppppppppp" << vecConn.size() << endl;
    		vecConn.push_back(elem);
			cout << "2" << endl;

    	}
//	   len_of_message= message->payloadlen;
//		 for(i=0; i<len_of_message; i++)
//		 {
//			 str_message += (*payloadptr++);
//		 }
//        printf("Message arrived\n");
//        printf("     topic: %s\n", topicName);
//        printf("   message: ");
//        cout << str_message;
//        putchar('\n');
        // clean the pointer of the message
    //     char* npoiter ="";
      //  message->payload =npoiter;
//    	  cout <<"GGGGGGGGGGGGGGGGG";
//    	  cout << str;
    	  printf("Message arrived\n");
    	  printf("     topic: %s\n", topicName);

    	  printf("   message: ");

    	  cout << elem << endl;

        MQTTClient_freeMessage(&message);
        MQTTClient_free(topicName);

        return 1;
    }

     int count = 0;
     int div, mod;
     div = PARTYID / 10;
     mod = PARTYID % 10;
     if(mod == 0)
     {
     	count = div;
     } else {
     	count = div + 1;
     }
     for(i=0; i<count+1;i++){
     	*payloadptr++;
     }
    len_of_message= message->payloadlen - (count+1);
     for(i=0; i<len_of_message; i++)
     {
         str_message += (*payloadptr++);
     }

    if(flagPs){
    	Ps = str_message;
    	flagPs = false;

    } else {
    	if (map_check.count(PARTYID) == 0) {
    	map_check[PARTYID] = str_message;
    	} else {
    		cout << endl;
    		cout << endl;
    		cout << "this party already send a message" << endl;
    	}
    }

    cout << str_message << "l"<<endl;

    if(topic == "SHARE_Yjk_VECTOR") {

		str_message = getCurrectMessage(str_message);
		addElement(str_message);
    } else {
    	str_message = getXVector(str_message);
    	calculate();
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

void old_func(char** carray, size_t size)
{
    for(size_t i = 0; i < size; ++i)
        std::cout << carray[i] << '\n';
}

int communication_broadcast(int party_id, string myMessage, int is_Ps)
{
    MQTTClient m_client;
    MQTTClient_connectOptions m_conn_opts = MQTTClient_connectOptions_initializer;
    int m_rc;
    int m_ch;

    int len;
    string myTopicForMessage="";

    // messages
    MQTTClient_message m_pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken m_token;

    PARTYID = party_id;

    // create client object

    const char* c ="party" +PARTYID;
    MQTTClient_create(&m_client, ADDRESS, c,
        MQTTCLIENT_PERSISTENCE_NONE, NULL);

    m_conn_opts.keepAliveInterval = 20;
    m_conn_opts.cleansession = 1;


    // msgarrvd - handle function
    MQTTClient_setCallbacks(m_client, NULL, connlost, msgarrvd, delivered);

    // try to connect to server
    if ((m_rc = MQTTClient_connect(m_client, &m_conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", m_rc);
        exit(EXIT_FAILURE);
    }

    //free topic
    // create topics
    string s1 = "SHARE_Ps_VECTOR";
    string s2 = "SHARE_Yjk_VECTOR";
    string s3 = "CONNECT";
    char** topic = new char*[3];
    topic[0] = (char*)s1.c_str();
    topic[1] = (char*)s2.c_str();
    topic[2] = (char*)s3.c_str();
    int QQS_ARR[3] = {1, 1, 1};

    // update the topics
    MQTTClient_subscribeMany(m_client, 3, topic, QQS_ARR);

	string s = to_string(PARTYID);

    // connect




	string strMessage = s + "$" + s;




	myTopicForMessage = "CONNECT";

	// update the details of message
	m_pubmsg.payload =(void*) strMessage.c_str();
	m_pubmsg.payloadlen = strMessage.size();
	m_pubmsg.qos = QOS;
	m_pubmsg.retained = 0;
	deliveredtoken = 0;


	cout << "mess  " << strMessage << endl;

	// publish the message to all parties
	MQTTClient_publishMessage(m_client, myTopicForMessage.c_str(), &m_pubmsg, &m_token);


	cout << "fff" << endl;


//N-
	while(vecConn.size() < 1){
		cout << vecConn.size() << endl;
		MQTTClient_publishMessage(m_client, myTopicForMessage.c_str(), &m_pubmsg, &m_token);
	}












    // check if the message valid
    if(myMessage =="Q" || myMessage == "q" || myMessage == "") return 0;

	len = myMessage.size();


	// add id party to the message
	myMessage = s + "$" + myMessage;


//	if(flag_is_first)
//	{
//		myTopicForMessage = topic[0];
//		flag_is_first = false;
//	} else {
//		myTopicForMessage = topic[1];
//	}

	if(is_Ps == 0)
	{
		myTopicForMessage = topic[0];
		flag_is_first = false;
	} else {
		myTopicForMessage = topic[1];
	}


	// update the details of message
	m_pubmsg.payload =(void*) myMessage.c_str();
	m_pubmsg.payloadlen = myMessage.size();
	m_pubmsg.qos = QOS;
	m_pubmsg.retained = 0;
	deliveredtoken = 0;

	// publish the message to all parties
	MQTTClient_publishMessage(m_client, myTopicForMessage.c_str(), &m_pubmsg, &m_token);

	// waiting until the message send
	while(deliveredtoken != m_token){};

	// after !


	while (map_check.size() < N-1) {}

	cout << "llllllllllllllllllllll" << endl;

	while(send_result) {}

	//
	myMessage = convertToString(vecResult);
	/////



	len = myMessage.size();

	myMessage = s + "$" + myMessage;

	myTopicForMessage = topic[1];

	m_pubmsg.payload =(void*) myMessage.c_str();

	m_pubmsg.payloadlen = myMessage.size();
	m_pubmsg.qos = QOS;
	m_pubmsg.retained = 0;
	deliveredtoken = 0;
	MQTTClient_publishMessage(m_client, myTopicForMessage.c_str(), &m_pubmsg, &m_token);


	while(deliveredtoken != m_token) {};


/////////////////////////////

        //calculate him


        // send to parties the vector

    MQTTClient_disconnect(m_client, 10000);
    MQTTClient_destroy(&m_client);
    return m_rc;
}



/*
 * argv[1] = party id
 * argv[2] = number of parties
 * argv[3] = Ps = 0 / other = 1
 */
int main(int argc, char* argv[])
{
	if(argc != 4)
	{
		cout << "error";
		return 0;
	}

	N = atoi(argv[2]);
	int partypid = atoi(argv[1]);
	int is_Ps = atoi(argv[3]);

	TParty t(partypid);

	GF2X irreduciblePolynomial = BuildSparseIrred_GF2X(8);
	GF2E::init(irreduciblePolynomial);

	GF2X p1, p2, p3;
	SetCoeff(p1, 5);
	SetCoeff(p1, 2);
	SetCoeff(p2, 3);
	SetCoeff(p3, 1);

	cout << p1 << endl;
	cout << p2 << endl;

	TFieldElement e1(p1);

	TFieldElement e2(p2);

	TFieldElement e3(p3);


	string x = e1.toString() +"*" +e2.toString() + "*"  +e3.toString();

	cout << x << endl;

	// start broadcast
	communication_broadcast(partypid, x, is_Ps);


}

