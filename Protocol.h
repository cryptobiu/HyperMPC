#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#include "stdlib.h"
#include "../../workspace/paho/src/MQTTClient.h"
#include "TParty.h"
#include "VDM.h"
#include "TGate.h"
#include "ArithmeticCircuit.h"
#include "Communication.h"
#include <vector>
#include <bitset>
#include "TFieldElement.h"
#include <iostream>
#include <fstream>
#include <chrono>

using namespace std;
using namespace std::chrono;

#define ADDRESS  "tcp://localhost:1883"

class Protocol {
private:
    int N, M, T, m_partyId;
    string inputsFile, outputFile;
    Communication* comm;
public:
    Protocol(int n, int id, string inputsFile, string outputFile);
    void split(const string &s, char delim, vector<string> &elems);
    vector<string> split(const string &s, char delim);
    bool broadcast(int party_id, string myMessage, vector<string> &recBufsdiff, HIM &mat);
    int processMultiplications(ArithmeticCircuit &circuit, vector<bool> &gateDoneArr,
                               vector<TFieldElement> &gateShareArr, vector<string> &sharingBuf,
                               vector<TFieldElement> &alpha, HIM &m);
    void inputAdjustment(string &diff, vector<TFieldElement> &gateValueArr, ArithmeticCircuit &circuit,
                         vector<TFieldElement> &gateShareArr, vector<bool> &GateDoneArr, HIM &mat);
    void initializationPhase(vector<TFieldElement> &gateValueArr, vector<TFieldElement> &gateShareArr,
                             vector<bool> &gateDoneArr,
                             HIM &matrix_him, VDM &matrix_vand, vector<TFieldElement> &alpha, HIM &m);
    void publicReconstruction(vector<string> &myShares, int &count, int d, vector<TFieldElement> &alpha, vector<TFieldElement> &valBuf, HIM &m);
    bool preparationPhase(VDM &matrix_vand, HIM &matrix_him, vector<string> &sharingBuf, vector<TFieldElement> &alpha, ArithmeticCircuit &circuit);
    bool inputPreparation(vector<string> &sharingBuf, vector<TFieldElement> &gateShareArr, ArithmeticCircuit &circuit, vector<TFieldElement> &alpha, vector<TFieldElement> &gateValueArr);
    bool checkConsistency(vector<TFieldElement> alpha,vector<TFieldElement> x, int d);
    int processAdditions(ArithmeticCircuit &circuit, vector<bool> &gateDoneArr, vector<TFieldElement> &gateShareArr);
    TFieldElement interpolate(vector<TFieldElement> alpha, vector<TFieldElement> x);
    void outputPhase(ArithmeticCircuit &circuit, vector<TFieldElement> &gateShareArr, vector<TFieldElement> alpha, vector<TFieldElement> &gateValueArr);
    void run();
    ~Protocol();
};


#endif /* PROTOCOL_H_ */
