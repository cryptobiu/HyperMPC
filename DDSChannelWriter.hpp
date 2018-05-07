/*
 * ChannelWriter.h
 *
 *  Created on: Apr 7, 2018
 *      Author: GalSchlesinger
 */

#ifndef DDSCHANNELWRITER_HPP_
#define DDSCHANNELWRITER_HPP_

#include <iostream>
#include <stdint.h>
#include <chrono>
#include <dds/domain/DomainParticipant.hpp>
#include <dds/pub/ddspub.hpp>
#include <dds/sub/ddssub.hpp>
#include <dds/pub/DataWriter.hpp>
#include "BIUDDSType.hpp"

using namespace std;
using namespace chrono;

class DDSChannelWriter {
public:
	DDSChannelWriter(dds::domain::DomainParticipant *_participant, dds::topic::Topic<BIUDDSStruct> _topic, string ownIP, unsigned long ownID, bool printDebugFlag = false);
	virtual ~DDSChannelWriter();
	void Write (const char* data,int size, int myID, string tag = "");
	string GetTopicIP() { return _topicIP; };
	string GetTopicID() { return _topicID; };
	bool isPublicationMatched (unsigned int elapsedTime);

private:
//	Methods

//	Variables
    dds::pub::DataWriter<BIUDDSStruct> *_writer;
	BIUDDSStruct _sample;
	string _topicIP;
	string _topicID;
	bool _printDebugFlag;
	unsigned int _samplesSequenceNumber = 1;
};

#endif /* DDSCHANNELWRITER_HPP_ */
