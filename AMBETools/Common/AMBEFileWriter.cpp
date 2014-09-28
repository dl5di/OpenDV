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

#include "AMBEFileWriter.h"
#include "DStarDefines.h"

#include <wx/wx.h>

static const char        AMBE_SIGNATURE[] = "AMBE";
static unsigned int AMBE_SIGNATURE_LENGTH = 4U;

CAMBEFileWriter::CAMBEFileWriter(const wxString& filename) :
m_filename(filename),
m_file()
{
}

CAMBEFileWriter::~CAMBEFileWriter()
{
}

wxString CAMBEFileWriter::getFilename()
{
	return m_filename;
}

bool CAMBEFileWriter::open()
{
	if (m_file.IsOpened())
		close();

	bool res = m_file.Open(m_filename, wxT("wb"));
	if (!res)
		return false;

	size_t n = m_file.Write(AMBE_SIGNATURE, AMBE_SIGNATURE_LENGTH);
	if (n != AMBE_SIGNATURE_LENGTH) {
		m_file.Close();
		return false;
	}

	return true;
}

bool CAMBEFileWriter::writeHeader(const CHeaderData& header)
{
	return true;
}

bool CAMBEFileWriter::writeHeader(const unsigned char* buffer, unsigned int length)
{
	wxASSERT(buffer != 0);
	wxASSERT(length > 0U);

	return true;
}

bool CAMBEFileWriter::writeFrame(const unsigned char* buffer, unsigned int length)
{
	wxASSERT(buffer != 0);
	wxASSERT(length > 0U);

	if (length > VOICE_FRAME_LENGTH_BYTES)
		length = VOICE_FRAME_LENGTH_BYTES;

	size_t n = m_file.Write(buffer, length);
	if (n != length) {
		m_file.Close();
		return false;
	}

	return true;
}

void CAMBEFileWriter::close()
{
	m_file.Close();
}
