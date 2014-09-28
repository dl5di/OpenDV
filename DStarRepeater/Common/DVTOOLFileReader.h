/*
 *   Copyright (C) 2009 by Jonathan Naylor G4KLX
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

#ifndef	DVTOOLFileReader_H
#define DVTOOLFileReader_H

#include "HeaderData.h"

enum DVTFR_TYPE {
	DVTFR_NONE,
	DVTFR_HEADER,
	DVTFR_DATA
};

#include <wx/wx.h>
#include <wx/ffile.h>

class CDVTOOLFileReader {
public:
	CDVTOOLFileReader();
	~CDVTOOLFileReader();

	wxString     getFileName() const;
	unsigned int getRecords() const;

	bool         open(const wxString& fileName);
	DVTFR_TYPE   read();
	CHeaderData* readHeader();
	unsigned int readData(unsigned char* buffer, unsigned int length, bool& end);
	void         close();

private:
	wxString       m_fileName;
	wxFFile        m_file;
	wxUint32       m_records;
	DVTFR_TYPE     m_type;
	unsigned char* m_buffer;
	unsigned int   m_length;
	bool           m_end;
};

#endif
