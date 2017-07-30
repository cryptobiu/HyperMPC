
#include <stdlib.h>
#include "Protocol.h"
#include "ZpMersenneIntElement.h"
#include "ZpMersenneLongElement.h"
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
 * @param argv[6] = address
 * @param argv[7] = fieldType
 * @return
 */
int main(int argc, char* argv[])
{

    /*int timesfield = 1000000;
    ZZ_p::init(ZZ(2147483647));

    //testing the mersenne field

    ZpMersenneIntElement a(2147483640);
    ZpMersenneIntElement b(2147483641);

    ZpMersenneIntElement c(2147483647);

    c = a+b;

    ZpMersenneIntElement mult(2147483647);
    ZpMersenneIntElement div(2147483647);



    auto t1 = high_resolution_clock::now();
    for(int i=0; i<timesfield; i++){
        mult = a*b;
    }
    auto t2 = high_resolution_clock::now();

    auto duration = duration_cast<milliseconds>(t2-t1).count();
    cout << "time in milliseconds for " << timesfield<< " mults: " << duration << endl;



    t1 = high_resolution_clock::now();
    for(int i=0; i<timesfield; i++){
        div = a/b;
    }
    t2 = high_resolution_clock::now();

    duration = duration_cast<milliseconds>(t2-t1).count();
    cout << "time in milliseconds for " << timesfield << " divs: " << duration << endl;


    cout<<"a + b = " << c.elem <<endl;
    cout<<"a * b = " << mult.elem<<endl;
    cout<<"a / b = " << div.elem<<endl;
    cout<<"a - b = " << a-b<<endl;


    cout<< "-1 % 2147483647 is " << (-1 % 2147483647) << endl;


    ZZ_p::init(ZZ(2147483647));

    ZZ_p x(2147483640);
    ZZ_p y(2147483641);
    ZZ_p divZ(5);
    ZZ_p multZ(5);


    t1 = high_resolution_clock::now();
    for(int i=0; i<timesfield; i++){
        multZ = x*y;
    }
    t2 = high_resolution_clock::now();

    duration = duration_cast<milliseconds>(t2-t1).count();
    cout << "time in milliseconds for " << timesfield<< " mults: " << duration << endl;




    t1 = high_resolution_clock::now();
    for(int i=0; i<timesfield; i++){
        divZ = x/y;
    }
    t2 = high_resolution_clock::now();

    duration = duration_cast<milliseconds>(t2-t1).count();
    cout << "time in milliseconds for " << timesfield << " divs: " << duration << endl;



    cout<<"zzp : a / b = " << to_uint(rep(divZ))<<endl;

    cout<<"zzp: a * b = " << (x*y) <<endl;

    cout<<"zzp: a - b = " << (x-y) <<endl;

    return 0;
*/
    if(argc != 9)
    {
        cout << "error";
        return 0;
    }

    string fieldType(argv[7]);

    int times = 5;
    string outputTimerFileName = string(argv[5]) + "Times" + string(argv[1]) + "ForGroup" + argv[8] + fieldType + ".csv";
    ProtocolTimer p(times, outputTimerFileName);


    ofstream totalTime;
    string outputTotalTime = string(argv[5]) + "Times" + string(argv[1]) + "ForGroup" + argv[8] + fieldType + ".txt";
    totalTime.open (outputTotalTime);


    if(fieldType.compare("ZpMensenne") == 0)
    {
        TemplateField<ZpMersenneIntElement> *field = new TemplateField<ZpMersenneIntElement>(2147483647);

       /* //test bytes to elemet and vice versa
        ZpMersenneIntElement test,test2;

        ZpMersenneIntElement answer(0);

        byte* testBytes = new byte[4];

        field->elementToBytes(testBytes, test);

        auto returnedElem = field->bytesToElement(testBytes);

        ZZ_p::init(ZZ(2147483647));
        ZZ_p result(5);

        for(int i=0; i<100000; i++){

            ZpMersenneIntElement index(i*10000);

            test = field->Random();
            test2 = field->Random();

            answer = 1;
            result = 1;

            answer *= i;//test * test2;
            result *= i;//ZZ_p(test.elem) * ZZ_p(test2.elem);



            field->elementToBytes(testBytes, answer);

            auto returnedElem = field->bytesToElement(testBytes);

            if(returnedElem!=answer){
                cout<<"problem parsing"<<endl;
            }

            if(to_uint(rep(result)) !=answer.elem){

                cout<<"problem for test = " << test << "index =" <<index << "test2 = " << test2<<endl;
                cout<<"answer for zzp = " << result << "answer for mensenne =" <<answer<<endl;

                cout<<"mensenne mult = " <<  (test * index)<<endl;
                cout<<"zzp mult = " <<  to_uint(rep((ZZ_p(test.elem) * ZZ_p(index.elem))))<<endl;


            }
        }


*/


        Protocol<ZpMersenneIntElement> protocol(atoi(argv[2]), atoi(argv[1]), field, argv[3], argv[4], argv[5], argv[6], &p,atoi(argv[8]));
        auto t1 = high_resolution_clock::now();


        //totalTime <<put_time(std::localtime(&now_c), "%H h %M m %S s ") << '\n';
        totalTime<< "Start Time in Milliseconds = " << chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now().time_since_epoch()).count()<< endl;;
        for(int i=0; i<times; i++) {
            protocol.run(i);
        }
        auto t2 = high_resolution_clock::now();

        auto duration = duration_cast<milliseconds>(t2-t1).count();
        totalTime<< "End Time in Milliseconds =" << chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now().time_since_epoch()).count()<< endl;;
        totalTime <<"Duration : "<<duration<<endl;
        cout << "time in milliseconds for " << times << " runs: " << duration << endl;

        delete field;

        p.writeToFile();

        cout << "end main" << '\n';
    }

    else if(fieldType.compare("ZpMensenne61") == 0)
    {
        TemplateField<ZpMersenneLongElement> *field = new TemplateField<ZpMersenneLongElement>(0);


        Protocol<ZpMersenneLongElement> protocol(atoi(argv[2]), atoi(argv[1]), field, argv[3], argv[4], argv[5], argv[6], &p,atoi(argv[8]));
        auto t1 = high_resolution_clock::now();
        totalTime<< "Start Time in Milliseconds = " << chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now().time_since_epoch()).count()<< endl;;
        for(int i=0; i<times; i++) {
            protocol.run(i);
        }
        auto t2 = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(t2-t1).count();
        totalTime<< "End Time in Milliseconds =" << chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now().time_since_epoch()).count()<< endl;;
        totalTime <<"Duration : "<<duration<<endl;

        cout << "time in milliseconds for " << times << " runs: " << duration << endl;


        delete field;

        p.writeToFile();

        cout << "end main" << '\n';
    }
    else if(fieldType.compare("GF2_8LookupTable") == 0)
    {

        cout<<"lookup table"<<endl;
        TemplateField<GF2_8LookupTable> *field = new TemplateField<GF2_8LookupTable>(0);


        Protocol<GF2_8LookupTable> protocol(atoi(argv[2]), atoi(argv[1]), field, argv[3], argv[4], argv[5], argv[6], &p, atoi(argv[8]));
        auto t1 = high_resolution_clock::now();
        totalTime<< "Start Time in Milliseconds = " << chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now().time_since_epoch()).count()<< endl;;
        for(int i=0; i<times; i++) {

            protocol.run(i);
            cout<<"lookup table"<<endl;
        }
        auto t2 = high_resolution_clock::now();

        auto duration = duration_cast<milliseconds>(t2-t1).count();
        totalTime<< "End Time in Milliseconds =" << chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now().time_since_epoch()).count()<< endl;;
        totalTime <<"Duration : "<<duration<<endl;
        cout << "time in milliseconds for " << times << " runs: " << duration << endl;

        delete field;

        p.writeToFile();

        cout << "end main" << '\n';
    }



    else if(fieldType.compare("GF2m") == 0)
    {
        TemplateField<GF2E> *field = new TemplateField<GF2E>(8);

        Protocol<GF2E> protocol(atoi(argv[2]), atoi(argv[1]), field, argv[3], argv[4], argv[5], argv[6], &p,atoi(argv[8]));
        auto t1 = high_resolution_clock::now();
        totalTime<< "Start Time in Milliseconds = " << chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now().time_since_epoch()).count()<< endl;;
        for(int i=0; i<times; i++) {
            protocol.run(i);
        }
        auto t2 = high_resolution_clock::now();

        auto duration = duration_cast<milliseconds>(t2-t1).count();
        totalTime<< "End Time in Milliseconds =" << chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now().time_since_epoch()).count()<< endl;;
        totalTime <<"Duration : "<<duration<<endl;
        cout << "time in milliseconds for " << times << " runs: " << duration << endl;

        delete field;

        p.writeToFile();

        cout << "end main" << '\n';
    }

    else if(fieldType.compare("Zp") == 0)
    {
        TemplateField<ZZ_p> * field = new TemplateField<ZZ_p>(2147483647);

        Protocol<ZZ_p> protocol(atoi(argv[2]), atoi(argv[1]),field, argv[3], argv[4], argv[5], argv[6], &p,atoi(argv[8]));

        auto t1 = high_resolution_clock::now();
        totalTime<< "Start Time in Milliseconds = " << chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now().time_since_epoch()).count()<< endl;;
        for(int i=0; i<times; i++) {
            protocol.run(i);
        }
        auto t2 = high_resolution_clock::now();

        auto duration = duration_cast<milliseconds>(t2-t1).count();
        totalTime<< "End Time in Milliseconds =" << chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now().time_since_epoch()).count()<< endl;;
        totalTime <<"Duration : "<<duration<<endl;
        cout << "time in milliseconds for " << times << " runs: " << duration << endl;

        delete field;

        p.writeToFile();

        cout << "end main" << '\n';

    }

    totalTime.close();
    return 0;
}
