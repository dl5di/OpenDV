/*
 *   Copyright (C) 2013 by Jonathan Naylor G4KLX
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

#include "DStarDefines.h"
#include "CCSData.h"
#include "Utils.h"

CCCSData::CCCSData(const wxString& local, double latitude, double longitude, double frequency, double offset, const wxString& description1, const wxString& description2, const wxString& url, CC_TYPE type) :
m_local(local),
m_remote(),
m_latitude(latitude),
m_longitude(longitude),
m_frequency(frequency),
m_offset(offset),
m_description1(description1),
m_description2(description2),
m_url(url),
m_type(type),
m_yourAddress(),
m_yourPort(0U),
m_myPort(0U)
{
}

CCCSData::CCCSData(const wxString& local, const wxString& remote, CC_TYPE type) :
m_local(local),
m_remote(remote),
m_latitude(0.0),
m_longitude(0.0),
m_frequency(0.0),
m_offset(0.0),
m_description1(),
m_description2(),
m_url(),
m_type(type),
m_yourAddress(),
m_yourPort(0U),
m_myPort(0U)
{
}

CCCSData::CCCSData() :
m_local(),
m_remote(),
m_latitude(0.0),
m_longitude(0.0),
m_frequency(0.0),
m_offset(0.0),
m_description1(),
m_description2(),
m_url(),
m_type(),
m_yourAddress(),
m_yourPort(0U),
m_myPort(0U)
{
}

CCCSData::~CCCSData()
{
}

bool CCCSData::setCCSData(const unsigned char *data, unsigned int length, const in_addr& yourAddress, unsigned int yourPort, unsigned int myPort)
{
	wxASSERT(data != NULL);

	switch (length) {
		case 100U:
			m_remote = wxString((char*)(data + 0U), wxConvLocal, LONG_CALLSIGN_LENGTH);

			if (::memcmp(data + 8U, "0001", 4U) == 0) {
				m_type = CT_TERMINATE;
			} else {
				// CUtils::dump(wxT("Invalid CCS packet"), data, length);
				return false;
			}

			m_local = wxString((char*)(data + 12U), wxConvLocal, LONG_CALLSIGN_LENGTH);
			break;

		case 20U:
			if (::memcmp(data + 0U, "DTMF_CALL:", 10U) == 0) {
				m_type = CT_DTMFFOUND;
			} else {
				CUtils::dump(wxT("Invalid CCS packet"), data, length);
				return false;
			}

			m_remote = wxString((char*)(data + 10U), wxConvLocal, LONG_CALLSIGN_LENGTH);
			break;

		case 17U:
			if (::memcmp(data + 0U, "NODTMFCALL", 10U) == 0) {
				m_type = CT_DTMFNOTFOUND;
			} else {
				CUtils::dump(wxT("Invalid CCS packet"), data, length);
				return false;
			}
			break;

		default:
			CUtils::dump(wxT("Invalid CCS packet"), data, length);
			return false;
	}

	m_yourAddress = yourAddress;
	m_yourPort    = yourPort;
	m_myPort      = myPort;

	return true;
}

unsigned int CCCSData::getCCSData(unsigned char* data, unsigned int length) const
{
	wxASSERT(data != NULL);
	wxASSERT(length >= 133U);

	if (m_type == CT_TERMINATE) {
		::memset(data, ' ', 38U);

		for (unsigned int i = 0U; i < m_remote.Len() && i < LONG_CALLSIGN_LENGTH; i++)
			data[i + 0U] = m_remote.GetChar(i);

		::memcpy(data + 8U, "0001", 4U);

		for (unsigned int i = 0U; i < m_local.Len() && i < LONG_CALLSIGN_LENGTH; i++)
			data[i + 12U] = m_local.GetChar(i);

		return 38U;
	} else if (m_type == CT_INFO) {
		wxString buffer;
		buffer.Printf(wxT("IRPT%.7s %s%-10.4lf%-10.4lf%-10.4lf%-10.4lf%-20s%-20s%-40s"), m_local.Mid(0U, LONG_CALLSIGN_LENGTH - 1U).c_str(), m_local.Mid(LONG_CALLSIGN_LENGTH - 1U, 1U).c_str(), m_latitude, m_longitude, m_frequency, m_offset, m_description1.c_str(), m_description2.c_str(), m_url.c_str());

		for (unsigned int i = 0U; i < buffer.Len() && i < 133U; i++)
			data[i] = buffer.GetChar(i);

		return 133U;
	}

	return 0U;
}

wxString CCCSData::getLocal() const
{
	return m_local;
}

wxString CCCSData::getRemote() const
{
	return m_remote;
}

CC_TYPE CCCSData::getType() const
{
	return m_type;
}

void CCCSData::setDestination(const in_addr& address, unsigned int port)
{
	m_yourAddress = address;
	m_yourPort    = port;
}

in_addr CCCSData::getYourAddress() const
{
	return m_yourAddress;
}

unsigned int CCCSData::getYourPort() const
{
	return m_yourPort;
}

unsigned int CCCSData::getMyPort() const
{
	return m_myPort;
}
