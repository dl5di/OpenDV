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

#ifndef	FirstOrderIIR_H
#define	FirstOrderIIR_H

#include <wx/wx.h>

class CFirstOrderIIR {
public:
	CFirstOrderIIR(wxFloat32 a1, wxFloat32 b0, wxFloat32 b1, wxFloat32 gain);
	~CFirstOrderIIR();

	wxFloat32 process(wxFloat32 val);
	void      process(const wxFloat32* in, wxFloat32* out, unsigned int length);
	void      process(wxFloat32* inOut, unsigned int length);

	void reset();

private:
	wxFloat32 m_a1;
	wxFloat32 m_b0;
	wxFloat32 m_b1;
	wxFloat32 m_gain;
	wxFloat32 m_x0;
	wxFloat32 m_x1;
	wxFloat32 m_y0;
	wxFloat32 m_y1;
};

#endif
