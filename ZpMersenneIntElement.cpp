//
// Created by meital on 01/02/17.
//

#include "ZpMersenneIntElement.h"
#include <iostream>
#include <cmath>
#include "NTL/ZZ_p.h"
#include "NTL/ZZ.h"

int shiftNum = ((1 << 31) - 1);

int trailingZeros[] =  { 32, 0, 1, 26, 2, 23, 27, 0, 3, 16, 24, 30, 28, 11, 0, 13, 4, 7, 17,
                         0, 25, 22, 31, 15, 29, 10, 12, 6, 0, 21, 14, 9, 5, 20, 8, 19, 18 };

unsigned int pows2[] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192,
                        16384, 32768, 65536, 131072, 262144, 524288, 1048576, 2097152,
                        4194304, 8388608, 16777216, 33554432, 67108864, 134217728,
                        268435456, 536870912, 1073741824, 2147483648};




using namespace std;
using namespace NTL;



ZpMersenneIntElement::ZpMersenneIntElement(int p, int elem) : p(p){

    this->elem = elem & shiftNum;

}
ZpMersenneIntElement ZpMersenneIntElement::operator-(ZpMersenneIntElement& f2)
{
    ZpMersenneIntElement answer(p);

    answer.elem = (unsigned int)(((int)elem - (int)f2.elem) %p);

    return answer;
}

ZpMersenneIntElement ZpMersenneIntElement::operator+(ZpMersenneIntElement& f2)
{
    ZpMersenneIntElement answer(p);

    answer.elem = (elem + f2.elem) %p;

    return answer;
}

ZpMersenneIntElement ZpMersenneIntElement::operator*(ZpMersenneIntElement& f2)
{


    //ZpMersenneIntElement answer(p);


    long multLong = (long)elem * (long) f2.elem;

    //get the bottom 31 bit
    unsigned int bottom = multLong & shiftNum;

    //get the top 31 bits
    unsigned int top = (multLong>>31) & shiftNum;

    //answer.elem = (bottom + top) %p;

    //answer.elem = ((long)elem * (long) f2.elem) %p;


    return ZpMersenneIntElement(p, (bottom + top) %p);

}

ZpMersenneIntElement ZpMersenneIntElement::operator/(ZpMersenneIntElement& f2)
{



    /*ZZ_p x(elem);
    ZZ_p y(f2.elem);
    //ZZ_p divZ;

    //divZ = x/y ;

    //ZpMersenneIntElement answer(p);

    //answer.elem = to_uint(rep(x/y));

    return ZpMersenneIntElement(p, to_uint(rep(x/y)));*/

    /*
    //find the inverse

    unsigned int a = 1;
    unsigned int b = 0;
    unsigned int y = f2.elem;
    unsigned int z = p;
    unsigned pow2;

    int temp;
    ZpMersenneIntElement inverse(p);

    ZpMersenneIntElement aMen(p);
    ZpMersenneIntElement powMen(p);

    while(y!=1){

        int e = NumberOfTrailingZeroOpt(y);

        y = y/pows2[e];

        aMen.elem = a;
        powMen.elem = pows2[31-e];

        aMen = aMen * powMen;

        //a = pows2[31-e]*a%p;

        a = aMen.elem;

        //set a as the answer
        if(y==1) {
            inverse.elem = a;
        }
        else {
            temp = a;
            a = (a + b);
            b = temp;
            temp = y;
            y = (y + z);
            z = temp;
        }

    }

    //cout<< "invers of "<< f2.elem <<" is " << inverse.elem<<endl;


    ZpMersenneIntElement answer(p);
    answer = inverse* (*this);

    return answer;*/

    //long d,  s,  t,  a,  b;

    unsigned int a = f2.elem;
    unsigned int b = p;
    unsigned int s;

    unsigned int  u, v, q, r;
    unsigned long u0, v0, u1, v1, u2, v2;

    unsigned int aneg = 0, bneg = 0;

    if (a < 0) {
        if (a < -NTL_MAX_LONG) Error("XGCD: integer overflow");
        a = -a;
        aneg = 1;
    }

    if (b < 0) {
        if (b < -NTL_MAX_LONG) Error("XGCD: integer overflow");
        b = -b;
        bneg = 1;
    }

    u1=1; v1=0;
    u2=0; v2=1;
    u = a; v = b;

    while (v != 0) {
        q = u / v;
        r = u % v;
        u = v;
        v = r;
        u0 = u2;
        v0 = v2;
        u2 =  u1 - q*u2;
        v2 = v1- q*v2;
        u1 = u0;
        v1 = v0;
    }

    if (aneg)
        u1 = -u1;


    s = u1;



    ZpMersenneIntElement inverse(p,s);
    //ZpMersenneIntElement answer(p);
    //answer = inverse* (*this);

    return inverse* (*this);







}


unsigned int ZpMersenneIntElement::NumberOfTrailingZeroOpt(unsigned int i)
{
    return trailingZeros[(i & -i) % 37];
}

unsigned int ZpMersenneIntElement::numberOfTrailingZero(unsigned int number) {
    unsigned int v;      // 32-bit word input to count zero bits on right
    unsigned int c = 32; // c will be the number of zero bits on the right
    number &= -signed(number);
    if (number) c--;
    if (number & 0x0000FFFF) c -= 16;
    if (number & 0x00FF00FF) c -= 8;
    if (number & 0x0F0F0F0F) c -= 4;
    if (number & 0x33333333) c -= 2;
    if (number & 0x55555555) c -= 1;

    return c;
}

