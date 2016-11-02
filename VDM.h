#ifndef VDM_H_
#define VDM_H_

#include "TFieldElement.h"
#include "TField.h"
#include "TFieldZp.h"
#include "TFieldElementZp.h"
#include "Def.h"
#include <vector>
#include <stdio.h>

class VDM {
private:
    int m_n,m_m;
    TFIELD_ELEMENT** m_matrix;
public:
    VDM(int n, int m);
    ~VDM();
    void InitVDM();
    void Print();
    void MatrixMult(std::vector<TFIELD_ELEMENT> &vector, std::vector<TFIELD_ELEMENT> &answer);

};

#endif /* VDM_H_ */