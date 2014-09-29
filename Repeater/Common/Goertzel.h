/*
 *	Copyright (C) 2009,2011 by Jonathan Naylor, G4KLX
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

#ifndef	Goertzel_H
#define	Goertzel_H

#include <wx/wx.h>

#include "Utils.h"

class CGoertzel {
public:
	CGoertzel(unsigned int sampleRate, wxFloat32 freq, unsigned int n, wxFloat32 threshold);
	~CGoertzel();

	void setThreshold(wxFloat32 threshold);

	wxFloat32 getResult() const;

	void process(const wxFloat32* data, unsigned int length);

	TRISTATE getDetected();

private:
	wxFloat32    m_freq;		// XXX
	unsigned int m_n;
	wxFloat32    m_threshold;
	wxFloat32    m_coeff;
	wxFloat32    m_q1;
	wxFloat32    m_q2;
	wxFloat32    m_value;
	unsigned int m_count;
	bool         m_result;
};

#endif
