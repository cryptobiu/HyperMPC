
#include "stdlib.h"
#include "../../workspace/paho/src/MQTTClient.h"
#include "TParty.h"
#include "VDM.h"
#include "TGate.h"
#include "protocol.h"

using namespace std;


int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        cout << "error";
        return 0;
    }
    createObjectOfCommunicationAndStartTheProtocol(argc, argv);
}