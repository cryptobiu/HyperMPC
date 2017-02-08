//
// Created by meital on 01/02/17.
//

#ifndef SECRET_SHARING_ZPMERSENNEINTELEMENT_H
#define SECRET_SHARING_ZPMERSENNEINTELEMENT_H

#include "NTL/ZZ_p.h"
#include "NTL/ZZ.h"


using namespace std;
using namespace NTL;



class ZpMersenneIntElement {

//private:
public: //TODO return to private after tesing

    unsigned int p;
    unsigned int elem;

    unsigned int numberOfTrailingZero(unsigned int number);
    unsigned int NumberOfTrailingZeroOpt(unsigned int i);


public:

    ZpMersenneIntElement(int p): p(p){};
    ZpMersenneIntElement(int p, int elem);

    ZpMersenneIntElement operator+(ZpMersenneIntElement& f2);
    ZpMersenneIntElement operator-(ZpMersenneIntElement& f2);
    ZpMersenneIntElement operator/(ZpMersenneIntElement& f2);
    ZpMersenneIntElement operator*(ZpMersenneIntElement& f2);

    void operator+=(ZpMersenneIntElement& f2);
    void operator*=(ZpMersenneIntElement& f2);

};


#endif //SECRET_SHARING_ZPMERSENNEINTELEMENT_H
