/*
 * HIM.cpp
 *
 *  Created on: Aug 9, 2016
 *      Author: hila
 */

#include "HIM.h"


using namespace std;
using namespace NTL;

/**
 * input: size of matrix
 * The function allocate the matrix m*n
 * A hyper-invertible matrix is a matrix of which every
 * (non-trivial) square sub-matrix is invertible.
 */
HIM::HIM(int m, int n) {
	this->m_m = m;
	this->m_n = n;
	this->m_matrix = new TFieldElement*[m_m];
	for (int i = 0; i < m_m; i++)
	{
		m_matrix[i] = new TFieldElement[m_n];
	}
}

TFieldElement** HIM::InitHIMByVectors(vector<TFieldElement> &alpha, vector<TFieldElement> &beta)
{
	TFieldElement lambda;
	TFieldElement temp;
	TFieldElement temp1;
	TFieldElement temp2;

	int m = beta.size();
	int n = alpha.size();
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			//lambda = TFieldElement(GF2X::zero());
			lambda = *(TField::getInstance()->GetOne());

			// calculate lambda i,j
			for (int k = 0; k < n; k++)
			{
				if (k == j)
				{
					continue;
				}
                temp1 = (beta[i]) - (alpha[k]);
                temp2 = (alpha[j]) - (alpha[k]);
				temp = temp1 / temp2;
				lambda = lambda * temp;
			}
			(m_matrix[i][j]).setPoly(lambda.getElement());
		}
	}

	// to do: delete alpha and beta
	return m_matrix;
}


/**
 * the function create 2 vectors of polynomial
 * and init the matrix according it
 */
TFieldElement** HIM::CheckInitHIM()
{
	int i;
	vector<TFieldElement> alpha;
    alpha.resize(m_n);
	vector<TFieldElement> beta;
    beta.resize(m_m);
	// check if valid
	if (256 <= m_m*m_n)
	{
		cout << "error";
	}

	cout << "vector: alpha: ";

	// Let alpha_j and beta_i be arbitrary field elements
	for (i = 0; i < m_n; i++)
	{
		 alpha[i] = (TField::getInstance()->GetElement(i));
		 cout << alpha[i].getElement() << "  ";
	}

	cout << '\n';

	cout << "vector: beta: ";
	for (i = 0; i < m_m; i++)
	{
		beta[i] = (TField::getInstance()->GetElement(m_n+i));
		 cout << beta[i].getElement() << "  ";
	}
	cout << '\n';
	return(InitHIMByVectors(alpha,beta));
}

//TFieldElement** HIM::InitHIM()
//{
//	int i;
//    vector<TFieldElement*> alpha;
//    alpha.resize(m_n);
//    vector<TFieldElement*> beta;
//    beta.resize(m_m);
//
//	// check if valid
//	if (256 <= m_m*m_n)
//	{
//		cout << "error";
//	}
//
//	// Let alpha_j and beta_i be arbitrary field elements
//	for (i = 0; i < m_n; i++)
//	{
//		 alpha[i] = (TField::getInstance()->GetElement(i));
//	}
//
//	for (i = 0; i < m_m; i++)
//	{
//		beta[i] = (TField::getInstance()->GetElement(m_n+i));
//	}
//
//	return(InitHIMByVectors(alpha,beta));
//}

TFieldElement** HIM::InitHIM()
{
	int i;
	vector<TFieldElement> alpha;
	alpha.resize(m_n);
	vector<TFieldElement> beta;
	beta.resize(m_m);

	// check if valid
	if (256 <= m_m*m_n)
	{
		cout << "error";
	}

	// Let alpha_j and beta_i be arbitrary field elements
	for (i = 0; i < m_n; i++)
	{
		alpha[i] = (TField::getInstance()->GetElement(i));
	}

	for (i = 0; i < m_m; i++)
	{
		beta[i] = (TField::getInstance()->GetElement(m_n+i));
	}

	return(InitHIMByVectors(alpha,beta));
}


/**
 * the function print the matrix
 */
void HIM::Print()
{
	for (int i = 0; i < m_m; i++) {
		for (int j = 0; j < m_n; j++) {
			cout << (m_matrix[i][j]).getElement() << " ";
		}

		cout << " " << '\n';
	}

}


void HIM::MatrixMult(std::vector<TFieldElement> &vector, std::vector<TFieldElement> &answer)
{

	TFieldElement temp;
	TFieldElement temp1;

	for(int i = 0; i < m_m; i++)
	{
		answer[i] = TFieldElement(GF2X::zero());
		for(int j=0; j < m_n; j++)
		{

			temp1 = m_matrix[i][j] * vector[j];
			answer[i] = answer[i] + temp1;
		}
	//	answer[i]=temp;
	}
}

HIM::~HIM() {
	for (int i = 0; i < m_m; i++)
	{
        delete[] m_matrix[i];
	}
   delete[] m_matrix;
}

