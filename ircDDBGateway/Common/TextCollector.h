/*
 *   Copyright (C) 2010 by Jonathan Naylor G4KLX
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

#ifndef TextCollector_H
#define TextCollector_H

#include "AMBEData.h"
#include "Defs.h"

#include <wx/wx.h>

class CTextCollector {
public:
	CTextCollector();
	~CTextCollector();

	void writeData(const CAMBEData& data);

	void sync();

	void reset();

	bool hasData() const;

	wxString getData();

private:
	char*          m_data;
	unsigned char* m_buffer;
	SLOWDATA_STATE m_slowData;
	bool           m_has0;
	bool           m_has1;
	bool           m_has2;
	bool           m_has3;
};

#endif
