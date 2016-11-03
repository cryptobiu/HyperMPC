/*
 * TField.h
 *
 *  Created on: Aug 9, 2016
 *      Author: hila
 */

#ifndef TFIELD_H_
#define TFIELD_H_
#include "TFieldElement.h"
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


class TField {
private:
    static bool m_instanceFlag;
    static TField *m_single;
    TFieldElement* m_ZERO;
    TFieldElement* m_ONE;
    /**
     * the function create a field by:
     * generate the irreducible polynomial x^8 + x^4 + x^3 + x + 1 to work with
     * init the field with the newly generated polynomial
     */
	TField();
public:
	/**
	 * return the field
	 */
	static TField* getInstance();
	TFieldElement* GetZero();
	TFieldElement* GetOne();
	void InitOne();
	/*
	 * The i-th field element. The ordering is arbitrary, *except* that
	 * the 0-th field element must be the neutral w.r.t. addition, and the
	 * 1-st field element must be the neutral w.r.t. multiplication.
	 */
	TFieldElement GetElement(TYPE b);
//	TFieldElement GetElementByValue(uint8_t b);
	TFieldElement Random();
	virtual ~TField();
};


#endif /* TFIELD_H_ */
