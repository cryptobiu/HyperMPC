
#include "stdlib.h"
#include "../../workspace/paho/src/MQTTClient.h"
#include "TParty.h"
#include "VDM.h"
#include "TGate.h"
#include "protocol.h"
#include "Communication.h"
#include <thread>
using namespace std;


int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        cout << "error";
        return 0;
    }

    Communication::getInstance(atoi(argv[2]), atoi(argv[1]));

    std::thread::id main_thread_id = std::this_thread::get_id();
    createObjectOfCommunicationAndStartTheProtocol(argc, argv);
}
