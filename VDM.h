#ifndef VDM_H_
#define VDM_H_

#include "TFieldElement.h"
#include "TField.h"
#include <vector>
#include <stdio.h>

class VDM {
private:
    int m_n,m_m;
    TFieldElement** m_matrix;
public:
    VDM(int n, int m);
    void InitVDM();
    void Print();
    void MatrixMult(std::vector<TFieldElement> &vector, std::vector<TFieldElement> &answer);

};

#endif /* VDM_H_ */