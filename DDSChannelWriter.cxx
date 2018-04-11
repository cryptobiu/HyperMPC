/*
 * ChannelWriter.cpp
 *
 *  Created on: Apr 7, 2018
 *      Author: GalSchlesinger
 */


#include "DDSChannelWriter.hpp"


DDSChannelWriter::DDSChannelWriter(dds::domain::DomainParticipant *participant, dds::topic::Topic<MyStruct> DDSTopic, string ownIP) {
	//cout<<"CreateChannelWriter"<<endl;

	_writer = new dds::pub::DataWriter<MyStruct> (dds::pub::Publisher(*participant), DDSTopic);
	//Initialize once for all messages
	_sample.ID(ownIP);
}

DDSChannelWriter::~DDSChannelWriter() {
}


void DDSChannelWriter::Write (const char* my_data,int size){
	//cout<<"writing"<<endl;
	_sample.data(vector<char>(my_data,my_data+size));
    _writer->write(_sample);
}

string DDSChannelWriter::GetTopicName (){
	return _writer->topic()->name();
}

bool DDSChannelWriter::isPublicationMatched (){
	//cout << "isPublicationMatched" << endl;
	unsigned int publicationMatchedCount;
	auto startTime = high_resolution_clock::now();

	while (true){
		publicationMatchedCount = _writer->publication_matched_status()->current_count();
		if (publicationMatchedCount > 0){
			return true;
		}

		auto checkTime = high_resolution_clock::now();
		auto timeSpanSeconds = duration_cast<seconds>(checkTime - startTime);
		if (timeSpanSeconds.count() > 5){
			throw runtime_error("Initialization time elapsed with party" + this->GetTopicName());
		}

		rti::util::sleep(dds::core::Duration::from_millisecs(100));
	}
}

