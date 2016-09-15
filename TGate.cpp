#include "TGate.h"

TGate::TGate(){};

TGateType TGate::getTGateType()
{
    return this->GateType;
}

int TGate::getParty()
{
    return this->Party;
}