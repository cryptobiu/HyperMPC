/*
 * HIM.cpp
 *
 *  Created on: Aug 9, 2016
 *      Author: hila
 */

#include "HIM.h"

using namespace std;
using namespace NTL;


HIM::HIM(int m, int n) {
    // m rows, n columns
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
			// lambda = 1
			lambda = *(TField::getInstance()->GetOne());

            // compute value for matrix[i,j]
			for (int k = 0; k < n; k++)
			{
				if (k == j)
				{
					continue;
				}
                temp1 = (beta[i]) - (alpha[k]);
                temp2 = (alpha[j]) - (alpha[k]);
				temp = temp1 / temp2;
			//	lambda = lambda * temp;
				lambda *= temp;
			}

            // set the matrix
			(m_matrix[i][j]).setPoly(lambda.getElement());
		}
	}
	return m_matrix;
}

TFieldElement** HIM::InitHIM()
{
	int i;
	vector<TFieldElement> alpha(m_n);
	vector<TFieldElement> beta(m_m);

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
	TFieldElement temp1;
	for(int i = 0; i < m_m; i++)
	{
        // answer[i] = 0
		answer[i] = TFieldElement(GF2X::zero());

        for(int j=0; j < m_n; j++)
		{
			temp1 = m_matrix[i][j] * vector[j];
			//answer[i] = answer[i] + temp1;
			answer[i] += temp1;
		}
	}
}

HIM::~HIM() {
	for (int i = 0; i < m_m; i++)
	{
        delete[] m_matrix[i];
	}
   delete[] m_matrix;
}

