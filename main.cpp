
#include "stdlib.h"
#include "../../workspace/paho/src/MQTTClient.h"
#include "TParty.h"
#include "VDM.h"
#include "TGate.h"
#include "Communication.h"
#include <thread>
#include "Protocol.h"
using namespace std;

/**
 * The main structure of our protocol is as follows:
 * 1. Initialization Phase: Initialize some global variables (parties, field, circuit, etc).
 * 2. Preparation Phase: Prepare enough random double-sharings: a random double-sharing is a pair of
 *  two sharings of the same random value, one with degree t, and one with degree 2t. One such double-
 *  sharing is consumed for multiplying two values. We also consume double-sharings for input gates
 *  and for random gates (this is slightly wasteful, but we assume that the number of multiplication
 *  gates is dominating the number of input and random gates).
 * 3. Input Phase: For each input gate, reconstruct one of the random sharings towards the input party.
 *  Then, all input parties broadcast a vector of correction values, namely the differences of the inputs
 *  they actually choose and the random values they got. These correction values are then added on
 *  the random sharings.
 * 4. Computation Phase: Walk through the circuit, and evaluate as many gates as possible in parallel.
 *  Addition gates and random gates can be evaluated locally (random gates consume a random double-
 *  sharing). Multiplication gates are more involved: First, every party computes local product of the
 *  respective shares; these shares form de facto a 2t-sharing of the product. Then, from this sharing,
 *  a degree-2t sharing of a random value is subtracted, the difference is reconstructed and added on
 *  the degree-t sharing of the same random value.
 * 5. Output Phase: The value of each output gate is reconstructed towards the corresponding party.
 * @param argc
 * @param argv[1] = id of parties (1,...,N)
 * @param argv[2] = N: number of parties
 * @return
 */
int main(int argc, char* argv[])
{

//    TFieldElement answer;
//    TFieldElement a,b;
//
//    a = (TField::getInstance()->GetElementByValue(83));
//    b = (TField::getInstance()->GetElementByValue(202));
//    NTL::GF2X f;
//    MulMod(f, a.getElement(),  b.getElement(), BuildSparseIrred_GF2X(8));
//    answer.setPoly(f);
//    cout << "irr " << BuildSparseIrred_GF2X(8) << endl;
//    cout << "answer is : " << answer.toString() << endl;
    if(argc != 3)
    {
        cout << "error";
        return 0;
    }
    Communication* c = Communication::getInstance(atoi(argv[2]), atoi(argv[1]));
    Protocol protocol(atoi(argv[2]), atoi(argv[1]));
    protocol.run();
    delete(c);
    cout << "end main" << '\n';
    return 0;
}
