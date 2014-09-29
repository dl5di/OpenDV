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

#include "SplitRepeaterHeaderData.h"
#include "CCITTChecksumReverse.h"
#include "DStarDefines.h"
#include "Utils.h"

void CSplitRepeaterHeaderData::initialise()
{
	wxDateTime now = wxDateTime::UNow();
	::srand(now.GetMillisecond());
}

void CSplitRepeaterHeaderData::finalise()
{
}

unsigned int CSplitRepeaterHeaderData::createId()
{
	return (::rand() % 65535U) + 1U;
}

CSplitRepeaterHeaderData::CSplitRepeaterHeaderData() :
m_id(0U),
m_flag1(0U),
m_flag2(0U),
m_flag3(0U),
m_myCall1(NULL),
m_myCall2(NULL),
m_yourCall(NULL),
m_rptCall1(NULL),
m_rptCall2(NULL),
m_address(),
m_port(0U),
m_errors(0U)
{
	m_myCall1  = new unsigned char[LONG_CALLSIGN_LENGTH];
	m_myCall2  = new unsigned char[SHORT_CALLSIGN_LENGTH];
	m_yourCall = new unsigned char[LONG_CALLSIGN_LENGTH];
	m_rptCall1 = new unsigned char[LONG_CALLSIGN_LENGTH];
	m_rptCall2 = new unsigned char[LONG_CALLSIGN_LENGTH];
}

CSplitRepeaterHeaderData::CSplitRepeaterHeaderData(const CSplitRepeaterHeaderData& header) :
m_id(header.m_id),
m_flag1(header.m_flag1),
m_flag2(header.m_flag2),
m_flag3(header.m_flag3),
m_myCall1(NULL),
m_myCall2(NULL),
m_yourCall(NULL),
m_rptCall1(NULL),
m_rptCall2(NULL),
m_address(header.m_address),
m_port(header.m_port),
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

CSplitRepeaterHeaderData::CSplitRepeaterHeaderData(const wxString& myCall1,  const wxString& myCall2, const wxString& yourCall,
						 const wxString& rptCall1, const wxString& rptCall2, unsigned char flag1,
						 unsigned char flag2, unsigned char flag3) :
m_id(0U),
m_flag1(flag1),
m_flag2(flag2),
m_flag3(flag3),
m_myCall1(NULL),
m_myCall2(NULL),
m_yourCall(NULL),
m_rptCall1(NULL),
m_rptCall2(NULL),
m_address(),
m_port(0U),
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

CSplitRepeaterHeaderData::~CSplitRepeaterHeaderData()
{
	delete[] m_myCall1;
	delete[] m_myCall2;
	delete[] m_yourCall;
	delete[] m_rptCall1;
	delete[] m_rptCall2;
}

bool CSplitRepeaterHeaderData::setRepeaterData(const unsigned char *data, unsigned int length, bool check, const in_addr& address, unsigned int port)
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

	m_address = address;
	m_port    = port;

	if (check) {
		CCCITTChecksumReverse cksum;
		cksum.update(data + 8U, RADIO_HEADER_LENGTH_BYTES - 2U);
		bool valid = cksum.check(data + 8U + RADIO_HEADER_LENGTH_BYTES - 2U);

		return valid;
	} else {
		return true;
	}
}

unsigned int CSplitRepeaterHeaderData::getRepeaterData(unsigned char *data, unsigned int length, bool check) const
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

unsigned int CSplitRepeaterHeaderData::getId() const
{
	return m_id;
}

void CSplitRepeaterHeaderData::setId(unsigned int id)
{
	m_id = id;
}

unsigned char CSplitRepeaterHeaderData::getFlag1() const
{
	return m_flag1;
}

unsigned char CSplitRepeaterHeaderData::getFlag2() const
{
	return m_flag2;
}

unsigned char CSplitRepeaterHeaderData::getFlag3() const
{
	return m_flag3;
}

void CSplitRepeaterHeaderData::setFlags(unsigned char flag1, unsigned char flag2, unsigned char flag3)
{
	m_flag1 = flag1;
	m_flag2 = flag2;
	m_flag3 = flag3;
}

wxString CSplitRepeaterHeaderData::getMyCall1() const
{
	return wxString((const char*)m_myCall1, wxConvLocal, LONG_CALLSIGN_LENGTH);
}

wxString CSplitRepeaterHeaderData::getMyCall2() const
{
	return wxString((const char*)m_myCall2, wxConvLocal, SHORT_CALLSIGN_LENGTH);
}

wxString CSplitRepeaterHeaderData::getYourCall() const
{
	return wxString((const char*)m_yourCall, wxConvLocal, LONG_CALLSIGN_LENGTH);
}

wxString CSplitRepeaterHeaderData::getRptCall1() const
{
	return wxString((const char*)m_rptCall1, wxConvLocal, LONG_CALLSIGN_LENGTH);
}

wxString CSplitRepeaterHeaderData::getRptCall2() const
{
	return wxString((const char*)m_rptCall2, wxConvLocal, LONG_CALLSIGN_LENGTH);
}

bool CSplitRepeaterHeaderData::isAck() const
{
	return (m_flag1 & ACK_FLAG) == ACK_FLAG;
}

bool CSplitRepeaterHeaderData::isNoResponse() const
{
	return (m_flag1 & NO_RESPONSE) == NO_RESPONSE;
}

bool CSplitRepeaterHeaderData::isRelayUnavailable() const
{
	return (m_flag1 & RELAY_UNAVAILABLE) == RELAY_UNAVAILABLE;
}

void CSplitRepeaterHeaderData::setRepeaterMode(bool set)
{
	if (set)
		m_flag1 |= REPEATER_MASK;
	else
		m_flag1 &= ~REPEATER_MASK;
}

bool CSplitRepeaterHeaderData::isRepeaterMode() const
{
	return (m_flag1 & REPEATER_MASK) == REPEATER_MASK;
}

bool CSplitRepeaterHeaderData::isDataPacket() const
{
	return (m_flag1 & DATA_MASK) == DATA_MASK;
}

void CSplitRepeaterHeaderData::setFlag1(unsigned char flag)
{
	m_flag1 = flag;
}

void CSplitRepeaterHeaderData::setFlag2(unsigned char flag)
{
	m_flag2 = flag;
}

void CSplitRepeaterHeaderData::setFlag3(unsigned char flag)
{
	m_flag3 = flag;
}

void CSplitRepeaterHeaderData::setMyCall1(const wxString& my1)
{
	::memset(m_myCall1, ' ', LONG_CALLSIGN_LENGTH);

	for (unsigned int i = 0U; i < my1.Len(); i++)
		m_myCall1[i] = my1.GetChar(i);
}

void CSplitRepeaterHeaderData::setMyCall2(const wxString& my2)
{
	::memset(m_myCall2, ' ', SHORT_CALLSIGN_LENGTH);

	for (unsigned int i = 0U; i < my2.Len(); i++)
		m_myCall2[i] = my2.GetChar(i);
}

void CSplitRepeaterHeaderData::setYourCall(const wxString& your)
{
	::memset(m_yourCall, ' ', LONG_CALLSIGN_LENGTH);

	for (unsigned int i = 0U; i < your.Len(); i++)
		m_yourCall[i] = your.GetChar(i);
}

void CSplitRepeaterHeaderData::setRptCall1(const wxString& rpt1)
{
	::memset(m_rptCall1, ' ', LONG_CALLSIGN_LENGTH);

	for (unsigned int i = 0U; i < rpt1.Len(); i++)
		m_rptCall1[i] = rpt1.GetChar(i);
}

void CSplitRepeaterHeaderData::setRptCall2(const wxString& rpt2)
{
	::memset(m_rptCall2, ' ', LONG_CALLSIGN_LENGTH);

	for (unsigned int i = 0U; i < rpt2.Len(); i++)
		m_rptCall2[i] = rpt2.GetChar(i);
}

void CSplitRepeaterHeaderData::setRepeaters(const wxString& rpt1, const wxString& rpt2)
{
	::memset(m_rptCall1, ' ', LONG_CALLSIGN_LENGTH);
	::memset(m_rptCall2, ' ', LONG_CALLSIGN_LENGTH);

	for (unsigned int i = 0U; i < rpt1.Len(); i++)
		m_rptCall1[i] = rpt1.GetChar(i);

	for (unsigned int i = 0U; i < rpt2.Len(); i++)
		m_rptCall2[i] = rpt2.GetChar(i);
}

void CSplitRepeaterHeaderData::setCQCQCQ()
{
	::memcpy(m_yourCall, "CQCQCQ  ", LONG_CALLSIGN_LENGTH);
}

bool CSplitRepeaterHeaderData::setData(const unsigned char *data, unsigned int length)
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

	CCCITTChecksumReverse cksum;
	cksum.update(data, RADIO_HEADER_LENGTH_BYTES - 2U);

	return cksum.check(data + RADIO_HEADER_LENGTH_BYTES - 2U);
}

unsigned int CSplitRepeaterHeaderData::getData(unsigned char *data, unsigned int length) const
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

	CCCITTChecksumReverse csum;
	csum.update(data, RADIO_HEADER_LENGTH_BYTES - 2U);
	csum.result(data + RADIO_HEADER_LENGTH_BYTES - 2U);

	return RADIO_HEADER_LENGTH_BYTES;
}

void CSplitRepeaterHeaderData::setDestination(const in_addr& address, unsigned int port)
{
	m_address = address;
	m_port    = port;
}

in_addr CSplitRepeaterHeaderData::getAddress() const
{
	return m_address;
}

unsigned int CSplitRepeaterHeaderData::getPort() const
{
	return m_port;
}
