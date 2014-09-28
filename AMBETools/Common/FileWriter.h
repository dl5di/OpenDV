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

#ifndef	FileWriter_H
#define FileWriter_H

#include "HeaderData.h"

#include <wx/wx.h>

class IFileWriter {
public:
	virtual ~IFileWriter() = 0;

	virtual wxString getFilename() = 0;

	virtual bool open() = 0;
	virtual bool writeHeader(const CHeaderData& header) = 0;
	virtual bool writeHeader(const unsigned char* buffer, unsigned int length) = 0;
	virtual bool writeFrame(const unsigned char* buffer, unsigned int length) = 0;
	virtual void close() = 0;
};

#endif
