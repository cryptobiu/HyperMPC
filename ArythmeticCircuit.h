#ifndef CIRCUIT_H_
#define CIRCUIT_H_

#include "TGate.h"
#include <vector>


#include <vector>
#include "TGate.h"

/**
* A software representation of the structure of an arythmetic circuit.
* The circuit consists of Input, Addition, Multiplication, and Output gates. Technically, a circuit
* is essentially an array of gates, with some bookkeeping information. Each gate has associated input gates
* (at most 2) and output gates (at most 1). Input and Output gates also have an associated party.
* We assume that the gates in the circuit are ordered, i.e., each gate only depends on gates with smaller
* index.
*
*/

using namespace std;



class ArythmeticCircuit
{
private:

    vector<TGate> gates;
    int nrOfMultiplicationGates = 0;
    int nrOfAdditionGates = 0;
    int nrOfInputGates = 0;
    int nrOfOutputGates = 0;

public:

    ArythmeticCircuit(int i);
    ArythmeticCircuit();
    ~ArythmeticCircuit();

    /**
    * This method reads text file and creates an object of ArythmeticCircuit according to the file.
    * This includes creating the gates and other information about the parties involved.
    *
    */
    void readCircuit(const char* fileName);

    //get functions
    int getNrOfMultiplicationGates() { return nrOfMultiplicationGates; }
    int getNrOfAdditionGates() { return nrOfAdditionGates; }
    int getNrOfInputGates() { return nrOfInputGates; }
    int getNrOfOutputGates() { return nrOfOutputGates; }
    int getNrOfGates() { return (nrOfMultiplicationGates + nrOfAdditionGates + nrOfInputGates + nrOfOutputGates); }

    vector<TGate> const & getGates() const {	return gates;};
};

#endif /* CIRCUIT_H_ */