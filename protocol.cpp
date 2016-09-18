
#include "stdlib.h"
#include "../../workspace/paho/src/MQTTClient.h"
#include "TParty.h"
#include "VDM.h"
#include "TGate.h"
#include "TCircuit.h"
#include "Communication.h"

using namespace std;

#define ADDRESS     "tcp://localhost:1883"

void RunTheProtocol(int party_id, MQTTClient &m_client, MQTTClient_message &m_pubmsg,
                    MQTTClient_deliveryToken &m_token, char** &topic, int &m_rc, string &s3);

void InitializationPhase(vector<TGate> &GateValueArr, vector<TGate> &GateShareArr, vector<bool> &GateDoneArr,
                         HIM &matrix_him,  VDM &matrix_vand);

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

void Broadcaster(string &myMessage, const MQTTClient &m_client, char *const *topic, string &myTopicForMessage,
                 MQTTClient_message &m_pubmsg, MQTTClient_deliveryToken &m_token, string &s) {

    string temp = myMessage;
    Communication::getInstance()->vec[Communication::getInstance()->PARTYID - 1] = temp;

    Communication::getInstance()->SendXVectorToAllParties(myMessage, m_client, topic, myTopicForMessage, m_pubmsg, m_token, s);
    // Round Function
    while (Communication::getInstance()->countXRecieve < Communication::getInstance()->N - 1) {}

    for(int i=0; i< Communication::getInstance()->vec.size(); i++)
    {
        cout << "this is: " << i << "  " << Communication::getInstance()->vec[i] << endl;
    }
}

void getXVector(string str, int pid)
{
    vector<string> arr = {};
    arr = split(str, '*');
    string str1;
    string str2 ="";
    TFieldElement* n;
    for (int i = 0; i < arr.size() ; i++) {
        str1 = arr[i];
        str2 += str1;
        n = new TFieldElement(str1);
        Communication::getInstance()->vecRecX.push_back(n);
    }

}

void ConcatenateEverything(vector<string> &buffers, int &no_buckets, HIM &matrix)
{
    vector<TFieldElement*> X1;
    vector<TFieldElement*> Y1;

    int N= Communication::getInstance()->N;
    int T= Communication::getInstance()->T;

    X1.resize(Communication::getInstance()->N);
    Y1.resize(Communication::getInstance()->N);
    matrix.InitHIM();

    for(int i=0; i< Communication::getInstance()->N; i++)
    {
        getXVector(Communication::getInstance()->vec[i], i + 1);
    }

    // total number of values
    int count = Communication::getInstance()->vecRecX.size();

    // nr of buckets
    no_buckets = count / (Communication::getInstance()->N - Communication::getInstance()->T) + 1;
    TFieldElement* n;

    for(int k = 0; k < no_buckets; k++)
    {
        for(int i = 0; i < Communication::getInstance()->N; i++)
        {
            if((i < N-T) && (k*(N-T)+i < count))
            {
                X1[i]= Communication::getInstance()->vecRecX[i];
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
    int N= Communication::getInstance()->N;
    while (Communication::getInstance()->countYRecieve < N - 1) {}

    string temp;
    for(int i=0; i< Communication::getInstance()->vecRecForCheck.size(); i++) {

        vector<string> arr = {};
        arr = split(Communication::getInstance()->vecRecForCheck[i], '*');
        if(arr.size() > 0) {
            temp = arr[0];
            for (int i = 1; i < arr.size() - 1; i++) {
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
               MQTTClient_deliveryToken &m_token, char** &topic, int &m_rc, HIM &him_matrix)
{
    int N= Communication::getInstance()->N;
    int no_buckets;
    vector<string> buffers;
    buffers.resize(N);
    Communication::getInstance()->vecRecForCheck.resize(N);
    string myTopicForMessage="";
    string s = to_string(Communication::getInstance()->PARTYID);

    Broadcaster(myMessage, m_client, topic, myTopicForMessage, m_pubmsg, m_token, s);

    ConcatenateEverything(buffers, no_buckets, him_matrix);

    Communication::getInstance()->SendTheResult(myMessage, m_client, myTopicForMessage, m_pubmsg, m_token, s, buffers);

    return CheckIfHappyOrCry(no_buckets);

}

int createObjectOfCommunicationAndStartTheProtocol(int argc, char* argv[])
{
    // finish intialize the connection to server

    RunTheProtocol(Communication::getInstance()->PARTYID, Communication::getInstance()->m_client,
                   Communication::getInstance()->m_pubmsg,
                   Communication::getInstance()->m_token,
                   Communication::getInstance()->topic,
                   Communication::getInstance()->m_rc,
                   Communication::getInstance()->s3);

    MQTTClient_disconnect(Communication::getInstance()->m_client, 10000);
    MQTTClient_destroy(&Communication::getInstance()->m_client);


}

string test() {
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

    return x;
}

void RunTheProtocol(int party_id, MQTTClient &m_client, MQTTClient_message &m_pubmsg,
                    MQTTClient_deliveryToken &m_token, char** &topic, int &m_rc, string &s3) {

    int N= Communication::getInstance()->N;
    TParty t(party_id);

    GF2X irreduciblePolynomial = BuildSparseIrred_GF2X(8);
    GF2E::init(irreduciblePolynomial);

    string x = test();

    // the value of the gate (for my input and output gates)
    vector<TGate> GateValueArr;
    // my share of the gate (for all gates)
    vector<TGate> GateShareArr;
    // true is the gate is processed
    vector<bool> GateDoneArr;
    HIM matrix_him(N,N);
    VDM matrix_vand(N,N);

    string s = to_string(Communication::getInstance()->PARTYID);

    Communication::getInstance()->ConnectionToServer(m_client, s, s3, m_pubmsg, m_token);

    InitializationPhase(GateValueArr, GateShareArr, GateDoneArr, matrix_him, matrix_vand);

    // start broadcast if cheating - halt
    if(broadcast(party_id, x, m_client, m_pubmsg, m_token, topic, m_rc, matrix_him) == false) {
        cout << "cheating!!!" << endl;
        return;
    }
    else {
        cout << "no cheating!!!" << endl;
    }
}

/// TO DO TO DO !!
void InputAdjustment(vector<TFieldElement> &diff)
{
    int M= Communication::getInstance()->M;
//    DiffBuf : TBuffer;
//    d : TFieldElement;
//
//    BEGIN Reset(SendBufs,TCircuit.nr_of_inputs_gates);
//    Reset(RecBufs,TCircuit.nr_of_inputs_gates);
//    Reset(DiffBuf,TCircuit.nr_of_inputs_gates);
//
    vector<TGate> circuit;
    // Ask Party for inputs
    TFieldElement d;
    for (int k = 0; k < M; k++)
    {
        if(circuit[k].getTGateType() == InputGate && circuit[k].getParty() == Communication::getInstance()->PARTYID)
        {
            //    BEGIN
            //   Writeln(’Enter input for gate nr ’,k,’:’);
            //   Readln(x);
            // return the diff;
            // DiffBuf = x-GateValueArr[k]
            // i will send it to this function

            // i need to concatenate the difference
            // this can abort in case of cheating


        }
    }


    // Broadcast(DiffBuf,RecBufs);

//    handle after broadcast
//    FOR k := 0 TO M-1 DO
//    IF TCircuit[k].GateType = InputGate THEN
//            BEGIN
//    Read(RecBuf[TCircuit[k].Party],d);
//    GateShareArr[k] := GateShareArr[k] + d;
//    GateDoneArr[k] := true;
//    END

}

void InitializationPhase(vector<TGate> &GateValueArr, vector<TGate> &GateShareArr, vector<bool> &GateDoneArr,
                         HIM &matrix_him,  VDM &matrix_vand)
{
    int M= Communication::getInstance()->M;
    // Compute Vandermonde matrix VDM
    matrix_vand.InitVDM();
    matrix_vand.Print();

    // Prepare an N-by-N hyper-invertible matrix
    matrix_him.InitHIM();

    // Initialize the array to keep shares and values of gates
    GateValueArr.resize(M);
    GateShareArr.resize(M);
    GateDoneArr.resize(M);

    for(int i=0; i<GateDoneArr.size(); i++)
    {
        GateDoneArr[i] = false;
    }

}

// for multiplication - will do after
void publicReconstruction(vector<TFieldElement*> alpha)
{
    int N= Communication::getInstance()->N;
    int T= Communication::getInstance()->T;
    //  Length(MyShares)
    int count;
    int no_buckets = count / (N-T) + 1;

    HIM m(T, N-T);

    vector<TFieldElement*> alpha1;
    vector<TFieldElement*> alpha2;

    for(int i = 0; i < N-T; i++)
    {
        alpha1[i] = alpha[i];
    }
    for(int i = N-T; i < alpha.size(); i++)
    {
        alpha2[i] = alpha[i];
    }

    m.InitHIMByVectors(alpha1, alpha2);


    for(int k=0; k < no_buckets; k++)
    {
        for(int i = 0; i < N-T; i++)
        {
            if( k*(N-T)+i < count)
            {
                // Read(MyShares,x1[i])
            } else {
                // x1[i] := ZERO;
            }

        }
        // y1 := MatrixMul(M,x1[0..N-T-1]);
        for(int i = 0; i < T; i++)
        {
            // x1[N-T+i] := y1[i];
        }
        for(int i = 0; i < N; i++)
        {
            // Write(SendBufs[i],x1[i]);
        }
    }

    // RoundFunction(SendBufs,RecBufs);

    for(int k=0; k < no_buckets; k++) {
        for (int i = 0; i < N - 1; i++) {
            // Read(RecBufs[i],x1[i]);
        }
        // y1 := MatrixMul(M,x1[0..N-T-1]);
        for (int i = 0; i < T; i++) {
          //  if (x1[N - T + i] != y1[i]) {
                // Halt
          //  }
        }
        for(int i = 0; i < N-T; i++)
        {
            if( k*(N-T)+i < count)
            {
                // Write(ValBuf,x1[i])
            }
        }
    }
}

void preparationPhase(VDM &matrix_vand, HIM &matrix_him)
{
    int N= Communication::getInstance()->N;
    int T= Communication::getInstance()->T;
    TCircuit circuit;
    int no_random = circuit.getnumOfMultiplicationGates() + circuit.getnumOfRandomGates() + circuit.getnumOfInputGates();

    vector<TFieldElement*> x1,x2,y1,y2;
    vector<string> sendBufs;

    x1.resize(T+1);
    x2.resize(2*T+1);
    y1.resize(T+1);
    y2.resize(2*T+1);

    // the number of buckets (each bucket requires one double-sharing
    // from each party and gives N-2T random double-sharings)
    // ASK MEYTAL why this is n and not N !!!
    int no_buckets = (no_random / (N-2*T))+1;
    PRG prg =PRG::instance();
    // return int 32 bit
    uint8_t b = prg.getRandom();


    sendBufs.resize(no_buckets*2);



    for(int k=0; k < no_buckets; k++)
    {
        // generate random degree-T polynomial
        for(int i = 0; i < T+1; i++)
        {
            b = prg.getRandom();
            x1[i] = TField::getInstance()->GetElement(b);
        }
        // same secret
        x2[0] = x1[0];
        for(int i = 1; i < 2*T+1; i++)
        {
            b = prg.getRandom();
            x2[i] = TField::getInstance()->GetElement(b);
        }
        matrix_vand.MatrixMult3(x1, y1);
        matrix_vand.MatrixMult3(x2, y2);
        for(int i=0; i < N; i++)
        {
            sendBufs[i] = y1[i]->toString() + "*";
            sendBufs[i] += y2[i]->toString();
        }
    }

    // RoundFunction(SendBufs,RecBufs)

    vector<string> recBufs;
    string sharingBuf = "";
    int robin = 0;
    string strX1, strX2, str;


    for(int k=0; k < no_buckets; k++) {
        // generate random degree-T polynomial
        for (int i = 0; i < N; i++) {
            //    Read(RecBufs[i],x1[i]);
            //    Read(RecBufs[i],x2[i]);
            str = recBufs[i];
            //
//            strX1 = recBufs[i];
//            strX2 = recBufs[i];

        }
        matrix_him.MatrixMult3(x1, y1);
        matrix_him.MatrixMult3(x2, y2);
        for (int i = 0; i < 2 * T; i++) {
            sendBufs[robin] = y1[i]->toString() + "*";
            sendBufs[robin] += y2[i]->toString();
            robin = (robin+1)%N;
        }
        for (int i = 2 * T; i < N; i++) {
            sharingBuf += y1[i]->toString() + "*" + y2[i]->toString();
        }
    }

    // RoundFunction(SendBufs,RecBufs);

    int count = no_buckets * (2*T) / N;
    if(no_buckets * (2*T)%N > Communication::getInstance()->PARTYID)
    {
        count++;
    }
}