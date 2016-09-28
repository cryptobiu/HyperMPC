#include "ArythmeticCircuit.h"

#include <fstream>      // std::ifstream




ArythmeticCircuit::ArythmeticCircuit()
{
}

ArythmeticCircuit::ArythmeticCircuit(int i)
{
   // vector<TGate> gates;
    gates.resize(14);
    TGate p1;
    p1.gateType = INPUT;
    p1.party = 1;
    p1.input1 = -1;
    p1.input2 = -1;
    p1.output = 0;
    gates[0] = p1;
    TGate p2;
    p2.gateType = INPUT;
    p2.party = 2;
    p2.input1 = -1;
    p2.input2 = -1;
    p2.output = 1;
    gates[1] = p2;
    TGate p3;
    p3.gateType = INPUT;
    p3.party = 3;
    p3.input1 = -1;
    p3.input2 = -1;
    p3.output = 2;
    gates[2] = p3;
    TGate p4;
    p4.gateType = INPUT;
    p4.party = 4;
    p4.input1 = -1;
    p4.input2 = -1;
    p4.output = 3;
    gates[3] = p4;
    TGate p5;
    p5.gateType = INPUT;
    p5.party = 5;
    p5.input1 = -1;
    p5.input2 = -1;
    p5.output =4;
    gates[4] = p5;
    TGate p6;
    p6.gateType = INPUT;
    p6.party = 6;
    p6.input1 = -1;
    p6.input2 = -1;
    p6.output = 5;
    gates[5] = p6;
    TGate p7;
    p7.gateType = INPUT;
    p7.party = 7;
    p7.input1 = -1;
    p7.input2 = -1;
    p7.output = 6;
    gates[6] = p7;

    TGate p8;
    p8.gateType = ADD;
    p8.party = -1;
    p8.input1 = 0;
    p8.input2 = 1;
    p8.output = 7;
    gates[7] = p8;
    TGate p9;
    p9.gateType = ADD;
    p9.party = -1;
    p9.input1 = 2;
    p9.input2 = 3;
    p9.output = 8;
    gates[8] = p9;
    TGate p10;
    p10.gateType = ADD;
    p10.party = -1;
    p10.input1 = 4;
    p10.input2 = 5;
    p10.output = 9;
    gates[9] = p10;
    TGate p11, p12, p13, p14;
    p11.gateType = ADD;
    p11.party = -1;
    p11.input1 = 7;
    p11.input2 = 8;
    p11.output = 10;
    gates[10] = p11;
    p12.gateType = ADD;
    p12.party = -1;
    p12.input1 = 9;
    p12.input2 = 6;
    p12.output = 11;
    gates[11] = p12;
    p13.gateType = ADD;
    p13.party = -1;
    p13.input1 = 10;
    p13.input2 = 11;
    p13.output = 12;
    gates[12] = p13;
    p14.gateType = OUTPUT;
    p14.party = 7;
    p14.input1 = 12;
    p14.input2 = -1;
    p14.output = -1;
    gates[13] = p14;

    nrOfMultiplicationGates = 0;
    nrOfAdditionGates = 6;
    nrOfInputGates = 7;
    nrOfOutputGates = 1;
}

ArythmeticCircuit::~ArythmeticCircuit()
{
}

void ArythmeticCircuit::readCircuit(const char* fileName)
{

    int inFan, outFan, input1, input2, output, type, numOfinputsForParty, numOfoutputsForParty;
    int numberOfGates, numberOfParties, numberOfOutputs, currentPartyNumber;
    int gateIndex = 0;
    ifstream myfile;


    myfile.open(fileName);


    int **partiesInputs;

    if (myfile.is_open())
    {

        myfile >> numberOfGates;//get the gates
        myfile >> numberOfParties;


        //inputs
        int *numOfInputsForEachParty = new int[numberOfParties];
        int **partiesInputs = new int*[numberOfParties];

        for (int j = 0; j<numberOfParties; j++) {
            myfile >> currentPartyNumber;

            myfile >> numOfinputsForParty;
            numOfInputsForEachParty[currentPartyNumber - 1] = numOfinputsForParty;

            partiesInputs[currentPartyNumber - 1] = new int[numOfInputsForEachParty[currentPartyNumber - 1]];

            for (int i = 0; i<numOfInputsForEachParty[currentPartyNumber - 1]; i++) {
                myfile >> partiesInputs[currentPartyNumber - 1][i];
            }
        }

        //outputs
        int *numOfOutputsForEachParty = new int[numberOfParties];
        int **partiesOutputs = new int*[numberOfParties];

        for (int j = 0; j<numberOfParties; j++) {
            myfile >> currentPartyNumber;

            myfile >> numOfoutputsForParty;
            numOfOutputsForEachParty[currentPartyNumber - 1] = numOfoutputsForParty;

            partiesOutputs[currentPartyNumber - 1] = new int[numOfOutputsForEachParty[currentPartyNumber - 1]];

            for (int i = 0; i<numOfOutputsForEachParty[currentPartyNumber - 1]; i++) {
                myfile >> partiesOutputs[currentPartyNumber - 1][i];
            }
        }




        //calculate the total number of inputs and outputs
        for (int i = 0; i<numberOfParties; i++) {
            nrOfInputGates += numOfInputsForEachParty[i];
            nrOfOutputGates += numOfOutputsForEachParty[i];
        }

        //allocate memory for the gates, We add one gate for the all-one gate whose output is always 1 and thus have a wire who is always 1 without the
        //involvement of the user. This will be useful to turn a NOT gate into a XORGate
        gates.resize(numberOfGates + nrOfInputGates + nrOfOutputGates);

        //create the input gates

        //create the input gates for each party
        for (int i = 0; i < numberOfParties; i++) {

            for (int j = 0; j < numOfInputsForEachParty[i];j++) {

                gates[gateIndex].gateType = 0;
                gates[gateIndex].input1 = -1;//irrelevant
                gates[gateIndex].input2 = -1;//irrelevant
                gates[gateIndex].output = partiesInputs[i][j];//the wire index
                gates[gateIndex].party = i + 1;

                gateIndex++;

            }
        }

        //go over the file and create gate by gate
        for (int i = nrOfInputGates; i<numberOfGates + nrOfInputGates; i++)
        {

            //get  each row that represents a gate
            myfile >> inFan;
            myfile >> outFan;
            myfile >> input1;
            myfile >> input2;
            myfile >> output;
            myfile >> type;

            gates[i].input1 = input1;
            gates[i].input2 = input2;
            gates[i].output = output;
            gates[i].gateType = type;
            gates[i].party = -1;//irrelevant

            if (type == 1) {
                nrOfAdditionGates++;
            }
            else if (type = 2) {
                nrOfMultiplicationGates++;
            }

        }

        gateIndex = numberOfGates + nrOfInputGates;
        //create the output gates for each party
        for (int i = 0; i < numberOfParties; i++) {

            for (int j = 0; j < numOfOutputsForEachParty[i]; j++) {

                gates[gateIndex].gateType = 3;
                gates[gateIndex].input1 = partiesOutputs[i][j];
                gates[gateIndex].input2 = 0;//irrelevant
                gates[gateIndex].output = 0;//irrelevant
                gates[gateIndex].party = i + 1;

                gateIndex++;

            }
        }


        for (int i = 0; i < numberOfParties; ++i) {
            delete[] partiesInputs[i];
            delete[] partiesOutputs[i];
        }

        delete[] numOfInputsForEachParty;
        delete[] numOfOutputsForEachParty;
        delete[] partiesInputs;
        delete[] partiesOutputs;

    }
    myfile.close();
}