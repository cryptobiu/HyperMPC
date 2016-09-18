#include "TCircuit.h"

TCircuit::TCircuit()
{

}

int TCircuit::getnumOfMultiplicationGates()
{
    return this->numOfAdditionGates;
}

int TCircuit::getnumOfRandomGates()
{
    return this->numOfRandomGates;
}

int TCircuit::getnumOfInputGates()
{
    return this->numOfInputGates;
}
