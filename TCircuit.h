#ifndef CIRCUIT_H_
#define CIRCUIT_H_

#include "TGate.h"
#include <vector>

using namespace std;

class TCircuit {
private:
    vector<TGate> m_gates;
    int numOfMultiplicationGates;
    int numOfRandomGates;
    int numOfInputGates;
    int numOfOutputGates;
    int numOfAdditionGates;
public:
    TCircuit();
};

#endif /* CIRCUIT_H_ */