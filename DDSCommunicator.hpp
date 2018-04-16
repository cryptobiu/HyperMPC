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

#include "Test.hpp"

#include "DDSChannelWriter.hpp"



using namespace std;
using namespace chrono;

class DDSCommunicator {
public:
	DDSCommunicator(int id, int numParties, string configFile, unsigned int elapsedTime = 5, bool addIPtoPeerList = true);
	virtual ~DDSCommunicator();
	DDSChannelWriter GetDDSChannelWriter (string PartyIP);
	void Read (map<string,rti::core::bounded_sequence<char,(MAX_SEQUENCE_SIZE)>> *readinputMap);
	string getMyIP() { return _ownIP; };
    std::vector<string> _partiesIPList;


private:
//	Methods
	void ParsePartiesIPs (string PartiesIPs);
	void CreateDDSTopics ();
	void CreateDDSTopicChannelWriters ();
	void CreateOwnTopicReader ();
	void VerifyConnections ();
	bool isSubscriptionMatched ();

// Variables
//	std::vector<string> _partiesIPList;
	std::vector<dds::topic::Topic<MyStruct>> _DDSTopicsList;
	std::vector<DDSChannelWriter> _DDSChannelWriterList;
	dds::domain::DomainParticipant *_participant;
	dds::sub::DataReader<MyStruct> *_reader;
	dds::sub::LoanedSamples<MyStruct> _samples;
	dds::sub::LoanedSamples<MyStruct>::iterator _sampleIt;
	const int domain_id = 7;
	string _ownIP;
	unsigned int _id;
	unsigned int _numParties;
	unsigned int _elapsedTime;
};

#endif /* DDSCOMMUNICATOR_HPP_ */
