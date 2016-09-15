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

/*
 * before you need to create matrixHIM (d+1, (n-1)-(d+1)+1)
 * n-1-d-1+1
 * n-1-d
 * HIM(d+1, n-d-1)
 */
bool HIM::CheckConsistency(TFieldElement* alpha, int d, int size_of_alpha, int n)
{
    // Interpolate first d+1 positions of (alpha,x)
    vector<TFieldElement*> Y;
    vector<TFieldElement*> X;
    MatrixMult3(X, Y);
    for(int i=0; i< n-d-1; i++)
    {
        if(Y[i] != X[d+1+i])
        {
            return false;
        }
    }
    return true;
}



/**
 * input: vectors
 * the function init the matrix for that maps
 * evaluations points over alpha to evaluation points over beta
 */
TFieldElement** HIM::InitHIMByVectors(vector<TFieldElement*> &alpha, vector<TFieldElement*> &beta)
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
				temp1 = *(beta[i]) - *(alpha[k]);
				temp2 = *(alpha[j]) - *(alpha[k]);
				temp = temp1 / temp2;
				lambda = lambda * temp;
			}
			(m_matrix[i][j]).setPoly(lambda.getElement());
		}
	}

	// to do: delete alpha and beta
	return m_matrix;
}

/*
 * HIM* matrix = new HIM(n, 1);
 */
TFieldElement HIM::Interpolate (vector<TFieldElement*> alpha, int n, vector<TFieldElement*> x)
{
    vector<TFieldElement*> y;
	vector<TFieldElement*> beta;
	beta.resize(1);
	beta[0] = (TField::getInstance()->GetZero());
    InitHIMByVectors(alpha, beta);
    MatrixMult3(x, y);
	return *y[0];
}

/**
 * the function create 2 vectors of polynomial
 * and init the matrix according it
 */
TFieldElement** HIM::CheckInitHIM()
{
	int i;
	vector<TFieldElement*> alpha;
    alpha.resize(m_n);
	vector<TFieldElement*> beta;
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
		 cout << alpha[i]->getElement() << "  ";
	}

	cout << endl;

	cout << "vector: beta: ";
	for (i = 0; i < m_m; i++)
	{
		beta[i] = (TField::getInstance()->GetElement(m_n+i));
		 cout << beta[i]->getElement() << "  ";
	}
	cout << endl;
	return(InitHIMByVectors(alpha,beta));
}

TFieldElement** HIM::InitHIM()
{
	int i;
    vector<TFieldElement*> alpha;
    alpha.resize(m_n);
    vector<TFieldElement*> beta;
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
	for (int i = 0; i < m_m; i++)
	{
		for(int j = 0; j < m_n; j++)
		{
			cout << (m_matrix[i][j]).getElement() << " ";

		}
		cout << " " << endl;
	}

}

void HIM::CalculateDet()
{
	TFieldElement temp;
	TFieldElement temp1;
	TFieldElement temp2;

	temp1 = m_matrix[0][1] * m_matrix[2][1];
//	cout << temp1.getElement() << endl;
	temp2 = m_matrix[1][1] * m_matrix[2][0];
//	cout << temp2.getElement() << endl;
	temp = temp1 - temp2;
	cout << temp.getElement() << endl;

}

TFieldElement* HIM::MatrixMult2(TFieldElement* vector)
{
	TFieldElement temp;
	TFieldElement temp1;
	TFieldElement* answer = new TFieldElement[m_m];
	for(int i = 0; i < m_m; i++)
	{
		temp = TFieldElement(GF2X::zero());
		for(int j=0; j < m_n; j++)
		{

			temp1 = m_matrix[i][j] * vector[j];
			temp = temp + temp1;
		}
		answer[i].setPoly(temp.getElement());
	}

	return answer;
}

// vector with padding 0
vector<TFieldElement> HIM::MatrixMult(std::vector<TFieldElement> vector)
{

	TFieldElement temp;
	TFieldElement temp1;
	std::vector<TFieldElement> answer;

	cout << "123  " <<  vector.size()<< endl;

	for(int i = 0; i < m_m; i++)
	{
		temp = TFieldElement(GF2X::zero());
		for(int j=0; j < m_n; j++)
		{

			temp1 = m_matrix[i][j] * vector[j];
			temp = temp + temp1;
		}

		answer.push_back(temp);

	//	answer[i].setPoly(temp.getElement());

	}

	return answer;
}


void HIM::MatrixMult3(std::vector<TFieldElement*> vector, std::vector<TFieldElement*> &answer)
{

	TFieldElement* temp;
	TFieldElement temp1;

	cout << "123  " <<  vector.size()<< endl;

	for(int i = 0; i < m_m; i++)
	{
		temp = new TFieldElement(GF2X::zero());
		for(int j=0; j < m_n; j++)
		{

			temp1 = m_matrix[i][j] * *vector[j];
			*temp = *temp + temp1;
		}

		answer[i]=temp;

		//	answer[i].setPoly(temp.getElement());

	}

}


HIM::~HIM() {}

