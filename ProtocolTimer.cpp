//
// Created by meital on 18/12/16.
//

#include "ProtocolTimer.h"


void ProtocolTimer::writeToFile() {

    ofstream myfile;
    myfile.open (fileName);
    myfile << "-------------Timings-----------.\n";
    //columns header
    myfile << "preparationPhaseArr,inputPreparationArr,inputAdjustmentArr,computationPhaseArr,outputPhaseArr, totalTimeArr,\n";

    for(int i=0; i<times; i++) {
        myfile << preparationPhaseArr[i] << ","
        << inputPreparationArr[i] << ","
        << inputAdjustmentArr[i] << ","
        << computationPhaseArr[i] << ","
        << outputPhaseArr[i] << ","
        << totalTimeArr[i] << ",\n";

        cout<< "Times" <<preparationPhaseArr[i] << ","
        << inputPreparationArr[i] << ","
        << inputAdjustmentArr[i] << ","
        << computationPhaseArr[i] << ","
        << outputPhaseArr[i] << ","
        << totalTimeArr[i] << ",\n";
    }

    myfile.close();

}

ProtocolTimer::ProtocolTimer(int times, string fileName) : fileName(fileName), times(times){


    preparationPhaseArr = new int[times];
    inputPreparationArr = new int[times];
    inputAdjustmentArr= new int[times];
    computationPhaseArr= new int[times];
    outputPhaseArr= new int[times];
    totalTimeArr= new int[times];

}



