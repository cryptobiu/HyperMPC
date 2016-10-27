/*
 * TFieldElement.h
 *
 *  Created on: Aug 9, 2016
 *      Author: hila
 */

#ifndef TFIELDELEMENT_H_
#define TFIELDELEMENT_H_
#include <iostream>
#include <NTL/GF2X.h>
#include <stdint.h>
#include <NTL/GF2E.h>
#include <bitset>
#include <string>
#include<NTL/GF2XFactoring.h>
#include <sstream>

using namespace std;

class TFieldElement {
private:
	NTL::GF2X m_poly;
	NTL::GF2X irreduciblePolynomial;
public:
	TFieldElement();
	TFieldElement(NTL::GF2X poly);
	TFieldElement(string str);
	NTL::GF2X& getElement();
	int setPoly(NTL::GF2X poly);
	NTL::GF2X getIrreducible();
	TFieldElement operator+(TFieldElement& f2);
	TFieldElement operator-(TFieldElement& f2);
	TFieldElement operator/(TFieldElement& f2);
	TFieldElement operator*(TFieldElement& f2);
	void operator+=(TFieldElement& f2);
	void operator*=(TFieldElement& f2);
	string toString();
	void operator=(const TFieldElement& f2);
	virtual ~TFieldElement();
};

#endif /* TFIELDELEMENT_H_ */
