/*
 *	Copyright (C) 2013 by Jonathan Naylor, G4KLX
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

#ifndef	APRSTX_H
#define	APRSTX_H

#include <wx/wx.h>

class CAPRSTX {
public:
	CAPRSTX(const wxString& callsign, wxFloat32 latitude, wxFloat32 longitude, int altitude, const wxString& text);
	~CAPRSTX();

	unsigned int getAudio(wxFloat32* audio, unsigned int length, wxFloat32 amplitude);

	bool isEmpty() const;

	void reset();

private:
	wxFloat32*   m_audio;
	unsigned int m_length;
	unsigned int m_offset;

	void insertSilence(unsigned int ms);
};

#endif
