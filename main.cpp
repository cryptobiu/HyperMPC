
#include <stdlib.h>
#include "comm_client_factory.h"
#include "ProtocolParty.h"
#include "GF2_8LookupTable.h"
#include <log4cpp/Category.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/SimpleLayout.hh>
#include <log4cpp/RollingFileAppender.hh>
#include <log4cpp/SimpleLayout.hh>
#include <log4cpp/BasicLayout.hh>
#include <log4cpp/PatternLayout.hh>
#include "psmpc_ac_gf28lt.h"
#include "psmpc_ac_m31.h"


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

void init_log(const char * a_log_file, const char * a_log_dir, const int log_level, const char * logcat);

int main(int argc, char* argv[])
{


    CmdParser parser;
    auto parameters = parser.parseArguments("", argc, argv);
    int times = stoi(parser.getValueByKey(parameters, "internalIterationsNumber"));
    string fieldType = parser.getValueByKey(parameters, "fieldType");

    
    if(fieldType.compare("ZpMersenne") == 0)
    {
        int ng = stoi(parser.getValueByKey(parameters, "NG"));
        if (0 == ng)
        {
			ProtocolParty<ZpMersenneIntElement> protocol(argc, argv);

			auto t1 = high_resolution_clock::now();
			protocol.run();

			auto t2 = high_resolution_clock::now();

			auto duration = duration_cast<milliseconds>(t2-t1).count();
			cout << "time in milliseconds for " << times << " runs: " << duration << endl;

			cout << "end main" << '\n';
        }
        else
        {
            int id = stoi(parser.getValueByKey(parameters, "partyID"));
            int parties = stoi(parser.getValueByKey(parameters, "partiesNumber"));
            std::string partiesFile = parser.getValueByKey(parameters, "partiesFile");
            std::string circuitName = parser.getValueByKey(parameters, "circuitFile");

            char buffer[32];
            snprintf(buffer, 32, "psmpc_%04d.log", id);
            init_log(buffer, "./", 500, "ps");
            psmpc_ac_m31 ps(argc, argv, "ps");
            ps.run_ac_protocol(id ,parties, partiesFile.c_str(), 180);
        }
    }

    else if(fieldType.compare("ZpMersenne61") == 0)
    {

        ProtocolParty<ZpMersenneLongElement> protocol(argc, argv);
        auto t1 = high_resolution_clock::now();
        protocol.run();

        auto t2 = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(t2-t1).count();

        cout << "time in milliseconds for " << times << " runs: " << duration << endl;

        cout << "end main" << '\n';
    }
    else if(fieldType.compare("GF2_8LookupTable") == 0)
    {
        int ng = stoi(parser.getValueByKey(parameters, "NG"));
        if (0 == ng)
        {
            ProtocolParty<GF2_8LookupTable> protocol(argc, argv);
            auto t1 = high_resolution_clock::now();

            protocol.run();

            auto t2 = high_resolution_clock::now();

            auto duration = duration_cast<milliseconds>(t2 - t1).count();

            cout << "time in milliseconds for " << times << " runs: " << duration << endl;

            cout << "end main" << '\n';
        }
        else
        {
            int id = stoi(parser.getValueByKey(parameters, "partyID"));
            int parties = stoi(parser.getValueByKey(parameters, "partiesNumber"));
            std::string partiesFile = parser.getValueByKey(parameters, "partiesFile");
            std::string circuitName = parser.getValueByKey(parameters, "circuitFile");

            char buffer[32];
            snprintf(buffer, 32, "psmpc_%04d.log", id);
            init_log(buffer, "./", 500, "ps");
            psmpc_ac_gf28lt ps(argc, argv, "ps");
            ps.run_ac_protocol(id ,parties, partiesFile.c_str(), 180);
        }
    }

    else if(fieldType.compare("GF2E") == 0)
    {
        ProtocolParty<GF2E> protocol(argc, argv);
        auto t1 = high_resolution_clock::now();

        protocol.run();

        auto t2 = high_resolution_clock::now();

        auto duration = duration_cast<milliseconds>(t2-t1).count();

        cout << "time in milliseconds for " << times << " runs: " << duration << endl;

        cout << "end main" << '\n';
    }

    else if(fieldType.compare("Zp") == 0)
    {
        ProtocolParty<ZZ_p> protocol(argc, argv);

        auto t1 = high_resolution_clock::now();

        protocol.run();

        auto t2 = high_resolution_clock::now();

        auto duration = duration_cast<milliseconds>(t2-t1).count();

        cout << "time in milliseconds for " << times << " runs: " << duration << endl;

        cout << "end main" << '\n';

    }
    else
    {
        cout << "Wrong field type. The options are:\n"
                "1. ZpMersenne\n"
                "2. ZpMersenne61\n"
                "3. GF2_8LookupTable\n"
                "4. GF2E\n"
                "5. Zp\n";
        exit(-1);
    }

    return 0;
}

void init_log(const char * a_log_file, const char * a_log_dir, const int log_level, const char * logcat)
{
    static const char the_layout[] = "%d{%y-%m-%d %H:%M:%S.%l}| %-6p | %-15c | %m%n";

    std::string log_file = a_log_file;
    log_file.insert(0, "/");
    log_file.insert(0, a_log_dir);

    log4cpp::Layout * log_layout = NULL;
    log4cpp::Appender * appender = new log4cpp::RollingFileAppender("rlf.appender", log_file.c_str(), 10*1024*1024, 5);

    bool pattern_layout = false;
    try
    {
        log_layout = new log4cpp::PatternLayout();
        ((log4cpp::PatternLayout *)log_layout)->setConversionPattern(the_layout);
        appender->setLayout(log_layout);
        pattern_layout = true;
    }
    catch(...)
    {
        pattern_layout = false;
    }

    if(!pattern_layout)
    {
        log_layout = new log4cpp::BasicLayout();
        appender->setLayout(log_layout);
    }

    log4cpp::Category::getInstance(logcat).addAppender(appender);
    log4cpp::Category::getInstance(logcat).setPriority((log4cpp::Priority::PriorityLevel)log_level);
    log4cpp::Category::getInstance(logcat).notice("log start");
}
