
#include "stdlib.h"
#include "../../workspace/paho/src/MQTTClient.h"
#include "TParty.h"
#include "VDM.h"
#include "TGate.h"
#include "Communication.h"
#include <thread>
#include "Protocol.h"
using namespace std;


int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        cout << "error";
        return 0;
    }
    Communication::getInstance(atoi(argv[2]), atoi(argv[1]));
    Protocol protocol(atoi(argv[2]), atoi(argv[1]));
    protocol.run();
    cout << "njfkl" << endl;
    return 0;
}
