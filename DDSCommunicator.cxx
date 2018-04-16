/*
 * DDSCommunicator.cpp
 *
 *  Created on: Apr 7, 2018
 *      Author: GalSchlesinger
 */


#include <fstream>
#include "DDSCommunicator.hpp"


DDSCommunicator::DDSCommunicator(int id, int numParties, string configFile, unsigned int elapsedTime, bool addIPtoPeerList) {
	//cout << "DDSCommunicator Constructor" << endl;
	_id = id;
	_numParties = numParties;
	_elapsedTime = elapsedTime;
	ParsePartiesIPs(configFile);

// Loading DDS QOS
	rti::core::QosProviderParams provider_params;
	auto QOSConfigFile = "BIU_QOS_Definition.xml";
	dds::core::StringSeq stringseq(1);
	stringseq.at(0) = QOSConfigFile;
	provider_params.url_profile(stringseq);
	dds::core::QosProvider::Default()->default_provider_params(provider_params);

//For monitoring purposes, setting the role name of the participant to OwnIP.
	dds::domain::qos::DomainParticipantQos parQOS = dds::core::QosProvider::Default()->participant_qos();
	parQOS->participant_name.role_name(_ownIP);
// Create DDS Participant
	_participant = new dds::domain::DomainParticipant(domain_id,parQOS,NULL,dds::core::status::StatusMask::all());

//adding the peers list to the Participant for discovery purposes.
	for (auto IP : _partiesIPList) {
		if (addIPtoPeerList == false) break;
		(*_participant)->add_peer(IP);
	}

	this->CreateDDSTopics();
	this->CreateDDSTopicChannelWriters();
	this->CreateOwnTopicReader();
	this->VerifyConnections();
}

DDSCommunicator::~DDSCommunicator() {
	// TODO Auto-generated destructor stub
}

void DDSCommunicator::ParsePartiesIPs(string PartiesIPs) {
	//cout << "ParsePartiesIPs" << endl;

    ifstream file(PartiesIPs.c_str());
    string line;
    string data;

    while (getline(file, data))
    {
        if(!data.length())
            continue;
        line += data;
    }

	istringstream PartiesIPsStream(line);
	string IP;
	while (getline(PartiesIPsStream, IP, ',')) {
		_partiesIPList.push_back(IP);
	}
	_ownIP=_partiesIPList[_id];
}

void DDSCommunicator::CreateDDSTopics() {
	//cout << "CreateDDSTopics" << endl;

	for (auto IP : _partiesIPList) {
		string DDSTopicName = IP;
		dds::topic::Topic<MyStruct> topic(*_participant, DDSTopicName.c_str());
		_DDSTopicsList.push_back(topic);
	}
}

void DDSCommunicator::CreateDDSTopicChannelWriters() {
	//cout << "CreateDDSTopicChannelWriters" << endl;
	for (auto DDSTopic : _DDSTopicsList) {
		//don't need to create a writer to my own topic
		if (_DDSTopicsList[_id]->instance_handle()
				== DDSTopic->instance_handle()){
			continue;
		}
		DDSChannelWriter writer(_participant, DDSTopic, _ownIP);
		_DDSChannelWriterList.push_back(writer);
	}
}

void DDSCommunicator::CreateOwnTopicReader() {
	_reader = new dds::sub::DataReader<MyStruct>(
			dds::sub::Subscriber(*_participant), _DDSTopicsList[_id]);
}

DDSChannelWriter DDSCommunicator::GetDDSChannelWriter(string PartyIP) {
	for (auto DDSChannelWriter : _DDSChannelWriterList) {
		if (DDSChannelWriter.GetTopicName() == PartyIP) {
			return DDSChannelWriter;
		}
	}
	throw runtime_error("No channel was found for the specified IP");
}

void DDSCommunicator::Read(map<string,rti::core::bounded_sequence<char,(MAX_SEQUENCE_SIZE)>> *readinputMap) {
	//cout << "Read" << endl;

	map<string,rti::core::bounded_sequence<char,(MAX_SEQUENCE_SIZE)>> readInputMap;

	for (unsigned int samplesCounter = 0; samplesCounter < (_numParties - 1);) {
		_samples = _reader->take();//todo change history depth QOS
		for (_sampleIt = _samples.begin() ; _sampleIt != _samples.end() ; ++_sampleIt) {
			if (_sampleIt->info().valid()) {
				samplesCounter++;
				string source = _sampleIt->data().ID();
				readinputMap->insert(make_pair(source,_sampleIt->data().data()));
			}
			else{
				throw runtime_error("Received non-valid data");
			}
		 }
	}
	if (readinputMap->size()!=(_numParties-1)){
		throw runtime_error("Not all data was received and the timeout has passed");
	}
}


bool DDSCommunicator::isSubscriptionMatched (){
	//cout << "isSubscriptionMatched" << endl;
	unsigned int SubscriptionMatchedCount;
	auto startTime = high_resolution_clock::now();

	while (true){
		SubscriptionMatchedCount = _reader->subscription_matched_status()->current_count();
		if (SubscriptionMatchedCount == (_numParties-1)){
			return true;
		}

		auto checkTime = high_resolution_clock::now();
		auto timeSpanSeconds = duration_cast<seconds>(checkTime - startTime);
		if (timeSpanSeconds.count() > _elapsedTime){
			throw runtime_error("Initialization time elapsed on communicator reader");
		}

		rti::util::sleep(dds::core::Duration::from_millisecs(100));
	}
}

void DDSCommunicator::VerifyConnections (){
	//cout << "VerifyConnections" << endl;
	for (auto channelWriter : _DDSChannelWriterList){
		channelWriter.isPublicationMatched(_elapsedTime);
	}
	this->isSubscriptionMatched();
}
