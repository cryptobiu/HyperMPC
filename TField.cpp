/*
 * TField.cpp
 *
 *  Created on: Aug 9, 2016
 *      Author: hila
 */

#include "TField.h"

bool TField::m_instanceFlag = false;
TField* TField::m_single = NULL;
/**
 * the function create a field by:
 * generate the irreducible polynomial x^8 + x^4 + x^3 + x + 1 to work with
 * init the field with the newly generated polynomial
 */
TField::TField() {

	GF2X irreduciblePolynomial = BuildSparseIrred_GF2X(8);
	GF2E::init(irreduciblePolynomial);

    m_ZERO = new TFieldElement(GF2X(0));
	m_ONE = new TFieldElement(GF2X(1));
}

/*
 * The i-th field element. The ordering is arbitrary, *except* that
 * the 0-th field element must be the neutral w.r.t. addition, and the
 * 1-st field element must be the neutral w.r.t. multiplication.
 */
TFieldElement TField::GetElement(uint8_t b) {

	if(b == 1)
	{
		return *GetOne();
	}
	if(b == 0)
	{
		return *GetZero();
	}
	TFieldElement element;
	bitset<8> bits(b);

	for(int i=0; i < 8; i++) {
			// set the coefficient of x^i to 1
			SetCoeff(element.getElement(),i,bits[i]);
	}
	return element;
}

//
///*
// * The i-th field element. The ordering is arbitrary, *except* that
// * the 0-th field element must be the neutral w.r.t. addition, and the
// * 1-st field element must be the neutral w.r.t. multiplication.
// */
//TFieldElement TField::GetElementByValue(uint8_t b) {
//
//	if(b == 1)
//	{
//		return *this->GetOne();
//	}
//	if(b == 0)
//	{
//		return *this->GetZero();
//	}
//	TFieldElement element;
//	bitset<8> bits(b);
//
//	for(int i=0; i < 8; i++) {
//		// set the coefficient of x^i to 1
//		SetCoeff(element.getElement(),i,bits[i]);
//	}
//	return element;
//}

/**
 * return the field
 */
TField* TField::getInstance()
{
    if(!m_instanceFlag)
    {
        m_single = new TField();
        m_instanceFlag = true;
        return m_single;
    }
    else
    {
        return m_single;
    }
}


/**
 * A random random field element, uniform distribution
 */
TFieldElement TField::Random() {
	TFieldElement randomElement;
	PRG & prg = PRG::instance();
	uint8_t b = prg.getRandom();
	randomElement = GetElement(b);
	return randomElement;
}

TFieldElement* TField::GetZero()
{
	return m_ZERO;
}

TFieldElement* TField::GetOne()
{
	return m_ONE;
}

TField::~TField() {
	m_instanceFlag = false;
	delete m_ZERO;
	delete m_ONE;
}


