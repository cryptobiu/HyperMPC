#ifndef SECRET_SHARING_WIRE_H
#define SECRET_SHARING_WIRE_H

#include <iostream>
#include <vector>
#include "stdlib.h"
#include "TFieldElement.h"

using namespace std;

class Wire {
public:
    TFieldElement* data;
    vector<int> in;
    vector<int> out;
    int index;
};


#endif //SECRET_SHARING_WIRE_H
