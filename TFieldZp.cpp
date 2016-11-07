#include "TFieldZp.h"


bool TFieldZp::m_instanceFlag = false;
TFieldZp* TFieldZp::m_single = NULL;
/**
 * the function create a field by:
 * generate the irreducible polynomial x^8 + x^4 + x^3 + x + 1 to work with
 * init the field with the newly generated polynomial
 */
TFieldZp::TFieldZp(int p) {

    ZZ_p::init(ZZ(p));

    m_ZERO = new TFieldElementZp(ZZ_p(0));
    m_ONE = new TFieldElementZp(ZZ_p(1));
}

/*
 * The i-th field element. The ordering is arbitrary, *except* that
 * the 0-th field element must be the neutral w.r.t. addition, and the
 * 1-st field element must be the neutral w.r.t. multiplication.
 */
TFieldElementZp TFieldZp::GetElement(int b) {
    ZZ_p element(b);
    if(b == 1)
    {
        return *m_ONE;
    }
    if(b == 0)
    {
        return *m_ZERO;
    }
    else{
        return TFieldElementZp(element);
    }
}


/**
 * return the field
 */
TFieldZp* TFieldZp::getInstance()
{
    if(!m_instanceFlag)
    {
        //cout << "enter p" << endl;
        int p = 7;
        //cin >> p;
        m_single = new TFieldZp(p);
        m_instanceFlag = true;
        return m_single;
    }
    else
    {
        return m_single;
    }
}


/**
 * A random random field element, uniform distribution
 */
TFieldElementZp TFieldZp::Random() {
    TFieldElementZp randomElement;
    PRG & prg = PRG::instance();
    uint8_t b = prg.getRandom();
    randomElement = GetElement(b);
    return randomElement;
}

TFieldElementZp* TFieldZp::GetZero()
{
    return m_ZERO;
}

TFieldElementZp* TFieldZp::GetOne()
{
    return m_ONE;
}

TFieldZp::~TFieldZp() {
    m_instanceFlag = false;
    delete m_ZERO;
    delete m_ONE;
}
