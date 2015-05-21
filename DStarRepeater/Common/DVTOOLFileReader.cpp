/*
 *   Copyright (C) 2009,2013,2015 by Jonathan Naylor G4KLX
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
#include "DStarDefines.h"
#include "Utils.h"

#include <cstdio>
#include <cassert>

static const char              DVTOOL_SIGNATURE[] = "DVTOOL";
static const unsigned int DVTOOL_SIGNATURE_LENGTH = 6U;

static const char              DSVT_SIGNATURE[] = "DSVT";
static const unsigned int DSVT_SIGNATURE_LENGTH = 4U;

static const unsigned int FIXED_DATA_LENGTH = 9U;

static const unsigned char HEADER_FLAG   = 0x10;
static const unsigned char DATA_FLAG     = 0x20;

static const unsigned char HEADER_MASK   = 0x80;
static const unsigned char TRAILER_MASK  = 0x40;

const unsigned int BUFFER_LENGTH = 255U;


CDVTOOLFileReader::CDVTOOLFileReader() :
m_fileName(),
m_file(NULL),
m_records(0U),
m_type(DVTFR_NONE),
m_buffer(NULL),
m_length(0U),
m_end(false)
{
	m_buffer = new unsigned char[BUFFER_LENGTH];
}

CDVTOOLFileReader::~CDVTOOLFileReader()
{
	delete[] m_buffer;
}

std::string CDVTOOLFileReader::getFileName() const
{
	return m_fileName;
}

unsigned int CDVTOOLFileReader::getRecords() const
{
	return m_records;
}

bool CDVTOOLFileReader::open(const std::string& fileName)
{
	m_fileName = fileName;

	m_file = ::fopen(fileName.c_str(), "rb");
	if (m_file == NULL)
		return false;

	unsigned char buffer[DVTOOL_SIGNATURE_LENGTH];
	size_t n = ::fread(buffer, 1, DVTOOL_SIGNATURE_LENGTH, m_file);
	if (n != DVTOOL_SIGNATURE_LENGTH) {
		::fclose(m_file);
		m_file = NULL;
		return false;
	}

	if (::memcmp(buffer, DVTOOL_SIGNATURE, DVTOOL_SIGNATURE_LENGTH) != 0) {
		::fclose(m_file);
		m_file = NULL;
		return false;
	}

	n = ::fread(buffer, 1, sizeof(uint32_t), m_file);
	if (n != sizeof(uint32_t)) {
		::fclose(m_file);
		m_file = NULL;
		return false;
	}

	m_records = (buffer[0U] << 24) + (buffer[1U] << 16) + (buffer[2U] << 8) + (buffer[3U] << 0);
	m_end     = false;

	return true;
}

DVTFR_TYPE CDVTOOLFileReader::read()
{
	unsigned char bytes[FIXED_DATA_LENGTH];
	size_t n = ::fread(bytes, 1, sizeof(uint16_t), m_file);
	if (n != sizeof(uint16_t))
		return DVTFR_NONE;

	m_length = (bytes[0U] << 0) + (bytes[1U] << 8) - 15U;

	n = ::fread(bytes, 1, DSVT_SIGNATURE_LENGTH, m_file);
	if (n != DSVT_SIGNATURE_LENGTH)
		return DVTFR_NONE;

	if (::memcmp(bytes, DSVT_SIGNATURE, DSVT_SIGNATURE_LENGTH) != 0)
		return DVTFR_NONE;

	char flag;
	n = ::fread(&flag, 1, 1, m_file);
	if (n != 1U)
		return DVTFR_NONE;

	m_type = (flag == HEADER_FLAG) ? DVTFR_HEADER : DVTFR_DATA;

	n = ::fread(bytes, 1, FIXED_DATA_LENGTH, m_file);
	if (n != FIXED_DATA_LENGTH)
		return DVTFR_NONE;

	n = ::fread(&flag, 1, 1, m_file);
	if (n != 1U)
		return DVTFR_NONE;

	if (m_type == DVTFR_DATA) {
		if ((flag & TRAILER_MASK) == TRAILER_MASK)
			m_end = true;
	}

	n = ::fread(m_buffer, 1, m_length, m_file);
	if (n != m_length)
		return DVTFR_NONE;

	return m_type;
}

CHeaderData* CDVTOOLFileReader::readHeader()
{
	if (m_type != DVTFR_HEADER)
		return NULL;

	if (m_buffer[39U] == 0xFFU && m_buffer[40U] == 0xFFU)
		return new CHeaderData(m_buffer, RADIO_HEADER_LENGTH_BYTES, false);

	// Header checksum testing is enabled
	CHeaderData* header = new CHeaderData(m_buffer, RADIO_HEADER_LENGTH_BYTES, true);

	if (!header->isValid()) {
		CUtils::dump("Header checksum failure", m_buffer, RADIO_HEADER_LENGTH_BYTES);
		delete header;
		return NULL;
	}

	return header;
}

unsigned int CDVTOOLFileReader::readData(unsigned char* buffer, unsigned int length, bool& end)
{
	assert(buffer != NULL);
	assert(length > 0U);

	if (m_type != DVTFR_DATA)
		return 0U;

	if (length > m_length)
		length = m_length;

	end = m_end;

	::memcpy(buffer, m_buffer, length);

	return length;
}

void CDVTOOLFileReader::close()
{
	::fclose(m_file);
	m_file = NULL;
}
