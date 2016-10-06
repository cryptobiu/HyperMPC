#include "Protocol.h"

Protocol::Protocol(int n, int id)
{
    N = n;
    T = n/3 - 1;
    if(n%3 > 0)
    {
        T++;
    }
    m_partyId = id;
}

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

void Protocol::getXVector(string str, int pid)
{
    vector<string> arr = {};
    arr = split(str, '*');
    string str1;
    string str2 ="";
    TFieldElement n;
    for (int i = 0; i < arr.size() ; i++) {
        str1 = arr[i];
        str2 += str1;
        n = TFieldElement(str1);
        Communication::getInstance()->vecRecX.push_back(n);
    }
}

/**
 *
 *
 */
bool Protocol::broadcast(int party_id, string myMessage ,MQTTClient &m_client, MQTTClient_message &m_pubmsg,
                         MQTTClient_deliveryToken &m_token, char** &topic, int &m_rc, vector<string> &recBufsdiff)
{
    cout <<" start br" << '\n';
    int no_buckets;
    vector<string> buffers(N);

    string myTopicForMessage="";
    string s = to_string(m_partyId);


    string temp = myMessage;
    Communication::getInstance()->vec[m_partyId - 1] = temp;
    cout <<" in br 1" << '\n';
    vector<string> recBufs2(N);
    vector<string> valBufs(N);
    cout <<" in br 1.5" << '\n';
    Communication::getInstance()->SendXVectorToAllParties(myMessage, m_client, topic, myTopicForMessage, m_pubmsg, m_token, s, recBufsdiff);
    cout <<" in br 2" << '\n';
    for(int i=0; i< Communication::getInstance()->vec.size(); i++)
    {
        cout << "this is: " << i << "  " << Communication::getInstance()->vec[i] << '\n';
        cout << "this is: " << i << "  " << recBufsdiff[i] << '\n';
    }

    cout << "after Broadcaster" << '\n';


    HIM mat(N,N);
    mat.InitHIM();
    cout <<" in br 3" << '\n';
    vector<TFieldElement> X1(N);
    vector<TFieldElement> Y1(N);

    // total number of values
    int count = 0;
    int index = 0;
    for(int l=0; l< N; l++)
    {
        vector<string> arr = {};
        arr = split(recBufsdiff[l], '*');
        count += arr.size();
        for (int i = 0; i < arr.size() ; i++) {
            valBufs[index] = arr[i];
            index++;
        }
    }
    cout <<" in br 4" << '\n';
    cout << "index" << index << '\n';
    index = 0;

    // nr of buckets
    no_buckets = count / (N - T) + 1;

    cout << "count  " <<count << '\n';

    cout << "no_buckets  " <<no_buckets << '\n';

    for(int k = 0; k < no_buckets; k++)
    {
        for(int i = 0; i < N; i++)
        {
            if((i < N-T) && (k*(N-T)+i < count))
            {
                X1[i]= TFieldElement(valBufs[index]);
                index++;
            }
                // padding zero
            else
            {
                X1[i] = TFieldElement(GF2X::zero());
            }
        }

        mat.MatrixMult(X1, Y1);
        cout << "Y1[i]" << '\n';
        for(int i = 0; i < N; i++) {
            cout << i << '\n';
            cout << Y1[i].toString() << '\n';
        }

        for(int i = 0; i < N; i++) {
            buffers[i] += Y1[i].toString() + "*";
        }
        for(int i = 0; i < N; i++)
        {
            X1[i].setPoly(GF2X::zero());
            Y1[i].setPoly(GF2X::zero());
        }
    }

    cout << "index  2 time :" << index << '\n';

    // no cheating: all parties have same y1
    // ‘‘Reconstruct’’ values towards some party (‘‘reconstruct’’ with degree 0)

    // buffers[0] --> the buffer of party with num id is 1
    cout << "Result" << '\n';
    for(int i = 0; i < N; i++) {
        cout << buffers[i] << '\n';
    }

    cout << "after ConcatenateEverything" << '\n';

    Communication::getInstance()->SendTheResult(myMessage, m_client, myTopicForMessage, m_pubmsg, m_token, s, buffers, recBufs2);

    cout << "after SendTheResult" << '\n';

    string temp1;
    for(int k=0; k< no_buckets; k++) {

        vector<string> arr = {};
        arr = split(Communication::getInstance()->vecRecForCheck[k], '*');
        if(arr.size() > 0) {
            temp1 = arr[0];
            for (int i = 1; i < arr.size()-1; i++) {
                if(temp1 != arr[i])
                {
                    return false;
                }
            }
        }
    }

    return true;

}

void Protocol::run() {
    TParty t(m_partyId);

    GF2X irreduciblePolynomial = BuildSparseIrred_GF2X(8);
    GF2E::init(irreduciblePolynomial);

    // the value of the gate (for my input and output gates)
    vector<TFieldElement> gateValueArr;
    // my share of the gate (for all gates)
    vector<TFieldElement> gateShareArr;
    vector<TFieldElement> alpha;

    // true is the gate is processed
    vector<bool> gateDoneArr;
    HIM matrix_him(N,N);
    VDM matrix_vand(N,N);
    matrix_vand.InitVDM();
    matrix_him.InitHIM();
    vector<string> sharingBuf;

    string s = to_string(m_partyId);

 //  ArythmeticCircuit circuit(1);
    ArythmeticCircuit circuit;
    circuit.readCircuit("/home/hila/ClionProjects/Secret-Sharing/test.txt");
    cout << "size of circuit" << circuit.getGates().size();

    Communication::getInstance()->M=circuit.getNrOfGates();
    M = circuit.getNrOfGates();

    Communication::getInstance()->ConnectionToServer(Communication::getInstance()->m_client, s, Communication::getInstance()->s3, Communication::getInstance()->m_pubmsg, Communication::getInstance()->m_token);

    initializationPhase(gateValueArr, gateShareArr, gateDoneArr, matrix_him, matrix_vand, alpha);


    if(preparationPhase(matrix_vand, matrix_him, sharingBuf, alpha, circuit) == false) {
        cout << "cheating!!!" << '\n';
        return;
    }
    else {
        cout << "no cheating!!!" << '\n' << "finish Preparation Phase" << '\n';
    }

    if(inputPreparation(sharingBuf, gateShareArr, circuit, alpha, gateValueArr) == false) {
        cout << "cheating!!!" << '\n';
        return;
    }
    else {
        cout << "no cheating!!!" << '\n' << "finish Input Preparation" << '\n';
    }

    string sss = "";

    inputAdjustment(sss, gateValueArr, circuit, gateShareArr, gateDoneArr);

    cout << "after Input Adjustment " << '\n';

     processAdditions(circuit,gateDoneArr, gateShareArr);

    cout << "after process Additions " << '\n';

    processMultiplications(circuit, gateDoneArr, gateShareArr, sharingBuf, alpha);

    cout << "after process Multiplications 1" << '\n';

    processMultiplications(circuit, gateDoneArr, gateShareArr, sharingBuf, alpha);

    cout << "after process Multiplications 2" << '\n';

    processMultiplications(circuit, gateDoneArr, gateShareArr, sharingBuf, alpha);

    cout << "after process Multiplications 3" << '\n';

   outputPhase(circuit, gateShareArr, alpha, gateValueArr);

    cout << "after output Phase " << '\n';
}

/**
 * the function implements the second step of Input Phase:
 * the party broadcasts for each input gate the difference between
 * the random secret and the actual input value.
 * @param diff
 * @param gateValueArr
 * @param circuit
 * @param gateShareArr
 * @param gateDoneArr
 */
void Protocol::inputAdjustment(string &diff, vector<TFieldElement> &gateValueArr, ArythmeticCircuit &circuit,
                               vector<TFieldElement> &gateShareArr, vector<bool> &gateDoneArr)
{
    int input;

    // Ask Party for inputs
    for (int k = 0; k < M; k++)
    {
        if(circuit.getGates()[k].gateType == INPUT && circuit.getGates()[k].party == m_partyId)
        {
            cout << "enter your real input : " << '\n';
            cin >> input;
            // Ok, the value is GateValue[k], but should be input.
            TFieldElement myinput = TField::getInstance()->GetElementByValue(input);

            cout << "gatevaluearr" << k << "  " << gateValueArr[k].getElement() << '\n';

            TFieldElement different = myinput - gateValueArr[k];
            cout << different.getElement()<< '\n';
            string str = different.toString();

            diff += str + "*";

            // i need to concatenate the difference
            // this can abort in case of cheating
        }
    }
    cout << "try to print diff" << '\n';
    cout << diff << '\n';

    vector<string> recBufsdiff(N);

    cout << "malloc is fine " << '\n';
    // Broadcast the difference between GateValue[k] to x.
    // start broadcast if cheating - halt
    if(broadcast(m_partyId, diff, Communication::getInstance()->m_client, Communication::getInstance()->m_pubmsg, Communication::getInstance()->m_token, Communication::getInstance()->topic, Communication::getInstance()->m_rc, recBufsdiff) == false) {
        cout << "cheating!!!" << '\n';
        return;
    }
    else {
        cout << "no cheating!!!" <<  '\n' << "finish Broadcast" << '\n';
    }

    // handle after broadcast
    string str;
    TFieldElement db;
    cout<<"m " << M << '\n';
    for (int k = 0; k < M; k++)
    {
        if(circuit.getGates()[k].gateType == INPUT)
        {
            str = recBufsdiff[circuit.getGates()[k].party - 1];
            vector<string> arr = {};
            arr = split(str, '*');
            for(int i=0; i<arr.size(); i++) {
                db = TFieldElement(arr[i]);
                gateShareArr[k] = gateShareArr[k] + db;
            }
            gateDoneArr[k] = true;
        }
    }

}

/**
 * some global variables are initialized
 * @param GateValueArr
 * @param GateShareArr
 * @param GateDoneArr
 * @param matrix_him
 * @param matrix_vand
 * @param alpha
 */
void Protocol::initializationPhase(vector<TFieldElement> &gateValueArr, vector<TFieldElement> &gateShareArr,
                                   vector<bool> &GateDoneArr,
                                   HIM &matrix_him, VDM &matrix_vand, vector<TFieldElement> &alpha)
{
    // Compute Vandermonde matrix VDM
    matrix_vand.InitVDM();
    matrix_vand.Print();
    alpha.resize(N);

    // Prepare an N-by-N hyper-invertible matrix
    matrix_him.InitHIM();

    // Initialize the array to keep shares and values of gates
    gateValueArr.resize(M);
    gateShareArr.resize(M);
    GateDoneArr.resize(M);

    for(int i=0; i<N; i++)
    {
        alpha[i]=(TField::getInstance()->GetElementByValue(i+1));
    }

    for(int i=0; i<GateDoneArr.size(); i++)
    {
        GateDoneArr[i] = false;
    }

}

/**
 * The function compute t shared authentication checks and to reconstruct the n sharings,
 * one towards each party, who then computes the secret and sends it to everybody.
 * Each party receives n − t secrets and t authentication checks.
 * @param myShares
 * @param alpha
 * @param valBuf
 */
void Protocol::publicReconstruction(vector<string> &myShares, int &count, int d, vector<TFieldElement> &alpha, vector<TFieldElement> &valBuf)
{
    // count =  Length(MyShares)
    int no_buckets = count / (N-T) + 1;
    cout << " count " << count;
    cout << " no_buckets " << no_buckets;
    //  M := CreateHIM(alpha[0..N-T-1],alpha[N-T..N-1]);
    // t-by-(n-t) HIM
    HIM m(T, N-T);
    TFieldElement x;
    vector<TFieldElement> alpha1(N-T);
    vector<TFieldElement> alpha2(T);
    vector<TFieldElement> x1(N);
    vector<TFieldElement> y1(N);
    vector<TFieldElement> y2(N);
    vector<string> sendBufs(N);
    vector<string> sendBufs2(N);
    vector<string> recBufs(N);
    vector<string> recBufs2(N);

    for(int i = 0; i < N; i++)
    {
        cout << "alpha[i]" << i << " " << alpha[i].toString() << endl;
    }

    for(int i = 0; i < N-T; i++)
    {
        alpha1[i] = alpha[i];
        cout << "alpha1[i]" << i << " " << alpha1[i].toString() << endl;
    }
    for(int i = N-T; i < N; i++)
    {
        alpha2[i - (N-T)] = alpha[i];
        cout << "alpha2[i]" << i << " " << alpha2[i - (N-T)].toString() << endl;
    }




    for(int i = 0; i < N; i++)
    {
        recBufs[i] = "";
        recBufs2[i] = "";
        sendBufs[i] = "";
        sendBufs2[i] = "";
    }

    m.InitHIMByVectors(alpha1, alpha2);
    cout << "the martix" << endl;
    m.Print();

    // init x to be vector of degree-d (d=2*t) shares of n−t secret
    for(int k=0; k < no_buckets; k++)
    {
        for(int i = 0; i < N-T; i++)
        {
            if( k*(N-T)+i < count)
            {
                x1[i] = TFieldElement(myShares[k*(N-T)+i]);
            }
            else
            {
                x1[i] = *TField::getInstance()->GetZero();
            }
        }

        cout << "x1[i]" << '\n';

        for(int i = 0; i < N; i++)
        {
            cout << "x1[i] : " << i <<  "  : " << x1[i].toString() << '\n';
        }



        cout << "before mult y1[i] : " << endl;

        for(int i = 0; i < T; i++)
        {
            cout << "y1[i] : " << i <<  "  : " << y1[i].toString() << '\n';
        }

        // compute y = M*x and append it to x
        m.MatrixMult(x1, y1);

        cout << "after mult y1[i] : " << endl;

        for(int i = 0; i < T; i++)
        {
            cout << "y1[i] : " << i <<  "  : " << y1[i].toString() << '\n';
        }

        for(int i = 0; i < T; i++)
        {
            x1[N-T+i] = y1[i];
        }


        cout << "x1[i]" << '\n';

        for(int i = 0; i < N; i++)
        {
            cout << "x1[" << i << "] : " << x1[i].toString() << '\n';
        }


        // ∀i, j: Pi sends xj to Pj
        for(int i = 0; i < N; i++)
        {
            sendBufs[i] += x1[i].toString() + "*";
        }
    }

    cout << " send buf send buf" << '\n';
    cout<< "---------------------------------" << '\n';

    for(int i = 0; i < N; i++)
    {
        cout<<sendBufs[i] << '\n';
    }
    cout<< "---------------------------------" << '\n';







    cout << "before sendRecon" << '\n';
    Communication::getInstance()->sendRecon(sendBufs, recBufs);

    cout << "after sendRecon" << '\n';

    cout << "rec buf" << '\n';
    cout<< "---------------------------------" << '\n';

    for(int i = 0; i < N; i++)
    {
        cout<<recBufs[i] << '\n';
    }
    cout<< "---------------------------------" << '\n';


    for(int k=0; k < no_buckets; k++) {

        cout << "before {xj}i : x1[i]" << '\n';

        for(int i = 0; i < N; i++)
        {
            cout << "x1[" << i << "] : " << x1[i].toString() << '\n';
        }

        for (int i = 0; i < N; i++) {
            vector<string> arr = {};
            arr = split(recBufs[i], '*');
            x1[i] = TFieldElement(arr[k]);
            cout << "x1 " << x1[i].toString() << '\n';
        }

        cout << "after {xj}i : x1[i]" << '\n';

        for(int i = 0; i < N; i++)
        {
            cout << "x1[" << i << "] : " << x1[i].toString() << '\n';
        }

        // checking that {xj}i are d-consistent and interpolate them to x j .
        if (!checkConsistency(alpha, x1, d)) {
            // halt
            // cheating detected
            cout << "cheating" << '\n';
        }

        // interpolate {xj}i to x
        x = interpolate(alpha, x1);

        // send x to all parties
        for (int i = 0; i < N; i++) {
            sendBufs2[i] += x.toString() + "*";
        }
    }

    cout << "before roundfunction10" << '\n';


    for(int i = 0; i < N; i++)
    {
        cout<<sendBufs2[i] << '\n';
    }
    cout<< "---------------------------------" << '\n';

     Communication::getInstance()->roundfunction10(sendBufs2, recBufs2);

    cout << "                           after roundfunction10" << '\n';


    for(int i = 0; i < N; i++)
    {
        cout<<recBufs2[i] << '\n';
    }
    cout<< "---------------------------------" << '\n';

    int index = 0;
    for(int k=0; k < no_buckets; k++) {
        for (int i = 0; i < N; i++) {
            vector<string> arr = {};
            arr = split(recBufs2[i], '*');
            x1[i] = TFieldElement(arr[k]);
        }
//        for (int i = N-T; i < N; i++) {
//            x1[i] = TFieldElement("[]");
//        }

        cout<< "y before mult" << '\n';
        for(int i = 0; i < T; i++)
        {
            cout<< y1[i].toString() << '\n';
        }

        cout<< "x before mult" << '\n';
        for(int i = 0; i <N; i++)
        {
            cout<< x1[i].toString() << '\n';
        }

        // checking that (Xn−t,...,Xn) = M*(X1,...,Xn−t)
        m.MatrixMult(x1, y1);

        cout<< "after mult" << '\n';
        for(int i = 0; i < T; i++)
        {
            cout<< y1[i].toString() << '\n';
        }

        for (int i = 0; i < T; i++) {
            if(x1[N-T+i].toString() != y1[i].toString())
            {
                // halt !
                cout << "cheating" << '\n';
            }
        }

        for(int i = 0; i < N; i++)
        {
            cout <<  " this is the final x vector to myval :  : ::::   x1[i] : " << i <<  "  : " << x1[i].toString() << '\n';
        }

        for (int i = 0; i < N-T; i++) {
            if(k*(N-T)+i < count)
            {
                valBuf[index] = x1[i];
                index++;
            }
        }
    }

}

bool Protocol::preparationPhase(VDM &matrix_vand, HIM &matrix_him, vector<string> &sharingBuf, vector<TFieldElement> &alpha, ArythmeticCircuit &circuit)
{
    vector<string> recBufs;
    recBufs.resize(N);
    int robin = 0;
    string strX1, strX2, str;

    // 0 -> random gate
    int no_random = circuit.getNrOfMultiplicationGates() + circuit.getNrOfInputGates();
    vector<TFieldElement> x1,x2,y1,y2;
    vector<string> sendBufs;

    x1.resize(N);
    x2.resize(N);
    y1.resize(N);
    y2.resize(N);

    // the number of buckets (each bucket requires one double-sharing
    // from each party and gives N-2T random double-sharings)
    int no_buckets = (no_random / (N-2*T))+1;

    sharingBuf.resize(no_buckets*(N-2*T));

    sendBufs.resize(N);
    for(int i=0; i < N; i++)
    {
        sendBufs[i] = "";
    }

    for(int k=0; k < no_buckets; k++)
    {
        // generate random degree-T polynomial
        for(int i = 0; i < T+1; i++)
        {
            x1[i] = TField::getInstance()->Random();
        }

        // same secret
        TFieldElement secret;
        secret.setPoly(x1[0].getElement());
        //x2[0] = x1[0];
        x2[0] = secret;
        for(int i = 1; i < 2*T+1; i++)
        {
            x2[i] = TField::getInstance()->Random();
        }





        /**
         * delete!!!!
         */

//        x2[0] = TField::getInstance()->GetElement(5);
//        x1[0] = TField::getInstance()->GetElement(5);

        cout << "                        the secret is  : " << x1[0].getElement() << '\n';
        cout << "                        the secret is  : " << x2[0].getElement() << '\n';

        /**
         * delete!!!!
         */


        cout <<"x1 " << endl;
        cout << "------------------------------ "<< '\n';
        for(int i=0; i < N; i++)
        {
            cout << x1[i].toString() << '\n';
        }
        cout <<"x2" << endl;
        cout << "------------------------------ "<< '\n';
        for(int i=0; i < N; i++)
        {
            cout << x2[i].toString() << '\n';
        }

        cout << "------------------------------ "<< '\n';

        matrix_vand.MatrixMult(x1, y1);
        matrix_vand.MatrixMult(x2, y2);

        cout << "------------------------------ "<< '\n';
        for(int i=0; i < N; i++)
        {
            cout << y1[i].toString() << '\n';
        }

        cout << "------------------------------ "<< '\n';
        for(int i=0; i < N; i++)
        {
            cout << y2[i].toString() << '\n';
        }

        cout << "------------------------------ "<< '\n';


        for(int i=0; i < N; i++)
        {
            sendBufs[i] += y1[i].toString() + "*";
            sendBufs[i] += y2[i].toString() + "$";
        }

    }

    Communication::getInstance()->sendPartOfPoly(sendBufs,recBufs);

    vector<string> sendBufs1(N);
    for(int i=0; i < N; i++)
    {
        sendBufs1[i] = "";

    }

    for(int k=0; k < no_buckets; k++) {
        // generate random degree-T polynomial
        for (int i = 0; i < N; i++) {
            str = recBufs[i];

            vector<string> arr1 = {};
            arr1 = split(str, '$');

            vector<string> arr = {};
            arr = split(arr1[k], '*');
            if(arr.size() >1 ) {
                strX1 = arr[0];
                strX2 = arr[1];
                x1[i] = TFieldElement(strX1);
                x2[i] = TFieldElement(strX2);
//
//                cout<< "strX1 " << strX1 << '\n';
//                cout<< "strX2 " << strX1 << '\n';
            }
        }
        matrix_him.MatrixMult(x1, y1);
        matrix_him.MatrixMult(x2, y2);
        for (int i = 0; i < 2 * T; i++) {
            sendBufs1[robin] += y1[i].toString() + "*";
            sendBufs1[robin] += y2[i].toString() + "$";
            robin = (robin+1)%N;
        }
        // Y1 : the degree-t shares of my poly
        // Y2 : the degree 2t shares of my poly
        for (int i = 2 * T; i < N; i++) {
            sharingBuf[k*(N-2*T) + i - 2*T] = y1[i].toString() + "*" + y2[i].toString();
        }
    }

    Communication::getInstance()->sendDoubleShare(sendBufs1,recBufs);

    int count = no_buckets * (2*T) / N;
    if(no_buckets * (2*T)%N > m_partyId)
    {
        count++;
    }

    for(int k=0; k < count; k++) {
        for (int i = 0; i < N; i++) {

            str = recBufs[i];

            vector<string> arr1 = {};
            arr1 = split(str, '$');

            vector<string> arr = {};
            arr = split(arr1[k], '*');
            if (arr.size() == 2) {
                strX1 = arr[0];
                strX2 = arr[1];
            } else {
                // change !!!!
                strX1 = "";
                strX2 = "";
            }

            x1[i] = TFieldElement(strX1);
            x2[i] = TFieldElement(strX2);
        }

        if(!checkConsistency(alpha,x1,T) || !checkConsistency(alpha,x1,2*T) || (interpolate(alpha, x1).toString() != interpolate(alpha, x2).toString()) ) {
            // cheating detected, abort
            return false;
        }
    }
    return true;
}

/**
 * the function implements the first step of Input Phase:
 * for each input gate, a prepared t-sharings is reconstructed
 * towards the party giving input
 */
bool Protocol::inputPreparation(vector<string> &sharingBuf, vector<TFieldElement> &gateShareArr, ArythmeticCircuit &circuit, vector<TFieldElement> &alpha, vector<TFieldElement> &gateValueArr)
{
    vector<string> sendBufs;
    vector<string> recBufs;


    vector<TFieldElement> x1;
    TFieldElement elem;
    int i;
    // upper bound
    sendBufs.resize(N);
    recBufs.resize(N);
    // vector for the shares of my inputs
    x1.resize(N);
    HIM mat(N, N);
    mat.InitHIM();

    for(int k = 0; k < M; k++)
    {
        if((circuit.getGates())[k].gateType == INPUT)
        {
            // use the t-sharing (correction will come)
            string str = sharingBuf[k];

            // change !!!
            if(str!="") {
                vector<string> arr = {};
                arr = split(str, '*');
                // waste the 2T-sharing
                elem = TFieldElement(arr[0]);
            } else {
                elem = TFieldElement("[]");
            }
            gateShareArr[k] = elem;
            i = (circuit.getGates())[k].party;
            // reconstruct sharing towards input party
            sendBufs[i-1] = gateShareArr[k].toString();
        }
    }

    Communication::getInstance()->sendGateShareArr(sendBufs,recBufs);

    // reconstruct my random input values
    TFieldElement secret;
    for(int k = 0; k < M; k++) {
        if (circuit.getGates()[k].gateType == INPUT && circuit.getGates()[k].party == m_partyId) {
            // my input: reconstruct received shares
            // FOR i := 0 TO N-1 DO Read(RecBufs[i],x1[i]);
            //MEITAL
            for (int i = 0; i < N; i++) {
                string str = recBufs[i];
                elem = TFieldElement(str);
                x1[i] = elem;
            }
            if(!checkConsistency(alpha, x1, T))
            {
                // someone cheated!
                return false;
            }
            // the (random) secret
            secret = TFieldElement();
            secret.setPoly(interpolate(alpha, x1).getElement());
            gateValueArr[k] = secret;
        }
    }

    return true;

}

/**
 * Check whether given points lie on polynomial of degree d. This check is performed by interpolating x on
 * the first d + 1 positions of α and check the remaining positions.
 */
bool Protocol::checkConsistency(vector<TFieldElement> alpha, vector<TFieldElement> x, int d)
{
    vector<TFieldElement> alpha_until_d(d+1);
    vector<TFieldElement> y(N-1-d);
    vector<TFieldElement> alpha_from_d(N-1-d);

    for(int i=0; i<d+1; i++)
    {
        // MEITAL !
        //alpha_until_d[i]=(TField::getInstance()->GetElement(i+1));
        alpha_until_d[i]= alpha[i];
    }
    for(int i=d+1; i<N; i++)
    {
       // alpha_from_d[i-(d+1)]=(TField::getInstance()->GetElement(i-(d+1)+1));
        alpha_from_d[i-(d+1)]= alpha[i];
    }
    HIM matrix(N-1-d,d+1);
    matrix.InitHIMByVectors(alpha_until_d, alpha_from_d);
    matrix.MatrixMult(alpha_until_d, y);
    // compare that the result is equal to the according positions in x

    // n-d-2 or n-d-1 ? ?

    for(int i=0; i<N-d-2; i++)
    {
        if(y[i].toString() != alpha_from_d[i].toString())
        {
            return false;
        }
    }
 //   matrix.Print();
    return true;


}

TFieldElement Protocol::interpolate(vector<TFieldElement> alpha, vector<TFieldElement> x)
{
    vector<TFieldElement> beta(1);
    vector<TFieldElement> y(N);
    beta[0] = TField::getInstance()->GetElement(0);
    HIM matrix(1,N);
    matrix.InitHIMByVectors(alpha, beta);
    matrix.MatrixMult(x, y);
    return y[0];
}

/**
 * the function process all addition gates which are ready.
 * @param circuit
 * @param gateDoneArr
 * @param gateShareArr
 * @return the number of processed gates.
 */
int Protocol::processAdditions(ArythmeticCircuit &circuit, vector<bool> &gateDoneArr, vector<TFieldElement> &gateShareArr)
{
    int count =0;

    for(int k=0; k < M; k++)
    {
        // its an addition which not yet processed and ready
        if(circuit.getGates()[k].gateType == ADD && !gateDoneArr[k]
                && gateDoneArr[circuit.getGates()[k].input1]
                && gateDoneArr[circuit.getGates()[k].input2])
        {
            gateShareArr[k] = gateShareArr[circuit.getGates()[k].input1] + gateShareArr[circuit.getGates()[k].input2];
            gateDoneArr[k] = true;
            count++;
        }

    }

    cout << count << '\n';
    cout << "Gatesharearr" << '\n';

    for(int i=0; i < M; i++)
    {
        cout << circuit.getGates()[i].gateType << "   " << gateShareArr[i].getElement() << '\n';
    }

    return count;
}

/**
 * the Function process all multiplications which are ready.
 * @param circuit
 * @param gateDoneArr
 * @param gateShareArr
 * @param sharingBuf
 * @param alpha
 * @return the number of processed gates.
 */
int Protocol::processMultiplications(ArythmeticCircuit &circuit, vector<bool> &gateDoneArr,
                                     vector<TFieldElement> &gateShareArr, vector<string> &sharingBuf,
                                     vector<TFieldElement> &alpha)
{
    int count =0;
    int index = 0;
    TFieldElement p2, d2;
    TFieldElement r1, r2;
    vector<TFieldElement> valBuf(M);
    TFieldElement d;
    int indexForValBuf = 0;
    vector<string> ReconsBuf(M);

    cout << "processMultiplications - gateShareArr : " << endl;

    for(int i=0; i < M; i++)
    {
        cout << circuit.getGates()[i].gateType << "   " << gateShareArr[i].getElement() << '\n';
    }



    for(int k = 0; k < M; k++)
    {
        // its a multiplication which not yet processed and ready
        if(circuit.getGates()[k].gateType == MULT && !gateDoneArr[k]
           && gateDoneArr[circuit.getGates()[k].input1]
           && gateDoneArr[circuit.getGates()[k].input2])
        {
            vector<string> arr = {};
            arr = split(sharingBuf[k], '*');
            // t-share of random r
            r1 = TFieldElement(arr[0]);
            // t2-share of same r
            r2 = TFieldElement(arr[1]);
            p2 = gateShareArr[circuit.getGates()[k].input1] * gateShareArr[circuit.getGates()[k].input2];
            d2 = p2 - r2;
            ReconsBuf[index] = d2.toString();
            index++;
            cout <<"index " << k << "  : " << index << '\n';
            // for now gateShareArr[k] is random sharing, needs to be adjusted (later)
            gateShareArr[k] = r1;
        }

    }

    cout <<"index for publicReconstruction " << index << '\n';

    // publicReconstruction(vector<string> &myShares, int &count, int d, vector<TFieldElement> &alpha, vector<TFieldElement> &valBuf)
    // reconstruct the differences into valBuf
    publicReconstruction(ReconsBuf, index, 2*T, alpha, valBuf);

    cout <<"val buf " << valBuf[0].toString() << '\n';

    for(int k=0; k < M; k++)
    {
        // its a multiplication which not yet processed and ready
        if(circuit.getGates()[k].gateType == MULT && !gateDoneArr[k]
           && gateDoneArr[circuit.getGates()[k].input1]
           && gateDoneArr[circuit.getGates()[k].input2]
              && index > 0   )
        {//

            // the difference
            d = valBuf[indexForValBuf];

            cout << "D : " << d.toString() << endl;

            indexForValBuf++;
            // the adjustment
            gateShareArr[k] = gateShareArr[k] + d;
            gateDoneArr[k] = true;
            count++;
            index--;
        }
    }

    return count;
}


/**
 * the function Walk through the circuit and reconstruct output gates.
 * @param circuit
 * @param gateShareArr
 * @param alpha
 */
void Protocol::outputPhase(ArythmeticCircuit &circuit, vector<TFieldElement> &gateShareArr, vector<TFieldElement> alpha, vector<TFieldElement> &gateValueArr)
{
    int count =0;


    cout << "outputPhase - gateShareArr : " << endl;

    for(int i=0; i < M; i++)
    {
        cout << circuit.getGates()[i].gateType << "   " << gateShareArr[i].getElement() << '\n';
    }



    // vector for the shares of my outputs
    vector<TFieldElement> x1(N);

    vector<string> sendBuf(N);
    vector<string> recBuf(N);

    cout << " start output"<< '\n';

    for(int i=0; i < N; i++)
    {
        sendBuf[i] = "";
    }

    for(int k=0; k < M; k++)
    {
        if(circuit.getGates()[k].gateType == OUTPUT)
        {
            // send to party (which need this gate) your share for this gate
            cout <<  " the share we use :  " << gateShareArr[circuit.getGates()[k].input1].toString() << '\n';
            sendBuf[circuit.getGates()[k].party - 1] = gateShareArr[circuit.getGates()[k].input1].toString();
        }
    }


    Communication::getInstance()->Lastsend(sendBuf,recBuf);

    cout << " after last send" << '\n';


    TFieldElement num;

    string strX1;

    for(int k=0; k < M; k++) {
        if(circuit.getGates()[k].gateType == OUTPUT && circuit.getGates()[k].party == m_partyId)
        {
            for(int i=0; i < N; i++) {
                // change from string to field element
                num = TFieldElement(recBuf[i]);
                x1[i] = num;
            }

            // my output: reconstruct received shares
            if (!checkConsistency(alpha, x1, T))
            {
                cout << "cheating!!!" << '\n';
                return;
            }
            cout << "the result is : " << interpolate(alpha, x1).toString() << '\n';
        }
    }
}

