
#include <stdlib.h>
#include <MQTTClient.h>
#include "Protocol.h"


#include "NTL/ZZ_p.h"
#include "NTL/ZZ.h"


using namespace std;
using namespace NTL;

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
 * @param argv[3] = path of inputs file
 * @param argv[4] = path of output file
 * @param argv[5] = path of circuit file
 * @param argv[6] = address
 * @return
 */
int main(int argc, char* argv[])
{
    if(argc != 7)
    {
        cout << "error";
        return 0;
    }

    int times = 5;
    string outputTimerFileName = string(argv[5]) + "Times" + string(argv[1]) + ".csv";
    ProtocolTimer p(times, outputTimerFileName);

    TemplateField<GF2E> * field = new TemplateField<GF2E>(8);

    Protocol<GF2E> protocol(atoi(argv[2]), atoi(argv[1]),field, argv[3], argv[4], argv[5], argv[6], &p);

//    TemplateField<ZZ_p> * field = new TemplateField<ZZ_p>(22943);
//
//    Protocol<ZZ_p> protocol(atoi(argv[2]), atoi(argv[1]),field, argv[3], argv[4], argv[5], argv[6], &p);




    //protocol.run();

    auto t1 = high_resolution_clock::now();
    for(int i=0; i<times; i++) {
        protocol.run(i);
    }
    auto t2 = high_resolution_clock::now();

    auto duration = duration_cast<milliseconds>(t2-t1).count();
    cout << "time in milliseconds for " << times << " runs: " << duration << endl;

    delete field;

    p.writeToFile();

    cout << "end main" << '\n';


    return 0;
}
