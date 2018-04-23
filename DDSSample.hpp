/*
* ChannelWriter.cpp
*
*  Created on: Apr 19, 2018
*      Author: GalSchlesinger
*/

#ifndef DDSSAMPLE_HPP_
#define DDSSAMPLE_HPP_

#include <dds/sub/ddssub.hpp>
#include <dds/dds.hpp>

#include <iostream>

#include "BIUDDSType.hpp"


using namespace std;
using namespace dds::sub;

class DDSSample
{
public:
	DDSSample();
	~DDSSample();
	string getTag() { return _sample.data().tag(); };
	unsigned long getSourceID() { return _sample.data().sourceID(); };
	string getSourceIP() { return _sample.data().sourceIP(); };
	const char* getPayload() {return _sample.data().payload().data(); };
	unsigned long getSequenceNumber() { return _sample.data().sequenceNumber(); };
	Sample<BIUDDSStruct>* getUnderlyingSample() { return &_sample; };


private:
	Sample<BIUDDSStruct> _sample;
};


#endif /* DDSSAMPLE_HPP_ */

