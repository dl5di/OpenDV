/*
 *   Copyright (C) 2012,2013 by Jonathan Naylor G4KLX
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef DTMF_H
#define	DTMF_H

#include <wx/wx.h>

class CDTMF {
public:
	CDTMF();
	~CDTMF();

	bool decode(const unsigned char* ambe, bool end);

	bool hasCommand() const;

	wxString translate();

	void reset();

private:
	wxString     m_data;
	wxString     m_command;
	bool         m_pressed;
	unsigned int m_releaseCount;
	unsigned int m_pressCount;
	wxChar       m_lastChar;

	wxString processReflector(const wxString& prefix, const wxString& command) const;
	wxString processCCS(const wxString& command) const;
};

#endif
