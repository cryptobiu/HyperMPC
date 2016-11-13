//
// Created by meital on 07/11/16.
//

#ifndef SECRET_SHARING_TFIELDELEMENTGF2E_H
#define SECRET_SHARING_TFIELDELEMENTGF2E_H

#include <iostream>
#include <NTL/GF2X.h>
#include <stdint.h>
#include <NTL/GF2E.h>
#include <bitset>
#include <string>
#include<NTL/GF2XFactoring.h>
#include <sstream>
#include "Def.h"

using namespace std;

class TFieldElementGF2E {
private:
    NTL::GF2E m_element;
public:
    TFieldElementGF2E();
    TFieldElementGF2E(NTL::GF2E element);
    TFieldElementGF2E(string str);
    NTL::GF2E getElement() const;
    TFieldElementGF2E operator+(TFieldElementGF2E& f2);
    TFieldElementGF2E operator-(TFieldElementGF2E& f2);
    TFieldElementGF2E operator/(TFieldElementGF2E& f2);
    TFieldElementGF2E operator*(TFieldElementGF2E& f2);
    TFieldElementGF2E& operator+=(TFieldElementGF2E& f2);
    TFieldElementGF2E& operator*=(TFieldElementGF2E& f2);
    string toString();
    TFieldElementGF2E& operator=(const TFieldElementGF2E& f2);
    virtual ~TFieldElementGF2E();


};


#endif //SECRET_SHARING_TFIELDELEMENTGF2E_H
