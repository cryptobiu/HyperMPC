//
// Created by hila on 03/11/16.
//

#ifndef SECRET_SHARING_FIELD_H
#define SECRET_SHARING_FIELD_H

#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>
#include <stdexcept>
#include "AES_PRG.h"
#include <stdint.h>
#include <bitset>
#include "TFieldElement.h"
#include "TFieldElementZp.h"
#include "Def.h"

using namespace std;
using namespace NTL;

using namespace std;
template <class T>
class Field {
private:
    static bool m_instanceFlag;
    static Field *m_single;
    TFIELD_ELEMENT* m_ZERO;
    TFIELD_ELEMENT* m_ONE;
public:
    static Field* getInstance();
    TFIELD_ELEMENT* GetZero();
    TFIELD_ELEMENT* GetOne();
    void InitOne();
    /*
     * The i-th field element. The ordering is arbitrary, *except* that
     * the 0-th field element must be the neutral w.r.t. addition, and the
     * 1-st field element must be the neutral w.r.t. multiplication.
     */
    T GetElement(TYPE b);
    TFIELD_ELEMENT Random();
    virtual ~Field();
};


#endif //SECRET_SHARING_FIELD_H
