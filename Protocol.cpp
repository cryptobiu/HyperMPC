#include "Protocol.h"

Protocol::Protocol(int n, int id, string inputsFile, string outputFile)
{
    comm = Communication::getInstance(n, id);
    N = n;
    T = n/3 - 1;
    this->inputsFile = inputsFile;
    this->outputFile = outputFile;
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

/**
 * Protocol Broadcast:
 *  0. Ps holds input vector x = (X1,...,Xn−t).
 *  1. ∀j: Ps sends x to Pj . Denote the received vector as x (j) (P j -th view on the sent vector).
 *  2. ∀j: Pj applies hyper-invertible matrix M and calculate (y1,...,yn) = M(X1,...,Xn−t,0,...,0), padding t zeroes.
 *  3. ∀j,k: Pj sends yk to Pk .
 *  4. ∀k: Pk checks whether all received values are equal. If so, be happy, otherwise cry.
 *
 *  This protocol uses when Ps wants to broadcast exactly n−t values.
 *  if Ps wants more than n-t values we divide the values to buckes.
 *  Every bucket contains n-t values.
 *
 *  @param myMessage = vector of all the values which Ps wants to broadcast.
 *  @param recBufsdiff = the values which received from the protocol.
 */
bool Protocol::broadcast(int party_id, string myMessage, vector<string> &recBufsdiff, HIM &mat)
{
    int no_buckets;
    vector<string> buffers(N);

    string temp = myMessage;
    // delete
    comm->vec[m_partyId - 1] = temp;
    vector<string> recBufs2(N);
    // TO DO : update the size

    // Ps sends his values to all parties and received there values.
    comm->SendXVectorToAllParties1(myMessage, recBufsdiff);

    cout << "recBufsdiff" << endl;
    for (int i=0; i<N; i++)
    {
        cout << i << "  " <<recBufsdiff[i] << endl;
    }

//    HIM mat(N,N);
//    mat.InitHIM();
    vector<TFieldElement> X1(N);
    vector<TFieldElement> Y1(N);

//
//    count := 0;
//    FOR i := 0 TO N-1 DO count := count + Length(RecBufs[i]); // total number of values
//    Reset(ValBuf,count);
//    FOR i := 0 TO N-1 DO
//    WHILE Read(RecBufs[i],x) DO Write(ValBuf,x);
//// concatenate everything
//    no_buckets := count Div (N-T) + 1;
//// nr of buckets


    // calculate total number of values which received
    int count = 0;
    for(int i=0; i< N; i++)
    {
        vector<string> arr = {};
        arr = split(recBufsdiff[i], '*');
        count += arr.size();
    }


    vector<string> valBufs(count);


    int index = 0;
    for(int l=0; l< N; l++)
    {
        cout << "recBufsdiff[l]  "  <<recBufsdiff[l]<< endl;
        vector<string> arr = {};
        arr = split(recBufsdiff[l], '*');
        for (int i = 0; i < arr.size() ; i++) {
            cout << "den" << endl;
            valBufs[index] = arr[i];
            index++;
        }
        cout << "fjfcfmfcmfm" << endl;
    }
    index = 0;
    cout << "valBufs " <<endl;
    for(int k = 0; k < count; k++)
    {
        cout << "valBufs " << k << " " << valBufs[k] << endl;
    }

    // nr of buckets
    no_buckets = count / (N - T) + 1;
    cout << " before the for " << '\n';
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
        for(int i = 0; i < N; i++)
        {
            cout << "X1[i]" << i << " " << X1[i].getElement() << endl;
        }

        mat.MatrixMult(X1, Y1);
        cout << "X1[i] after mult" << endl;
        for(int i = 0; i < N; i++)
        {
            cout << "X1[i]" << i << " " << X1[i].getElement() << endl;
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

    cout  << "before SendTheResult1 " << endl;
    for(int k=0; k< N; k++) {
        cout << k << "  " << buffers[k] << endl;
    }


    comm->SendTheResult1(buffers, recBufs2);

    cout  << "after SendTheResult1 " << endl;
    for(int k=0; k< N; k++) {
        cout << k << "  " << recBufs2[k] << endl;
    }

  //  cout << "after SendTheResult" << '\n';

    cout << "no_buckets  " << no_buckets << endl;
    vector<string> arr = {};
    vector<string> arr1 = {};
    string temp1;
    for(int k=0; k < no_buckets; k++) {

        cout << "fff  " << k<< endl;
        arr = split(recBufs2[0], '*');
        if(arr.size() > 0) {
            temp1 = arr[k];
            //  arr.size()-1
            for (int i = 1; i < arr.size(); i++) {
                arr1 = split(recBufs2[i], '*');
                if(temp1 != arr1[k])
                {
                    cout << "                 cheating" << endl;
                    return false;
                }
            }
        }
    }

    return true;

}

void Protocol::run() {

    GF2X irreduciblePolynomial = BuildSparseIrred_GF2X(8);
    GF2E::init(irreduciblePolynomial);

//   ArithmeticCircuit circuit(1);
    ArithmeticCircuit circuit;
    circuit.readCircuit("/home/hila/ClionProjects/Secret-Sharing/test1.txt");
  //  cout << "size of circuit" << circuit.getGates().size();

    M = circuit.getNrOfGates();

    // the value of the gate (for my input and output gates)
    vector<TFieldElement> gateValueArr(M);
    // my share of the gate (for all gates)
    vector<TFieldElement> gateShareArr(M);
    vector<TFieldElement> alpha(N);
    // true is the gate is processed
    vector<bool> gateDoneArr(M);
    HIM matrix_him(N,N);
    VDM matrix_vand(N,N);
    HIM m(T, N-T);
    matrix_vand.InitVDM();
    matrix_him.InitHIM();
    vector<string> sharingBuf;

    string s = to_string(m_partyId);

    comm->ConnectionToServer(comm->m_client, s, comm->s3, comm->m_pubmsg, comm->m_token);


    initializationPhase(gateValueArr, gateShareArr, gateDoneArr, matrix_him, matrix_vand, alpha, m);

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

    auto t1 = high_resolution_clock::now();

    inputAdjustment(sss, gateValueArr, circuit, gateShareArr, gateDoneArr, matrix_him);

    cout << "after Input Adjustment " << '\n';

    int count =0;
    do {
        count = processAdditions(circuit,gateDoneArr, gateShareArr);
        count += processMultiplications(circuit, gateDoneArr, gateShareArr, sharingBuf, alpha, m);

    } while(count!=0);

   outputPhase(circuit, gateShareArr, alpha, gateValueArr);

    cout << "after output Phase " << '\n';
    auto t2 = high_resolution_clock::now();

    auto duration = duration_cast<milliseconds>(t2-t1).count();

    cout << "time in milliseconds : " << duration << endl;
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
void Protocol::inputAdjustment(string &diff, vector<TFieldElement> &gateValueArr, ArithmeticCircuit &circuit,
                               vector<TFieldElement> &gateShareArr, vector<bool> &gateDoneArr, HIM &mat)
{
    int input;
    ifstream myfile;

    myfile.open(inputsFile);

    // Ask Party for inputs
    for (int k = 0; k < M; k++)
    {
        if(circuit.getGates()[k].gateType == INPUT && circuit.getGates()[k].party == m_partyId)
        {
        //    cout << "enter your real input : " << '\n';

            myfile >> input;

            cout << "input  " << input << endl;
            // Ok, the value is GateValue[k], but should be input.
            TFieldElement myinput = TField::getInstance()->GetElementByValue(input);

            cout << "gateValueArr "<<k<< "   " << gateValueArr[k].toString() << endl;

            TFieldElement different = myinput - gateValueArr[k];
         //  cout << different.getElement()<< '\n';
            string str = different.toString();

            diff += str + "*";

            // i need to concatenate the difference
            // this can abort in case of cheating
        }
    }

    myfile.close();

    cout << "try to print diff" << '\n';
   cout << diff << '\n';

    vector<string> recBufsdiff(N);

 //  cout << "malloc is fine " << '\n';
    // Broadcast the difference between GateValue[k] to x.
    // start broadcast if cheating - halt
    if(broadcast(m_partyId, diff, recBufsdiff, mat) == false) {
        cout << "cheating!!!" << '\n';
        return;
    }
    else {
        cout << "no cheating!!!" <<  '\n' << "finish Broadcast" << '\n';
    }

    cout << "recBufsdiff" << endl;
    for (int k = 0; k < N; k++)
    {
        cout << "recBufsdiff" << k << "  "<< recBufsdiff[k] <<  endl;
    }

    // handle after broadcast
    string str;
    TFieldElement db;
 //   cout<<"m " << M << '\n';
    vector<string> arr = {};
    for (int k = 0; k < M; k++)
    {
        if(circuit.getGates()[k].gateType == INPUT)
        {
            str = recBufsdiff[circuit.getGates()[k].party - 1];

            arr = split(str, '*');
        //    for(int i=0; i<arr.size(); i++) {
            cout <<"before" << recBufsdiff[circuit.getGates()[k].party - 1] << endl;
            db = TFieldElement(arr[0]);
            cout << "db" << db.toString() << endl;
            gateShareArr[k] = gateShareArr[k] + db;
            recBufsdiff[circuit.getGates()[k].party - 1] = "";
            for(int i=1; i<arr.size(); i++) {
                recBufsdiff[circuit.getGates()[k].party - 1] += arr[i] + "*";
            }
            cout <<"after" << recBufsdiff[circuit.getGates()[k].party - 1] << endl;
       //     }
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
                                   HIM &matrix_him, VDM &matrix_vand, vector<TFieldElement> &alpha, HIM &m)
{

    vector<TFieldElement> alpha1(N-T);
    vector<TFieldElement> alpha2(T);

    // Compute Vandermonde matrix VDM
    matrix_vand.InitVDM();
    matrix_vand.Print();

    // Prepare an N-by-N hyper-invertible matrix
    matrix_him.InitHIM();


    for(int i=0; i<N; i++)
    {
        alpha[i]=(TField::getInstance()->GetElementByValue(i+1));
    }

    for(int i = 0; i < N-T; i++)
    {
        alpha1[i] = alpha[i];
    }
    for(int i = N-T; i < N; i++)
    {
        alpha2[i - (N-T)] = alpha[i];
    }

    m.InitHIMByVectors(alpha1, alpha2);

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
void Protocol::publicReconstruction(vector<string> &myShares, int &count, int d, vector<TFieldElement> &alpha, vector<TFieldElement> &valBuf, HIM &m)
{
    int no_buckets = count / (N-T) + 1;

    cout << "public reconstruction" << endl;
    cout << "no buckets" << no_buckets << endl;
    TFieldElement x;

    vector<TFieldElement> x1(N);
    vector<TFieldElement> y1(N);
    vector<TFieldElement> y2(N);
    vector<string> sendBufs(N);
    vector<string> sendBufs2(N);
    vector<string> recBufs(N);
    vector<string> recBufs2(N);

    for(int i = 0; i < N; i++)
    {
        recBufs[i] = "";
        recBufs2[i] = "";
        sendBufs[i] = "";
        sendBufs2[i] = "";
    }

    for(int i=0; i < myShares.size(); i++)
    {
        cout << "myShares " << i << "   " << myShares[i] << endl;
    }

    // init x to be vector of degree-d (d=2*t) shares of n−t secret
    for(int k=0; k < no_buckets; k++)
    {
        for(int i = 0; i < N-T; i++)
        {
            if( k*(N-T)+i < count)
            {
                // k*(N-T)+i
                x1[i] = TFieldElement(myShares[k*(N-T)+i]);
            }
            else
            {
                x1[i] = *TField::getInstance()->GetZero();
            }
        }

        // compute y = M*x and append it to x
        m.MatrixMult(x1, y1);

        for(int i = 0; i < T; i++)
        {
            x1[N-T+i] = y1[i];
        }

        // ∀i, j: Pi sends xj to Pj
        for(int i = 0; i < N; i++)
        {
            sendBufs[i] += x1[i].toString() + "*";
        }
    }

    cout << "sendBufs[i]" << endl;
    for(int i = 0; i < N; i++)
    {
        cout << sendBufs[i] << endl;
    }

 //   cout << "before sendRecon" << '\n';
    comm->sendRecon(sendBufs, recBufs);

    cout << "recBufs[i]" << endl;
    for(int i = 0; i < N; i++)
    {
        cout << recBufs[i] << endl;
    }
 //   cout << "after sendRecon" << '\n';
    vector<string> arr = {};
    for(int k=0; k < no_buckets; k++) {

        for (int i = 0; i < N; i++) {

            arr = split(recBufs[i], '*');
            x1[i] = TFieldElement(arr[k]);
        }

        cout << "x1[i]" << endl;
        for(int i = 0; i < N; i++)
        {
            cout << x1[i].toString() << endl;
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

    cout << "sendBufs2[i]" << endl;
    for(int i = 0; i < N; i++)
    {
        cout << sendBufs2[i] << endl;
    }
     comm->roundfunction10(sendBufs2, recBufs2);

    cout << "recBufs2[i]" << endl;
    for(int i = 0; i < N; i++)
    {
        cout << recBufs2[i] << endl;
    }
//    vector<string> arr = {};
    int index = 0;
    for(int k=0; k < no_buckets; k++) {
        for (int i = 0; i < N; i++) {

            arr = split(recBufs2[i], '*');
            x1[i] = TFieldElement(arr[k]);
        }

        // checking that (Xn−t,...,Xn) = M*(X1,...,Xn−t)
        m.MatrixMult(x1, y1);

        for (int i = 0; i < T; i++) {
            if(x1[N-T+i].toString() != y1[i].toString())
            {
                // halt !
                cout << "                  cheating" << '\n';
            }
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

bool Protocol::preparationPhase(VDM &matrix_vand, HIM &matrix_him, vector<string> &sharingBuf, vector<TFieldElement> &alpha, ArithmeticCircuit &circuit)
{
    vector<string> recBufs(N);
    int robin = 0;
    string strX1, strX2, str;

    // 0 -> random gate
    int no_random = circuit.getNrOfMultiplicationGates() + circuit.getNrOfInputGates();
    vector<TFieldElement> x1(N),x2(N),y1(N),y2(N);
    vector<string> sendBufs(N);

    // the number of buckets (each bucket requires one double-sharing
    // from each party and gives N-2T random double-sharings)
    int no_buckets = (no_random / (N-2*T))+1;

    sharingBuf.resize(no_buckets*(N-2*T));

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

//        cout << "                        the secret is  : " << x1[0].getElement() << '\n';
//        cout << "                        the secret is  : " << x2[0].getElement() << '\n';

        /**
         * delete!!!!
         */


//        cout <<"x1 " << endl;
//        cout << "------------------------------ "<< '\n';
//        for(int i=0; i < N; i++)
//        {
//            cout << x1[i].toString() << '\n';
//        }
//        cout <<"x2" << endl;
//        cout << "------------------------------ "<< '\n';
//        for(int i=0; i < N; i++)
//        {
//            cout << x2[i].toString() << '\n';
//        }

    //    cout << "------------------------------ "<< '\n';

        matrix_vand.MatrixMult(x1, y1);
        matrix_vand.MatrixMult(x2, y2);

//        cout << "------------------------------ "<< '\n';
//        for(int i=0; i < N; i++)
//        {
//            cout << y1[i].toString() << '\n';
//        }
//
//        cout << "------------------------------ "<< '\n';
//        for(int i=0; i < N; i++)
//        {
//            cout << y2[i].toString() << '\n';
//        }
//
//        cout << "------------------------------ "<< '\n';


        for(int i=0; i < N; i++)
        {
            sendBufs[i] += y1[i].toString() + "*";
            sendBufs[i] += y2[i].toString() + "$";
        }

    }

    comm->sendPartOfPoly(sendBufs,recBufs);

    vector<string> sendBufs1(N);
    for(int i=0; i < N; i++)
    {
        sendBufs1[i] = "";

    }


    vector<string> arr1 = {};
    vector<string> arr = {};
    for(int k=0; k < no_buckets; k++) {
        // generate random degree-T polynomial
        for (int i = 0; i < N; i++) {
            str = recBufs[i];

            arr1 = split(str, '$');


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

    comm->sendDoubleShare(sendBufs1,recBufs);

    int count = no_buckets * (2*T) / N;
    if(no_buckets * (2*T)%N > m_partyId)
    {
        count++;
    }

    for(int k=0; k < count; k++) {
        for (int i = 0; i < N; i++) {

            str = recBufs[i];

           // vector<string> arr1 = {};
            arr1 = split(str, '$');

           // vector<string> arr = {};
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
bool Protocol::inputPreparation(vector<string> &sharingBuf, vector<TFieldElement> &gateShareArr, ArithmeticCircuit &circuit, vector<TFieldElement> &alpha, vector<TFieldElement> &gateValueArr)
{
    // upper bound
    vector<string> sendBufs(N);
    vector<string> recBufs(N);

    // vector for the shares of my inputs
    vector<TFieldElement> x1(N);

    TFieldElement elem;
    int i;




    for(int k = 0; k < M; k++)
    {
        if((circuit.getGates())[k].gateType == INPUT)
        {
            // use the t-sharing (correction will come)
            string str = sharingBuf[k];
         //   arr = split(sharingBuf[k], '*');
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
            sendBufs[i-1] += gateShareArr[k].toString() + "*";

        }
    }

    cout << "sendBufs[i] in input preperation" << endl;
    for(int i=0; i< N; i++)
    {
        cout << sendBufs[i] << endl;
    }

    comm->sendGateShareArr(sendBufs,recBufs);


    for(int k = 0; k < N; k++) {
        cout << "sendGateShareArr recBufs" << k << " " << recBufs[k] << endl;
    }

    // reconstruct my random input values
    TFieldElement secret;

    for(int k = 0; k < M; k++) {
        if (circuit.getGates()[k].gateType == INPUT && circuit.getGates()[k].party == m_partyId) {
            // my input: reconstruct received shares
            // FOR i := 0 TO N-1 DO Read(RecBufs[i],x1[i]);
            //MEITAL
            for (int i = 0; i < N; i++) {
//                string str = recBufs[i];
//                elem = TFieldElement(str);
//                x1[i] = elem;

                vector<string> arr = {};
                arr = split(recBufs[i], '*');
                elem = TFieldElement(arr[0]);
                x1[i] = elem;

                recBufs[i]="";
                for(int l=1; l<arr.size(); l++)
                {
                    recBufs[i] += arr[l] + "*";
                }

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

            cout << "           the secret is " << secret.toString() << endl;
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
int Protocol::processAdditions(ArithmeticCircuit &circuit, vector<bool> &gateDoneArr, vector<TFieldElement> &gateShareArr)
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

//    cout << count << '\n';
//    cout << "Gatesharearr" << '\n';

//    for(int i=0; i < M; i++)
//    {
//        cout << circuit.getGates()[i].gateType << "   " << gateShareArr[i].getElement() << '\n';
//    }

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
int Protocol::processMultiplications(ArithmeticCircuit &circuit, vector<bool> &gateDoneArr,
                                     vector<TFieldElement> &gateShareArr, vector<string> &sharingBuf,
                                     vector<TFieldElement> &alpha, HIM &m)
{
    int count =0;
    int index = 0;
    TFieldElement p2, d2;
    TFieldElement r1, r2;
    vector<TFieldElement> valBuf(M);
    TFieldElement d;
    int indexForValBuf = 0;
    vector<string> ReconsBuf(M);

 //   cout << "processMultiplications - gateShareArr : " << endl;

//    for(int i=0; i < M; i++)
//    {
//        cout << circuit.getGates()[i].gateType << "   " << gateShareArr[i].getElement() << '\n';
//    }


    vector<string> arr = {};
    for(int k = 0; k < M; k++)
    {
        // its a multiplication which not yet processed and ready
        if(circuit.getGates()[k].gateType == MULT && !gateDoneArr[k]
           && gateDoneArr[circuit.getGates()[k].input1]
           && gateDoneArr[circuit.getGates()[k].input2])
        {

            arr = split(sharingBuf[k], '*');
            // t-share of random r
            r1 = TFieldElement(arr[0]);
            // t2-share of same r
            r2 = TFieldElement(arr[1]);
            p2 = gateShareArr[circuit.getGates()[k].input1] * gateShareArr[circuit.getGates()[k].input2];
            d2 = p2 - r2;
            ReconsBuf[index] = d2.toString();
            index++;
    //        cout <<"index " << k << "  : " << index << '\n';
            // for now gateShareArr[k] is random sharing, needs to be adjusted (later)
            gateShareArr[k] = r1;
        }

    }



    // publicReconstruction(vector<string> &myShares, int &count, int d, vector<TFieldElement> &alpha, vector<TFieldElement> &valBuf)
    // reconstruct the differences into valBuf
    if(index == 0)
    {
        return 0;
    }
    cout <<"index for publicReconstruction " << index << '\n';
    publicReconstruction(ReconsBuf, index, 2*T, alpha, valBuf, m);

  //  cout <<"val buf " << valBuf[0].toString() << '\n';

    for(int k=M-1; k >= 0; k--)
    {
        // its a multiplication which not yet processed and ready
        if(circuit.getGates()[k].gateType == MULT && !gateDoneArr[k]
           && gateDoneArr[circuit.getGates()[k].input1]
           && gateDoneArr[circuit.getGates()[k].input2])
        {//

            // the difference
            d = valBuf[indexForValBuf];

  //          cout << "D : " << d.toString() << endl;

            indexForValBuf++;
            // the adjustment
            gateShareArr[k] = gateShareArr[k] + d;
            gateDoneArr[k] = true;
            count++;
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
void Protocol::outputPhase(ArithmeticCircuit &circuit, vector<TFieldElement> &gateShareArr, vector<TFieldElement> alpha, vector<TFieldElement> &gateValueArr)
{
    int count =0;


 //   cout << "outputPhase - gateShareArr : " << endl;

//    for(int i=0; i < M; i++)
//    {
//        cout << circuit.getGates()[i].gateType << "   " << gateShareArr[i].getElement() << '\n';
//    }



    // vector for the shares of my outputs
    vector<TFieldElement> x1(N);

    vector<string> sendBuf(N);
    vector<string> recBuf(N);

//    cout << " start output"<< '\n';

    for(int i=0; i < N; i++)
    {
        sendBuf[i] = "";
    }

    for(int k=0; k < M; k++)
    {
        if(circuit.getGates()[k].gateType == OUTPUT)
        {
            // send to party (which need this gate) your share for this gate
  //          cout <<  " the share we use :  " << gateShareArr[circuit.getGates()[k].input1].toString() << '\n';
            sendBuf[circuit.getGates()[k].party - 1] = gateShareArr[circuit.getGates()[k].input1].toString();
        }
    }


    comm->Lastsend(sendBuf,recBuf);

 //   cout << " after last send" << '\n';


    TFieldElement num;

    ofstream myfile;
    myfile.open(outputFile);

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
            myfile << interpolate(alpha, x1).toString();

        }
    }
    myfile.close();
}

Protocol::~Protocol()
{
    delete comm;
}