#include "VDM.h"

VDM::VDM(int n, int m) {
    this->m_m = m;
    this->m_n = n;
    this->m_matrix = new TFieldElement*[m_n];
    for (int i = 0; i < m_n; i++)
    {
        m_matrix[i] = new TFieldElement[m_m];
    }
}

void VDM::InitVDM() {
    vector<TFieldElement *> alpha;
    alpha.resize(m_n);
   // TFieldElement *elem;
    for (int i = 0; i < m_n; i++) {
        alpha[i] = (TField::getInstance()->GetElement(i + 1));
    }

    for (int i = 0; i < m_n; i++) {
        m_matrix[i][0] = *(TField::getInstance()->GetOne());
        for (int k = 1; k < m_n; k++) {
            m_matrix[i][k] = m_matrix[i][k - 1] * *(alpha[i]);
        }
    }
}

/**
 * the function print the matrix
 */
void VDM::Print()
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


//FOR k := 0 TO no_buckets-1 DO
//        BEGIN
//FOR i := 0 TO T DO x1[i] := Field.random(); // random degree-T polynomial
//x2[0] := x1[0];
//// same secret
//FOR i := 1 TO 2*T DO x2[i] := Field.random(); // otherwise random
//y1 := MatrixMult(VDM,x1);
//// eval poly at alpha-positions
//y2 := MatrixMult(VDM,x2);
//// eval poly at alpha-positions
//FOR i := 0 TO N-1 DO
//        BEGIN
//// prepare shares to be sent
//Write(SendBufs[i],y1[i]);
//// the degree-t shares of my poly
//Write(SendBufs[i],y2[i]);
//// the degree 2t shares of my poly
//END;
//END;