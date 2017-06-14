//
// Created by meital on 17/05/17.
//

#include "GF2_8LookupTable.h"




using namespace std;
using namespace NTL;

byte GF2_8LookupTable::multTable[256][256] = {};

void GF2_8LookupTable::initTable(){


    GF2X irreduciblePolynomial = BuildSparseIrred_GF2X(8);
    GF2E::init(irreduciblePolynomial);


    byte elem1[1];
    byte elem2[1];
    byte resultInBytes[1];


    //first create a GF2X
    GF2X polynomialElement1, polynomialElement2;


    //use the NTL lib to create the lookup table
    for(unsigned int i=0; i<256; i++)
    {
        elem1[0] = (byte)i;
        //translate the bytes into a GF2X element
        GF2XFromBytes(polynomialElement1, elem1, 1);
        for(unsigned int j=0; j<256; j++){


            elem2[0] = (byte)j;
            GF2XFromBytes(polynomialElement2, elem2, 1);


            auto result = to_GF2E(polynomialElement1) * to_GF2E(polynomialElement2);

            BytesFromGF2X(resultInBytes,rep(result),1);

            GF2_8LookupTable::multTable[i][j] = resultInBytes[0];
        }
    }


}


/*GF2_8LookupTable& GF2_8LookupTable::operator=(const GF2_8LookupTable& other) // copy assignment
{
    if (this != &other) { // self-assignment check expected
        elem = other.elem;
    }
    return *this;
}*/




/*GF2_8LookupTable GF2_8LookupTable::operator*(const GF2_8LookupTable& f2)
{

    GF2_8LookupTable answer;

//    auto elemleft = (unsigned int)f2.elem;
//    auto elemright = (unsigned int)elem;
//
//    if(elemleft>255 || elemright>255) {
//        cout << "wrong values: elemleft = " << elemleft << "elemright = " << elemright;
//        exit(0);
//    }
//
//    answer.elem = multTable[elemleft][elemright];

    answer.elem = multTable[elem][f2.elem];

    return answer;

}*/

/*GF2_8LookupTable& GF2_8LookupTable::operator*=(const GF2_8LookupTable& f2){

//    auto elemleft = (unsigned int)f2.elem;
//    auto elemright = (unsigned int)elem;
//
//    if(elemleft>255 || elemright>255) {
//        cout << "wrong values: elemleft = " << elemleft << "elemright = " << elemright;
//        exit(0);
//    }
//
//    elem = multTable[elemleft][elemright];

    elem = multTable[elem][f2.elem];

}*/

GF2_8LookupTable GF2_8LookupTable::operator/(const GF2_8LookupTable& f2) {

    GF2_8LookupTable answer;

    byte elem1[1];
    byte elem2[1];
    byte resultInBytes[1];


    //first create a GF2X
    GF2X polynomialElement1, polynomialElement2;


    elem1[0] = (byte) elem;
    //translate the bytes into a GF2X element
    GF2XFromBytes(polynomialElement1, elem1, 1);


    elem2[0] = (byte) f2.elem;
    GF2XFromBytes(polynomialElement2, elem2, 1);


    auto result = to_GF2E(polynomialElement1) / to_GF2E(polynomialElement2);

    BytesFromGF2X(resultInBytes, rep(result), 1);

    answer.elem = resultInBytes[0];

    return answer;

}
