#ifndef VDM_H_
#define VDM_H_

#include "TFieldElement.h"
#include "TFieldElementGF2E.h"
#include "TField.h"
#include "TFieldZp.h"
#include "TFieldElementZp.h"
#include "TFieldGF2E.h"
#include "Def.h"
#include <vector>
#include <stdio.h>
#include "TemplateField.h"

template<typename FieldType>
class VDM {
private:
    int m_n,m_m;
    FieldType** m_matrix;
    TemplateField<FieldType> *field;
public:
    VDM(int n, int m, TemplateField<FieldType> *field);
    ~VDM();
    void InitVDM();
    void Print();
    void MatrixMult(std::vector<FieldType> &vector, std::vector<FieldType> &answer);

};


template<typename FieldType>
VDM<FieldType>::VDM(int n, int m, TemplateField<FieldType> *field) {
    this->m_m = m;
    this->m_n = n;
    this->field = field;
    this->m_matrix = new FieldType*[m_n];
    for (int i = 0; i < m_n; i++)
    {
        m_matrix[i] = new FieldType[m_m];
    }
}

template<typename FieldType>
void VDM<FieldType>::InitVDM() {
    vector<FieldType> alpha(m_n);
    for (int i = 0; i < m_n; i++) {
        alpha[i] = field->GetElement(i + 1);
    }

    for (int i = 0; i < m_n; i++) {
        m_matrix[i][0] = *(field->GetOne());
        for (int k = 1; k < m_n; k++) {
            m_matrix[i][k] = m_matrix[i][k - 1] * (alpha[i]);
        }
    }
}

/**
 * the function print the matrix
 */
template<typename FieldType>
void VDM<FieldType>::Print()
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

template<typename FieldType>
void VDM<FieldType>::MatrixMult(std::vector<FieldType> &vector, std::vector<FieldType> &answer)
{
    FieldType temp1;
    for(int i = 0; i < m_m; i++)
    {
        // answer[i] = 0
        answer[i] = FieldType(ZERO::zero());

        for(int j=0; j < m_n; j++)
        {
            temp1 = m_matrix[i][j] * vector[j];
            answer[i] += temp1;
        }
    }

}
//
template<typename FieldType>
VDM<FieldType>::~VDM() {
    for (int i = 0; i < m_n; i++) {
        delete[] m_matrix[i];
    }
    delete[] m_matrix;
}


#endif /* VDM_H_ */