
#include <stdlib.h>
#include "ProtocolParty.h"
#include "GF2_8LookupTable.h"


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
 * @param argv[6] = fieldType
 * @param argv[7] = number of times to run the protocol
 * @return
 */
int main(int argc, char* argv[])
{


    CmdParser parser;
    auto parameters = parser.parseArguments("", argc, argv);
    int times = stoi(parameters["internalIterationsNumber"]);

    string fieldType = parameters["fieldType"];

    ofstream totalTime;
    string outputTotalTime = string(argv[5]) + "Times" + string(argv[1]) + "ForGroup" + argv[8]+ ".txt";
    totalTime.open (outputTotalTime);


    if(fieldType.compare("ZpMersenne") == 0)
    {
        ProtocolParty<ZpMersenneIntElement> protocol(argc, argv);

        auto t1 = high_resolution_clock::now();

        totalTime<< "Start Time in Milliseconds = " << chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now().time_since_epoch()).count()<< endl;;
        protocol.run();

        auto t2 = high_resolution_clock::now();

        auto duration = duration_cast<milliseconds>(t2-t1).count();
        totalTime<< "End Time in Milliseconds =" << chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now().time_since_epoch()).count()<< endl;;
        totalTime <<"Duration : "<<duration<<endl;
        cout << "time in milliseconds for " << times << " runs: " << duration << endl;

        cout << "end main" << '\n';
    }

    else if(fieldType.compare("ZpMersenne61") == 0)
    {

        ProtocolParty<ZpMersenneLongElement> protocol(argc, argv);
        auto t1 = high_resolution_clock::now();
        totalTime<< "Start Time in Milliseconds = " << chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now().time_since_epoch()).count()<< endl;;
        protocol.run();

        auto t2 = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(t2-t1).count();
        totalTime<< "End Time in Milliseconds =" << chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now().time_since_epoch()).count()<< endl;;
        totalTime <<"Duration : "<<duration<<endl;

        cout << "time in milliseconds for " << times << " runs: " << duration << endl;

        cout << "end main" << '\n';
    }
    else if(fieldType.compare("GF2_8LookupTable") == 0)
    {

        cout<<"lookup table"<<endl;

        ProtocolParty<GF2_8LookupTable> protocol(argc, argv);
        auto t1 = high_resolution_clock::now();
        totalTime<< "Start Time in Milliseconds = " << chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now().time_since_epoch()).count()<< endl;;

        protocol.run();

        auto t2 = high_resolution_clock::now();

        auto duration = duration_cast<milliseconds>(t2-t1).count();
        totalTime<< "End Time in Milliseconds =" << chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now().time_since_epoch()).count()<< endl;;
        totalTime <<"Duration : "<<duration<<endl;
        cout << "time in milliseconds for " << times << " runs: " << duration << endl;

        cout << "end main" << '\n';
    }


    else if(fieldType.compare("GF2E") == 0)
    {
        ProtocolParty<GF2E> protocol(argc, argv);
        auto t1 = high_resolution_clock::now();
        totalTime<< "Start Time in Milliseconds = " << chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now().time_since_epoch()).count()<< endl;;

        protocol.run();

        auto t2 = high_resolution_clock::now();

        auto duration = duration_cast<milliseconds>(t2-t1).count();
        totalTime<< "End Time in Milliseconds =" << chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now().time_since_epoch()).count()<< endl;;
        totalTime <<"Duration : "<<duration<<endl;
        cout << "time in milliseconds for " << times << " runs: " << duration << endl;

        cout << "end main" << '\n';
    }

    else if(fieldType.compare("Zp") == 0)
    {
        ProtocolParty<ZZ_p> protocol(argc, argv);

        auto t1 = high_resolution_clock::now();
        totalTime<< "Start Time in Milliseconds = " << chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now().time_since_epoch()).count()<< endl;;

        protocol.run();

        auto t2 = high_resolution_clock::now();

        auto duration = duration_cast<milliseconds>(t2-t1).count();
        totalTime<< "End Time in Milliseconds =" << chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now().time_since_epoch()).count()<< endl;;
        totalTime <<"Duration : "<<duration<<endl;
        cout << "time in milliseconds for " << times << " runs: " << duration << endl;

        cout << "end main" << '\n';

    }

    totalTime.close();
    return 0;
}
