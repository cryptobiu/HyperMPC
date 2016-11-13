//
// Created by meital on 07/11/16.
//

#ifndef SECRET_SHARING_TFIELDGF2E_H
#define SECRET_SHARING_TFIELDGF2E_H

#include "TFieldElementGF2E.h"
#include "AES_PRG.h"
#include <stdint.h>
#include <bitset>
#include "Def.h"

using namespace std;
using namespace NTL;

/**
 * We consider extension fields GF(b^e), where a field element is a polynomial of degree at most e over Zb,
 * and can be represented as an array.
 * We could restrict ourselves to fields with characteristic 2 and represent field elements as integers, where
 * the i-th bit is the i-th coefficient. We assume that the operators + and ∗ are overloaded for field elements;
 * We need to be able to enumerate field elements in some arbitrary order. For simplicity, we assume that
 * the 0-element is the zero element (neutral of addition), and the 1-element is the one element (neutral of
 * multiplication). Furthermore, we need to sample with uniform distribution in the field.
 * Note: Multiplication table could be pre-computed.
 *
 * We focus on GF(2^8).
 */


class TFieldGF2E {
private:
    static bool m_instanceFlag;
    static TFieldGF2E *m_single;
    TFieldElementGF2E* m_ZERO;
    TFieldElementGF2E* m_ONE;
    /**
     * the function create a field by:
     * generate the irreducible polynomial x^8 + x^4 + x^3 + x + 1 to work with
     * init the field with the newly generated polynomial
     */
    TFieldGF2E();
public:
    /**
     * return the field
     */
    static TFieldGF2E* getInstance();
    TFieldElementGF2E* GetZero();
    TFieldElementGF2E* GetOne();
    void InitOne();
    /*
     * The i-th field element. The ordering is arbitrary, *except* that
     * the 0-th field element must be the neutral w.r.t. addition, and the
     * 1-st field element must be the neutral w.r.t. multiplication.
     */
    TFieldElementGF2E GetElement(TYPE b);
//	TFieldElement GetElementByValue(uint8_t b);
    TFieldElementGF2E Random();
    virtual ~TFieldGF2E();

};


#endif //SECRET_SHARING_TFIELDGF2E_H
