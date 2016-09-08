//
//#include <iostream>
//#include <NTL/GF2X.h>
//#include <NTL/mat_GF2.h>
//#include <stdint.h>
//#include<stdio.h>
//#include <NTL/GF2E.h>
//#include <bitset>
//#include "TFieldElement.h"
//#include "TField.h"
//#include "HIM.h"
//
//using namespace std;
//using namespace NTL;
//
//int main()
//{
//	int n;
//	// choose the alphas
//	uint8_t b;
//	TFieldElement* alpha = new TFieldElement[n];
//	for (int i=0; i<n; i++)
//	{
//		b = i + 1;
//		alpha[i] = *(TField::getInstance()->GetElement(b));
//	}
//
//	// compute vandermonde matrix
//	TFieldElement** VDM = new TFieldElement*[n];
//	for (int i = 0; i < n; i++)
//	{
//		VDM[i] = new TFieldElement[n];
//	}
//
//	b=1;
//	for (int i=0; i<n; i++)
//	{
//		VDM[i][0] = *(TField::getInstance()->GetElement(b));
//		for (int k=1; k < n; k++)
//		{
//			VDM[i][k] = VDM[i][k-1] * alpha[i];
//		}
//	}
//
//}
