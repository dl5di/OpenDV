/*
 *   Copyright (C) 2010,2011,2012 by Jonathan Naylor G4KLX
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

#include "DExtraGatewayHeaderData.h"
#include "CCITTChecksumReverse.h"
#include "DStarDefines.h"
#include "Utils.h"

unsigned int CDExtraGatewayHeaderData::createId()
{
	return (::rand() % 65535U) + 1U;
}

CDExtraGatewayHeaderData::CDExtraGatewayHeaderData() :
m_id(0U),
m_flag1(0U),
m_flag2(0U),
m_flag3(0U),
m_myCall1(NULL),
m_myCall2(NULL),
m_yourCall(NULL),
m_rptCall1(NULL),
m_rptCall2(NULL)
{
	m_myCall1  = new unsigned char[LONG_CALLSIGN_LENGTH];
	m_myCall2  = new unsigned char[SHORT_CALLSIGN_LENGTH];
	m_yourCall = new unsigned char[LONG_CALLSIGN_LENGTH];
	m_rptCall1 = new unsigned char[LONG_CALLSIGN_LENGTH];
	m_rptCall2 = new unsigned char[LONG_CALLSIGN_LENGTH];
}

CDExtraGatewayHeaderData::CDExtraGatewayHeaderData(const CDExtraGatewayHeaderData& header) :
m_id(header.m_id),
m_flag1(header.m_flag1),
m_flag2(header.m_flag2),
m_flag3(header.m_flag3),
m_myCall1(NULL),
m_myCall2(NULL),
m_yourCall(NULL),
m_rptCall1(NULL),
m_rptCall2(NULL)
{
	m_myCall1  = new unsigned char[LONG_CALLSIGN_LENGTH];
	m_myCall2  = new unsigned char[SHORT_CALLSIGN_LENGTH];
	m_yourCall = new unsigned char[LONG_CALLSIGN_LENGTH];
	m_rptCall1 = new unsigned char[LONG_CALLSIGN_LENGTH];
	m_rptCall2 = new unsigned char[LONG_CALLSIGN_LENGTH];

	::memcpy(m_myCall1,  header.m_myCall1,  LONG_CALLSIGN_LENGTH);
	::memcpy(m_myCall2,  header.m_myCall2,  SHORT_CALLSIGN_LENGTH);
	::memcpy(m_yourCall, header.m_yourCall, LONG_CALLSIGN_LENGTH);
	::memcpy(m_rptCall1, header.m_rptCall1, LONG_CALLSIGN_LENGTH);
	::memcpy(m_rptCall2, header.m_rptCall2, LONG_CALLSIGN_LENGTH);
}

CDExtraGatewayHeaderData::~CDExtraGatewayHeaderData()
{
	delete[] m_myCall1;
	delete[] m_myCall2;
	delete[] m_yourCall;
	delete[] m_rptCall1;
	delete[] m_rptCall2;
}

bool CDExtraGatewayHeaderData::setRepeaterData(const unsigned char *data, unsigned int length, bool check)
{
	wxASSERT(data != NULL);

	if (length < 49U) {
		wxLogMessage(wxT("Header data from the repeater is too short, %u < 51"), length);
		return false;
	}

	if (data[0] != 'D' || data[1] != 'S' || data[2] != 'R' || data[3] != 'P') {
		CUtils::dump(wxT("Invalid signature from the repeater"), data, length);
		return false;
	}

	m_id = data[5] * 256U + data[6];

	if (data[4] != 0x20U) {
		CUtils::dump(wxT("Invalid data type from the repeater"), data, length);
		return false;
	}

	m_flag1 = data[8U];
	m_flag2 = data[9U];
	m_flag3 = data[10U];

	::memcpy(m_rptCall2, data + 11U, LONG_CALLSIGN_LENGTH);
	::memcpy(m_rptCall1, data + 19U, LONG_CALLSIGN_LENGTH);
	::memcpy(m_yourCall, data + 27U, LONG_CALLSIGN_LENGTH);
	::memcpy(m_myCall1,  data + 35U, LONG_CALLSIGN_LENGTH);
	::memcpy(m_myCall2,  data + 43U, SHORT_CALLSIGN_LENGTH);

	if (check) {
		CCCITTChecksumReverse cksum;
		cksum.update(data + 8U, RADIO_HEADER_LENGTH_BYTES - 2U);
		bool valid = cksum.check(data + 8U + RADIO_HEADER_LENGTH_BYTES - 2U);

		if (!valid)
			CUtils::dump(wxT("Header checksum failure from the repeater"), data + 8U, RADIO_HEADER_LENGTH_BYTES);

		return valid;
	} else {
		return true;
	}
}

bool CDExtraGatewayHeaderData::setDExtraData(const unsigned char *data, unsigned int length, bool check)
{
	wxASSERT(data != NULL);

	if (length < 56U) {
		wxLogMessage(wxT("Header data from DExtra is too short, %u < 56"), length);
		return false;
	}

	if (data[0] != 'D' || data[1] != 'S' || data[2] != 'V' || data[3] != 'T') {
		CUtils::dump(wxT("Invalid signature from DExtra"), data, length);
		return false;
	}

	m_id = data[12] * 256U + data[13];

	if (data[14] != 0x80) {
		CUtils::dump(wxT("Invalid data type from DExtra"), data, length);
		return false;
	}

	m_flag1 = data[15U];
	m_flag2 = data[16U];
	m_flag3 = data[17U];

	::memcpy(m_rptCall2, data + 18U, LONG_CALLSIGN_LENGTH);
	::memcpy(m_rptCall1, data + 26U, LONG_CALLSIGN_LENGTH);
	::memcpy(m_yourCall, data + 34U, LONG_CALLSIGN_LENGTH);
	::memcpy(m_myCall1,  data + 42U, LONG_CALLSIGN_LENGTH);
	::memcpy(m_myCall2,  data + 50U, SHORT_CALLSIGN_LENGTH);

	if (check) {
		CCCITTChecksumReverse cksum;
		cksum.update(data + 15U, RADIO_HEADER_LENGTH_BYTES - 2U);
		bool valid = cksum.check(data + 15U + RADIO_HEADER_LENGTH_BYTES - 2U);

		if (!valid)
			CUtils::dump(wxT("Header checksum failure from DExtra"), data + 15U, RADIO_HEADER_LENGTH_BYTES);

		return valid;
	} else {
		return true;
	}
}

unsigned int CDExtraGatewayHeaderData::getRepeaterData(unsigned char *data, unsigned int length, bool check) const
{
	wxASSERT(data != NULL);
	wxASSERT(length >= 49U);

	data[0] = 'D';
	data[1] = 'S';
	data[2] = 'R';
	data[3] = 'P';

	data[4] = 0x20U;

	data[5] = m_id / 256U;			// Unique session id
	data[6] = m_id % 256U;

	data[7] = 0U;

	data[8]  = m_flag1;				// Flags 1, 2, and 3
	data[9]  = m_flag2;
	data[10] = m_flag3;

	::memcpy(data + 11U, m_rptCall2, LONG_CALLSIGN_LENGTH);
	::memcpy(data + 19U, m_rptCall1, LONG_CALLSIGN_LENGTH);
	::memcpy(data + 27U, m_yourCall, LONG_CALLSIGN_LENGTH);
	::memcpy(data + 35U, m_myCall1,  LONG_CALLSIGN_LENGTH);
	::memcpy(data + 43U, m_myCall2,  SHORT_CALLSIGN_LENGTH);

	if (check) {
		CCCITTChecksumReverse csum;
		csum.update(data + 8U, 4U * LONG_CALLSIGN_LENGTH + SHORT_CALLSIGN_LENGTH + 3U);
		csum.result(data + 47U);
	} else {
		data[47] = 0xFF;
		data[48] = 0xFF;
	}

	return 49U;
}

unsigned int CDExtraGatewayHeaderData::getDExtraData(unsigned char* data, unsigned int length, bool check) const
{
	wxASSERT(data != NULL);
	wxASSERT(length >= 56U);

	data[0]  = 'D';
	data[1]  = 'S';
	data[2]  = 'V';
	data[3]  = 'T';

	data[4]  = 0x10;
	data[5]  = 0x00;
	data[6]  = 0x00;
	data[7]  = 0x00;
	data[8]  = 0x20;
	data[9]  = 0x00;
	data[10] = 0x01;
	data[11] = 0x02;

	data[12] = m_id % 256U;			// Unique session id
	data[13] = m_id / 256U;

	data[14] = 0x80;

	data[15] = 0x00;				// Flags 1, 2, and 3
	data[16] = 0x00;
	data[17] = 0x00;

	::memcpy(data + 18U, m_rptCall2, LONG_CALLSIGN_LENGTH);
	::memcpy(data + 26U, m_rptCall1, LONG_CALLSIGN_LENGTH);
	::memcpy(data + 34U, "CQCQCQ  ", LONG_CALLSIGN_LENGTH);
	::memcpy(data + 42U, m_myCall1,  LONG_CALLSIGN_LENGTH);
	::memcpy(data + 50U, m_myCall2,  SHORT_CALLSIGN_LENGTH);

	if (check) {
		CCCITTChecksumReverse csum;
		csum.update(data + 15, 4U * LONG_CALLSIGN_LENGTH + SHORT_CALLSIGN_LENGTH + 3U);
		csum.result(data + 54);
	} else {
		data[54] = 0xFF;
		data[55] = 0xFF;
	}

	return 56U;
}

unsigned int CDExtraGatewayHeaderData::getId() const
{
	return m_id;
}

void CDExtraGatewayHeaderData::setId(unsigned int id)
{
	m_id = id;
}

unsigned char CDExtraGatewayHeaderData::getFlag1() const
{
	return m_flag1;
}

unsigned char CDExtraGatewayHeaderData::getFlag2() const
{
	return m_flag2;
}

unsigned char CDExtraGatewayHeaderData::getFlag3() const
{
	return m_flag3;
}

wxString CDExtraGatewayHeaderData::getMyCall1() const
{
	return wxString((const char*)m_myCall1, wxConvLocal, LONG_CALLSIGN_LENGTH);
}

wxString CDExtraGatewayHeaderData::getMyCall2() const
{
	return wxString((const char*)m_myCall2, wxConvLocal, SHORT_CALLSIGN_LENGTH);
}

wxString CDExtraGatewayHeaderData::getYourCall() const
{
	return wxString((const char*)m_yourCall, wxConvLocal, LONG_CALLSIGN_LENGTH);
}

wxString CDExtraGatewayHeaderData::getRptCall1() const
{
	return wxString((const char*)m_rptCall1, wxConvLocal, LONG_CALLSIGN_LENGTH);
}

wxString CDExtraGatewayHeaderData::getRptCall2() const
{
	return wxString((const char*)m_rptCall2, wxConvLocal, LONG_CALLSIGN_LENGTH);
}

void CDExtraGatewayHeaderData::setFlag1(unsigned char flag)
{
	m_flag1 = flag;
}

void CDExtraGatewayHeaderData::setFlag2(unsigned char flag)
{
	m_flag2 = flag;
}

void CDExtraGatewayHeaderData::setFlag3(unsigned char flag)
{
	m_flag3 = flag;
}

void CDExtraGatewayHeaderData::setMyCall1(const wxString& my1)
{
	::memset(m_myCall1, ' ', LONG_CALLSIGN_LENGTH);

	for (unsigned int i = 0U; i < my1.Len(); i++)
		m_myCall1[i] = my1.GetChar(i);
}

void CDExtraGatewayHeaderData::setMyCall2(const wxString& my2)
{
	::memset(m_myCall2, ' ', SHORT_CALLSIGN_LENGTH);

	for (unsigned int i = 0U; i < my2.Len(); i++)
		m_myCall2[i] = my2.GetChar(i);
}

void CDExtraGatewayHeaderData::setYourCall(const wxString& your)
{
	::memset(m_yourCall, ' ', LONG_CALLSIGN_LENGTH);

	for (unsigned int i = 0U; i < your.Len(); i++)
		m_yourCall[i] = your.GetChar(i);
}

void CDExtraGatewayHeaderData::setRptCall1(const wxString& rpt1)
{
	::memset(m_rptCall1, ' ', LONG_CALLSIGN_LENGTH);

	for (unsigned int i = 0U; i < rpt1.Len(); i++)
		m_rptCall1[i] = rpt1.GetChar(i);
}

void CDExtraGatewayHeaderData::setRptCall2(const wxString& rpt2)
{
	::memset(m_rptCall2, ' ', LONG_CALLSIGN_LENGTH);

	for (unsigned int i = 0U; i < rpt2.Len(); i++)
		m_rptCall2[i] = rpt2.GetChar(i);
}

void CDExtraGatewayHeaderData::setReflector(const wxString& reflector)
{
	::memset(m_rptCall2, ' ', LONG_CALLSIGN_LENGTH);

	for (unsigned int i = 0U; i < reflector.Len(); i++)
		m_rptCall2[i] = reflector.GetChar(i);

	// Set RPT1 to the base reflector name + 'G'
	::memcpy(m_rptCall1, m_rptCall2, LONG_CALLSIGN_LENGTH);
	m_rptCall1[LONG_CALLSIGN_LENGTH - 1U] = 'G';
}
