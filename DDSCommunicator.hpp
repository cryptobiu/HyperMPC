/*
 * DDSCommunicator.h
 *
 *  Created on: Apr 7, 2018
 *      Author: root
 */

#ifndef DDSCOMMUNICATOR_HPP_
#define DDSCOMMUNICATOR_HPP_

#include <dds/domain/DomainParticipant.hpp>
#include <dds/pub/ddspub.hpp>
#include <dds/sub/ddssub.hpp>
#include <dds/pub/DataWriter.hpp>
#include <dds/core/ddscore.hpp>
#include <dds/dds.hpp>
#include <dds/sub/DataReader.hpp>

#include <iostream>
#include <exception>
#include <map>
#include <vector>

#include "BIUDDSType.hpp"

#include "DDSChannelWriter.hpp"
#include "DDSSample.hpp"


using namespace std;
using namespace chrono;
using namespace dds::sub::status;
using namespace dds::sub::cond;
using namespace dds::sub;
using namespace dds::core::cond;

class DDSCommunicator {
public:
	DDSCommunicator(int id, int numParties, string configFile, unsigned int elapsedTime = 5, bool addIPtoPeerList = true, bool printDebugFlag = false);
	virtual ~DDSCommunicator();
	DDSChannelWriter* getDDSChannelWriter (int partyId);
	void ReadAllInputs (map<unsigned long, DDSSample *> *samplesMap);
	void ReadInput(DDSSample* sample);
	string getOwnIP() { return _ownIP; };
	unsigned int getOwnID() { return _ownID; };
	std::vector<string> getPartiesIPList() { return _partiesIPList; };
	std::vector<DDSChannelWriter*> getChannelWriters() { return _DDSChannelWriterList; }
	std::vector<dds::topic::Topic<BIUDDSStruct>> getTopics(){ return _DDSTopicsList; };

private:
//	Methods
	void ParsePartiesIPs (string configFileDir);
	void CreateDDSTopics ();
	void CreateDDSTopicChannelWriters ();
	void CreateOwnTopicReader ();
	void VerifyConnections ();
	bool isSubscriptionMatched ();
	
// Variables
	std::vector<string> _partiesIPList;
	std::vector<dds::topic::Topic<BIUDDSStruct>> _DDSTopicsList;
	std::vector<DDSChannelWriter*> _DDSChannelWriterList;
	dds::domain::DomainParticipant *_participant;
	DataReader<BIUDDSStruct> *_reader;
	LoanedSamples<BIUDDSStruct> _loandSamples;
	LoanedSamples<BIUDDSStruct>::iterator _sampleIt;
	ReadCondition *_readCondition;
	WaitSet *_waitset;

	const int domain_id = 7;
	string _ownIP;
	unsigned int _ownID;
	unsigned int _numParties;
	unsigned int _elapsedTime;
	bool _printDebugFlag;
};

#endif /* DDSCOMMUNICATOR_HPP_ */
