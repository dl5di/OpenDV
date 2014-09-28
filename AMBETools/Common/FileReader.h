/*
 *   Copyright (C) 2014 by Jonathan Naylor G4KLX
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

#ifndef	FileReader_H
#define FileReader_H

enum DVTFR_TYPE {
	DVTFR_HEADER,
	DVTFR_DETAIL,
	DVTFR_TRAILER
};

#include <wx/wx.h>

class IFileReader {
public:
	virtual ~IFileReader() = 0;

	virtual wxString     getFileName() = 0;

	virtual bool         open(const wxString& fileName) = 0;
	virtual unsigned int read(unsigned char* buffer, unsigned int length, DVTFR_TYPE& type) = 0;
	virtual void         close() = 0;
};

#endif
