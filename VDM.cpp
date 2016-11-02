#include "VDM.h"

VDM::VDM(int n, int m) {
    this->m_m = m;
    this->m_n = n;
    this->m_matrix = new TFIELD_ELEMENT*[m_n];
    for (int i = 0; i < m_n; i++)
    {
        m_matrix[i] = new TFIELD_ELEMENT[m_m];
    }
}

void VDM::InitVDM() {
    vector<TFIELD_ELEMENT> alpha(m_n);
    for (int i = 0; i < m_n; i++) {
        alpha[i] = (TFIELD::getInstance()->GetElement(i + 1));
    }

    for (int i = 0; i < m_n; i++) {
        m_matrix[i][0] = *(TFIELD::getInstance()->GetOne());
        for (int k = 1; k < m_n; k++) {
            m_matrix[i][k] = m_matrix[i][k - 1] * (alpha[i]);
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
        cout << " " << '\n';
    }

}


void VDM::MatrixMult(std::vector<TFIELD_ELEMENT> &vector, std::vector<TFIELD_ELEMENT> &answer)
{
    TFIELD_ELEMENT temp1;
    for(int i = 0; i < m_m; i++)
    {
        // answer[i] = 0
        answer[i] = TFIELD_ELEMENT(ZERO::zero());

        for(int j=0; j < m_n; j++)
        {
            temp1 = m_matrix[i][j] * vector[j];
            answer[i] += temp1;
        }
    }

}
//
VDM::~VDM() {
    for (int i = 0; i < m_n; i++) {
        delete[] m_matrix[i];
    }
    delete[] m_matrix;
}
