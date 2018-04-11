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
#include "Test.hpp"

using namespace std;
using namespace chrono;

class DDSChannelWriter {
public:
	DDSChannelWriter(dds::domain::DomainParticipant *_participant, dds::topic::Topic<MyStruct> _topic, string ownIP);
	virtual ~DDSChannelWriter();
	void Write (const char* data,int size);
	string GetTopicName ();
	bool isPublicationMatched ();

private:
//	Methods

//	Variables
    dds::pub::DataWriter<MyStruct> *_writer;
    MyStruct _sample;

};

#endif /* DDSCHANNELWRITER_HPP_ */
