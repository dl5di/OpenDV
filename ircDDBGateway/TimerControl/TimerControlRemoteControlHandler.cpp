/*
 *   Copyright (C) 2011,2013 by Jonathan Naylor G4KLX
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

#include "TimerControlRemoteControlHandler.h"
#include "DStarDefines.h"

const unsigned int BUFFER_LENGTH = 2000U;

const unsigned int MAX_RETRIES = 3U;

CTimerControlRemoteControlHandler::CTimerControlRemoteControlHandler(const wxString& address, unsigned int port) :
m_socket(wxEmptyString, 0U),
m_address(),
m_port(port),
m_loggedIn(false),
m_retryCount(0U),
m_type(TCT_NONE),
m_inBuffer(NULL),
m_inLength(0U),
m_outBuffer(NULL),
m_outLength(0U)
{
	wxASSERT(!address.IsEmpty());
	wxASSERT(port > 0U);

	m_address = CUDPReaderWriter::lookup(address);

	m_inBuffer  = new unsigned char[BUFFER_LENGTH];
	m_outBuffer = new unsigned char[BUFFER_LENGTH];
}

CTimerControlRemoteControlHandler::~CTimerControlRemoteControlHandler()
{
	delete[] m_inBuffer;
	delete[] m_outBuffer;
}

bool CTimerControlRemoteControlHandler::open()
{
	return m_socket.open();
}

TC_TYPE CTimerControlRemoteControlHandler::readType()
{
	m_type = TCT_NONE;

	in_addr address;
	unsigned int port;

	int length = m_socket.read(m_inBuffer, BUFFER_LENGTH, address, port);
	if (length <= 0)
		return m_type;

	m_inLength = length;

	if (::memcmp(m_inBuffer, "ACK", 3U) == 0) {
		m_retryCount = 0U;
		m_type = TCT_ACK;
		return m_type;
	} else if (::memcmp(m_inBuffer, "NAK", 3U) == 0) {
		m_retryCount = 0U;
		m_type = TCT_NAK;
		return m_type;
	} else if (::memcmp(m_inBuffer, "RND", 3U) == 0) {
		m_retryCount = 0U;
		m_type = TCT_RANDOM;
		return m_type;
	} else if (::memcmp(m_inBuffer, "CAL", 3U) == 0) {
		m_retryCount = 0U;
		m_type = TCT_CALLSIGNS;
		return m_type;
	}

	return m_type;
}

wxString CTimerControlRemoteControlHandler::readNAK()
{
	if (m_type != TCT_NAK)
		return wxEmptyString;

	wxString text((char*)(m_inBuffer + 3U), wxConvLocal);

	return text;
}

unsigned int CTimerControlRemoteControlHandler::readRandom()
{
	if (m_type != TCT_RANDOM)
		return 0U;

	wxUint32 random;
	::memcpy(&random, m_inBuffer + 3U, sizeof(wxUint32));

	return wxUINT32_SWAP_ON_BE(random);
}

wxArrayString CTimerControlRemoteControlHandler::readCallsigns()
{
	wxArrayString data;

	if (m_type != TCT_CALLSIGNS)
		return data;

	unsigned char* p = m_inBuffer + 3U;
	unsigned int pos = 3U;

	while (pos < m_inLength) {
		unsigned char type = *p;
		pos += 1U;
		p += 1U;

		wxString callsign((char*)p, wxConvLocal, LONG_CALLSIGN_LENGTH);
		pos += LONG_CALLSIGN_LENGTH;
		p += LONG_CALLSIGN_LENGTH;

		if (type == 'R')
			data.Add(callsign);
	}

	return data;
}

bool CTimerControlRemoteControlHandler::login()
{
	if (m_loggedIn)
		return false;

	if (m_address.s_addr == INADDR_NONE)
		return false;

	::memcpy(m_outBuffer, "LIN", 3U);
	m_outLength = 3U;

	bool ret = m_socket.write(m_outBuffer, m_outLength, m_address, m_port);
	if (!ret) {
		m_retryCount = 0U;
		return false;
	} else {
		m_retryCount = 1U;
		return true;
	}
}

void CTimerControlRemoteControlHandler::setLoggedIn(bool set)
{
	m_loggedIn = set;
}

bool CTimerControlRemoteControlHandler::getCallsigns()
{
	if (!m_loggedIn || m_retryCount > 0U)
		return false;

	::memcpy(m_outBuffer, "GCS", 3U);
	m_outLength = 3U;

	bool ret = m_socket.write(m_outBuffer, m_outLength, m_address, m_port);
	if (!ret) {
		m_retryCount = 0U;
		return false;
	} else {
		m_retryCount = 1U;
		return true;
	}
}

bool CTimerControlRemoteControlHandler::sendHash(const unsigned char* hash, unsigned int length)
{
	wxASSERT(hash != NULL);
	wxASSERT(length > 0U);

	if (m_loggedIn || m_retryCount > 0U)
		return false;

	unsigned char* p = m_outBuffer;
	m_outLength = 0U;

	::memcpy(p, "SHA", 3U);
	m_outLength += 3U;
	p += 3U;

	::memcpy(p, hash, length);
	m_outLength += length;
	p += length;

	bool ret = m_socket.write(m_outBuffer, m_outLength, m_address, m_port);
	if (!ret) {
		m_retryCount = 0U;
		return false;
	} else {
		m_retryCount = 1U;
		return true;
	}
}

bool CTimerControlRemoteControlHandler::link(const wxString& callsign, RECONNECT reconnect, const wxString& reflector)
{
	wxASSERT(!callsign.IsEmpty());

	if (!m_loggedIn || m_retryCount > 0U)
		return false;

	unsigned char* p = m_outBuffer;
	m_outLength = 0U;

	::memcpy(p, "LNK", 3U);
	m_outLength += 3U;
	p += 3U;

	::memset(p, ' ', LONG_CALLSIGN_LENGTH);
	for (unsigned int i = 0U; i < callsign.Len(); i++)
		p[i] = callsign.GetChar(i);

	m_outLength += LONG_CALLSIGN_LENGTH;
	p += LONG_CALLSIGN_LENGTH;

	wxInt32 temp1 = wxInt32(reconnect);
	wxInt32 temp2 = wxINT32_SWAP_ON_BE(temp1);
	::memcpy(p, &temp2, sizeof(wxInt32));
	m_outLength += sizeof(wxInt32);
	p += sizeof(wxInt32);

	::memset(p, ' ', LONG_CALLSIGN_LENGTH);
	for (unsigned int i = 0U; i < reflector.Len(); i++)
		p[i] = reflector.GetChar(i);

	m_outLength += LONG_CALLSIGN_LENGTH;
	p += LONG_CALLSIGN_LENGTH;

	bool ret = m_socket.write(m_outBuffer, m_outLength, m_address, m_port);
	if (!ret) {
		m_retryCount = 0U;
		return false;
	} else {
		m_retryCount = 1U;
		return true;
	}
}

bool CTimerControlRemoteControlHandler::logout()
{
	if (!m_loggedIn || m_retryCount > 0U)
		return false;

	::memcpy(m_outBuffer, "LOG", 3U);
	m_outLength = 3U;

	for (unsigned int i = 0U; i < 5U; i++) {
		bool ret = m_socket.write(m_outBuffer, m_outLength, m_address, m_port);
		if (!ret) {
			m_retryCount = 0U;
			return false;
		}
	}

	m_retryCount = 1U;

	return true;
}

bool CTimerControlRemoteControlHandler::retry()
{
	if (m_retryCount > 0U) {
		m_retryCount++;
		if (m_retryCount >= MAX_RETRIES) {
			m_retryCount = 0U;
			return false;
		}

		m_socket.write(m_outBuffer, m_outLength, m_address, m_port);
	}

	return true;
}

void CTimerControlRemoteControlHandler::close()
{
	m_socket.close();
}
