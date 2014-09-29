/*
 *	Copyright (C) 2009 by Jonathan Naylor, G4KLX
 *
 *	Viterbi decoder using Traceback method.
 *	Original Source was written by Sho Tamaoki and Tom Wada
 *	See http://www.lsi.ie.u-ryukyu.ac.jp/~sho/midterm/
 *
 *	Modifed by Satoshi Yasuda 7m3tjz/ad6gz
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; version 2 of the License.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 */

#include "DStarFECDecoder.h"

#include <wx/wx.h>


CDStarFECDecoder::CDStarFECDecoder() :
m_pathMemory0(NULL),
m_pathMemory1(NULL),
m_pathMemory2(NULL),
m_pathMemory3(NULL),
m_pathMetric(NULL)
{
	m_pathMemory0 = new bool[330U];
	m_pathMemory1 = new bool[330U];
	m_pathMemory2 = new bool[330U];
	m_pathMemory3 = new bool[330U];
	m_pathMetric  = new int[4U];
}

CDStarFECDecoder::~CDStarFECDecoder()
{
	delete[] m_pathMemory0;
	delete[] m_pathMemory1;
	delete[] m_pathMemory2;
	delete[] m_pathMemory3;
	delete[] m_pathMetric;
}

bool CDStarFECDecoder::decode(const bool* in, bool* out, unsigned int inLen, unsigned int& outLen)
{
	wxASSERT(in != NULL);
	wxASSERT(out != NULL);

	for (unsigned int i = 0U; i < 4U; i++)
		m_pathMetric[i] = 0;

	unsigned int n = 0U;
	for (unsigned int i = 0U; i < 660U; i += 2U, n++) {
		int data[2];

		if (in[i + 0U])
			data[1] = 1;
		else
			data[1] = 0;

		if (in[i + 1U])
			data[0] = 1;
		else
			data[0] = 0;

		viterbiDecode(n, data);
	}

	traceBack(out, outLen);

	// Swap endian-ness
	for (unsigned int i = 0U; i < 330U; i += 8U) {
		bool temp;
		temp = out[i + 0U]; out[i + 0U] = out[i + 7U]; out[i + 7U] = temp;
		temp = out[i + 1U]; out[i + 1U] = out[i + 6U]; out[i + 6U] = temp;
		temp = out[i + 2U]; out[i + 2U] = out[i + 5U]; out[i + 5U] = temp;
		temp = out[i + 3U]; out[i + 3U] = out[i + 4U]; out[i + 4U] = temp;
	}

	return true;
}

void CDStarFECDecoder::acs(unsigned int n, int* metric)
{
	int tempMetric[4];

	// Pres. state = S0, Prev. state = S0 & S2
	int m1 = metric[0] + m_pathMetric[0];
	int m2 = metric[4] + m_pathMetric[2];
	if (m1 < m2) {
		m_pathMemory0[n] = false;
		tempMetric[0]    = m1;
	} else {
		m_pathMemory0[n] = true;
		tempMetric[0]    = m2;
	}

	// Pres. state = S1, Prev. state = S0 & S2
	m1 = metric[1] + m_pathMetric[0];
	m2 = metric[5] + m_pathMetric[2];
	if (m1 < m2) {
		m_pathMemory1[n] = false;
		tempMetric[1]    = m1;
	} else {
		m_pathMemory1[n] = true;
		tempMetric[1]    = m2;
	}

	// Pres. state = S2, Prev. state = S2 & S3
	m1 = metric[2] + m_pathMetric[1];
	m2 = metric[6] + m_pathMetric[3];
	if (m1 < m2) {
		m_pathMemory2[n] = false;
		tempMetric[2]    = m1;
	} else {
		m_pathMemory2[n] = true;
		tempMetric[2]    = m2;
	}

	// Pres. state = S3, Prev. state = S1 & S3
	m1 = metric[3] + m_pathMetric[1];
	m2 = metric[7] + m_pathMetric[3];
	if (m1 < m2) {
		m_pathMemory3[n] = false;
		tempMetric[3]    = m1;
	} else {
		m_pathMemory3[n] = true;
		tempMetric[3]    = m2;
	}

	for (unsigned int i = 0U; i < 4U; i++)
		m_pathMetric[i] = tempMetric[i];
}

void CDStarFECDecoder::viterbiDecode(unsigned int n, int* data)
{
	int metric[8];

	metric[0] = (data[1] ^ 0) + (data[0] ^ 0);
	metric[1] = (data[1] ^ 1) + (data[0] ^ 1);
	metric[2] = (data[1] ^ 1) + (data[0] ^ 0);
	metric[3] = (data[1] ^ 0) + (data[0] ^ 1);
	metric[4] = (data[1] ^ 1) + (data[0] ^ 1);
	metric[5] = (data[1] ^ 0) + (data[0] ^ 0);
	metric[6] = (data[1] ^ 0) + (data[0] ^ 1);
	metric[7] = (data[1] ^ 1) + (data[0] ^ 0);

	acs(n, metric);
}

enum FEC_STATE {
	S0,
	S1,
	S2,
	S3
};

void CDStarFECDecoder::traceBack(bool* out, unsigned int& length)
{
	// Start from the S0, t=31
	FEC_STATE state = S0;

	length = 0U;

	for (int i = 329; i >= 0; i--, length++) {
	    switch (state) {
			case S0: // if state = S0
				if (m_pathMemory0[i])
					state = S2;				// lower path
				else
					state = S0;				// upper path
				out[i] = false;
				break;

			case S1: // if state = S1
				if (m_pathMemory1[i])
					state = S2;				// lower path
				else
					state = S0;				// upper path
				out[i] = true;
				break;

			case S2: // if state = S2
				if (m_pathMemory2[i])
					state = S3;				// lower path
				else
					state = S1;				// upper path
				out[i] = false;
				break;

			case S3: // if state = S3
				if (m_pathMemory3[i])
					state = S3;				// lower path
				else
					state = S1;				// upper path
				out[i] = true;
				break;
		}
	}
}
