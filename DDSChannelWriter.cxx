/*
 * ChannelWriter.cpp
 *
 *  Created on: Apr 7, 2018
 *      Author: GalSchlesinger
 */


#include "DDSChannelWriter.hpp"


DDSChannelWriter::DDSChannelWriter(dds::domain::DomainParticipant *participant, dds::topic::Topic<BIUDDSStruct> DDSTopic, string ownIP, unsigned long ownID, bool printDebugFlag) {
	_printDebugFlag = printDebugFlag;
	if (_printDebugFlag) cout<<"CreateChannelWriter"<<endl;

	_writer = new dds::pub::DataWriter<BIUDDSStruct> (dds::pub::Publisher(*participant), DDSTopic);
	
	stringstream topicName = stringstream(DDSTopic.name());
	getline(topicName, _topicID, '-');
	getline(topicName, _topicIP, '-');
	
	//Initialize once for all messages
	_sample.sourceIP(ownIP);
	_sample.sourceID(ownID);
}

DDSChannelWriter::~DDSChannelWriter() {
}


void DDSChannelWriter::Write (const char* my_data,int size, int myID, string tag ){
	if (_printDebugFlag) cout<<"writing"<<endl;

	_sample.payload(vector<char>(my_data,my_data+size));
	_sample.tag(tag);
	_sample.sequenceNumber(_samplesSequenceNumber);
	_sample.sourceID(myID);
//	cout << "Writing data len : " << _sample.payload().size() << " seq number is : " << _samplesSequenceNumber <<
//         " my id : " << myID << endl;
    _writer->write(_sample);
	_samplesSequenceNumber++;
}

bool DDSChannelWriter::isPublicationMatched (unsigned int elapsedTime){
	if (_printDebugFlag) cout << "isPublicationMatched" << endl;
	unsigned int publicationMatchedCount;
	auto startTime = high_resolution_clock::now();

	while (true){
		publicationMatchedCount = _writer->publication_matched_status()->current_count();
		if (publicationMatchedCount > 0){
			return true;
		}

		auto checkTime = high_resolution_clock::now();
		auto timeSpanSeconds = duration_cast<seconds>(checkTime - startTime);
		if (timeSpanSeconds.count() > elapsedTime){
			throw runtime_error("Initialization time elapsed with party " + this->GetTopicIP());
		}

		rti::util::sleep(dds::core::Duration::from_millisecs(100));
	}
}

