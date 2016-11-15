//
// Created by meital on 07/11/16.
//

#include "TFieldGF2E.h"

bool TFieldGF2E::m_instanceFlag = false;
TFieldGF2E* TFieldGF2E::m_single = NULL;
/**
 * the function create a field by:
 * generate the irreducible polynomial x^8 + x^4 + x^3 + x + 1 to work with
 * init the field with the newly generated polynomial
 */
TFieldGF2E::TFieldGF2E() {

    GF2X irreduciblePolynomial = BuildSparseIrred_GF2X(NUM);
    GF2E::init(irreduciblePolynomial);

    m_ZERO = new TFieldElementGF2E(GF2E(0));
    m_ONE = new TFieldElementGF2E(GF2E(1));
}

/*
 * The i-th field element. The ordering is arbitrary, *except* that
 * the 0-th field element must be the neutral w.r.t. addition, and the
 * 1-st field element must be the neutral w.r.t. multiplication.
 */
TFieldElementGF2E TFieldGF2E::GetElement(TYPE b) {

    if(b == 1)
    {
        return *GetOne();
    }
    if(b == 0)
    {
        return *GetZero();
    }
    GF2X element;
    bitset<NUM> bits(b);

    for(int i=0; i < NUM; i++) {
        // set the coefficient of x^i to 1
        SetCoeff(element,i,bits[i]);
    }
    return to_GF2E(element);
}

/**
 * return the field
 */
TFieldGF2E* TFieldGF2E::getInstance()
{
    if(!m_instanceFlag)
    {
        m_single = new TFieldGF2E();
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
TFieldElementGF2E TFieldGF2E::Random() {
    TFieldElementGF2E randomElement;
    PRG & prg = PRG::instance();
    TYPE b = prg.getRandom();

    return GetElement(b);
}

TFieldElementGF2E* TFieldGF2E::GetZero()
{
    return m_ZERO;
}

TFieldElementGF2E* TFieldGF2E::GetOne()
{
    return m_ONE;
}

TFieldGF2E::~TFieldGF2E() {
    m_instanceFlag = false;
    delete m_ZERO;
    delete m_ONE;
}

