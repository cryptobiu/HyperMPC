//
// Created by hila on 15/09/16.
//

#ifndef TGATE_H_
#define TGATE_H_

enum TGateType {InputGate, OutputGate, AdditionGate, MultiplicationGate};

class TGate {
private:
    int Input1;
    int Input2;
    int Output;
    int Party;
    TGateType GateType;
public:
    TGate();

};

#endif /* TGATE_H_ */