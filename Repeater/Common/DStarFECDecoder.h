/*
 *	Copyright (C) 2009,2013 by Jonathan Naylor, G4KLX
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

#ifndef	DStarFECDecoder_H
#define	DStarFECDecoder_H

class CDStarFECDecoder {
public:
	CDStarFECDecoder();
	~CDStarFECDecoder();

	bool decode(const bool* in, bool* out, unsigned int inLen, unsigned int& outLen);

private:
	bool* m_pathMemory0;
	bool* m_pathMemory1;
	bool* m_pathMemory2;
	bool* m_pathMemory3;
	int*  m_pathMetric;

	void acs(unsigned int n, int* metric);
	void viterbiDecode(unsigned int n, int* data);
	void traceBack(bool* out, unsigned int& length);
};

#endif
