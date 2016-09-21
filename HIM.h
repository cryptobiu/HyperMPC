/*
 * HIM.h
 *
 *  Created on: Aug 9, 2016
 *      Author: hila
 */

#ifndef HIM_H_
#define HIM_H_

#include <iostream>
#include "TFieldElement.h"
#include "TField.h"
#include <NTL/GF2X.h>
#include <iostream>
#include <vector>
#include <array>

using namespace std;

class HIM {
private:
	int m_n,m_m;
	TFieldElement** m_matrix;
public:
	/**
	 * input: size of matrix
	 * The function allocate the matrix m*n
	 */
	HIM(int m, int n);
	/**
	 * input: vectors
	 * the function init the matrix for that maps
	 * evaluations points over alpha to evaluation points over beta
	 */
	TFieldElement** InitHIMByVectors(vector<TFieldElement*> &alpha, vector<TFieldElement*> &beta);
	/**
	 * the function create 2 vectors of polynomial
	 * and init the matrix according it
	 */
	TFieldElement** InitHIM();
	/**
	 * the function print the matrix
	 */
	void Print();
	void CalculateDet();
	TFieldElement** CheckInitHIM();
	vector<TFieldElement> MatrixMult(vector<TFieldElement> vector);
	TFieldElement* MatrixMult2(TFieldElement* vector);
	void MatrixMult3(std::vector<TFieldElement*> vector, std::vector<TFieldElement*> &answer);
	bool CheckConsistency(int d, int n, vector<TFieldElement*> &x1);
	TFieldElement Interpolate (vector<TFieldElement*> alpha, vector<TFieldElement*> x);
	virtual ~HIM();
};

#endif /* HIM_H_ */
