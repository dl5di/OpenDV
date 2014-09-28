/*
 *   Copyright (C) 2009,2010,2014 by Jonathan Naylor G4KLX
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

#include "FileReader.h"

#include <wx/wx.h>
#include <wx/ffile.h>

class CDVTOOLFileReader : public IFileReader {
public:
	CDVTOOLFileReader();
	virtual ~CDVTOOLFileReader();

	virtual wxString     getFileName();

	virtual bool         open(const wxString& fileName);
	virtual unsigned int read(unsigned char* buffer, unsigned int length, DVTFR_TYPE& type);
	virtual void         close();

private:
	wxString m_fileName;
	wxFFile  m_file;
	wxUint32 m_records;
};

#endif
