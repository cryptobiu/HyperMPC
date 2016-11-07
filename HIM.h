#ifndef HIM_H_
#define HIM_H_

#include <iostream>
#include "Def.h"
#include "TFieldElement.h"
#include "TFieldElementZp.h"
#include "TFieldElementGF2E.h"
#include "TField.h"
#include "TFieldZp.h"
#include "TFieldGF2E.h"
#include <NTL/GF2X.h>
#include <iostream>
#include <vector>
#include <array>

/**
 * A hyper-invertible matrix is a matrix of which every (non-trivial) square sub-matrix is invertible. Given
 * a hyper-invertible matrix M and vectors x and y satisfying y = M x, then given any |x| components of x
 * and y (any mixture is fine!), one can compute all other components of x and y as a linear function from
 * the given components.
 * Such matrices provide very good diversion and concentration properties: Given a vector x with random-ness in some components;
 * then this very same randomness can be observed in any components of y.
 * Similarly, given a vector x with up to k non-zero elements, then either y will have a non-zero element in
 * each subset of k components, or x is the zero-vector.
 * We present a construction of hyper-invertible matrices and a bunch of applications.
 */

using namespace std;

class HIM {
private:
	int m_n,m_m;
	TFIELD_ELEMENT** m_matrix;
public:

	/**
	 * This method allocate m-by-n matrix.
	 * m rows, n columns.
	 */
	HIM(int m, int n);

	HIM();

	/**
	 * This method is a construction of a hyper-invertible m-by-n matrix M over a finite field F with |F| ≥ 2n.
	 * Let α1,...,αn , β1,...,βm denote fixed distinct elements in F according the vectors alpha and beta,
	 * and consider the function f:Fn → Fm,
	 * mapping (x1,...,xn) to (y1,...,ym) such that the points (β1,y1),...,(βm,ym) lie on the polynomial g(·)
	 * of degree n−1 defined by the points (α1,x1),...,(αn,xn).
	 * Due to the linearity of Lagrange interpolation, f is linear and can be expressed as a matrix:
	 * M = {λi,j} j=1,...n i=1,...,m
	 * where λ i,j = {multiplication}k=1,..n (βi−αk)/(αj−αk)
	 */
	TFIELD_ELEMENT** InitHIMByVectors(vector<TFIELD_ELEMENT> &alpha, vector<TFIELD_ELEMENT> &beta);

	/**
	 * This method create vectors alpha and beta,
	 * and init the matrix by the method InitHIMByVectors(alpha, beta).
	 */
	TFIELD_ELEMENT** InitHIM();

	/**
	 * This method print the matrix
	 */
	void Print();

	/**
	 * matrix/vector multiplication.
	 * The result is the answer vector.
	 */
	void MatrixMult(std::vector<TFIELD_ELEMENT> &vector, std::vector<TFIELD_ELEMENT> &answer);

	void allocate(int m, int n);

	virtual ~HIM();
};

#endif /* HIM_H_ */
