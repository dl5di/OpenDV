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

#ifndef	TimeoutTones_H
#define	TimeoutTones_H

#include <wx/wx.h>

enum ANALOGUE_TIMEOUT_TYPE {
	ATT_DL,
	ATT_UK,
	ATT_US
};

class CTimeoutTones {
public:
	CTimeoutTones(unsigned int sampleRate, ANALOGUE_TIMEOUT_TYPE type);
	~CTimeoutTones();

	void getAudio(wxFloat32* audio, unsigned int length, wxFloat32 amplitude = 1.0F);

private:
	wxFloat32*   m_data;
	unsigned int m_length;
	unsigned int m_current;
};

#endif
