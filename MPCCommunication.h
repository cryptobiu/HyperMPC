//
// Created by moriya on 04/01/17.
//

#ifndef GMW_MPCCOMMUNICATION_H
#define GMW_MPCCOMMUNICATION_H

#include "../../include/interactive_mid_protocols/OTBatch.hpp"
#include "../../include/interactive_mid_protocols/OTExtensionBristol.hpp"

class ProtocolPartyData {
private:
    int id;
    shared_ptr<CommParty> channel;  // Channel between this party to every other party in the protocol.

public:
    ProtocolPartyData() {}
    ProtocolPartyData(int id, shared_ptr<CommParty> channel)
            : id (id), channel(channel){
    }

    int getID() { return id; }
    shared_ptr<CommParty> getChannel() { return channel; }
};

class MPCCommunication {

public:
    static vector<shared_ptr<ProtocolPartyData>> setCommunication(boost::asio::io_service & io_service, int id, int numParties, string configFile,int groupID);
};


#endif //GMW_MPCCOMMUNICATION_H
