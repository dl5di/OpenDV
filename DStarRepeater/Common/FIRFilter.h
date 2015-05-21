/*
 *	Copyright (C) 2009,2013,2015 by Jonathan Naylor, G4KLX
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

#ifndef	FIRFilter_H
#define	FIRFilter_H

class CFIRFilter {
public:
	CFIRFilter(const float* taps, unsigned int length);
	CFIRFilter();
	~CFIRFilter();

	void  setTaps(const float* taps, unsigned int length);

	float process(float val);
	void  process(float* inOut, unsigned int length);
	void  process(const float* in, float* out, unsigned int length);

	void  reset();

private:
	float*       m_taps;
	unsigned int m_length;
	float*       m_buffer;
	unsigned int m_bufLen;
	unsigned int m_pointer;
};

#endif
