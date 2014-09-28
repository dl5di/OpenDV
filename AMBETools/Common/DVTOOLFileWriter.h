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

#ifndef	DVTOOLFileWriter_H
#define DVTOOLFileWriter_H

#include "FileWriter.h"

#include <wx/wx.h>
#include <wx/ffile.h>

class CDVTOOLFileWriter : public IFileWriter {
public:
	CDVTOOLFileWriter(const wxString& filename);
	virtual ~CDVTOOLFileWriter();

	virtual wxString getFilename();

	virtual bool open();
	virtual bool writeHeader(const CHeaderData& header);
	virtual bool writeHeader(const unsigned char* buffer, unsigned int length);
	virtual bool writeFrame(const unsigned char* buffer, unsigned int length);
	virtual void close();

private:
	wxString     m_filename;
	wxFFile      m_file;
	wxUint32     m_count;
	unsigned int m_sequence;
	wxFileOffset m_offset;

	bool writeTrailer();
};

#endif
