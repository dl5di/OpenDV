/*
 *   Copyright (C) 2010-2013 by Jonathan Naylor G4KLX
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

#include "HeaderData.h"

#include "CCITTChecksum.h"
#include "DStarDefines.h"
#include "Utils.h"

void CHeaderData::initialise()
{
	wxDateTime now = wxDateTime::UNow();
	::srand(now.GetMillisecond());
}

void CHeaderData::finalise()
{
}

unsigned int CHeaderData::createId()
{
	return (::rand() % 65535U) + 1U;
}

CHeaderData::CHeaderData() :
m_rptSeq(0U),
m_id(0U),
m_band1(0x00U),
m_band2(0x02U),
m_band3(0x01U),
m_flag1(0U),
m_flag2(0U),
m_flag3(0U),
m_myCall1(NULL),
m_myCall2(NULL),
m_yourCall(NULL),
m_rptCall1(NULL),
m_rptCall2(NULL),
m_yourAddress(),
m_yourPort(0U),
m_myPort(0U),
m_errors(0U)
{
	m_myCall1  = new unsigned char[LONG_CALLSIGN_LENGTH];
	m_myCall2  = new unsigned char[SHORT_CALLSIGN_LENGTH];
	m_yourCall = new unsigned char[LONG_CALLSIGN_LENGTH];
	m_rptCall1 = new unsigned char[LONG_CALLSIGN_LENGTH];
	m_rptCall2 = new unsigned char[LONG_CALLSIGN_LENGTH];
}

CHeaderData::CHeaderData(const CHeaderData& header) :
m_rptSeq(header.m_rptSeq),
m_id(header.m_id),
m_band1(header.m_band1),
m_band2(header.m_band2),
m_band3(header.m_band3),
m_flag1(header.m_flag1),
m_flag2(header.m_flag2),
m_flag3(header.m_flag3),
m_myCall1(NULL),
m_myCall2(NULL),
m_yourCall(NULL),
m_rptCall1(NULL),
m_rptCall2(NULL),
m_yourAddress(header.m_yourAddress),
m_yourPort(header.m_yourPort),
m_myPort(header.m_myPort),
m_errors(header.m_errors)
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

CHeaderData::CHeaderData(const wxString& myCall1,  const wxString& myCall2, const wxString& yourCall,
						 const wxString& rptCall1, const wxString& rptCall2, unsigned char flag1,
						 unsigned char flag2, unsigned char flag3) :
m_rptSeq(0U),
m_id(0U),
m_band1(0U),
m_band2(0U),
m_band3(0U),
m_flag1(flag1),
m_flag2(flag2),
m_flag3(flag3),
m_myCall1(NULL),
m_myCall2(NULL),
m_yourCall(NULL),
m_rptCall1(NULL),
m_rptCall2(NULL),
m_yourAddress(),
m_yourPort(0U),
m_myPort(0U),
m_errors(0U)
{
	m_myCall1  = new unsigned char[LONG_CALLSIGN_LENGTH];
	m_myCall2  = new unsigned char[SHORT_CALLSIGN_LENGTH];
	m_yourCall = new unsigned char[LONG_CALLSIGN_LENGTH];
	m_rptCall1 = new unsigned char[LONG_CALLSIGN_LENGTH];
	m_rptCall2 = new unsigned char[LONG_CALLSIGN_LENGTH];

	::memset(m_myCall1,  ' ', LONG_CALLSIGN_LENGTH);
	::memset(m_myCall2,  ' ', SHORT_CALLSIGN_LENGTH);
	::memset(m_yourCall, ' ', LONG_CALLSIGN_LENGTH);
	::memset(m_rptCall1, ' ', LONG_CALLSIGN_LENGTH);
	::memset(m_rptCall2, ' ', LONG_CALLSIGN_LENGTH);

	for (unsigned int i = 0U; i < myCall1.Len() && i < LONG_CALLSIGN_LENGTH; i++)
		m_myCall1[i] = myCall1.GetChar(i);

	for (unsigned int i = 0U; i < myCall2.Len() && i < SHORT_CALLSIGN_LENGTH; i++)
		m_myCall2[i] = myCall2.GetChar(i);

	for (unsigned int i = 0U; i < yourCall.Len() && i < LONG_CALLSIGN_LENGTH; i++)
		m_yourCall[i] = yourCall.GetChar(i);

	for (unsigned int i = 0U; i < rptCall1.Len() && i < LONG_CALLSIGN_LENGTH; i++)
		m_rptCall1[i] = rptCall1.GetChar(i);

	for (unsigned int i = 0U; i < rptCall2.Len() && i < LONG_CALLSIGN_LENGTH; i++)
		m_rptCall2[i] = rptCall2.GetChar(i);
}

CHeaderData::~CHeaderData()
{
	delete[] m_myCall1;
	delete[] m_myCall2;
	delete[] m_yourCall;
	delete[] m_rptCall1;
	delete[] m_rptCall2;
}

bool CHeaderData::setIcomRepeaterData(const unsigned char *data, unsigned int length, bool check, const in_addr& yourAddress, unsigned int yourPort)
{
	wxASSERT(data != NULL);
	wxASSERT(length >= 58U);

	m_rptSeq = data[4]  * 256U + data[5];
	m_band1  = data[11];
	m_band2  = data[12];
	m_band3  = data[13];
	m_id     = data[14] * 256U + data[15];

	m_flag1 = data[17U];
	m_flag2 = data[18U];
	m_flag3 = data[19U];

	::memcpy(m_rptCall2, data + 20U, LONG_CALLSIGN_LENGTH);
	::memcpy(m_rptCall1, data + 28U, LONG_CALLSIGN_LENGTH);
	::memcpy(m_yourCall, data + 36U, LONG_CALLSIGN_LENGTH);
	::memcpy(m_myCall1,  data + 44U, LONG_CALLSIGN_LENGTH);
	::memcpy(m_myCall2,  data + 52U, SHORT_CALLSIGN_LENGTH);

	m_yourAddress = yourAddress;
	m_yourPort    = yourPort;

	if (check) {
		CCCITTChecksum cksum;
		cksum.update(data + 17U, RADIO_HEADER_LENGTH_BYTES - 2U);
		bool valid = cksum.check(data + 17U + RADIO_HEADER_LENGTH_BYTES - 2U);

		if (!valid)
			CUtils::dump(wxT("Header checksum failure from the repeater"), data + 17U, RADIO_HEADER_LENGTH_BYTES);

		return valid;
	} else {
		return true;
	}
}

bool CHeaderData::setHBRepeaterData(const unsigned char *data, unsigned int length, bool check, const in_addr& yourAddress, unsigned int yourPort)
{
	wxASSERT(data != NULL);
	wxASSERT(length >= 49U);

	m_id     = data[5U] * 256U + data[6U];
	m_errors = data[7U];

	m_flag1 = data[8U];
	m_flag2 = data[9U];
	m_flag3 = data[10U];

	::memcpy(m_rptCall2, data + 11U, LONG_CALLSIGN_LENGTH);
	::memcpy(m_rptCall1, data + 19U, LONG_CALLSIGN_LENGTH);
	::memcpy(m_yourCall, data + 27U, LONG_CALLSIGN_LENGTH);
	::memcpy(m_myCall1,  data + 35U, LONG_CALLSIGN_LENGTH);
	::memcpy(m_myCall2,  data + 43U, SHORT_CALLSIGN_LENGTH);

	m_yourAddress = yourAddress;
	m_yourPort    = yourPort;

	if (check) {
		CCCITTChecksum cksum;
		cksum.update(data + 8U, RADIO_HEADER_LENGTH_BYTES - 2U);
		bool valid = cksum.check(data + 8U + RADIO_HEADER_LENGTH_BYTES - 2U);

		if (!valid)
			CUtils::dump(wxT("Header checksum failure from the repeater"), data + 8U, RADIO_HEADER_LENGTH_BYTES);

		return valid;
	} else {
		return true;
	}
}

void CHeaderData::setDCSData(const unsigned char *data, unsigned int length, const in_addr& yourAddress, unsigned int yourPort, unsigned int myPort)
{
	wxASSERT(data != NULL);
	wxASSERT(length >= 100U);

	m_id = data[44U] * 256U + data[43U];

	m_flag1 = data[4U];
	m_flag2 = data[5U];
	m_flag3 = data[6U];

	::memcpy(m_rptCall2, data + 7U,  LONG_CALLSIGN_LENGTH);
	::memcpy(m_rptCall1, data + 15U, LONG_CALLSIGN_LENGTH);
	::memcpy(m_yourCall, data + 23U, LONG_CALLSIGN_LENGTH);
	::memcpy(m_myCall1,  data + 31U, LONG_CALLSIGN_LENGTH);
	::memcpy(m_myCall2,  data + 39U, SHORT_CALLSIGN_LENGTH);

	m_yourAddress = yourAddress;
	m_yourPort    = yourPort;
	m_myPort      = myPort;
}

void CHeaderData::setCCSData(const unsigned char *data, unsigned int length, const in_addr& yourAddress, unsigned int yourPort, unsigned int myPort)
{
	wxASSERT(data != NULL);
	wxASSERT(length >= 100U);

	m_id = data[44U] * 256U + data[43U];

	::memcpy(m_rptCall2, data + 7U,  LONG_CALLSIGN_LENGTH);
	::memcpy(m_rptCall1, data + 15U, LONG_CALLSIGN_LENGTH);
	::memcpy(m_yourCall, data + 23U, LONG_CALLSIGN_LENGTH);
	::memcpy(m_myCall1,  data + 31U, LONG_CALLSIGN_LENGTH);
	::memcpy(m_myCall2,  data + 39U, SHORT_CALLSIGN_LENGTH);

	m_yourAddress = yourAddress;
	m_yourPort    = yourPort;
	m_myPort      = myPort;
}

bool CHeaderData::setG2Data(const unsigned char *data, unsigned int length, bool check, const in_addr& yourAddress, unsigned int yourPort)
{
	wxASSERT(data != NULL);
	wxASSERT(length >= 56U);

	m_band1 = data[9];
	m_band2 = data[10];
	m_band3 = data[11];
	m_id    = data[12] * 256U + data[13];

	m_flag1 = data[15U];
	m_flag2 = data[16U];
	m_flag3 = data[17U];

	::memcpy(m_rptCall2, data + 18U, LONG_CALLSIGN_LENGTH);
	::memcpy(m_rptCall1, data + 26U, LONG_CALLSIGN_LENGTH);
	::memcpy(m_yourCall, data + 34U, LONG_CALLSIGN_LENGTH);
	::memcpy(m_myCall1,  data + 42U, LONG_CALLSIGN_LENGTH);
	::memcpy(m_myCall2,  data + 50U, SHORT_CALLSIGN_LENGTH);

	m_yourAddress = yourAddress;
	m_yourPort    = yourPort;

	if (check) {
		CCCITTChecksum cksum;
		cksum.update(data + 15U, RADIO_HEADER_LENGTH_BYTES - 2U);
		bool valid = cksum.check(data + 15U + RADIO_HEADER_LENGTH_BYTES - 2U);

		if (!valid)
			CUtils::dump(wxT("Header checksum failure from G2"), data + 15U, RADIO_HEADER_LENGTH_BYTES);

		return valid;
	} else {
		return true;
	}
}

bool CHeaderData::setDExtraData(const unsigned char *data, unsigned int length, bool check, const in_addr& yourAddress, unsigned int yourPort, unsigned int myPort)
{
	wxASSERT(data != NULL);
	wxASSERT(length >= 56U);

	m_band1  = data[9];
	m_band2  = data[10];
	m_band3  = data[11];
	m_id     = data[12] * 256U + data[13];

	m_flag1 = data[15U];
	m_flag2 = data[16U];
	m_flag3 = data[17U];

	::memcpy(m_rptCall2, data + 18U, LONG_CALLSIGN_LENGTH);
	::memcpy(m_rptCall1, data + 26U, LONG_CALLSIGN_LENGTH);
	::memcpy(m_yourCall, data + 34U, LONG_CALLSIGN_LENGTH);
	::memcpy(m_myCall1,  data + 42U, LONG_CALLSIGN_LENGTH);
	::memcpy(m_myCall2,  data + 50U, SHORT_CALLSIGN_LENGTH);

	m_yourAddress = yourAddress;
	m_yourPort    = yourPort;
	m_myPort      = myPort;

	if (check) {
		CCCITTChecksum cksum;
		cksum.update(data + 15U, RADIO_HEADER_LENGTH_BYTES - 2U);
		bool valid = cksum.check(data + 15U + RADIO_HEADER_LENGTH_BYTES - 2U);

		if (!valid)
			CUtils::dump(wxT("Header checksum failure from DExtra"), data + 15U, RADIO_HEADER_LENGTH_BYTES);

		return valid;
	} else {
		return true;
	}
}

bool CHeaderData::setDPlusData(const unsigned char *data, unsigned int length, bool check, const in_addr& yourAddress, unsigned int yourPort, unsigned int myPort)
{
	wxASSERT(data != NULL);
	wxASSERT(length >= 58U);

	if (data[0] != 0x3A || data[1] != 0x80) {
		CUtils::dump(wxT("Invalid header length from D-Plus"), data, length);
		return false;
	}

	m_band1  = data[11];
	m_band2  = data[12];
	m_band3  = data[13];
	m_id     = data[14] * 256U + data[15];

	m_flag1 = data[17U];
	m_flag2 = data[18U];
	m_flag3 = data[19U];

	::memcpy(m_rptCall2, data + 20U, LONG_CALLSIGN_LENGTH);
	::memcpy(m_rptCall1, data + 28U, LONG_CALLSIGN_LENGTH);
	::memcpy(m_yourCall, data + 36U, LONG_CALLSIGN_LENGTH);
	::memcpy(m_myCall1,  data + 44U, LONG_CALLSIGN_LENGTH);
	::memcpy(m_myCall2,  data + 52U, SHORT_CALLSIGN_LENGTH);

	m_yourAddress = yourAddress;
	m_yourPort    = yourPort;
	m_myPort      = myPort;

	if (check) {
		CCCITTChecksum cksum;
		cksum.update(data + 17U, RADIO_HEADER_LENGTH_BYTES - 2U);
		bool valid = cksum.check(data + 17U + RADIO_HEADER_LENGTH_BYTES - 2U);

		if (!valid)
			CUtils::dump(wxT("Header checksum failure from D-Plus"), data + 17U, RADIO_HEADER_LENGTH_BYTES);

		return valid;
	} else {
		return true;
	}
}

unsigned int CHeaderData::getIcomRepeaterData(unsigned char *data, unsigned int length, bool check) const
{
	wxASSERT(data != NULL);
	wxASSERT(length >= 58U);

	data[0] = 'D';
	data[1] = 'S';
	data[2] = 'T';
	data[3] = 'R';

	data[4] = m_rptSeq / 256U;		// Packet sequence number
	data[5] = m_rptSeq % 256U;

	data[6] = 0x73;					// Not a response
	data[7] = 0x12;					// Data type

	data[8] = 0x00;					// Length of 48 bytes following
	data[9] = 0x30;

	data[10] = 0x20;				// AMBE plus Slow Data following

	data[11] = m_band1;
	data[12] = m_band2;
	data[13] = m_band3;

	data[14] = m_id / 256U;			// Unique session id
	data[15] = m_id % 256U;

	data[16] = 0x80;

	data[17] = m_flag1;				// Flags 1, 2, and 3
	data[18] = m_flag2;
	data[19] = m_flag3;

	::memcpy(data + 20U, m_rptCall2, LONG_CALLSIGN_LENGTH);
	::memcpy(data + 28U, m_rptCall1, LONG_CALLSIGN_LENGTH);
	::memcpy(data + 36U, m_yourCall, LONG_CALLSIGN_LENGTH);
	::memcpy(data + 44U, m_myCall1,  LONG_CALLSIGN_LENGTH);
	::memcpy(data + 52U, m_myCall2,  SHORT_CALLSIGN_LENGTH);

	if (check) {
		CCCITTChecksum csum;
		csum.update(data + 17, 4U * LONG_CALLSIGN_LENGTH + SHORT_CALLSIGN_LENGTH + 3U);
		csum.result(data + 56);
	} else {
		data[56] = 0xFF;
		data[57] = 0xFF;
	}

	return 58U;
}

unsigned int CHeaderData::getHBRepeaterData(unsigned char *data, unsigned int length, bool check) const
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
		CCCITTChecksum csum;
		csum.update(data + 8U, 4U * LONG_CALLSIGN_LENGTH + SHORT_CALLSIGN_LENGTH + 3U);
		csum.result(data + 47U);
	} else {
		data[47] = 0xFF;
		data[48] = 0xFF;
	}

	return 49U;
}

void CHeaderData::getDCSData(unsigned char *data, unsigned int length) const
{
	wxASSERT(data != NULL);
	wxASSERT(length >= 100U);

	data[4] = m_flag1;				// Flags 1, 2, and 3
	data[5] = m_flag2;
	data[6] = m_flag3;

	::memcpy(data + 7U,  m_rptCall2, LONG_CALLSIGN_LENGTH);
	::memcpy(data + 15U, m_rptCall1, LONG_CALLSIGN_LENGTH);
	::memcpy(data + 23U, m_yourCall, LONG_CALLSIGN_LENGTH);
	::memcpy(data + 31U, m_myCall1,  LONG_CALLSIGN_LENGTH);
	::memcpy(data + 39U, m_myCall2,  SHORT_CALLSIGN_LENGTH);
}

void CHeaderData::getCCSData(unsigned char *data, unsigned int length) const
{
	wxASSERT(data != NULL);
	wxASSERT(length >= 100U);

	::memcpy(data + 7U,  m_rptCall2, LONG_CALLSIGN_LENGTH);
	::memcpy(data + 15U, m_rptCall1, LONG_CALLSIGN_LENGTH);
	::memcpy(data + 23U, m_yourCall, LONG_CALLSIGN_LENGTH);
	::memcpy(data + 31U, m_myCall1,  LONG_CALLSIGN_LENGTH);
	::memcpy(data + 39U, m_myCall2,  SHORT_CALLSIGN_LENGTH);
}

unsigned int CHeaderData::getG2Data(unsigned char *data, unsigned int length, bool check) const
{
	wxASSERT(data != NULL);
	wxASSERT(length >= 56U);

	data[0] = 'D';
	data[1] = 'S';
	data[2] = 'V';
	data[3] = 'T';

	data[4]  = 0x10;
	data[5]  = 0x00;
	data[6]  = 0x15;
	data[7]  = 0x09;
	data[8]  = 0x20;

	data[9]  = m_band1;
	data[10] = m_band2;
	data[11] = m_band3;

	data[12] = m_id / 256U;			// Unique session id
	data[13] = m_id % 256U;

	data[14] = 0x80;

	data[15] = m_flag1;				// Flags 1, 2, and 3
	data[16] = m_flag2;
	data[17] = m_flag3;

	::memcpy(data + 18U, m_rptCall2, LONG_CALLSIGN_LENGTH);
	::memcpy(data + 26U, m_rptCall1, LONG_CALLSIGN_LENGTH);
	::memcpy(data + 34U, m_yourCall, LONG_CALLSIGN_LENGTH);
	::memcpy(data + 42U, m_myCall1,  LONG_CALLSIGN_LENGTH);
	::memcpy(data + 50U, m_myCall2,  SHORT_CALLSIGN_LENGTH);

	if (check) {
		CCCITTChecksum csum;
		csum.update(data + 15, 4U * LONG_CALLSIGN_LENGTH + SHORT_CALLSIGN_LENGTH + 3U);
		csum.result(data + 54);
	} else {
		data[54] = 0xFF;
		data[55] = 0xFF;
	}

	return 56U;
}

unsigned int CHeaderData::getDExtraData(unsigned char* data, unsigned int length, bool check) const
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

	data[9]  = m_band1;
	data[10] = m_band2;
	data[11] = m_band3;

	data[12] = m_id % 256U;			// Unique session id
	data[13] = m_id / 256U;

	data[14] = 0x80;

	data[15] = 0x00;				// Flags 1, 2, and 3
	data[16] = 0x00;
	data[17] = 0x00;

	::memcpy(data + 18U, m_rptCall2, LONG_CALLSIGN_LENGTH);
	::memcpy(data + 26U, m_rptCall1, LONG_CALLSIGN_LENGTH);
	::memcpy(data + 34U, m_yourCall, LONG_CALLSIGN_LENGTH);
	::memcpy(data + 42U, m_myCall1,  LONG_CALLSIGN_LENGTH);
	::memcpy(data + 50U, m_myCall2,  SHORT_CALLSIGN_LENGTH);

	if (check) {
		CCCITTChecksum csum;
		csum.update(data + 15, 4U * LONG_CALLSIGN_LENGTH + SHORT_CALLSIGN_LENGTH + 3U);
		csum.result(data + 54);
	} else {
		data[54] = 0xFF;
		data[55] = 0xFF;
	}

	return 56U;
}

unsigned int CHeaderData::getDPlusData(unsigned char* data, unsigned int length, bool check) const
{
	wxASSERT(data != NULL);
	wxASSERT(length >= 58U);

	data[0]  = 0x3A;
	data[1]  = 0x80;

	data[2]  = 'D';
	data[3]  = 'S';
	data[4]  = 'V';
	data[5]  = 'T';

	data[6]  = 0x10;
	data[7]  = 0x00;
	data[8]  = 0x00;
	data[9]  = 0x00;
	data[10] = 0x20;

	data[11] = m_band1;
	data[12] = m_band2;
	data[13] = m_band3;

	data[14] = m_id % 256U;			// Unique session id
	data[15] = m_id / 256U;

	data[16] = 0x80;

	data[17] = 0x00;				// Flags 1, 2, and 3
	data[18] = 0x00;
	data[19] = 0x00;

	::memcpy(data + 20U, m_rptCall2, LONG_CALLSIGN_LENGTH);
	::memcpy(data + 28U, m_rptCall1, LONG_CALLSIGN_LENGTH);
	::memcpy(data + 36U, m_yourCall, LONG_CALLSIGN_LENGTH);
	::memcpy(data + 44U, m_myCall1,  LONG_CALLSIGN_LENGTH);
	::memcpy(data + 52U, m_myCall2,  SHORT_CALLSIGN_LENGTH);

	if (check) {
		CCCITTChecksum csum;
		csum.update(data + 17, 4U * LONG_CALLSIGN_LENGTH + SHORT_CALLSIGN_LENGTH + 3U);
		csum.result(data + 56);
	} else {
		data[56] = 0xFF;
		data[57] = 0xFF;
	}

	return 58U;
}

unsigned int CHeaderData::getId() const
{
	return m_id;
}

void CHeaderData::setId(unsigned int id)
{
	m_id = id;
}

unsigned char CHeaderData::getBand1() const
{
	return m_band1;
}

unsigned char CHeaderData::getBand2() const
{
	return m_band2;
}

unsigned char CHeaderData::getBand3() const
{
	return m_band3;
}

void CHeaderData::setBands(unsigned char band1, unsigned char band2, unsigned char band3)
{
	m_band1 = band1;
	m_band2 = band2;
	m_band3 = band3;
}

unsigned int CHeaderData::getRptSeq() const
{
	return m_rptSeq;
}

void CHeaderData::setRptSeq(unsigned int seqNo)
{
	m_rptSeq = seqNo;
}

unsigned char CHeaderData::getFlag1() const
{
	return m_flag1;
}

unsigned char CHeaderData::getFlag2() const
{
	return m_flag2;
}

unsigned char CHeaderData::getFlag3() const
{
	return m_flag3;
}

void CHeaderData::setFlags(unsigned char flag1, unsigned char flag2, unsigned char flag3)
{
	m_flag1 = flag1;
	m_flag2 = flag2;
	m_flag3 = flag3;
}

wxString CHeaderData::getMyCall1() const
{
	return wxString((const char*)m_myCall1, wxConvLocal, LONG_CALLSIGN_LENGTH);
}

wxString CHeaderData::getMyCall2() const
{
	return wxString((const char*)m_myCall2, wxConvLocal, SHORT_CALLSIGN_LENGTH);
}

wxString CHeaderData::getYourCall() const
{
	return wxString((const char*)m_yourCall, wxConvLocal, LONG_CALLSIGN_LENGTH);
}

wxString CHeaderData::getRptCall1() const
{
	return wxString((const char*)m_rptCall1, wxConvLocal, LONG_CALLSIGN_LENGTH);
}

wxString CHeaderData::getRptCall2() const
{
	return wxString((const char*)m_rptCall2, wxConvLocal, LONG_CALLSIGN_LENGTH);
}

void CHeaderData::setFlag1(unsigned char flag)
{
	m_flag1 = flag;
}

void CHeaderData::setFlag2(unsigned char flag)
{
	m_flag2 = flag;
}

void CHeaderData::setFlag3(unsigned char flag)
{
	m_flag3 = flag;
}

void CHeaderData::setMyCall1(const wxString& my1)
{
	::memset(m_myCall1, ' ', LONG_CALLSIGN_LENGTH);

	for (unsigned int i = 0U; i < my1.Len(); i++)
		m_myCall1[i] = my1.GetChar(i);
}

void CHeaderData::setMyCall2(const wxString& my2)
{
	::memset(m_myCall2, ' ', SHORT_CALLSIGN_LENGTH);

	for (unsigned int i = 0U; i < my2.Len(); i++)
		m_myCall2[i] = my2.GetChar(i);
}

void CHeaderData::setYourCall(const wxString& your)
{
	::memset(m_yourCall, ' ', LONG_CALLSIGN_LENGTH);

	for (unsigned int i = 0U; i < your.Len(); i++)
		m_yourCall[i] = your.GetChar(i);
}

void CHeaderData::setRptCall1(const wxString& rpt1)
{
	::memset(m_rptCall1, ' ', LONG_CALLSIGN_LENGTH);

	for (unsigned int i = 0U; i < rpt1.Len(); i++)
		m_rptCall1[i] = rpt1.GetChar(i);
}

void CHeaderData::setRptCall2(const wxString& rpt2)
{
	::memset(m_rptCall2, ' ', LONG_CALLSIGN_LENGTH);

	for (unsigned int i = 0U; i < rpt2.Len(); i++)
		m_rptCall2[i] = rpt2.GetChar(i);
}

void CHeaderData::setRepeaters(const wxString& rpt1, const wxString& rpt2)
{
	::memset(m_rptCall1, ' ', LONG_CALLSIGN_LENGTH);
	::memset(m_rptCall2, ' ', LONG_CALLSIGN_LENGTH);

	for (unsigned int i = 0U; i < rpt1.Len(); i++)
		m_rptCall1[i] = rpt1.GetChar(i);

	for (unsigned int i = 0U; i < rpt2.Len(); i++)
		m_rptCall2[i] = rpt2.GetChar(i);
}

void CHeaderData::setCQCQCQ()
{
	::memcpy(m_yourCall, "CQCQCQ  ", LONG_CALLSIGN_LENGTH);
}

bool CHeaderData::setData(const unsigned char *data, unsigned int length)
{
	wxASSERT(data != NULL);
	wxASSERT(length >= RADIO_HEADER_LENGTH_BYTES);

	m_flag1 = data[0U];
	m_flag2 = data[1U];
	m_flag3 = data[2U];

	::memcpy(m_rptCall2, data + 3U,  LONG_CALLSIGN_LENGTH);
	::memcpy(m_rptCall1, data + 11U, LONG_CALLSIGN_LENGTH);
	::memcpy(m_yourCall, data + 19U, LONG_CALLSIGN_LENGTH);
	::memcpy(m_myCall1,  data + 27U, LONG_CALLSIGN_LENGTH);
	::memcpy(m_myCall2,  data + 35U, SHORT_CALLSIGN_LENGTH);

	CCCITTChecksum cksum;
	cksum.update(data, RADIO_HEADER_LENGTH_BYTES - 2U);

	return cksum.check(data + RADIO_HEADER_LENGTH_BYTES - 2U);
}

unsigned int CHeaderData::getData(unsigned char *data, unsigned int length) const
{
	wxASSERT(data != NULL);
	wxASSERT(length >= RADIO_HEADER_LENGTH_BYTES);

	data[0] = m_flag1;				// Flags 1, 2, and 3
	data[1] = m_flag2;
	data[2] = m_flag3;

	::memcpy(data + 3U,  m_rptCall2, LONG_CALLSIGN_LENGTH);
	::memcpy(data + 11U, m_rptCall1, LONG_CALLSIGN_LENGTH);
	::memcpy(data + 19U, m_yourCall, LONG_CALLSIGN_LENGTH);
	::memcpy(data + 27U, m_myCall1,  LONG_CALLSIGN_LENGTH);
	::memcpy(data + 35U, m_myCall2,  SHORT_CALLSIGN_LENGTH);

	CCCITTChecksum csum;
	csum.update(data, RADIO_HEADER_LENGTH_BYTES - 2U);
	csum.result(data + RADIO_HEADER_LENGTH_BYTES - 2U);

	return RADIO_HEADER_LENGTH_BYTES;
}

void CHeaderData::setDestination(const in_addr& address, unsigned int port)
{
	m_yourAddress = address;
	m_yourPort    = port;
}

in_addr CHeaderData::getYourAddress() const
{
	return m_yourAddress;
}

unsigned int CHeaderData::getYourPort() const
{
	return m_yourPort;
}

unsigned int CHeaderData::getMyPort() const
{
	return m_myPort;
}
