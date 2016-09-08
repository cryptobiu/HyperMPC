/*
 * TBuffer.h
 *
 *  Created on: Aug 16, 2016
 *      Author: hila
 */

#ifndef TBUFFER_H_
#define TBUFFER_H_
#include "TField.h"

class TBuffer {
private:
	int m_readIndex;
	int m_writeIndex;
	TFieldElement* m_array;
public:
	TBuffer();
	void Reset(int maxSize);
	void ResetAll(TFieldElement** array, int n, int maxSize);
	void Write(TFieldElement x);
	void Read(TFieldElement x);
	virtual ~TBuffer();
};

#endif /* TBUFFER_H_ */
