/*
 *   Copyright (C) 2009,2014 by Jonathan Naylor G4KLX
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

#include "DVTOOLFileReader.h"

#include <wx/wx.h>

static const char              DVTOOL_SIGNATURE[] = "DVTOOL";
static const unsigned int DVTOOL_SIGNATURE_LENGTH = 6U;

static const char              DSVT_SIGNATURE[] = "DSVT";
static const unsigned int DSVT_SIGNATURE_LENGTH = 4U;

static const unsigned int FIXED_DATA_LENGTH = 9U;

static const unsigned char HEADER_FLAG   = 0x10;
static const unsigned char DATA_FLAG     = 0x20;

static const unsigned char HEADER_MASK   = 0x80;
static const unsigned char TRAILER_MASK  = 0x40;

CDVTOOLFileReader::CDVTOOLFileReader() :
m_fileName(),
m_file(),
m_records(0U)
{
}

CDVTOOLFileReader::~CDVTOOLFileReader()
{
}

wxString CDVTOOLFileReader::getFileName()
{
	return m_fileName;
}

bool CDVTOOLFileReader::open(const wxString& fileName)
{
	m_fileName = fileName;

	bool res = m_file.Open(fileName, wxT("rb"));
	if (!res)
		return false;

	unsigned char buffer[DVTOOL_SIGNATURE_LENGTH];
	size_t n = m_file.Read(buffer, DVTOOL_SIGNATURE_LENGTH);
	if (n != DVTOOL_SIGNATURE_LENGTH) {
		m_file.Close();
		return false;
	}

	if (::memcmp(buffer, DVTOOL_SIGNATURE, DVTOOL_SIGNATURE_LENGTH) != 0) {
		m_file.Close();
		return false;
	}

	wxUint32 uint32;
	n = m_file.Read(&uint32, sizeof(wxUint32));
	if (n != sizeof(wxUint32)) {
		m_file.Close();
		return false;
	}

	m_records = wxUINT32_SWAP_ON_LE(uint32);

	return true;
}

unsigned int CDVTOOLFileReader::read(unsigned char* buffer, unsigned int length, DVTFR_TYPE& type)
{
	wxASSERT(buffer != 0);
	wxASSERT(length > 0U);

	wxUint16 uint16;
	size_t n = m_file.Read(&uint16, sizeof(wxUint16));
	if (n != sizeof(wxUint16))
		return 0U;

	wxUint16 len = wxUINT16_SWAP_ON_BE(uint16) - 15U;
	if (len > length)
		return 0U;

	unsigned char bytes[FIXED_DATA_LENGTH];
	n = m_file.Read(bytes, DSVT_SIGNATURE_LENGTH);
	if (n != DSVT_SIGNATURE_LENGTH)
		return 0U;

	if (::memcmp(bytes, DSVT_SIGNATURE, DSVT_SIGNATURE_LENGTH) != 0)
		return 0U;

	char flag;
	n = m_file.Read(&flag, 1U);
	if (n != 1U)
		return 0U;

	type = (flag == HEADER_FLAG) ? DVTFR_HEADER : DVTFR_DETAIL;

	n = m_file.Read(bytes, FIXED_DATA_LENGTH);
	if (n != FIXED_DATA_LENGTH)
		return 0U;

	n = m_file.Read(&flag, 1U);
	if (n != 1U)
		return 0U;

	if (type == DVTFR_DETAIL) {
		if ((flag & TRAILER_MASK) == TRAILER_MASK)
			type = DVTFR_TRAILER;
	}

	n = m_file.Read(buffer, len);
	if (n != len)
		return 0U;

	return len;
}

void CDVTOOLFileReader::close()
{
	m_file.Close();
}
