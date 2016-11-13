#include "TFieldElementZp.h"

using namespace NTL;
using namespace std;

TFieldElementZp::TFieldElementZp() : m_element() {
}

TFieldElementZp::TFieldElementZp(NTL::ZZ_p element) : m_element(element) {
    this->m_element = element;
}

TFieldElementZp::TFieldElementZp(string str) : m_element() {
    istringstream iss(str);
    iss >> this->m_element;
}


NTL::ZZ_p TFieldElementZp::getElement() const{
    return m_element;
}

TFieldElementZp TFieldElementZp::operator-(TFieldElementZp& f2)
{
    return m_element - f2.getElement();
}

TFieldElementZp TFieldElementZp::operator+(TFieldElementZp& f2)
{
    return m_element + f2.getElement();
}

TFieldElementZp& TFieldElementZp::operator+=(TFieldElementZp& f2)
{
    m_element += f2.getElement();

    return *this;
}

TFieldElementZp& TFieldElementZp::operator*=(TFieldElementZp& f2)
{
    mul(m_element, m_element,  f2.getElement());

    return *this;
}

TFieldElementZp TFieldElementZp::operator/(TFieldElementZp& f2)
{
    NTL::ZZ_p f;
    NTL::ZZ_p rf;
    // x = a^{-1} % f, error is a is not invertible
    inv(f,f2.getElement());
    mul(rf, f,  m_element);
    return rf;
}

TFieldElementZp TFieldElementZp::operator*(TFieldElementZp& f2)
{
    return this->getElement() *  f2.getElement();
}

TFieldElementZp& TFieldElementZp::operator=(const TFieldElementZp& f2)
{
    this->m_element = f2.getElement();

    return *this;
}

string TFieldElementZp::toString()
{
    ostringstream stream;
    stream << m_element;
    string str =  stream.str();
    return str;
}

TFieldElementZp::~TFieldElementZp() {}
