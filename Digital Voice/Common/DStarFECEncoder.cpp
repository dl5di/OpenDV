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

#include "DStarFECEncoder.h"

#include <wx/wx.h>


CDStarFECEncoder::CDStarFECEncoder()
{
}

CDStarFECEncoder::~CDStarFECEncoder()
{
}

void CDStarFECEncoder::encode(const bool* in, bool* out, unsigned int inLen, unsigned int& outLen)
{
	wxASSERT(in != NULL);
	wxASSERT(out != NULL);

	outLen = 0U;
	int d1 = 0;
	int d2 = 0;
	for (unsigned int i = 0U; i < 42U; i++) {
		for (int j = 7; j >= 0; j--) {
			int d = in[i * 8U + j] ? 1 : 0;

			int g0 = (d + d2) % 2;
			int g1 = (d + d1 + d2) % 2;

			d2 = d1;
			d1 = d;

			out[outLen++] = g1 == 1;
			out[outLen++] = g0 == 1;
		}
	}
}
