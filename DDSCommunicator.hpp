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
#include "DDSChannelWriter.hpp"

#include <iostream>
#include <exception>
#include <map>

#include <libscapi/include/infra/ConfigFile.hpp>

//#include "Test.hpp"

using namespace std;
using namespace chrono;

class DDSCommunicator {
public:
	DDSCommunicator(int id, int numParties, string configFile);
	virtual ~DDSCommunicator();
	DDSChannelWriter GetDDSChannelWriter (string PartyIP);
	void Read (map<string,rti::core::bounded_sequence<char,(MAX_SEQUENCE_SIZE)>> *readinputMap);
	string getMyIP(){ return _ownIP;}


private:
//	Methods
	void ParsePartiesIPs (string PartiesIPs);
	void CreateDDSTopics ();
	void CreateDDSTopicChannelWriters ();
	void CreateOwnTopicReader ();
	void VerifyConnections ();
	bool isSubscriptionMatched ();

// Variables
	vector<string> _partiesIPList;
	vector<dds::topic::Topic<MyStruct>> _DDSTopicsList;
	vector<DDSChannelWriter> _DDSChannelWriterList;
	dds::domain::DomainParticipant *_participant;
	dds::sub::DataReader<MyStruct> *_reader;
	dds::sub::LoanedSamples<MyStruct> _samples;
	dds::sub::LoanedSamples<MyStruct>::iterator _sampleIt;
	const int domain_id = 7;
	string _ownIP;
	unsigned int _id;
	unsigned int _numParties;
};

#endif /* DDSCOMMUNICATOR_HPP_ */
