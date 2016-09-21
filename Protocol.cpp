#include "Protocol.h"

Protocol::Protocol(){}

void Protocol::split(const string &s, char delim, vector<string> &elems) {
    stringstream ss;
    ss.str(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
}

vector<string> Protocol::split(const string &s, char delim) {
    vector<string> elems;
    split(s, delim, elems);
    return elems;
}

void Protocol::Broadcaster(string &myMessage, const MQTTClient &m_client, char *const *topic, string &myTopicForMessage,
                 MQTTClient_message &m_pubmsg, MQTTClient_deliveryToken &m_token, string &s) {

    string temp = myMessage;
    Communication::getInstance()->vec[Communication::getInstance()->PARTYID - 1] = temp;

    Communication::getInstance()->SendXVectorToAllParties(myMessage, m_client, topic, myTopicForMessage, m_pubmsg, m_token, s);

    for(int i=0; i< Communication::getInstance()->vec.size(); i++)
    {
        cout << "this is: " << i << "  " << Communication::getInstance()->vec[i] << endl;
    }
}

void Protocol::getXVector(string str, int pid)
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

void Protocol::ConcatenateEverything(vector<string> &buffers, int &no_buckets, HIM &matrix)
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

bool Protocol::CheckIfHappyOrCry(int &no_buckets)
{
    int N= Communication::getInstance()->N;
    //while (Communication::getInstance()->countYRecieve < N - 1) {}

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

bool Protocol::broadcast(int party_id, string myMessage ,MQTTClient &m_client, MQTTClient_message &m_pubmsg,
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

    cout << "after SendTheResult" << endl;

    return CheckIfHappyOrCry(no_buckets);

}

string Protocol::test() {
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
    SetCoeff(p7, 7);
    SetCoeff(p8, 1);
    SetCoeff(p6, 1);
    SetCoeff(p7, 6);
    SetCoeff(p8, 7);

    cout << p1 << endl;
    cout << p2 << endl;
    cout << p3 << endl;
    cout << p4 << endl;
    cout << p5 << endl;
    cout << p6 << endl;
    cout <<"p7"<< p7 << endl;
    cout << "p8"<<p8 << endl;


    TFieldElement e1(p1);

    TFieldElement e2(p2);

    TFieldElement e3(p3);

    TFieldElement e4(p4);

    TFieldElement e5(p5);

    TFieldElement e6(p5);

    TFieldElement e7(p7);

    TFieldElement e8(p8);

    TFieldElement e9(p5);

//    e9 = e7/e8;
  //  cout<< "result" << e9.toString()<<endl;

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

void Protocol::run() {

    int N= Communication::getInstance()->N;
    TParty t(Communication::getInstance()->PARTYID);

    GF2X irreduciblePolynomial = BuildSparseIrred_GF2X(8);
    GF2E::init(irreduciblePolynomial);


    string x = test();

    // the value of the gate (for my input and output gates)
    vector<TGate> GateValueArr;
    // my share of the gate (for all gates)
    vector<TFieldElement*> GateShareArr;
    // true is the gate is processed
    vector<bool> GateDoneArr;
    HIM matrix_him(N,N);
    VDM matrix_vand(N,N);
    matrix_vand.InitVDM();
    matrix_him.InitHIM();
    vector<string> sharingBuf;
    sharingBuf.resize(N);

    string s = to_string(Communication::getInstance()->PARTYID);

    ArythmeticCircuit circuit(1);
//    circuit.readCircuit("test.txt");
//    cout<< "size of circuit" << circuit.getGates().size();

    Communication::getInstance()->M=circuit.getNrOfGates();

    Communication::getInstance()->ConnectionToServer(Communication::getInstance()->m_client, s, Communication::getInstance()->s3, Communication::getInstance()->m_pubmsg, Communication::getInstance()->m_token);

    InitializationPhase(GateValueArr, GateShareArr, GateDoneArr, matrix_him, matrix_vand);


    if(preparationPhase(matrix_vand, matrix_him, sharingBuf) == false) {
        cout << "cheating!!!" << endl;
        return;
    }
    else {
        cout << "no cheating!!!" << endl << "finish Preparation Phase" << endl;
    }

    inputPreparation(sharingBuf, GateShareArr, circuit);


    // start broadcast if cheating - halt
//    if(broadcast(Communication::getInstance()->PARTYID, x, Communication::getInstance()->m_client, Communication::getInstance()->m_pubmsg, Communication::getInstance()->m_token, Communication::getInstance()->topic, Communication::getInstance()->m_rc, matrix_him) == false) {
//        cout << "cheating!!!" << endl;
//        return;
//    }
//    else {
//        cout << "no cheating!!!" <<  endl << "finish Broadcast" << endl;
//    }
}

/// TO DO TO DO !!
void Protocol::InputAdjustment(vector<TFieldElement> &diff)
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
        if(circuit[k].gateType == INPUT && circuit[k].party == Communication::getInstance()->PARTYID)
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

void Protocol::InitializationPhase(vector<TGate> &GateValueArr, vector<TFieldElement*> &GateShareArr, vector<bool> &GateDoneArr,
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
void Protocol::publicReconstruction(vector<TFieldElement*> alpha)
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

bool Protocol::preparationPhase(VDM &matrix_vand, HIM &matrix_him, vector<string> &sharingBuf)
{
    int N= Communication::getInstance()->N;
    int T= Communication::getInstance()->T;
    vector<string> recBufs;
    recBufs.resize(N);
    ArythmeticCircuit circuit;

    //
    int no_random = circuit.getNrOfMultiplicationGates() + 0 + circuit.getNrOfInputGates();
   //int no_random = 5;
    vector<TFieldElement*> x1,x2,y1,y2;
    vector<string> sendBufs;

    x1.resize(N);
    x2.resize(N);
    y1.resize(N);
    y2.resize(N);

    // the number of buckets (each bucket requires one double-sharing
    // from each party and gives N-2T random double-sharings)
    int no_buckets = (no_random / (N-2*T))+1;
    PRG prg =PRG::instance();

    // return int 32 bit
    uint8_t b = prg.getRandom();

    //sendBufs.resize(no_buckets*2);

    sendBufs.resize(N);
    for(int k=0; k < no_buckets; k++)
    {
        // generate random degree-T polynomial
        for(int i = 0; i < T+1; i++)
        {
            x1[i] = TField::getInstance()->Random();
        }
        // same secret
        TFieldElement* secret = new TFieldElement();
        secret->setPoly(x1[0]->getElement());
        //x2[0] = x1[0];
        x2[0] = secret;
        for(int i = 1; i < 2*T+1; i++)
        {
            x2[i] = TField::getInstance()->Random();
        }
        matrix_vand.MatrixMult3(x1, y1);
        matrix_vand.MatrixMult3(x2, y2);
        for(int i=0; i < N; i++)
        {
            sendBufs[i] = y1[i]->toString() + "*";
            sendBufs[i] += y2[i]->toString();
        }
    }

    Communication::getInstance()->sendPartOfPoly(sendBufs,recBufs);

//
//    x1.clear();
//    x2.clear();
//    x1.resize(N);
//    x2.resize(N);


//    string sharingBuf = "";
    int robin = 0;
    string strX1, strX2, str;

    for(int k=0; k < no_buckets; k++) {
        // generate random degree-T polynomial
        for (int i = 0; i < N; i++) {
            str = recBufs[i];
            vector<string> arr = {};
            arr = split(str, '*');
            if(arr.size() >1 ) {
                strX1 = arr[0];
                strX2 = arr[1];
                x1[i] = new TFieldElement(strX1);
                x2[i] = new TFieldElement(strX2);
            }
        }
        matrix_him.MatrixMult3(x1, y1);
        matrix_him.MatrixMult3(x2, y2);
        for (int i = 0; i < 2 * T; i++) {
            sendBufs[robin] = y1[i]->toString() + "*";
            sendBufs[robin] += y2[i]->toString();
            robin = (robin+1)%N;
        }
        for (int i = 2 * T; i < N; i++) {
            sharingBuf[i-2*T] = y1[i]->toString() + "*" + y2[i]->toString();
        }
    }

    Communication::getInstance()->sendDoubleShare(sendBufs,recBufs);

    int count = no_buckets * (2*T) / N;
    if(no_buckets * (2*T)%N > Communication::getInstance()->PARTYID)
    {
        count++;
    }

    for(int k=0; k < count; k++) {
        for (int i = 0; i < N; i++) {
            str = recBufs[i];
            vector<string> arr = {};
            arr = split(str, '*');
            strX1 = arr[0];
            strX2 = arr[1];
            x1[i] = new TFieldElement(strX1);
            x2[i] = new TFieldElement(strX2);
        }

        // Check that x1 is t-consistent and x2 is 2t-consistent and secret is the same
        vector<TFieldElement*> alpha_t1, alpha_2t1, alpha_t2, alpha_2t2;
        alpha_t1.resize(T+1);
        alpha_t2.resize(N-1-T);
        alpha_2t1.resize(2*T+1);
        alpha_2t2.resize(N-1-2*T);

        for(int i = 0; i < alpha_t1.size(); i++)
        {
            alpha_t1[i] =(TField::getInstance()->GetElement(i));
        }
        for(int i = alpha_t1.size(); i < alpha_t2.size(); i++)
        {
            alpha_t2[i] =(TField::getInstance()->GetElement(i));
        }

        for(int i = 0; i < alpha_2t1.size(); i++)
        {
            alpha_2t1[i] =(TField::getInstance()->GetElement(i));
        }
        for(int i = alpha_2t1.size(); i < alpha_2t2.size(); i++)
        {
            alpha_2t2[i] =(TField::getInstance()->GetElement(i));
        }

        HIM matrix_t(T+1, N-T-1);
        matrix_t.InitHIMByVectors(alpha_t1, alpha_t2);
        HIM matrix_2t(2*T+1, N-2*T-1);
        matrix_2t.InitHIMByVectors(alpha_2t1, alpha_2t2);
        if(!matrix_t.CheckConsistency(T,N,x1) || !matrix_2t.CheckConsistency(2*T,N,x2) ||
                matrix_t.Interpolate(alpha_t1, x1).toString() != matrix_2t.Interpolate(alpha_2t1, x2).toString() ) {
            // cheating detected, abort
            return false;
        }
    }
    return true;
}

bool Protocol::inputPreparation(vector<string> &sharingBuf, vector<TFieldElement*> &gateShareArr, ArythmeticCircuit &circuit)
{

    cout <<"start               inputPreparation" <<endl;
    int N= Communication::getInstance()->N;
    int T= Communication::getInstance()->T;
    int M= Communication::getInstance()->M;

    vector<string> sendBufs;
    vector<string> recBufs;

    vector<TFieldElement*> gateValueArr;
    vector<TFieldElement*> x1;
    TFieldElement* elem;
    int i;
    // upper bound
//    sendBufs.resize(circuit.getnumOfInputGates());
//    recBufs.resize(circuit.getnumOfInputGates());
    sendBufs.resize(N);
    recBufs.resize(N);
    x1.resize(N);
    HIM mat(N, N);
    mat.InitHIM();

    for(int k = 0; k < M; k++)
    {
        if((circuit.getGates())[k].gateType == INPUT)
        {
            // use the t-sharing (correction will come)
            string str = sharingBuf[k];
            vector<string> arr = {};
            arr = split(str, '*');
            // waste the 2T-sharing
            elem = new TFieldElement(arr[0]);
            gateShareArr[k] = elem;
            i = (circuit.getGates())[k].party;
            // reconstruct sharing towards input party
            sendBufs[i-1] = gateShareArr[k]->toString();
        }
    }

    Communication::getInstance()->sendGateShareArr(sendBufs,recBufs);

    // reconstruct my random input values
    //  SetLength(x1,N);
    // vector for the shares of my inputs

   /// TO DO!!!!!!!!!!!!!! put outside the matrix

    vector<TFieldElement*> alpha_t1, alpha_2t1, alpha_t2, alpha_2t2;
    alpha_t1.resize(T+1);
    alpha_t2.resize(N-1-T);
    alpha_2t1.resize(2*T+1);
    alpha_2t2.resize(N-1-2*T);

    for(int i = 0; i < alpha_t1.size(); i++)
    {
        alpha_t1[i] =(TField::getInstance()->GetElement(i));
    }
    for(int i = alpha_t1.size(); i < alpha_t2.size(); i++)
    {
        alpha_t2[i] =(TField::getInstance()->GetElement(i));
    }

    for(int i = 0; i < alpha_2t1.size(); i++)
    {
        alpha_2t1[i] =(TField::getInstance()->GetElement(i));
    }
    for(int i = alpha_2t1.size(); i < alpha_2t2.size(); i++)
    {
        alpha_2t2[i] =(TField::getInstance()->GetElement(i));
    }

    HIM matrix_t(T+1, N-T-1);
    matrix_t.InitHIMByVectors(alpha_t1, alpha_t2);

    ////////////



    for(int k = 0; k < M; k++) {
        if (circuit.getGates()[k].gateType == INPUT && circuit.getGates()[k].party == Communication::getInstance()->PARTYID) {
            // my input: reconstruct received shares
            for (int k = 0; k < N; k++) {
                if(!matrix_t.CheckConsistency(T,N,x1))
                {
                    // someone cheated!
                    return false;
                }
                // the (random) secret
                TFieldElement* elem = new TFieldElement(matrix_t.Interpolate(alpha_t1, x1).getElement());
                gateValueArr[k] = elem;
            }
        }
    }

    return true;

}