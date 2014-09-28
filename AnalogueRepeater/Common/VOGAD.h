/*
 *	Copyright (C) 2009 by Jonathan Naylor, G4KLX
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

#ifndef VOGAD_H
#define VOGAD_H

#include <wx/wx.h>

class CVOGAD {
public:
	CVOGAD(wxFloat32 maxGain, wxFloat32 maxLevel);
	~CVOGAD();

	void process(wxFloat32* inOut, unsigned int length);
	void process(const wxFloat32* in, wxFloat32* out, unsigned int length);

	void setMaxGain(wxFloat32 gain);

	wxFloat32 getGain() const;

	void reset();

private:
	wxFloat32 m_maxGain;
	wxFloat32 m_maxLevel;
	wxFloat32 m_maxAbsLevel;
	wxFloat32 m_gain;
};

#endif
