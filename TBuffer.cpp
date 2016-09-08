/*
 * TBuffer.cpp
 *
 *  Created on: Aug 16, 2016
 *      Author: hila
 */

#include "TBuffer.h"

TBuffer::TBuffer() {
	m_readIndex = 0;
	m_writeIndex = 0;
}

/**
 * empty buffer
 * set read and write index to 0
 * set buffer size to maxSize
 */
void TBuffer::Reset(int maxSize)
{
	m_readIndex = 0;
	m_writeIndex = 0;
	m_array = new TFieldElement[maxSize];
}

void TBuffer::ResetAll(TFieldElement** array, int n, int maxSize)
{
	for (int i = 0; i < n; i++) {
		array[i] = new TFieldElement[maxSize];
	}
}

void TBuffer::Write(TFieldElement x)
{

}

void TBuffer::Read(TFieldElement x)
{

}

TBuffer::~TBuffer() {
	// TODO Auto-generated destructor stub
}

