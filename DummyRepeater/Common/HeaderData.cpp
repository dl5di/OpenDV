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
#include "HeaderData.h"
#include "DStarDefines.h"

CHeaderData::CHeaderData() :
m_time(),
m_myCall1(),
m_myCall2(),
m_yourCall(),
m_rptCall1(),
m_rptCall2(),
m_flag1(0x00),
m_flag2(0x00),
m_flag3(0x00),
m_valid(false)
{
}

CHeaderData::CHeaderData(const CHeaderData& header) :
m_time(header.m_time),
m_myCall1(header.m_myCall1),
m_myCall2(header.m_myCall2),
m_yourCall(header.m_yourCall),
m_rptCall1(header.m_rptCall1),
m_rptCall2(header.m_rptCall2),
m_flag1(header.m_flag1),
m_flag2(header.m_flag2),
m_flag3(header.m_flag3),
m_valid(header.m_valid)
{
}

CHeaderData::CHeaderData(const unsigned char* data, unsigned int length, bool check) :
m_time(),
m_myCall1(),
m_myCall2(),
m_yourCall(),
m_rptCall1(),
m_rptCall2(),
m_flag1(0x00),
m_flag2(0x00),
m_flag3(0x00),
m_valid(true)
{
	wxASSERT(data != NULL);
	wxASSERT(length >= (RADIO_HEADER_LENGTH_BYTES - 2U));

	const unsigned char* p = data;
	m_flag1 = *p++;
	m_flag2 = *p++;
	m_flag3 = *p++;

	m_rptCall2 = wxString((const char*)p, wxConvLocal, LONG_CALLSIGN_LENGTH);
	p += LONG_CALLSIGN_LENGTH;

	m_rptCall1 = wxString((const char*)p, wxConvLocal, LONG_CALLSIGN_LENGTH);
	p += LONG_CALLSIGN_LENGTH;

	m_yourCall = wxString((const char*)p, wxConvLocal, LONG_CALLSIGN_LENGTH);
	p += LONG_CALLSIGN_LENGTH;

	m_myCall1  = wxString((const char*)p,  wxConvLocal, LONG_CALLSIGN_LENGTH);
	p += LONG_CALLSIGN_LENGTH;

	m_myCall2  = wxString((const char*)p,  wxConvLocal, SHORT_CALLSIGN_LENGTH);

	m_time.SetToCurrent();

	// We have a checksum, check it if asked
	if (length >= RADIO_HEADER_LENGTH_BYTES && check) {
		CCCITTChecksumReverse cksum;

		cksum.update(data, RADIO_HEADER_LENGTH_BYTES - 2U);

		m_valid = cksum.check(data + RADIO_HEADER_LENGTH_BYTES - 2U);
	}
}

CHeaderData::CHeaderData(const wxString& myCall1,  const wxString& myCall2, const wxString& yourCall,
						 const wxString& rptCall1, const wxString& rptCall2, unsigned char flag1,
						 unsigned char flag2, unsigned char flag3) :
m_time(),
m_myCall1(myCall1),
m_myCall2(myCall2),
m_yourCall(yourCall),
m_rptCall1(rptCall1),
m_rptCall2(rptCall2),
m_flag1(flag1),
m_flag2(flag2),
m_flag3(flag3),
m_valid(true)
{
	m_time.SetToCurrent();

	m_myCall1.Append(wxT(' '),  LONG_CALLSIGN_LENGTH);
	m_myCall2.Append(wxT(' '),  SHORT_CALLSIGN_LENGTH);
	m_yourCall.Append(wxT(' '), LONG_CALLSIGN_LENGTH);
	m_rptCall1.Append(wxT(' '), LONG_CALLSIGN_LENGTH);
	m_rptCall2.Append(wxT(' '), LONG_CALLSIGN_LENGTH);

	m_myCall1.Truncate(LONG_CALLSIGN_LENGTH);
	m_myCall2.Truncate(SHORT_CALLSIGN_LENGTH);
	m_yourCall.Truncate(LONG_CALLSIGN_LENGTH);
	m_rptCall1.Truncate(LONG_CALLSIGN_LENGTH);
	m_rptCall2.Truncate(LONG_CALLSIGN_LENGTH);
}

CHeaderData::~CHeaderData()
{
}

wxDateTime CHeaderData::getTime() const
{
	return m_time;
}

wxString CHeaderData::getMyCall1() const
{
	return m_myCall1;
}

wxString CHeaderData::getMyCall2() const
{
	return m_myCall2;
}

wxString CHeaderData::getYourCall() const
{
	return m_yourCall;
}

wxString CHeaderData::getRptCall1() const
{
	return m_rptCall1;
}

wxString CHeaderData::getRptCall2() const
{
	return m_rptCall2;
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

void CHeaderData::setMyCall1(const wxString& callsign)
{
	m_myCall1 = callsign;
	m_myCall1.Append(wxT(' '), LONG_CALLSIGN_LENGTH);
	m_myCall1.Truncate(LONG_CALLSIGN_LENGTH);
}

void CHeaderData::setMyCall2(const wxString& callsign)
{
	m_myCall2 = callsign;
	m_myCall2.Append(wxT(' '), SHORT_CALLSIGN_LENGTH);
	m_myCall2.Truncate(SHORT_CALLSIGN_LENGTH);
}

void CHeaderData::setYourCall(const wxString& callsign)
{
	m_yourCall = callsign;
	m_yourCall.Append(wxT(' '), LONG_CALLSIGN_LENGTH);
	m_yourCall.Truncate(LONG_CALLSIGN_LENGTH);
}

void CHeaderData::setRptCall1(const wxString& callsign)
{
	m_rptCall1 = callsign;
	m_rptCall1.Append(wxT(' '), LONG_CALLSIGN_LENGTH);
	m_rptCall1.Truncate(LONG_CALLSIGN_LENGTH);
}

void CHeaderData::setRptCall2(const wxString& callsign)
{
	m_rptCall2 = callsign;
	m_rptCall2.Append(wxT(' '), LONG_CALLSIGN_LENGTH);
	m_rptCall2.Truncate(LONG_CALLSIGN_LENGTH);
}

bool CHeaderData::isAck() const
{
	return (m_flag1 & ACK_FLAG) == ACK_FLAG;
}

bool CHeaderData::isNoResponse() const
{
	return (m_flag1 & NO_RESPONSE) == NO_RESPONSE;
}

bool CHeaderData::isRelayUnavailable() const
{
	return (m_flag1 & RELAY_UNAVAILABLE) == RELAY_UNAVAILABLE;
}

void CHeaderData::setRepeaterMode(bool set)
{
	if (set)
		m_flag1 |= REPEATER_MASK;
	else
		m_flag1 &= ~REPEATER_MASK;
}

bool CHeaderData::isRepeaterMode() const
{
	return (m_flag1 & REPEATER_MASK) == REPEATER_MASK;
}

void CHeaderData::setDataPacket(bool set)
{
	if (set)
		m_flag1 |= DATA_MASK;
	else
		m_flag1 &= ~DATA_MASK;
}

bool CHeaderData::isDataPacket() const
{
	return (m_flag1 & DATA_MASK) == DATA_MASK;
}

void CHeaderData::setInterrupted(bool set)
{
	if (set)
		m_flag1 |= INTERRUPTED_MASK;
	else
		m_flag1 &= ~INTERRUPTED_MASK;
}

bool CHeaderData::isInterrupted() const
{
	return (m_flag1 & INTERRUPTED_MASK) == INTERRUPTED_MASK;
}

void CHeaderData::setControlSignal(bool set)
{
	if (set)
		m_flag1 |= CONTROL_SIGNAL_MASK;
	else
		m_flag1 &= ~CONTROL_SIGNAL_MASK;
}

bool CHeaderData::isControlSignal() const
{
	return (m_flag1 & CONTROL_SIGNAL_MASK) == CONTROL_SIGNAL_MASK;
}

void CHeaderData::setUrgent(bool set)
{
	if (set)
		m_flag1 |= URGENT_MASK;
	else
		m_flag1 &= ~URGENT_MASK;
}

bool CHeaderData::isUrgent() const
{
	return (m_flag1 & URGENT_MASK) == URGENT_MASK;
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

void CHeaderData::setRepeaterFlags(unsigned char set)
{
	m_flag1 &= ~REPEATER_CONTROL_MASK;
	m_flag1 |= set & REPEATER_CONTROL_MASK;
}

unsigned char CHeaderData::getRepeaterFlags() const
{
	return m_flag1 & REPEATER_CONTROL_MASK;
}

bool CHeaderData::isValid() const
{
	return m_valid;
}

void CHeaderData::reset()
{
	m_myCall1  = wxT("        ");
	m_myCall2  = wxT("    ");
	m_yourCall = wxT("CQCQCQ  ");
	m_rptCall1 = wxT("DIRECT  ");
	m_rptCall2 = wxT("DIRECT  ");

	m_flag1 = 0x00;
	m_flag2 = 0x00;
	m_flag3 = 0x00;
}

CHeaderData& CHeaderData::operator=(const CHeaderData& header)
{
	if (&header != this) {
		m_time     = header.m_time;
		m_myCall1  = header.m_myCall1;
		m_myCall2  = header.m_myCall2;
		m_yourCall = header.m_yourCall;
		m_rptCall1 = header.m_rptCall1;
		m_rptCall2 = header.m_rptCall2;
		m_flag1    = header.m_flag1;
		m_flag2    = header.m_flag2;
		m_flag3    = header.m_flag3;
		m_valid    = header.m_valid;
	}

	return *this;
}
