#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#include "stdlib.h"
#include "../../workspace/paho/src/MQTTClient.h"
#include "TParty.h"
#include "VDM.h"
#include "TGate.h"
#include "ArithmeticCircuit.h"
#include "Communication.h"
#include <vector>
#include <bitset>
#include "TFieldElement.h"
#include <iostream>
#include <fstream>
#include <chrono>

using namespace std;
using namespace std::chrono;

#define ADDRESS  "tcp://localhost:1883"

class Protocol {
private:
    /**
     * N - number of parties
     * M - number of gates
     * T - number of malicious
     */
    int N, M, T, m_partyId;
    string inputsFile, outputFile;
    Communication* comm;
    ArithmeticCircuit circuit;
    vector<TFieldElement> gateValueArr; // the value of the gate (for my input and output gates)
    vector<TFieldElement> gateShareArr; // my share of the gate (for all gates)
    vector<TFieldElement> alpha; // N distinct non-zero field elements
    vector<bool> gateDoneArr; // true is the gate is processed
    vector<string> sharingBuf; // prepared double-sharings (my shares)
    vector<int> myInputs;
    string s;

public:
    Protocol(int n, int id, string inputsFile, string outputFile);
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
    void initializationPhase(HIM &matrix_him, VDM &matrix_vand, HIM &m);

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
    bool preparationPhase(VDM &matrix_vand, HIM &matrix_him);

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
    bool broadcast(int party_id, string myMessage, vector<string> &recBufsdiff, HIM &mat);

    /**
     * For multiplication and for output gates, we need public reconstruction of sharings (degree t and degree 2t).
     * The straight-forward protocol requires n 2 communication, which is too slow.
     * We present a protocol which efficiently reconstructs n − t sharings. The basic idea is to compute t shared
     * authentication checks and to reconstruct the n sharings, one towards each party, who then computes the
     * secret and sends it to everybody. Each party receives n − t secrets and t authentication checks.
     *
     * Protocol Public-Reconstruction:
     * 0. Every party P i holds a vector x(i) of degree-d shares of n−t secret values x. Let M be a t-by-(n−t)
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
    void publicReconstruction(vector<string> &myShares, int &count, int d, vector<TFieldElement> &valBuf, HIM &m);

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
    void inputAdjustment(string &diff, HIM &mat);

    /**
     * Check whether given points lie on polynomial of degree d.
     * This check is performed by interpolating x on the first d + 1 positions of α and check the remaining positions.
     */
    bool checkConsistency(vector<TFieldElement>& x, int d);

    /**
     * Process all additions which are ready.
     * Return number of processed gates.
     */
    int processAdditions();

    /**
     * Process all multiplications which are ready.
     * Return number of processed gates.
     */
    int processMultiplications(HIM &m);

    /**
     * Process all random gates.
     */
    void processRandoms();

    /**
     * Walk through the circuit and evaluate the gates. Always take as many gates at once as possible,
     * i.e., all gates whose inputs are ready.
     * We first process all random gates, then alternately process addition and multiplication gates.
     */
    void computationPhase(HIM &m);

    /**
     * The cheap way: Create a HIM from the αi’s onto ZERO (this is actually a row vector), and multiply
     * this HIM with the given x-vector (this is actually a scalar product).
     * The first (and only) element of the output vector is the secret.
     */
    TFieldElement interpolate(vector<TFieldElement> x);

    TFieldElement tinterpolate(vector<TFieldElement> x);

    /**
     * Walk through the circuit and reconstruct output gates.
     */
    void outputPhase();

    ~Protocol();
};


#endif /* PROTOCOL_H_ */
