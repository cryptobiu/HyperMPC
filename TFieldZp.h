#ifndef SECRET_SHARING_TFIELDZP_H
#define SECRET_SHARING_TFIELDZP_H

#include "TFieldElementZp.h"
#include "AES_PRG.h"
#include <stdint.h>
#include <bitset>

using namespace NTL;
using namespace std;

class TFieldZp {
private:
    static bool m_instanceFlag;
    static TFieldZp *m_single;
    TFieldElementZp* m_ZERO;
    TFieldElementZp* m_ONE;
    /**
     * the function create a field by:
     * generate the irreducible polynomial x^8 + x^4 + x^3 + x + 1 to work with
     * init the field with the newly generated polynomial
     */
    TFieldZp(int p);
public:
    /**
     * return the field
     */
    static TFieldZp* getInstance();
    TFieldElementZp* GetZero();
    TFieldElementZp* GetOne();
    void InitOne();
    /*
     * The i-th field element. The ordering is arbitrary, *except* that
     * the 0-th field element must be the neutral w.r.t. addition, and the
     * 1-st field element must be the neutral w.r.t. multiplication.
     */

    //Meital : change from uint8_t to int in order to support a larger amount of element creation
   // TFieldElementZp GetElement(uint8_t b);
    TFieldElementZp GetElement(int b);
    TFieldElementZp Random();
    virtual ~TFieldZp();
};


#endif //SECRET_SHARING_TFIELDZP_H
