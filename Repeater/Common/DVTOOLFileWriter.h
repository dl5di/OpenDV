/*
 *   Copyright (C) 2009,2013 by Jonathan Naylor G4KLX
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

#include "HeaderData.h"

#include <wx/wx.h>
#include <wx/ffile.h>

class CDVTOOLFileWriter {
public:
	CDVTOOLFileWriter();
	~CDVTOOLFileWriter();

	static void setDirectory(const wxString& dirName);

	wxString getFileName() const;

	bool open(const CHeaderData& header);
	bool open(const wxString& filename, const CHeaderData& header);
	bool write(const unsigned char* buffer, unsigned int length);
	void close();

private:
	static wxString m_dirName;

	wxString     m_fileName;
	wxFFile      m_file;
	wxUint32     m_count;
	unsigned int m_sequence;
	wxFileOffset m_offset;

	bool writeHeader(const CHeaderData& header);
	bool writeTrailer();
};

#endif
