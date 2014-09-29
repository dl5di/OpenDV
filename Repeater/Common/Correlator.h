/*
 *	Copyright (C) 2010 by Jonathan Naylor, G4KLX
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

#ifndef	Correlator_H
#define	Correlator_H

#include <wx/wx.h>

class CCorrelator {
public:
	CCorrelator(const wxFloat32* taps, unsigned int length);
	~CCorrelator();

	void process(wxFloat32 val);

	wxFloat32 getValue(bool normalised = false) const;

	void reset();

	void dump();

private:
	wxFloat32*   m_taps;
	unsigned int m_length;
	wxFloat32    m_aa;
	wxFloat32*   m_buffer;
	unsigned int m_bufLen;
	unsigned int m_pointer;
	wxFloat32    m_bb;
	wxFloat32    m_corr;
};

#endif
