#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#include "stdlib.h"
#include "../../workspace/paho/src/MQTTClient.h"
#include "VDM.h"
#include "HIM.h"
#include "TGate.h"
#include "ArithmeticCircuit.h"
#include "Communication.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <chrono>
#include "Def.h"
#include "TemplateField.h"

using namespace std;
using namespace std::chrono;

template <class FieldType>
class Protocol {
private:
    /**
     * N - number of parties
     * M - number of gates
     * T - number of malicious
     */
    int N, M, T, m_partyId;
    string inputsFile, outputFile, ADDRESS;
    vector<FieldType> beta;
    HIM<FieldType> matrix_for_interpolate;
    HIM<FieldType> matrix_for_t;
    HIM<FieldType> matrix_for_2t;
    TemplateField<FieldType> *field;

    Communication* comm;
    ArithmeticCircuit circuit;
    vector<FieldType> gateValueArr; // the value of the gate (for my input and output gates)
    vector<FieldType> gateShareArr; // my share of the gate (for all gates)
    vector<FieldType> alpha; // N distinct non-zero field elements
    vector<bool> gateDoneArr; // true is the gate is processed
    vector<string> sharingBuf; // prepared double-sharings (my shares)
    vector<int> myInputs;
    string s;

public:
    Protocol(int n, int id,TemplateField<GF2E> *field, string inputsFile, string outputFile, string circuitFile, string address);
    void split(const string &s, char delim, vector<string> &elems);
    vector<string> split(const string &s, char delim);

    /**
     * This method runs the protocol:
     * 1. Initialization Phase
     * 2. Preparation Phase
     * 3. Input Phase
     * 4. Computation Phase
     * 5. Output Phase
     */
    void run();

    /**
     * This method reads text file and inits a vector of Inputs according to the file.
     */
    void readMyInputs();

    /**
     * We describe the protocol initialization.
     * In particular, some global variables are declared and initialized.
     */
    void initializationPhase(HIM<FieldType> &matrix_him, VDM<FieldType> &matrix_vand, HIM<FieldType> &m);

    /**
     * A random double-sharing is a pair of two sharings of the same random value, where the one sharing is
     * of degree t, and the other sharing is of degree 2t. Such random double-sharing are of big help in the
     * multiplication protocol.
     * We use hyper-invertible matrices to generate random double-sharings. The basic idea is as follows:
     * Every party generates one random double-sharing. These n double-sharings are processes through a
     * hyper-invertible matrix. From the resulting n double-sharings, t are checked to be valid (correct degree,
     * same secret), and t are then kept as “good” double-sharings. This is secure due to the diversion property
     * of hyper-invertible matrix: We know that n − t of the input double-sharings are good. So, if there are t
     * valid output double-sharings, then all double-sharings must be valid. Furthermore, the adversary knows
     * his own up to t input double-sharings, and learns t output double sharings. So, n − 2t output double
     * sharings are random and unknown to the adversary.
     * For the sake of efficiency, we do not publicly reconstruct t of the output double-sharings. Rather, we
     * reconstruct 2t output double sharings, each to one dedicated party only. At least t of these parties are
     * honest and correctly validate the reconstructed double-sharing.
     *
     * The goal of this phase is to generate “enough” double-sharings to evaluate the circuit. The double-
     * sharings are stored in a buffer SharingBuf , where alternating a degree-t and a degree-2t sharing (of the same secret)
     * is stored (more precisely, a share of each such corresponding sharings is stored).
     * The creation of double-sharings is:
     *
     * Protocol Generate-Double-Sharings:
     * 1. ∀i: Pi selects random value x-(i) and computes degree-t shares x1-(i) and degree-2t shares x2-(i).
     * 2. ∀i,j: Pi sends the shares x1,j and X2,j to party Pj.
     * 3. ∀j: Pj applies a hyper-invertible matrix M on the received shares, i.e:
     *      (y1,j,..., y1,j) = M(x1,j,...,x1,j)
     *      (y2,j,...,y2,j) = M (x2,j,...,x2,)
     * 4. ∀j, ∀k ≤ 2t: Pj sends y1,j and y2,j to Pk.
     * 5. ∀k ≤ 2t: Pk checks:
     *      • that the received shares (y1,1,...,y1,n) are t-consistent,
     *      • that the received shares (y2,1,...,y2,n) are 2t-consistent, and
     *      • that both sharings interpolate to the same secret.
     *
     * We use this algorithm, but extend it to capture an arbitrary number of double-sharings.
     * This is, as usual, achieved by processing multiple buckets in parallel.
     */
    bool preparationPhase(VDM<FieldType> &matrix_vand, HIM<FieldType> &matrix_him);

    /**
     * We do not need robust broadcast (which we require an involved and expensive sub-protocol).
     * As we allow abort, broadcast can be achieved quite easily.
     * Note that the trivial approach (distribute value, one round of echoing) requires quadratic communication,
     * which is too much.
     * Goal: One party Ps wants to broadcast n−t values x = (x1,...,xn−t).
     *
     * Protocol Broadcast:
     * 0. Ps holds input vector x = (x1,...,xn−t).
     * 1. ∀j: Ps sends x to Pj. Denote the received vector as x(j) (P j -th view on the sent vector).
     * 2. ∀j: Pj applies hyper-invertible matrix M : (y1(j),...,yn(j))= M*(x1(j),..,xn−t(j),0,...,0).
     * 3. ∀j,k: Pj sends yk to Pk.
     * 4. ∀k: Pk checks whether all received values {yk(j)}j are equal. If so, be happy, otherwise cry.
     */
    bool broadcast(int party_id, string myMessage, vector<string> &recBufsdiff, HIM<FieldType> &mat);

    /**
     * For multiplication and for output gates, we need public reconstruction of sharings (degree t and degree 2t).
     * The straight-forward protocol requires n^2 communication, which is too slow.
     * We present a protocol which efficiently reconstructs n − t sharings. The basic idea is to compute t shared
     * authentication checks and to reconstruct the n sharings, one towards each party, who then computes the
     * secret and sends it to everybody. Each party receives n − t secrets and t authentication checks.
     *
     * Protocol Public-Reconstruction:
     * 0. Every party Pi holds a vector x(i) of degree-d shares of n−t secret values x. Let M be a t-by-(n−t)
     *  hyper-invertible matrix.
     * 1. ∀i: Compute y(i) = M*x(i) and append it to x(i). Note that this is now a vector of length n.
     * 2. ∀i,j: Pi sends xj to Pj.
     * 3. ∀j: Pj checks that {xj}i are d-consistent (otherwise cry), and interpolate them to xj.
     * 4. ∀j,i: Pj sends xj to Pi.
     * 5. ∀i: Pi checks that (xn−t,...,xn) = M*(x1,...,xn−t), otherwise cry.
     *
     * However, in our protocol, arbitrary many sharings can be reconstructed.
     * This is achieved by dividing the sharings into buckets of size n − t.
     */
    void publicReconstruction(vector<string> &myShares, int &count, int d, vector<FieldType> &valBuf, HIM<FieldType> &m);

    /**
     * The input phase proceeds in two steps: input preparation and input adjustment
     * First, input preparation -
     *      for each input gate, a prepared t-sharings is reconstructed towards the party giving input.
     * Then, input adjustment -
     *      the party broadcasts for each input gate the difference between the random secret and the actual input value.
     *
     * Note that the first step can still be performed in to offline phase.
     */
    bool inputPreparation();

    /**
      * The input phase proceeds in two steps: input preparation and input adjustment
      * First, input preparation -
      *      for each input gate, a prepared t-sharings is reconstructed towards the party giving input.
      * Then, input adjustment -
      *      the party broadcasts for each input gate the difference between the random secret and the actual input value.
      *
      * Note that the first step can still be performed in to offline phase.
      */
    void inputAdjustment(string &diff, HIM<FieldType> &mat);

    /**
     * Check whether given points lie on polynomial of degree d.
     * This check is performed by interpolating x on the first d + 1 positions of α and check the remaining positions.
     */
    bool checkConsistency(vector<FieldType>& x, int d);

    /**
     * Process all additions which are ready.
     * Return number of processed gates.
     */
    int processAdditions();

    /**
     * Process all multiplications which are ready.
     * Return number of processed gates.
     */
    int processMultiplications(HIM<FieldType> &m);

    /**
     * Process all random gates.
     */
    void processRandoms();

    int processSmul();

    /**
     * Walk through the circuit and evaluate the gates. Always take as many gates at once as possible,
     * i.e., all gates whose inputs are ready.
     * We first process all random gates, then alternately process addition and multiplication gates.
     */
    void computationPhase(HIM<FieldType> &m);

    /**
     * The cheap way: Create a HIM from the αi’s onto ZERO (this is actually a row vector), and multiply
     * this HIM with the given x-vector (this is actually a scalar product).
     * The first (and only) element of the output vector is the secret.
     */
    FieldType interpolate(vector<FieldType> x);

    FieldType tinterpolate(vector<FieldType> x);

    /**
     * Walk through the circuit and reconstruct output gates.
     */
    void outputPhase();

    ~Protocol();
};


template <class FieldType>
Protocol<FieldType>::Protocol(int n, int id, TemplateField<GF2E> *field, string inputsFile, string outputFile, string circuitFile, string address)
{

    this->field = field;
    ADDRESS = address;
    comm = Communication::getInstance(n, id, address);
    N = n;
    T = n/3 - 1;
    this->inputsFile = inputsFile;
    this->outputFile = outputFile;
    if(n%3 > 0)
    {
        T++;
    }
    m_partyId = id;
    s = to_string(m_partyId);
    circuit.readCircuit(circuitFile.c_str());
    M = circuit.getNrOfGates();
    myInputs.resize(M);
}

template <class FieldType>
void Protocol<FieldType>::split(const string &s, char delim, vector<string> &elems) {
    stringstream ss;
    ss.str(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
}

template <class FieldType>
vector<string> Protocol<FieldType>::split(const string &s, char delim) {
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
template <class FieldType>
bool Protocol<FieldType>::broadcast(int party_id, string myMessage, vector<string> &recBufsdiff, HIM<FieldType> &mat)
{
    int no_buckets;
    vector<string> buffers(N); // N parties = N buffers
    vector<string> recBufs2(N);

    // Ps sends his values to all parties and received there values.
    comm->roundfunction2(myMessage, recBufsdiff); // Values are in recBufsdiff

    if(flag_print) {
        cout << "recBufsdiff" << endl;
        for (int i = 0; i < N; i++) {
            cout << i << "  " << recBufsdiff[i] << endl;
        }
    }

    vector<FieldType> X1(N);
    vector<FieldType> Y1(N);

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

    // concatenate everything
    for(int l=0; l< N; l++)
    {
        if(flag_print) {
            cout << "recBufsdiff[l]  "  <<recBufsdiff[l]<< endl; }
        vector<string> arr = {};
        arr = split(recBufsdiff[l], '*');
        for (int i = 0; i < arr.size() ; i++) {
            valBufs[index] = arr[i];
            index++;
        }
    }

    index = 0;

    if(flag_print) {
        cout << "valBufs " <<endl;
        for(int k = 0; k < count; k++)
        {
            cout << "valBufs " << k << " " << valBufs[k] << endl;
        } }

    // nr of buckets
    no_buckets = count / (N - T) + 1; // nr of buckets

    if(flag_print) {
        cout << " before the for " << '\n';}

    for(int k = 0; k < no_buckets; k++)
    {
        for(int i = 0; i < N; i++)
        {
            if((i < N-T) && (k*(N-T)+i < count))
            {
                X1[i]= field->stringToElement(valBufs[index]);
                index++;
            }
            else
            {
                // padding zero
                X1[i] = *(field->GetZero());
            }
        }

        if(flag_print) {
            for(int i = 0; i < N; i++)
            {
                cout << "X1[i]" << i << " " << field->elementToString(X1[i]) << endl;
            } }

        // x1 contains (up to) N-T values from ValBuf
        mat.MatrixMult(X1, Y1); // no cheating: all parties have same y1

        if(flag_print) {
            cout << "X1[i] after mult" << endl;}

        // ‘‘Reconstruct’’ values towards some party (‘‘reconstruct’’ with degree 0)
        if(flag_print) {
            for(int i = 0; i < N; i++)
            {
                cout << "X1[i]" << i << " " << field->elementToString(X1[i])<< endl;
            } }
        for(int i = 0; i < N; i++) {
            buffers[i] += field->elementToString(Y1[i]) + "*";
        }
        for(int i = 0; i < N; i++)
        {
            X1[i] = *(field->GetZero());
            Y1[i] = *(field->GetZero());
        }
    }
    if(flag_print) {
        cout << "index  2 time :" << index << '\n';

        cout  << "before roundfunction3 " << endl;
        for(int k=0; k< N; k++) {
            cout << k << "  " << buffers[k] << endl;
        }}

    comm->roundfunction3(buffers, recBufs2);

    if(flag_print) {
        cout  << "after roundfunction3 " << endl;
        for(int k=0; k< N; k++) {
            cout << k << "  " << recBufs2[k] << endl;
        }

        cout << "no_buckets  " << no_buckets << endl;}
    vector<string> arr = {};
    vector<string> arr1 = {};
    string temp1;

    // no cheating: all parties have same y1
    // ‘‘Reconstruct’’ values towards some party (‘‘reconstruct’’ with degree 0)

    for(int k=0; k < no_buckets; k++) {
        if(flag_print) {
            cout << "fff  " << k<< endl;}
        arr = split(recBufs2[0], '*');
        if(arr.size() > 0) {
            temp1 = arr[k];
            //  arr.size()-1
            for (int i = 1; i < N; i++) {
                arr1 = split(recBufs2[i], '*');
                if(temp1 != arr1[k])
                {
                    // cheating detected!!!
                    if(flag_print) {
                        cout << "                 cheating" << endl;}
                    return false;
                }
            }
        }
    }

    return true;

}

template <class FieldType>
void Protocol<FieldType>::readMyInputs()
{
    ifstream myfile;
    int input;
    int i =0;
    myfile.open(inputsFile);
    do {
        myfile >> input;
        myInputs[i] = input;
        i++;
    } while(!(myfile.eof()));
    myfile.close();
}

template <class FieldType>
void Protocol<FieldType>::run() {

    HIM<FieldType> matrix_him(N,N,field);
    VDM<FieldType> matrix_vand(N,N,field);
    HIM<FieldType> m(T, N-T,field);
    matrix_vand.InitVDM();
    matrix_him.InitHIM();

    comm->ConnectionToServer(s);

    initializationPhase(matrix_him, matrix_vand, m);

    if(preparationPhase(matrix_vand, matrix_him) == false) {
        if(flag_print) {
            cout << "cheating!!!" << '\n';}
        return;
    }
    else {
        if(flag_print) {
            cout << "no cheating!!!" << '\n' << "finish Preparation Phase" << '\n';}
    }

    if(inputPreparation() == false) {
        if(flag_print) {
            cout << "cheating!!!" << '\n';}
        return;
    }
    else {
        if(flag_print) {
            cout << "no cheating!!!" << '\n' << "finish Input Preparation" << '\n';}
    }

    string sss = "";

    auto t1 = high_resolution_clock::now();
    auto t1start = high_resolution_clock::now();
    inputAdjustment(sss, matrix_him);

    auto t2 = high_resolution_clock::now();

    auto duration = duration_cast<milliseconds>(t2-t1).count();

    cout << "time in milliseconds inputAdjustment: " << duration << endl;

    if(flag_print) {
        cout << "after Input Adjustment " << '\n'; }

    t1 = high_resolution_clock::now();

    computationPhase(m);

    t2 = high_resolution_clock::now();

    duration = duration_cast<milliseconds>(t2-t1).count();

    cout << "time in milliseconds computationPhase: " << duration << endl;

    t1 = high_resolution_clock::now();

    outputPhase();

    t2 = high_resolution_clock::now();

    duration = duration_cast<milliseconds>(t2-t1).count();

    cout << "time in milliseconds outputPhase: " << duration << endl;

    auto t2end = high_resolution_clock::now();

    duration = duration_cast<milliseconds>(t2end-t1start).count();

    cout << "time in milliseconds for protocol: " << duration << endl;
}

template <class FieldType>
void Protocol<FieldType>::computationPhase(HIM<FieldType> &m) {
    int count = 0;
    processRandoms();
    do {
        count = processSmul();
        count += processAdditions();
        count += processMultiplications(m);
    } while(count!=0);
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
template <class FieldType>
void Protocol<FieldType>::inputAdjustment(string &diff, HIM<FieldType> &mat)
{
    int input;
    int index = 0;

    // read the inputs of the party
    for (int k = 0; k < M; k++)
    {
        if(circuit.getGates()[k].gateType == INPUT && circuit.getGates()[k].party == m_partyId)
        {
            input = myInputs[index];
            index++;
            if(flag_print) {
                cout << "input  " << input << endl;}
            // the value is gateValue[k], but should be input.
            FieldType myinput = field->GetElement(input);
            if(flag_print) {
                cout << "gateValueArr "<<k<< "   " << field->elementToString(gateValueArr[k]) << endl;}

            FieldType different = myinput - gateValueArr[k];
            string str = field->elementToString(different);

            diff += str + "*";

        }
    }
    if(flag_print) {
        cout << "try to print diff" << '\n';
        cout << diff << '\n';}

    vector<string> recBufsdiff(N);

    // Broadcast the difference between GateValue[k] to x.
    if(broadcast(m_partyId, diff, recBufsdiff, mat) == false) {
        if(flag_print) {
            cout << "cheating!!!" << '\n';}
        return;
    }
    else {
        if(flag_print) {
            cout << "no cheating!!!" <<  '\n' << "finish Broadcast" << '\n';}
    }

    if(flag_print) {

        cout << "recBufsdiff" << endl;
        for (int k = 0; k < N; k++) {
            cout << "recBufsdiff" << k << "  " << recBufsdiff[k] << endl;
        }
    }
    // handle after broadcast
    string str;
    FieldType db;

    vector<string> arr = {};
    for (int k = 0; k < M; k++)
    {
        if(circuit.getGates()[k].gateType == INPUT)
        {
            str = recBufsdiff[circuit.getGates()[k].party - 1];
            arr = split(str, '*');
            db = field->stringToElement(arr[0]);
            gateShareArr[k] = gateShareArr[k] + db; // adjustment
            recBufsdiff[circuit.getGates()[k].party - 1] = "";
            for(int i=1; i<arr.size(); i++) {
                recBufsdiff[circuit.getGates()[k].party - 1] += arr[i] + "*";
            }
            gateDoneArr[k] = true; // gate is processed
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
template <class FieldType>
void Protocol<FieldType>::initializationPhase(HIM<FieldType> &matrix_him, VDM<FieldType> &matrix_vand, HIM<FieldType> &m)
{
    beta.resize(1);
    gateValueArr.resize(M);  // the value of the gate (for my input and output gates)
    gateShareArr.resize(M); // my share of the gate (for all gates)
    alpha.resize(N); // N distinct non-zero field elements
    gateDoneArr.resize(M);  // true is the gate is processed
    vector<FieldType> alpha1(N-T);
    vector<FieldType> alpha2(T);





    beta[0] = field->GetElement(0); // zero of the field
    matrix_for_interpolate.allocate(1,N, field);

    // Compute Vandermonde matrix VDM[i,k] = alpha[i]^k
    matrix_vand.InitVDM();

    // Prepare an N-by-N hyper-invertible matrix
    matrix_him.InitHIM();

    // N distinct non-zero field elements
    for(int i=0; i<N; i++)
    {
        alpha[i]=field->GetElement(i+1);
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

    for(int i=0; i<gateDoneArr.size(); i++)
    {
        gateDoneArr[i] = false;
    }

    readMyInputs();

    matrix_for_interpolate.InitHIMByVectors(alpha, beta);

    vector<FieldType> alpha_until_t(T + 1);
    vector<FieldType> alpha_from_t(N - 1 - T);
    for(int i=0; i<T+1; i++)
    {
        alpha_until_t[i]= alpha[i];
    }
    for (int i = T + 1; i < N; i++) {
        alpha_from_t[i - (T + 1)] = alpha[i];
    }
    // Interpolate first d+1 positions of (alpha,x)
    matrix_for_t.allocate(N - 1 - T, T + 1, field); // slices, only positions from 0..d
    matrix_for_t.InitHIMByVectors(alpha_until_t, alpha_from_t);

    vector<FieldType> alpha_until_2t(2*T + 1);
    vector<FieldType> alpha_from_2t(N - 1 - 2*T);
    for(int i=0; i<2*T+1; i++)
    {
        alpha_until_2t[i]= alpha[i];
    }
    for (int i = 2*T + 1; i < N; i++) {
        alpha_from_2t[i - (2*T + 1)] = alpha[i];
    }
    // Interpolate first d+1 positions of (alpha,x)
    matrix_for_2t.allocate(N - 1 - 2*T, 2*T + 1, field); // slices, only positions from 0..d
    matrix_for_2t.InitHIMByVectors(alpha_until_2t, alpha_from_2t);


}

/**
 * The function compute t shared authentication checks and to reconstruct the n sharings,
 * one towards each party, who then computes the secret and sends it to everybody.
 * Each party receives n − t secrets and t authentication checks.
 * Reconstruct a bunch of degree-d sharings to all parties (into ValBuf)
 * @param myShares
 * @param alpha
 * @param valBuf
 */
template <class FieldType>
void Protocol<FieldType>::publicReconstruction(vector<string> &myShares, int &count, int d, vector<FieldType> &valBuf, HIM<FieldType> &m)
{
    int no_buckets = count / (N-T) + 1;
    if(flag_print) {
        cout << "public reconstruction" << endl;
        cout << "no buckets" << no_buckets << endl; }
    FieldType x;

    vector<FieldType> x1(N);
    vector<FieldType> y1(N);
    vector<FieldType> y2(N);
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
    if(flag_print) {
        for (int i = 0; i < myShares.size(); i++) {
            cout << "myShares " << i << "   " << myShares[i] << endl;
        }
    }
    // init x to be vector of degree-d (d=2*t) shares of n−t secret
    for(int k=0; k < no_buckets; k++)
    {
        for(int i = 0; i < N-T; i++)
        {
            if( k*(N-T)+i < count)
            {
                // k*(N-T)+i
                x1[i] = field->stringToElement(myShares[k*(N-T)+i]);
            }
            else
            {
                x1[i] = *(field->GetZero());
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
            sendBufs[i] += field->elementToString(x1[i]) + "*";
        }
    }
    if(flag_print) {
        cout << "sendBufs[i]" << endl;
        for (int i = 0; i < N; i++) {
            cout << sendBufs[i] << endl;
        }
    }
    //   cout << "before roundfunction1" << '\n';
    comm->roundfunction1(sendBufs, recBufs);
    if(flag_print) {
        cout << "recBufs[i]" << endl;
        for(int i = 0; i < N; i++)
        {
            cout << recBufs[i] << endl;
        }}
    //   cout << "after roundfunction1" << '\n';
    vector<string> arr = {};
    for(int k=0; k < no_buckets; k++) {

        for (int i = 0; i < N; i++) {

            arr = split(recBufs[i], '*');
            x1[i] = field->stringToElement(arr[k]);
        }
        if(flag_print) {
            cout << "x1[i]" << endl;
            for(int i = 0; i < N; i++)
            {
                cout << field->elementToString(x1[i]) << endl;
            } }

        // checking that {xj}i are d-consistent and interpolate them to x j .
        if (!checkConsistency(x1, d)) {
            // halt
            // cheating detected
            if(flag_print) {
                cout << "cheating" << '\n';}
        }

        // interpolate {xj}i to x
        x = interpolate(x1);

        // send x to all parties
        for (int i = 0; i < N; i++) {
            sendBufs2[i] += field->elementToString(x) + "*";
        }
    }
    if(flag_print) {
        cout << "sendBufs2[i]" << endl;
        for(int i = 0; i < N; i++)
        {
            cout << sendBufs2[i] << endl;
        } }
    comm->roundfunction8(sendBufs2, recBufs2);
    if(flag_print) {
        cout << "recBufs2[i]" << endl;
        for(int i = 0; i < N; i++)
        {
            cout << recBufs2[i] << endl;
        } }
//    vector<string> arr = {};
    int index = 0;
    for(int k=0; k < no_buckets; k++) {
        for (int i = 0; i < N; i++) {

            arr = split(recBufs2[i], '*');
            x1[i] = field->stringToElement(arr[k]);
        }

        // checking that (Xn−t,...,Xn) = M*(X1,...,Xn−t)
        m.MatrixMult(x1, y1);

        for (int i = 0; i < T; i++) {
            if(field->elementToString(x1[N-T+i]) != field->elementToString(y1[i]))
            {
                if(flag_print) {
                    // halt !
                    cout << "                  cheating" << '\n'; }
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

template <class FieldType>
bool Protocol<FieldType>::preparationPhase(VDM<FieldType> &matrix_vand, HIM<FieldType> &matrix_him)
{
    vector<string> recBufs(N);
    int robin = 0;
    string strX1, strX2, str;

    // the number of random double sharings we need altogether
    int no_random = circuit.getNrOfMultiplicationGates() + circuit.getNrOfInputGates();
    vector<FieldType> x1(N),x2(N),y1(N),y2(N);
    vector<string> sendBufs(N);

    // the number of buckets (each bucket requires one double-sharing
    // from each party and gives N-2T random double-sharings)
    int no_buckets = (no_random / (N-2*T))+1;

    sharingBuf.resize(no_buckets*(N-2*T)); // my shares of the double-sharings

    for(int i=0; i < N; i++)
    {
        sendBufs[i] = "";
    }

    /**
     *  generate double sharings.
     *  first degree t.
     *  subsequent: degree 2t with same secret.
     */


    for(int k=0; k < no_buckets; k++)
    {
        // generate random degree-T polynomial
        for(int i = 0; i < T+1; i++)
        {
            // A random field element, uniform distribution
            x1[i] = field->Random();
        }

        x2[0] = x1[0];

        if(flag_print) {
            cout << "k " << k << "s= " << field->elementToString(x1[0]) << endl;}

        for(int i = 1; i < 2*T+1; i++)
        {
            // otherwise random
            x2[i] = field->Random();
        }

        matrix_vand.MatrixMult(x1, y1); // eval poly at alpha-positions
        matrix_vand.MatrixMult(x2, y2); // eval poly at alpha-positions

        // prepare shares to be sent
        for(int i=0; i < N; i++)
        {
            sendBufs[i] += field->elementToString(y1[i]) + "*"; // the degree-t shares of my poly
            sendBufs[i] += field->elementToString(y2[i]) + "$"; // the degree 2t shares of my poly
        }


    }
    if(flag_print) {
        cout << "sendBufs" << endl;
        cout << "N" << N << endl;
        cout << "T" << T << endl;


        for (int i = 0; i < sendBufs.size(); i++) {
            cout << "sendBufs" << i << " :" << sendBufs[i] << endl;
        }
    }
    for(int i=0; i < recBufs.size(); i++)
    {
        recBufs[i] = "";
    }

    comm->roundfunction4(sendBufs, recBufs);
    if(flag_print) {
        for (int i=0; i < recBufs.size();i++)
        {
            cout << "recBufs" << i << " :" << recBufs[i] << endl;
        } }

    /**
     * Apply hyper-invertible matrix on each bucket.
     * From the resulting sharings, 2T are being reconstructed towards some party,
     * the remaining N-2T are kept as prepared sharings.
     * For balancing, we do round-robin the party how shall reconstruct and check!
     */

    vector<string> sendBufs1(N);
    for(int i=0; i < N; i++)
    {
        sendBufs1[i] = "";

    }

    vector<string> arr1 = {};
    vector<string> arr2 = {};
    vector<string> arr = {};

    // x1 : used for the N degree-t sharings
    // x2 : used for the N degree-2t sharings
    for(int k=0; k < no_buckets; k++) {
        // generate random degree-T polynomial
        for (int i = 0; i < N; i++) {
            str = recBufs[i];
            arr1 = split(str, '$');
            arr = split(arr1[k], '*');
            if(arr.size() >1 ) {
                strX1 = arr[0];
                strX2 = arr[1];
                x1[i] = field->stringToElement(strX1); // my share of the degree-t sharings
                x2[i] = field->stringToElement(strX2); // my share of the degree-2t sharings
            }
            else{
                if(flag_print) { cout << "                   problemmmmm" << endl;}
            }
        }
        matrix_him.MatrixMult(x1, y1);
        matrix_him.MatrixMult(x2, y2);
        // these shall be checked
        for (int i = 0; i < 2 * T; i++) {
            sendBufs1[robin] += field->elementToString(y1[i]) + "*";
            sendBufs1[robin] += field->elementToString(y2[i]) + "$";
            robin = (robin+1) % N; // next robin
        }
        // Y1 : the degree-t shares of my poly
        // Y2 : the degree 2t shares of my poly
        for (int i = 2 * T; i < N; i++) {
            sharingBuf[k*(N-2*T) + i - 2*T] = field->elementToString(y1[i]) + "*" + field->elementToString(y2[i]);
        }


        for (int i=0; i < sendBufs.size();i++)
        {
            x2[0] = *(field->GetZero());
            x1[0] = *(field->GetZero());
        }
    }
    if(flag_print) {
        cout << "before round" << endl;}
    comm->roundfunction5(sendBufs1, recBufs);
    if(flag_print) {
        cout << "after round" << endl;}
    int count = no_buckets * (2*T) / N; // nr of sharings *I* have to check
    // got one in the last round
    if(no_buckets * (2*T)%N > m_partyId) { // maybe -1
        count++;
    }

    //   count--;

    for(int k=0; k < count; k++) {
        for (int i = 0; i < N; i++) {
            str = recBufs[i];
            arr1 = split(str, '$');
            arr = split(arr1[k], '*');
            if (arr.size() == 2) {
                strX1 = arr[0];
                strX2 = arr[1];
            } else {
                // change !!!!
                strX1 = "";
                strX2 = "";
            }

            x1[i] = field->stringToElement(strX1);
            x2[i] = field->stringToElement(strX2);
        }


        vector<FieldType> x_until_d(N);
        for(int i=0; i<T; i++)
        {
            x_until_d[i] = x1[i];
        }
        for(int i=T; i<N; i++)
        {
            x_until_d[i] = *(field->GetZero());
        }
        if(flag_print) {

            //  cout <<"k "<<k<< "tinterpolate(x1).toString()  " << tinterpolate(x_until_d).toString() << endl;
            cout << "k " << k << "interpolate(x1).toString()  " << field->elementToString(interpolate(x1)) << endl;
            cout << "k " << k << "interpolate(x2).toString()  " << field->elementToString(interpolate(x2)) << endl;
        }
        // Check that x1 is t-consistent and x2 is 2t-consistent and secret is the same
        if(!checkConsistency(x1,T) || !checkConsistency(x2,2*T) ||
                field->elementToString(interpolate(x1)) != field->elementToString(interpolate(x2)))  {
            // cheating detected, abort
            if(flag_print) {
                cout << "k" << k<< endl;}
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
template <class FieldType>
bool Protocol<FieldType>::inputPreparation()
{
    vector<string> sendBufs(N); // upper bound
    vector<string> recBufs(N); // dito
    vector<FieldType> x1(N); // vector for the shares of my inputs
    FieldType elem;
    FieldType secret;
    int i;

    for(int k = 0; k < M; k++)
    {
        if((circuit.getGates())[k].gateType == INPUT)
        {
            string str = sharingBuf[k]; // use the t-sharing (correction will come)
            // change !!!
            if(str!="") {
                // waste the 2T-sharing
                vector<string> arr = {};
                arr = split(str, '*');
                elem = field->stringToElement(arr[0]);
            } else {
                elem = *(field->GetZero());
            }
            gateShareArr[k] = elem;
            i = (circuit.getGates())[k].party; // the number of party which has the input
            // reconstruct sharing towards input party
            sendBufs[i-1] += field->elementToString(gateShareArr[k]) + "*";

        }
    }
    if(flag_print) {
        cout << "sendBufs[i] in input preperation" << endl;
        for (int i = 0; i < N; i++) {
            cout << sendBufs[i] << endl;
        }
    }
    comm->roundfunction6(sendBufs, recBufs);

    if(flag_print) {
        for(int k = 0; k < N; k++) {
            cout << "roundfunction6 recBufs" << k << " " << recBufs[k] << endl;
        } }

    // reconstruct my random input values
    for(int k = 0; k < M; k++) {
        if (circuit.getGates()[k].gateType == INPUT && circuit.getGates()[k].party == m_partyId) {
            // my input: reconstruct received shares
            for (int i = 0; i < N; i++) {
                vector<string> arr = {};
                arr = split(recBufs[i], '*');
                elem = field->stringToElement(arr[0]);
                x1[i] = elem;

                recBufs[i]="";
                for(int l=1; l<arr.size(); l++)
                {
                    recBufs[i] += arr[l] + "*";
                }

            }
            if(!checkConsistency(x1, T))
            {
                // someone cheated!
                return false;
            }
            // the (random) secret
            secret = interpolate(x1);
            //  secret.setPoly(interpolate(x1).getElement());

            gateValueArr[k] = secret;
            if(flag_print) {
                cout << "           the secret is " << field->elementToString(secret) << endl;}
        }
    }

    return true;

}

/**
 * Check whether given points lie on polynomial of degree d. This check is performed by interpolating x on
 * the first d + 1 positions of α and check the remaining positions.
 */
template <class FieldType>
bool Protocol<FieldType>::checkConsistency(vector<FieldType>& x, int d)
{
    if(d == T)
    {
        vector<FieldType> y(N - 1 - d); // the result of multiplication
        vector<FieldType> x_until_t(T + 1);

        for (int i = 0; i < T + 1; i++) {
            x_until_t[i] = x[i];
        }


        matrix_for_t.MatrixMult(x_until_t, y);

        // compare that the result is equal to the according positions in x
        for (int i = 0; i < N - d - 1; i++)   // n-d-2 or n-d-1 ??
        {
            if (field->elementToString(y[i]) != field->elementToString(x[d + 1 + i])) {
                return false;
            }
        }
        return true;
    } else if (d == 2*T)
    {
        vector<FieldType> y(N - 1 - d); // the result of multiplication

        vector<FieldType> x_until_2t(2*T + 1);

        for (int i = 0; i < 2*T + 1; i++) {
            x_until_2t[i] = x[i];
        }

        matrix_for_2t.MatrixMult(x_until_2t, y);

        // compare that the result is equal to the according positions in x
        for (int i = 0; i < N - d - 1; i++)   // n-d-2 or n-d-1 ??
        {
            if (field->elementToString(y[i]) != field->elementToString(x[d + 1 + i])) {
                return false;
            }
        }
        return true;

    } else {
        vector<FieldType> alpha_until_d(d + 1);
        vector<FieldType> alpha_from_d(N - 1 - d);
        vector<FieldType> x_until_d(d + 1);
        vector<FieldType> y(N - 1 - d); // the result of multiplication

        for (int i = 0; i < d + 1; i++) {
            alpha_until_d[i] = alpha[i];
            x_until_d[i] = x[i];
        }
        for (int i = d + 1; i < N; i++) {
            alpha_from_d[i - (d + 1)] = alpha[i];
        }
        // Interpolate first d+1 positions of (alpha,x)
        HIM<FieldType> matrix(N - 1 - d, d + 1, field); // slices, only positions from 0..d
        matrix.InitHIMByVectors(alpha_until_d, alpha_from_d);
        matrix.MatrixMult(x_until_d, y);

        // compare that the result is equal to the according positions in x
        for (int i = 0; i < N - d - 1; i++)   // n-d-2 or n-d-1 ??
        {
            //if (field->elementToString(y[i]) != field->elementToString(x[d + 1 + i])) {
            if (y[i] != x[d + 1 + i]) {
                return false;
            }
        }
        return true;
    }
    return true;
}

// Interpolate polynomial at position Zero
template <class FieldType>
FieldType Protocol<FieldType>::interpolate(vector<FieldType> x)
{
    vector<FieldType> y(N); // result of interpolate
    matrix_for_interpolate.MatrixMult(x, y);
    return y[0];
}

/**
 * the function process all addition gates which are ready.
 * @param circuit
 * @param gateDoneArr
 * @param gateShareArr
 * @return the number of processed gates.
 */
template <class FieldType>
int Protocol<FieldType>::processAdditions()
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

    return count;
}

template <class FieldType>
int Protocol<FieldType>::processSmul()
{
    int count =0;

    for(int k=0; k < M; k++)
    {
        // its an addition which not yet processed and ready
        if(circuit.getGates()[k].gateType == SCALAR && !gateDoneArr[k]
           && gateDoneArr[circuit.getGates()[k].input1])
        {
            // scalar = circuit.getGates()[k].input2
            long scalar(circuit.getGates()[k].input2);
            FieldType e = field->GetElement(scalar);
            gateShareArr[k] = gateShareArr[circuit.getGates()[k].input1] * e;
            gateDoneArr[k] = true;
            count++;
        }

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
template <class FieldType>
int Protocol<FieldType>::processMultiplications(HIM<FieldType> &m)
{
    int count =0;
    int index = 0;
    FieldType p2, d2;
    FieldType r1, r2;
    vector<FieldType> valBuf(M); // Buffers for differences
    FieldType d;
    int indexForValBuf = 0;
    vector<string> ReconsBuf(M);

    vector<string> arr = {};
    for(int k = 0; k < M; k++)
    {
        // its a multiplication which not yet processed and ready
        if(circuit.getGates()[k].gateType == MULT && !gateDoneArr[k]
           && gateDoneArr[circuit.getGates()[k].input1]
           && gateDoneArr[circuit.getGates()[k].input2])
        {

            arr = split(sharingBuf[k], '*');
            r1 = field->stringToElement(arr[0]); // t-share of random r
            r2 = field->stringToElement(arr[1]); // t2-share of same r
            p2 = gateShareArr[circuit.getGates()[k].input1] * gateShareArr[circuit.getGates()[k].input2]; // product share (degree-2t)
            d2 = p2 - r2; // t2-share of difference
            ReconsBuf[index] = field->elementToString(d2); // reconstruct difference (later)
            index++;
            // for now gateShareArr[k] is random sharing, needs to be adjusted (later)
            gateShareArr[k] = r1;
        }

    }

    if(index == 0)
    {
        return 0;
    }
    if(flag_print) {
        cout <<"index for publicReconstruction " << index << '\n'; }

    // reconstruct the differences into valBuf
    publicReconstruction(ReconsBuf, index, 2*T, valBuf, m);
    indexForValBuf = index-1;

    for(int k=M-1; k >= 0; k--)
    {
        // its a multiplication which not yet processed and ready
        if(circuit.getGates()[k].gateType == MULT && !gateDoneArr[k]
           && gateDoneArr[circuit.getGates()[k].input1]
           && gateDoneArr[circuit.getGates()[k].input2])
        {
            if(flag_print) {
                cout << "indexForValBuf " << indexForValBuf << endl;}
            d = valBuf[indexForValBuf];  // the difference
            indexForValBuf--;
            gateShareArr[k] = gateShareArr[k] + d; // the adjustment
            gateDoneArr[k] = true; // the multiplication is done
            count++;
        }
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
template <class FieldType>
void Protocol<FieldType>::processRandoms()
{
    FieldType r1;
    vector<string> arr = {};
    for(int k = 0; k < M; k++)
    {
        if(circuit.getGates()[k].gateType == RANDOM)
        {
            arr = split(sharingBuf[k], '*');
            // t-share of random r. t2-share of same r, IGNORED!
            r1 = field->stringToElement(arr[0]);
            gateShareArr[k] = r1;
        }
    }
}

/**
 * the function Walk through the circuit and reconstruct output gates.
 * @param circuit
 * @param gateShareArr
 * @param alpha
 */
template <class FieldType>
void Protocol<FieldType>::outputPhase()
{
    int count=0;
    vector<FieldType> x1(N); // vector for the shares of my outputs
    vector<string> sendBuf(N);
    vector<string> recBuf(N);
    FieldType num;
    ofstream myfile;
    myfile.open(outputFile);

    for(int i=0; i < N; i++)
    {
        sendBuf[i] = "";
    }

    for(int k=0; k < M; k++)
    {
        if(circuit.getGates()[k].gateType == OUTPUT)
        {
            // send to party (which need this gate) your share for this gate
            sendBuf[circuit.getGates()[k].party - 1] = field->elementToString(gateShareArr[circuit.getGates()[k].input1]);
        }
    }

    comm->roundfunction7(sendBuf, recBuf);

    if(flag_print) {
        cout << "endnend" << endl;}
    for(int k=0; k < M; k++) {
        if(circuit.getGates()[k].gateType == OUTPUT && circuit.getGates()[k].party == m_partyId)
        {
            for(int i=0; i < N; i++) {
                // change from string to field element
                num = field->stringToElement(recBuf[i]);
                x1[i] = num;
            }

            // my output: reconstruct received shares
            if (!checkConsistency(x1, T))
            {
                // someone cheated!
                if(flag_print) {
                    cout << "cheating!!!" << '\n';}
                return;
            }
            cout << "the result is : " << field->elementToString(interpolate(x1)) << '\n';
            myfile << field->elementToString(interpolate(x1));

        }
    }

    // close output file
    myfile.close();
}
template <class FieldType>
Protocol<FieldType>::~Protocol()
{
    delete comm;
}





#endif /* PROTOCOL_H_ */
