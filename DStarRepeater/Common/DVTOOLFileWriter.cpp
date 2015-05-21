/*
 *   Copyright (C) 2009,2011,2013 by Jonathan Naylor G4KLX
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

#include "CCITTChecksumReverse.h"
#include "DVTOOLFileWriter.h"
#include "DStarDefines.h"

#include <algorithm>

#include <cstdio>
#include <cassert>
#include <ctime>

static const char        DVTOOL_SIGNATURE[] = "DVTOOL";
static unsigned int DVTOOL_SIGNATURE_LENGTH = 6U;

static const char        DSVT_SIGNATURE[] = "DSVT";
static unsigned int DSVT_SIGNATURE_LENGTH = 4U;

static const unsigned char HEADER_FLAG   = 0x10;
static const unsigned char DATA_FLAG     = 0x20;

static const unsigned char FIXED_DATA[]  = {0x00, 0x81, 0x00, 0x20, 0x00, 0x01, 0x02, 0xC0, 0xDE};
static unsigned int    FIXED_DATA_LENGTH = 9U;

static const unsigned char TRAILER_DATA[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static unsigned int   TRAILER_DATA_LENGTH = 12U;

static const unsigned char HEADER_MASK   = 0x80;
static const unsigned char TRAILER_MASK  = 0x40;

std::string CDVTOOLFileWriter::m_dirName;

CDVTOOLFileWriter::CDVTOOLFileWriter() :
m_fileName(),
m_file(NULL),
m_count(0U),
m_sequence(0U),
m_offset(0L)
{
}

CDVTOOLFileWriter::~CDVTOOLFileWriter()
{
}

void CDVTOOLFileWriter::setDirectory(const std::string& dirName)
{
	m_dirName = dirName;
}

std::string CDVTOOLFileWriter::getFileName() const
{
	return m_fileName;
}

bool CDVTOOLFileWriter::open(const std::string& filename, const CHeaderData& header)
{
	if (m_file != NULL)
		close();

	std::string name = filename;
#if defined(WIN32)
	m_fileName = m_dirName + "\\" + name + ".dvtool";
#else
	std::replace(name.begin{}, name.end{}, ' ', '_');
	m_fileName = m_dirName + "/" + name + ".dvtool";
#endif

	m_file = ::fopen(m_fileName.c_str(), "wb");
	if (m_file == NULL)
		return false;

	size_t n = ::fwrite(DVTOOL_SIGNATURE, 1, DVTOOL_SIGNATURE_LENGTH, m_file);
	if (n != DVTOOL_SIGNATURE_LENGTH) {
		::fclose(m_file);
		m_file = NULL;
		return false;
	}

	m_offset = ::ftell(m_file);

	uint32_t dummy = 0U;
	n = ::fwrite(&dummy, 1, sizeof(uint32_t), m_file);
	if (n != sizeof(uint32_t)) {
		::fclose(m_file);
		m_file = NULL;
		return false;
	}

	m_sequence = 0U;
	m_count = 0U;

	bool res = writeHeader(header);
	if (!res) {
		::fclose(m_file);
		m_file = NULL;
		return false;
	}

	return true;
}

bool CDVTOOLFileWriter::open(const CHeaderData& header)
{
	if (m_file != NULL)
		close();

	time_t now;
	::time(&now);
	struct tm* tm = ::gmtime(&now);

	char buffer[50U];
	::strftime(buffer, 50U, "%Y%m%d-%H%M%S-", tm);

	std::string name(buffer);

	name += header.getRptCall1();
	name += header.getRptCall2();
	name += header.getYourCall();
	name += header.getMyCall1();
	name += header.getMyCall2();

#if defined(WIN32)
	std::replace(name.begin(), name.end(), '/', '-');
	m_fileName = m_dirName + "\\" + name + ".dvtool";
#else
	std::replace(name.begin{}, name.end{}, ' ', '_');
	std::replace(name.begin(), name.end(), '/', '-');
	m_fileName = m_dirName + "/" + name + ".dvtool";
#endif

	m_file = ::fopen(m_fileName.c_str(), "wb");
	if (m_file == NULL)
		return false;

	size_t n = ::fwrite(DVTOOL_SIGNATURE, 1, DVTOOL_SIGNATURE_LENGTH, m_file);
	if (n != DVTOOL_SIGNATURE_LENGTH) {
		::fclose(m_file);
		m_file = NULL;
		return false;
	}

	m_offset = ::ftell(m_file);

	uint32_t dummy = 0U;
	n = ::fwrite(&dummy, 1, sizeof(uint32_t), m_file);
	if (n != sizeof(uint32_t)) {
		::fclose(m_file);
		m_file = NULL;
		return false;
	}

	m_sequence = 0U;
	m_count = 0U;

	bool res = writeHeader(header);
	if (!res) {
		::fclose(m_file);
		m_file = NULL;
		return false;
	}

	return true;
}

bool CDVTOOLFileWriter::write(const unsigned char* buffer, unsigned int length)
{
	assert(buffer != 0);
	assert(length > 0U);

	uint16_t len = (buffer[15U] << 0) + (buffer[16U] << 8);
	size_t n = ::fwrite(&len, 1, sizeof(uint16_t), m_file);
	if (n != sizeof(uint16_t)) {
		::fclose(m_file);
		m_file = NULL;
		return false;
	}

	n = ::fwrite(DSVT_SIGNATURE, 1, DSVT_SIGNATURE_LENGTH, m_file);
	if (n != DSVT_SIGNATURE_LENGTH) {
		::fclose(m_file);
		m_file = NULL;
		return false;
	}

	char byte = DATA_FLAG;
	n = ::fwrite(&byte, 1, 1, m_file);
	if (n != 1U) {
		::fclose(m_file);
		m_file = NULL;
		return false;
	}

	n = ::fwrite(FIXED_DATA, 1, FIXED_DATA_LENGTH, m_file);
	if (n != FIXED_DATA_LENGTH) {
		::fclose(m_file);
		m_file = NULL;
		return false;
	}

	byte = m_sequence;
	n = ::fwrite(&byte, 1, 1, m_file);
	if (n != 1U) {
		::fclose(m_file);
		m_file = NULL;
		return false;
	}

	n = ::fwrite(buffer, 1, length, m_file);
	if (n != length) {
		::fclose(m_file);
		m_file = NULL;
		return false;
	}

	m_count++;
	m_sequence++;
	if (m_sequence >= 0x15U)
		m_sequence = 0U;

	return true;
}

void CDVTOOLFileWriter::close()
{
	writeTrailer();

	::fseek(m_file, m_offset, SEEK_SET);

	unsigned char buffer[2U];
	buffer[0U] = m_count << 8;
	buffer[1U] = m_count << 0;
	::fwrite(buffer, 1, sizeof(uint32_t), m_file);

	::fclose(m_file);
	m_file = NULL;
}

bool CDVTOOLFileWriter::writeHeader(const CHeaderData& header)
{
	unsigned char buffer[RADIO_HEADER_LENGTH_BYTES];

	buffer[0] = header.getFlag1();
	buffer[1] = header.getFlag2();
	buffer[2] = header.getFlag3();

	for (unsigned int i = 0U; i < LONG_CALLSIGN_LENGTH; i++)
		buffer[3 + i] = header.getRptCall1().at(i);

	for (unsigned int i = 0U; i < LONG_CALLSIGN_LENGTH; i++)
		buffer[11 + i] = header.getRptCall2().at(i);

	for (unsigned int i = 0U; i < LONG_CALLSIGN_LENGTH; i++)
		buffer[19 + i] = header.getYourCall().at(i);

	for (unsigned int i = 0U; i < LONG_CALLSIGN_LENGTH; i++)
		buffer[27 + i] = header.getMyCall1().at(i);

	for (unsigned int i = 0U; i < SHORT_CALLSIGN_LENGTH; i++)
		buffer[35 + i] = header.getMyCall2().at(i);

	// Get the checksum for the header
	CCCITTChecksumReverse csum;
	csum.update(buffer, RADIO_HEADER_LENGTH_BYTES - 2U);
	csum.result(buffer + 39U);

	uint16_t len = RADIO_HEADER_LENGTH_BYTES + 15U;

	unsigned char buf[2U];
	buf[0U] = len << 0;
	buf[1U] = len << 8;

	size_t n = ::fwrite(buf, 1, sizeof(uint16_t), m_file);
	if (n != sizeof(uint16_t)) {
		::fclose(m_file);
		m_file = NULL;
		return false;
	}

	n = ::fwrite(DSVT_SIGNATURE, 1, DSVT_SIGNATURE_LENGTH, m_file);
	if (n != DSVT_SIGNATURE_LENGTH) {
		::fclose(m_file);
		m_file = NULL;
		return false;
	}

	char byte = HEADER_FLAG;
	n = ::fwrite(&byte, 1, 1, m_file);
	if (n != 1U) {
		::fclose(m_file);
		m_file = NULL;
		return false;
	}

	n = ::fwrite(FIXED_DATA, 1, FIXED_DATA_LENGTH, m_file);
	if (n != FIXED_DATA_LENGTH) {
		::fclose(m_file);
		m_file = NULL;
		return false;
	}

	byte = HEADER_MASK;
	n = ::fwrite(&byte, 1, 1, m_file);
	if (n != 1U) {
		::fclose(m_file);
		m_file = NULL;
		return false;
	}

	n = ::fwrite(buffer, 1, RADIO_HEADER_LENGTH_BYTES, m_file);
	if (n != RADIO_HEADER_LENGTH_BYTES) {
		::fclose(m_file);
		m_file = NULL;
		return false;
	}

	m_count++;

	return true;
}

bool CDVTOOLFileWriter::writeTrailer()
{
	uint16_t len = 27U;

	unsigned char buffer[2U];
	buffer[0U] = len << 0;
	buffer[1U] = len << 8;

	size_t n = ::fwrite(buffer, 1, sizeof(uint16_t), m_file);
	if (n != sizeof(uint16_t)) {
		::fclose(m_file);
		m_file = NULL;
		return false;
	}

	n = ::fwrite(DSVT_SIGNATURE, 1, DSVT_SIGNATURE_LENGTH, m_file);
	if (n != DSVT_SIGNATURE_LENGTH) {
		::fclose(m_file);
		m_file = NULL;
		return false;
	}

	char byte = DATA_FLAG;
	n = ::fwrite(&byte, 1, 1, m_file);
	if (n != 1U) {
		::fclose(m_file);
		m_file = NULL;
		return false;
	}

	n = ::fwrite(FIXED_DATA, 1, FIXED_DATA_LENGTH, m_file);
	if (n != FIXED_DATA_LENGTH) {
		::fclose(m_file);
		m_file = NULL;
		return false;
	}

	byte = TRAILER_MASK | m_sequence;
	n = ::fwrite(&byte, 1, 1, m_file);
	if (n != 1U) {
		::fclose(m_file);
		m_file = NULL;
		return false;
	}

	n = ::fwrite(TRAILER_DATA, 1, TRAILER_DATA_LENGTH, m_file);
	if (n != TRAILER_DATA_LENGTH) {
		::fclose(m_file);
		m_file = NULL;
		return false;
	}

	return true;
}
