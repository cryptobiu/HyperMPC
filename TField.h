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


using namespace std;
using namespace NTL;

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
	TFieldElement GetElement(uint8_t b);
	TFieldElement GetElementByValue(uint8_t b);
	TFieldElement Random();
	virtual ~TField();
};


#endif /* TFIELD_H_ */
