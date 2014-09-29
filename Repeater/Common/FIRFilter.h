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

#ifndef	FIRFilter_H
#define	FIRFilter_H

#include <wx/wx.h>

class CFIRFilter {
public:
	CFIRFilter(const wxFloat32* taps, unsigned int length);
	CFIRFilter();
	~CFIRFilter();

	void      setTaps(const wxFloat32* taps, unsigned int length);

	wxFloat32 process(wxFloat32 val);
	void      process(wxFloat32* inOut, unsigned int length);
	void      process(const wxFloat32* in, wxFloat32* out, unsigned int length);

	void      reset();

private:
	wxFloat32*   m_taps;
	unsigned int m_length;
	wxFloat32*   m_buffer;
	unsigned int m_bufLen;
	unsigned int m_pointer;
};

#endif
