/*
 * DDSCommunicator.cpp
 *
 *  Created on: Apr 7, 2018
 *      Author: GalSchlesinger
 */


#include <fstream>
#include "DDSCommunicator.hpp"


DDSCommunicator::DDSCommunicator(int id, int numParties, string configFile, unsigned int elapsedTime, bool AddDiscoveryPeers, bool printDebugFlag) {
	_printDebugFlag = printDebugFlag;
	if (_printDebugFlag) cout << "DDSCommunicator Constructor" << endl;
	_ownID = id;
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
	_participant = new dds::domain::DomainParticipant(domain_id, parQOS, NULL, dds::core::status::StatusMask::all());

	//adding the peers list to the Participant for discovery purposes.
	for (auto IP : _partiesIPList) {
		if (AddDiscoveryPeers == false) break;
		(*_participant)->add_peer(IP);
	}
	(*_participant)->add_peer("100@localhost");
	this->CreateDDSTopics();
	this->CreateDDSTopicChannelWriters();
	this->CreateOwnTopicReader();
	this->VerifyConnections();
}

DDSCommunicator::~DDSCommunicator() {
	// TODO Auto-generated destructor stub
}

void DDSCommunicator::ParsePartiesIPs(string configFileDir) {
	if (_printDebugFlag) cout << "ParsePartiesIPs" << endl;

	ifstream file(configFileDir.c_str());
	if (file.fail()) {
		throw runtime_error("Failed to open/find the IPs configFile");
	}
	string line;
	string data;

	while (getline(file, data))
	{
		if (!data.length())
			continue;
		line += data;
	}

	istringstream PartiesIPsStream(line);
	string IP;
	while (getline(PartiesIPsStream, IP, ',')) {
		_partiesIPList.push_back(IP);
	}

	if (_ownID<0 || _ownID>(_partiesIPList.size() - 1)) {
		throw runtime_error("ownID is is out of bounds");
	}

	_ownIP = _partiesIPList[_ownID];

	if (_partiesIPList.size() != _numParties) {
		throw runtime_error("_partiesIPList.size() != _numParties");
	}

}

void DDSCommunicator::CreateDDSTopics() {
	if (_printDebugFlag) cout << "CreateDDSTopics" << endl;
	unsigned int IPCounter = 0;
	string DDSTopicName;
	for (auto IP : _partiesIPList) {
		DDSTopicName = to_string(IPCounter);
		DDSTopicName += "-";
		DDSTopicName += IP;
		dds::topic::Topic<BIUDDSStruct> topic(*_participant, DDSTopicName.c_str());
		_DDSTopicsList.push_back(topic);

		IPCounter++;
	}
}

void DDSCommunicator::CreateDDSTopicChannelWriters() {
	if (_printDebugFlag) cout << "CreateDDSTopicChannelWriters" << endl;
	for (auto DDSTopic : _DDSTopicsList) {
		//don't need to create a writer to my own topic
		if (_DDSTopicsList[_ownID]->instance_handle()
			== DDSTopic->instance_handle()) {
			continue;
		}
		_DDSChannelWriterList.push_back(new DDSChannelWriter(_participant, DDSTopic, _ownIP, _ownID, _printDebugFlag));
	}
}

void DDSCommunicator::CreateOwnTopicReader() {
	_reader = new dds::sub::DataReader<BIUDDSStruct>(
		dds::sub::Subscriber(*_participant), _DDSTopicsList[_ownID]);

	_readCondition = new ReadCondition(
		*_reader, DataState(
			SampleState::not_read(),
			ViewState::any(),
			InstanceState::any()));

	_waitset = new WaitSet;

	_waitset->attach_condition(*_readCondition);
}

DDSChannelWriter* DDSCommunicator::getDDSChannelWriter(string PartyIP) {
	for (auto DDSChannelWriter : _DDSChannelWriterList) {
		if (DDSChannelWriter->GetTopicIP() == PartyIP) {
			return DDSChannelWriter;
		}
	}
	throw runtime_error("No channel was found for the specified IP");
}

void DDSCommunicator::ReadAllInputs(map<unsigned long , DDSSample* > *samplesMap) {

}

void DDSCommunicator::ReadInput(DDSSample* sample) {
	if (_printDebugFlag) cout << "ReadInput" << endl;

	_waitset->wait(dds::core::Duration(50000)); //practically infinite
		
	*_reader >> take >> max_samples(1) >> _loandSamples;
	_sampleIt = _loandSamples.begin();
	*(sample->getUnderlyingSample()) = *_sampleIt;

//	if (_sampleIt->info().valid()) {
//		*(sample->getUnderlyingSample()) = *_sampleIt;
//	}
//	else {
//		throw runtime_error("Received a non-valid DDS sample");
//	}
}



bool DDSCommunicator::isSubscriptionMatched() {
	if (_printDebugFlag) cout << "isSubscriptionMatched" << endl;
	unsigned int SubscriptionMatchedCount;
	auto startTime = high_resolution_clock::now();

	while (true) {
		SubscriptionMatchedCount = _reader->subscription_matched_status()->current_count();
		if (SubscriptionMatchedCount == (_numParties - 1)) {
			return true;
		}

		auto checkTime = high_resolution_clock::now();
		auto timeSpanSeconds = duration_cast<seconds>(checkTime - startTime);
		if (timeSpanSeconds.count() > _elapsedTime) {
			throw runtime_error("Initialization time elapsed on communicator reader");
		}

		rti::util::sleep(dds::core::Duration::from_millisecs(100));
	}
}

void DDSCommunicator::VerifyConnections() {
	if (_printDebugFlag) cout << "VerifyConnections" << endl;
	for (auto channelWriter : _DDSChannelWriterList) {
		channelWriter->isPublicationMatched(_elapsedTime);
	}
	this->isSubscriptionMatched();
}
