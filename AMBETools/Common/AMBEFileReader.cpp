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

#include "AMBEFileReader.h"
#include "DStarDefines.h"

#include <wx/wx.h>

static const char              AMBE_SIGNATURE[] = "AMBE";
static const unsigned int AMBE_SIGNATURE_LENGTH = 4U;

CAMBEFileReader::CAMBEFileReader() :
m_fileName(),
m_file()
{
}

CAMBEFileReader::~CAMBEFileReader()
{
}

wxString CAMBEFileReader::getFileName()
{
	return m_fileName;
}

bool CAMBEFileReader::open(const wxString& fileName)
{
	m_fileName = fileName;

	bool res = m_file.Open(fileName, wxT("rb"));
	if (!res)
		return false;

	unsigned char buffer[AMBE_SIGNATURE_LENGTH];
	size_t n = m_file.Read(buffer, AMBE_SIGNATURE_LENGTH);
	if (n != AMBE_SIGNATURE_LENGTH) {
		m_file.Close();
		return false;
	}

	if (::memcmp(buffer, AMBE_SIGNATURE, AMBE_SIGNATURE_LENGTH) != 0) {
		m_file.Close();
		return false;
	}

	return true;
}

unsigned int CAMBEFileReader::read(unsigned char* buffer, unsigned int length, DVTFR_TYPE& type)
{
	wxASSERT(buffer != 0);
	wxASSERT(length > 0U);

	if (length > VOICE_FRAME_LENGTH_BYTES)
		length = VOICE_FRAME_LENGTH_BYTES;

	size_t n = m_file.Read(buffer, length);
	if (n != length)
		return 0U;

	type = DVTFR_DETAIL;

	return length;
}

void CAMBEFileReader::close()
{
	m_file.Close();
}
