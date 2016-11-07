#ifndef SECRET_SHARING_TFIELDELEMENTZP_H
#define SECRET_SHARING_TFIELDELEMENTZP_H


#include <NTL/ZZ_p.h>
#include <string>
#include <sstream>
#include <iostream>

class TFieldElementZp {
private:
    NTL::ZZ_p m_element;
public:
    TFieldElementZp();
    TFieldElementZp(NTL::ZZ_p element);
    TFieldElementZp(std::string str);
    NTL::ZZ_p getElement() const;
    TFieldElementZp operator+(TFieldElementZp& f2);
    TFieldElementZp operator-(TFieldElementZp& f2);
    TFieldElementZp operator/(TFieldElementZp& f2);
    TFieldElementZp operator*(TFieldElementZp& f2);
    TFieldElementZp& operator+=(TFieldElementZp& f2);
    TFieldElementZp& operator*=(TFieldElementZp& f2);
    std::string toString();
    TFieldElementZp& operator=(const TFieldElementZp& f2);
    virtual ~TFieldElementZp();
};


#endif //SECRET_SHARING_TFIELDELEMENTZP_H
