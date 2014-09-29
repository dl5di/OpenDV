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

#include <wx/wx.h>
#include <wx/filename.h>

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

wxString CDVTOOLFileWriter::m_dirName = wxEmptyString;

CDVTOOLFileWriter::CDVTOOLFileWriter() :
m_fileName(),
m_file(),
m_count(0U),
m_sequence(0U),
m_offset(0)
{
}

CDVTOOLFileWriter::~CDVTOOLFileWriter()
{
}

void CDVTOOLFileWriter::setDirectory(const wxString& dirName)
{
	m_dirName = dirName;
}

wxString CDVTOOLFileWriter::getFileName() const
{
	return m_fileName;
}

bool CDVTOOLFileWriter::open(const wxString& filename, const CHeaderData& header)
{
	if (m_file.IsOpened())
		close();

	wxString name = filename;
#if !defined(__WINDOWS__)
	name.Replace(wxT(" "), wxT("_"));
#endif

	wxFileName fileName(m_dirName, name, wxT("dvtool"));
	m_fileName = fileName.GetFullPath();

	bool res = m_file.Open(m_fileName, wxT("wb"));
	if (!res)
		return false;

	size_t n = m_file.Write(DVTOOL_SIGNATURE, DVTOOL_SIGNATURE_LENGTH);
	if (n != DVTOOL_SIGNATURE_LENGTH) {
		m_file.Close();
		return false;
	}

	m_offset = m_file.Tell();

	wxUint32 dummy = 0U;
	n = m_file.Write(&dummy, sizeof(wxUint32));
	if (n != sizeof(wxUint32)) {
		m_file.Close();
		return false;
	}

	m_sequence = 0U;
	m_count = 0U;

	res = writeHeader(header);
	if (!res) {
		m_file.Close();
		return false;
	}

	return true;
}

bool CDVTOOLFileWriter::open(const CHeaderData& header)
{
	if (m_file.IsOpened())
		close();

	wxDateTime time;
	time.SetToCurrent();

	wxString name = time.Format(wxT("%Y%m%d-%H%M%S-"));

	name.Append(header.getRptCall1());
	name.Append(header.getRptCall2());
	name.Append(header.getYourCall());
	name.Append(header.getMyCall1());
	name.Append(header.getMyCall2());

#if !defined(__WINDOWS__)
	name.Replace(wxT(" "), wxT("_"));
#endif
	name.Replace(wxT("/"), wxT("-"));

	wxFileName fileName(m_dirName, name, wxT("dvtool"));
	m_fileName = fileName.GetFullPath();

	bool res = m_file.Open(m_fileName, wxT("wb"));
	if (!res)
		return false;

	size_t n = m_file.Write(DVTOOL_SIGNATURE, DVTOOL_SIGNATURE_LENGTH);
	if (n != DVTOOL_SIGNATURE_LENGTH) {
		m_file.Close();
		return false;
	}

	m_offset = m_file.Tell();

	wxUint32 dummy = 0U;
	n = m_file.Write(&dummy, sizeof(wxUint32));
	if (n != sizeof(wxUint32)) {
		m_file.Close();
		return false;
	}

	m_sequence = 0U;
	m_count = 0U;

	res = writeHeader(header);
	if (!res) {
		m_file.Close();
		return false;
	}

	return true;
}

bool CDVTOOLFileWriter::write(const unsigned char* buffer, unsigned int length)
{
	wxASSERT(buffer != 0);
	wxASSERT(length > 0U);

	wxUint16 len = wxUINT16_SWAP_ON_BE(length + 15U);
	size_t n = m_file.Write(&len, sizeof(wxUint16));
	if (n != sizeof(wxUint16)) {
		m_file.Close();
		return false;
	}

	n = m_file.Write(DSVT_SIGNATURE, DSVT_SIGNATURE_LENGTH);
	if (n != DSVT_SIGNATURE_LENGTH) {
		m_file.Close();
		return false;
	}

	char byte = DATA_FLAG;
	n = m_file.Write(&byte, 1U);
	if (n != 1U) {
		m_file.Close();
		return false;
	}

	n = m_file.Write(FIXED_DATA, FIXED_DATA_LENGTH);
	if (n != FIXED_DATA_LENGTH) {
		m_file.Close();
		return false;
	}

	byte = m_sequence;
	n = m_file.Write(&byte, 1U);
	if (n != 1U) {
		m_file.Close();
		return false;
	}

	n = m_file.Write(buffer, length);
	if (n != length) {
		m_file.Close();
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

	m_file.Seek(m_offset);

	wxUint32 count = wxUINT32_SWAP_ON_LE(m_count);
	m_file.Write(&count, sizeof(wxUint32));

	m_file.Close();
}

bool CDVTOOLFileWriter::writeHeader(const CHeaderData& header)
{
	unsigned char buffer[RADIO_HEADER_LENGTH_BYTES];

	buffer[0] = header.getFlag1();
	buffer[1] = header.getFlag2();
	buffer[2] = header.getFlag3();

	for (unsigned int i = 0U; i < LONG_CALLSIGN_LENGTH; i++)
		buffer[3 + i] = header.getRptCall1().GetChar(i);

	for (unsigned int i = 0U; i < LONG_CALLSIGN_LENGTH; i++)
		buffer[11 + i] = header.getRptCall2().GetChar(i);

	for (unsigned int i = 0U; i < LONG_CALLSIGN_LENGTH; i++)
		buffer[19 + i] = header.getYourCall().GetChar(i);

	for (unsigned int i = 0U; i < LONG_CALLSIGN_LENGTH; i++)
		buffer[27 + i] = header.getMyCall1().GetChar(i);

	for (unsigned int i = 0U; i < SHORT_CALLSIGN_LENGTH; i++)
		buffer[35 + i] = header.getMyCall2().GetChar(i);

	// Get the checksum for the header
	CCCITTChecksumReverse csum;
	csum.update(buffer, RADIO_HEADER_LENGTH_BYTES - 2U);
	csum.result(buffer + 39U);

	wxUint16 len = wxUINT16_SWAP_ON_BE(RADIO_HEADER_LENGTH_BYTES + 15U);
	size_t n = m_file.Write(&len, sizeof(wxUint16));
	if (n != sizeof(wxUint16)) {
		m_file.Close();
		return false;
	}

	n = m_file.Write(DSVT_SIGNATURE, DSVT_SIGNATURE_LENGTH);
	if (n != DSVT_SIGNATURE_LENGTH) {
		m_file.Close();
		return false;
	}

	char byte = HEADER_FLAG;
	n = m_file.Write(&byte, 1U);
	if (n != 1U) {
		m_file.Close();
		return false;
	}

	n = m_file.Write(FIXED_DATA, FIXED_DATA_LENGTH);
	if (n != FIXED_DATA_LENGTH) {
		m_file.Close();
		return false;
	}

	byte = HEADER_MASK;
	n = m_file.Write(&byte, 1U);
	if (n != 1U) {
		m_file.Close();
		return false;
	}

	n = m_file.Write(buffer, RADIO_HEADER_LENGTH_BYTES);
	if (n != RADIO_HEADER_LENGTH_BYTES) {
		m_file.Close();
		return false;
	}

	m_count++;

	return true;
}

bool CDVTOOLFileWriter::writeTrailer()
{
	wxUint16 len = wxUINT16_SWAP_ON_BE(27U);
	size_t n = m_file.Write(&len, sizeof(wxUint16));
	if (n != sizeof(wxUint16)) {
		m_file.Close();
		return false;
	}

	n = m_file.Write(DSVT_SIGNATURE, DSVT_SIGNATURE_LENGTH);
	if (n != DSVT_SIGNATURE_LENGTH) {
		m_file.Close();
		return false;
	}

	char byte = DATA_FLAG;
	n = m_file.Write(&byte, 1U);
	if (n != 1U) {
		m_file.Close();
		return false;
	}

	n = m_file.Write(FIXED_DATA, FIXED_DATA_LENGTH);
	if (n != FIXED_DATA_LENGTH) {
		m_file.Close();
		return false;
	}

	byte = TRAILER_MASK | m_sequence;
	n = m_file.Write(&byte, 1U);
	if (n != 1U) {
		m_file.Close();
		return false;
	}

	n = m_file.Write(TRAILER_DATA, TRAILER_DATA_LENGTH);
	if (n != TRAILER_DATA_LENGTH) {
		m_file.Close();
		return false;
	}

	return true;
}
