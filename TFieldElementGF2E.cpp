//
// Created by meital on 07/11/16.
//

#include "TFieldElementGF2E.h"




using namespace NTL;

TFieldElementGF2E::TFieldElementGF2E() {
}

TFieldElementGF2E::TFieldElementGF2E(NTL::GF2E element) : m_element(element) {

}

TFieldElementGF2E::TFieldElementGF2E(string str) {

    istringstream iss(str);
    iss >> this->m_element;
}


NTL::GF2E TFieldElementGF2E::getElement() const{
    return m_element;
}

TFieldElementGF2E TFieldElementGF2E::operator-(TFieldElementGF2E& f2)
{
    return m_element - f2.getElement();
}

TFieldElementGF2E TFieldElementGF2E::operator+(TFieldElementGF2E& f2)
{
    return  m_element + f2.getElement();
}

TFieldElementGF2E& TFieldElementGF2E::operator+=(TFieldElementGF2E& f2)
{
    m_element += f2.getElement();

    return *this;
}

TFieldElementGF2E& TFieldElementGF2E::operator*=(TFieldElementGF2E& f2)
{
    mul(m_element, m_element,  f2.getElement());

    return *this;
}

TFieldElementGF2E TFieldElementGF2E::operator/(TFieldElementGF2E& f2)
{
    return m_element/f2.getElement();
}

TFieldElementGF2E TFieldElementGF2E::operator*(TFieldElementGF2E& f2)
{
    return m_element *  f2.getElement();
}

TFieldElementGF2E& TFieldElementGF2E::operator=(const TFieldElementGF2E& f2)
{
    m_element = f2.getElement();

    return *this;
}


string TFieldElementGF2E::toString()
{
    ostringstream stream;
    stream << m_element;
    string str =  stream.str();
    return str;
}

TFieldElementGF2E::~TFieldElementGF2E() {}
