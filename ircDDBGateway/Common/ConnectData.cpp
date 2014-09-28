/*
 *   Copyright (C) 2010,2012,2013 by Jonathan Naylor G4KLX
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

#include "ConnectData.h"

#include "DStarDefines.h"
#include "Version.h"
#include "Utils.h"

const wxChar* HTML = wxT("<table border=\"0\" width=\"95%%\"><tr><td width=\"4%%\"><img border=\"0\" src=%s></td><td width=\"96%%\"><font size=\"2\"><b>%s</b> ircDDB Gateway %s</font></td></tr></table>");

CConnectData::CConnectData(GATEWAY_TYPE gatewayType, const wxString& repeater, const wxString& reflector, CD_TYPE type, const in_addr& yourAddress, unsigned int yourPort, unsigned int myPort) :
m_gatewayType(gatewayType),
m_repeater(repeater),
m_reflector(reflector),
m_type(type),
m_locator(),
m_yourAddress(yourAddress),
m_yourPort(yourPort),
m_myPort(myPort)
{
	wxASSERT(yourPort > 0U);
	wxASSERT(!repeater.IsEmpty());
	wxASSERT(!reflector.IsEmpty());
}

CConnectData::CConnectData(const wxString& repeater, const wxString& reflector, CD_TYPE type, const in_addr& yourAddress, unsigned int yourPort, unsigned int myPort) :
m_gatewayType(GT_REPEATER),
m_repeater(repeater),
m_reflector(reflector),
m_type(type),
m_locator(),
m_yourAddress(yourAddress),
m_yourPort(yourPort),
m_myPort(myPort)
{
	wxASSERT(yourPort > 0U);
	wxASSERT(!repeater.IsEmpty());
	wxASSERT(!reflector.IsEmpty());
}

CConnectData::CConnectData(const wxString& repeater, CD_TYPE type, const in_addr& yourAddress, unsigned int yourPort, unsigned int myPort) :
m_gatewayType(GT_REPEATER),
m_repeater(repeater),
m_reflector(),
m_type(type),
m_locator(),
m_yourAddress(yourAddress),
m_yourPort(yourPort),
m_myPort(myPort)
{
	wxASSERT(yourPort > 0U);
	wxASSERT(!repeater.IsEmpty());
}

CConnectData::CConnectData(const wxString& repeater, const in_addr& yourAddress, unsigned int yourPort, unsigned int myPort) :
m_gatewayType(GT_REPEATER),
m_repeater(repeater),
m_reflector(),
m_type(CT_UNLINK),
m_locator(),
m_yourAddress(yourAddress),
m_yourPort(yourPort),
m_myPort(myPort)
{
	wxASSERT(yourPort > 0U);
	wxASSERT(!repeater.IsEmpty());
}

CConnectData::CConnectData(CD_TYPE type, const in_addr& yourAddress, unsigned int yourPort, unsigned int myPort) :
m_gatewayType(GT_REPEATER),
m_repeater(),
m_reflector(),
m_type(type),
m_locator(),
m_yourAddress(yourAddress),
m_yourPort(yourPort),
m_myPort(myPort)
{
	wxASSERT(yourPort > 0U);
}

CConnectData::CConnectData() :
m_gatewayType(GT_REPEATER),
m_repeater(wxT("        ")),
m_reflector(),
m_type(CT_LINK1),
m_locator(),
m_yourAddress(),
m_yourPort(0U),
m_myPort(0U)
{
}

CConnectData::~CConnectData()
{
}

bool CConnectData::setDExtraData(const unsigned char* data, unsigned int length, const in_addr& yourAddress, unsigned int yourPort, unsigned int myPort)
{
	wxASSERT(data != NULL);
	wxASSERT(length >= 11U);
	wxASSERT(yourPort > 0U);

	m_repeater = wxString((const char*)data, wxConvLocal, LONG_CALLSIGN_LENGTH);
	m_repeater.SetChar(LONG_CALLSIGN_LENGTH - 1U, data[LONG_CALLSIGN_LENGTH + 0U]);

	m_reflector = wxT("        ");
	m_reflector.SetChar(LONG_CALLSIGN_LENGTH - 1U, data[LONG_CALLSIGN_LENGTH + 1U]);

	switch (length) {
		case 11U:
			if (m_reflector.IsSameAs(wxT("        ")))
				m_type = CT_UNLINK;
			else
				m_type = CT_LINK1;
			break;

		case 14U:
			if (data[LONG_CALLSIGN_LENGTH + 2U] == 'A' &&
				data[LONG_CALLSIGN_LENGTH + 3U] == 'C' &&
				data[LONG_CALLSIGN_LENGTH + 4U] == 'K')
				m_type = CT_ACK;
			else if (data[LONG_CALLSIGN_LENGTH + 2U] == 'N' &&
				     data[LONG_CALLSIGN_LENGTH + 3U] == 'A' &&
					 data[LONG_CALLSIGN_LENGTH + 4U] == 'K')
				m_type = CT_NAK;
			else
				return false;

			break;

		default:
			return false;
	}

	m_yourAddress = yourAddress;
	m_yourPort    = yourPort;
	m_myPort      = myPort;

	return true;
}

bool CConnectData::setDCSData(const unsigned char* data, unsigned int length, const in_addr& yourAddress, unsigned int yourPort, unsigned int myPort)
{
	wxASSERT(data != NULL);
	wxASSERT(length >= 11U);
	wxASSERT(yourPort > 0U);

	m_repeater = wxString((const char*)data, wxConvLocal, LONG_CALLSIGN_LENGTH);
	m_repeater.SetChar(LONG_CALLSIGN_LENGTH - 1U, data[LONG_CALLSIGN_LENGTH + 0U]);

	switch (length) {
		case 519U:
			m_reflector = wxString((const char*)(data + LONG_CALLSIGN_LENGTH + 3U), wxConvLocal, LONG_CALLSIGN_LENGTH);
			m_reflector.SetChar(LONG_CALLSIGN_LENGTH - 1U, data[LONG_CALLSIGN_LENGTH + 1U]);
			m_type = CT_LINK1;
			break;

		case 19U:
			m_reflector = wxString((const char*)(data + LONG_CALLSIGN_LENGTH + 3U), wxConvLocal, LONG_CALLSIGN_LENGTH);
			m_reflector.SetChar(LONG_CALLSIGN_LENGTH - 1U, data[LONG_CALLSIGN_LENGTH + 1U]);
			m_type = CT_UNLINK;
			break;

		case 14U:
			if (data[LONG_CALLSIGN_LENGTH + 2U] == 'A' &&
				data[LONG_CALLSIGN_LENGTH + 3U] == 'C' &&
				data[LONG_CALLSIGN_LENGTH + 4U] == 'K')
				m_type = CT_ACK;
			else if (data[LONG_CALLSIGN_LENGTH + 2U] == 'N' &&
				     data[LONG_CALLSIGN_LENGTH + 3U] == 'A' &&
					 data[LONG_CALLSIGN_LENGTH + 4U] == 'K')
				m_type = CT_NAK;
			else
				return false;

			break;

		default:
			return false;
	}

	m_yourAddress = yourAddress;
	m_yourPort    = yourPort;
	m_myPort      = myPort;

	return true;
}

bool CConnectData::setCCSData(const unsigned char* data, unsigned int length, const in_addr& yourAddress, unsigned int yourPort, unsigned int myPort)
{
	wxASSERT(data != NULL);
	wxASSERT(length >= 14U);
	wxASSERT(yourPort > 0U);

	m_repeater = wxString((const char*)data, wxConvLocal, LONG_CALLSIGN_LENGTH);
	m_repeater.SetChar(LONG_CALLSIGN_LENGTH - 1U, data[LONG_CALLSIGN_LENGTH + 0U]);

	if (data[LONG_CALLSIGN_LENGTH + 2U] == 'A' &&
		data[LONG_CALLSIGN_LENGTH + 3U] == 'C' &&
		data[LONG_CALLSIGN_LENGTH + 4U] == 'K')
		m_type = CT_ACK;
	else if (data[LONG_CALLSIGN_LENGTH + 2U] == 'N' &&
		     data[LONG_CALLSIGN_LENGTH + 3U] == 'A' &&
			 data[LONG_CALLSIGN_LENGTH + 4U] == 'K')
		m_type = CT_NAK;
	else
		return false;

	m_yourAddress = yourAddress;
	m_yourPort    = yourPort;
	m_myPort      = myPort;

	return true;
}

bool CConnectData::setDPlusData(const unsigned char* data, unsigned int length, const in_addr& yourAddress, unsigned int yourPort, unsigned int myPort)
{
	wxASSERT(data != NULL);
	wxASSERT(length >= 5U);
	wxASSERT(yourPort > 0U);

	switch (length) {
		case 5U:
			switch (data[4U]) {
				case 0x01:
					m_type = CT_LINK1;
					break;
				case 0x00:
					m_type = CT_UNLINK;
					break;
			}
			break;

		case 8U: {
				wxString reply((const char*)(data + 4U), wxConvLocal, 4U);
				wxLogMessage(wxT("D-Plus reply is %.4s"), reply.c_str());

				if (::memcmp(data + 4U, "OKRW", 4U) == 0)
					m_type = CT_ACK;
				else
					m_type = CT_NAK;
			}
			break;

		case 28U:
			m_repeater = wxString((const char*)(data + 4U), wxConvLocal);
			m_type = CT_LINK2;
			break;

		default:
			return false;
	}

	m_yourAddress = yourAddress;
	m_yourPort    = yourPort;
	m_myPort      = myPort;

	return true;
}

unsigned int CConnectData::getDExtraData(unsigned char *data, unsigned int length) const
{
	wxASSERT(data != NULL);
	wxASSERT(length >= 11U);

	::memset(data, ' ', LONG_CALLSIGN_LENGTH);

	for (unsigned int i = 0U; i < m_repeater.Len() && i < (LONG_CALLSIGN_LENGTH - 1U); i++)
		data[i] = m_repeater.GetChar(i);

	data[LONG_CALLSIGN_LENGTH + 0U] = m_repeater.GetChar(LONG_CALLSIGN_LENGTH - 1U);

	switch (m_type) {
		case CT_LINK1:
		case CT_LINK2:
			data[LONG_CALLSIGN_LENGTH + 1U] = m_reflector.GetChar(LONG_CALLSIGN_LENGTH - 1U);
			data[LONG_CALLSIGN_LENGTH + 2U] = 0x00;
			return 11U;

		case CT_UNLINK:
			data[LONG_CALLSIGN_LENGTH + 1U] = ' ';
			data[LONG_CALLSIGN_LENGTH + 2U] = 0x00;
			return 11U;

		case CT_ACK:
			data[LONG_CALLSIGN_LENGTH + 1U] = m_reflector.GetChar(LONG_CALLSIGN_LENGTH - 1U);
			data[LONG_CALLSIGN_LENGTH + 2U] = 'A';
			data[LONG_CALLSIGN_LENGTH + 3U] = 'C';
			data[LONG_CALLSIGN_LENGTH + 4U] = 'K';
			data[LONG_CALLSIGN_LENGTH + 5U] = 0x00;
			return 14U;

		case CT_NAK:
			data[LONG_CALLSIGN_LENGTH + 1U] = m_reflector.GetChar(LONG_CALLSIGN_LENGTH - 1U);
			data[LONG_CALLSIGN_LENGTH + 2U] = 'N';
			data[LONG_CALLSIGN_LENGTH + 3U] = 'A';
			data[LONG_CALLSIGN_LENGTH + 4U] = 'K';
			data[LONG_CALLSIGN_LENGTH + 5U] = 0x00;
			return 14U;

		default:
			return 0U;
	}
}

unsigned int CConnectData::getDCSData(unsigned char *data, unsigned int length) const
{
	wxASSERT(data != NULL);
	wxASSERT(length >= 519U);

	::memset(data, ' ', LONG_CALLSIGN_LENGTH);

	for (unsigned int i = 0U; i < m_repeater.Len() && i < (LONG_CALLSIGN_LENGTH - 1U); i++)
		data[i] = m_repeater.GetChar(i);

	data[LONG_CALLSIGN_LENGTH + 0U] = m_repeater.GetChar(LONG_CALLSIGN_LENGTH - 1U);

	switch (m_type) {
		case CT_LINK1:
		case CT_LINK2: {
				data[LONG_CALLSIGN_LENGTH + 1U] = m_reflector.GetChar(LONG_CALLSIGN_LENGTH - 1U);
				data[LONG_CALLSIGN_LENGTH + 2U] = 0x00U;
				::memset(data + 11U, ' ', LONG_CALLSIGN_LENGTH);
				for (unsigned int i = 0U; i < m_reflector.Len() && i < (LONG_CALLSIGN_LENGTH - 1U); i++)
					data[i + 11U] = m_reflector.GetChar(i);

				wxString html;
				switch (m_gatewayType) {
					case GT_HOTSPOT:
						html.Printf(HTML, wxT("hotspot.jpg"), wxT("HOTSPOT"), VERSION.c_str());
						break;
					case GT_DONGLE:
						html.Printf(HTML, wxT("dongle.jpg"), wxT("DONGLE"), VERSION.c_str());
						break;
					case GT_STARNET:
						html.Printf(HTML, wxT("hf.jpg"), wxT("STARNET"), VERSION.c_str());
						break;
					default:
						html.Printf(HTML, wxT("hf.jpg"), wxT("REPEATER"), VERSION.c_str());
						break;
				}

				::memset(data + 19U, 0x00U, 500U);
				for (unsigned int i = 0U; i < html.Len(); i++)
					data[i + 19U] = html.GetChar(i);
			}
			return 519U;

		case CT_UNLINK:
			data[LONG_CALLSIGN_LENGTH + 1U] = 0x20U;
			data[LONG_CALLSIGN_LENGTH + 2U] = 0x00U;
			::memset(data + 11U, ' ', LONG_CALLSIGN_LENGTH);
			for (unsigned int i = 0U; i < m_reflector.Len() && i < (LONG_CALLSIGN_LENGTH - 1U); i++)
				data[i + 11U] = m_reflector.GetChar(i);
			return 19U;

		case CT_ACK:
			data[LONG_CALLSIGN_LENGTH + 1U] = m_reflector.GetChar(LONG_CALLSIGN_LENGTH - 1U);
			data[LONG_CALLSIGN_LENGTH + 2U] = 'A';
			data[LONG_CALLSIGN_LENGTH + 3U] = 'C';
			data[LONG_CALLSIGN_LENGTH + 4U] = 'K';
			data[LONG_CALLSIGN_LENGTH + 5U] = 0x00;
			return 14U;

		case CT_NAK:
			data[LONG_CALLSIGN_LENGTH + 1U] = 0x20U;
			data[LONG_CALLSIGN_LENGTH + 2U] = 'N';
			data[LONG_CALLSIGN_LENGTH + 3U] = 'A';
			data[LONG_CALLSIGN_LENGTH + 4U] = 'K';
			data[LONG_CALLSIGN_LENGTH + 5U] = 0x00;
			return 14U;

		default:
			return 0U;
	}
}

unsigned int CConnectData::getCCSData(unsigned char *data, unsigned int length) const
{
	wxASSERT(data != NULL);
	wxASSERT(length >= 39U);

	::memset(data, ' ', 39U);

	for (unsigned int i = 0U; i < m_repeater.Len() && i < (LONG_CALLSIGN_LENGTH - 1U); i++)
		data[i] = m_repeater.GetChar(i);

	data[LONG_CALLSIGN_LENGTH + 0U] = m_repeater.GetChar(LONG_CALLSIGN_LENGTH - 1U);

	switch (m_type) {
		case CT_LINK1:
		case CT_LINK2: {
				data[9U]  = 0x41U;
				data[10U] = '@';

				for (unsigned int i = 0U; i < m_locator.Len(); i++)
					data[11U + i] = m_locator.GetChar(i);

				data[17U] = 0x20U;
				data[18U] = '@';

				wxString text;
				text.Printf(wxT("ircDDB_GW-%s"), VERSION.Left(8U).c_str());

				for (unsigned int i = 0U; i < text.Len(); i++)
					data[19U + i] = text.GetChar(i);
			}
			return 39U;

		case CT_UNLINK:
			return 19U;

		default:
			return 0U;
	}
}

unsigned int CConnectData::getDPlusData(unsigned char *data, unsigned int length) const
{
	wxASSERT(data != NULL);
	wxASSERT(length >= 28U);

	switch (m_type) {
		case CT_LINK1:
			data[0U] = 0x05;
			data[1U] = 0x00;
			data[2U] = 0x18;
			data[3U] = 0x00;
			data[4U] = 0x01;
			return 5U;

		case CT_LINK2: {
				data[0U]  = 0x1C;
				data[1U]  = 0xC0;
				data[2U]  = 0x04;
				data[3U]  = 0x00;

				for (unsigned int i = 4U; i < 20U; i++)
					data[i] = 0x00;

				wxString callsign = m_repeater;
				callsign.Trim();

				for (unsigned int i = 0U; i < callsign.Len(); i++)
					data[i + 4U] = callsign.GetChar(i);

				data[20U] = 'D';
				data[21U] = 'V';
				data[22U] = '0';
				data[23U] = '1';
				data[24U] = '9';
				data[25U] = '9';
				data[26U] = '9';
				data[27U] = '9';
			}
			return 28U;

		case CT_UNLINK:
			data[0U] = 0x05;
			data[1U] = 0x00;
			data[2U] = 0x18;
			data[3U] = 0x00;
			data[4U] = 0x00;
			return 5U;

		case CT_ACK:
			data[0U] = 0x08;
			data[1U] = 0xC0;
			data[2U] = 0x04;
			data[3U] = 0x00;
			data[4U] = 'O';
			data[5U] = 'K';
			data[6U] = 'R';
			data[7U] = 'W';
			return 8U;

		case CT_NAK:
			data[0U] = 0x08;
			data[1U] = 0xC0;
			data[2U] = 0x04;
			data[3U] = 0x00;
			data[4U] = 'B';
			data[5U] = 'U';
			data[6U] = 'S';
			data[7U] = 'Y';
			return 8U;

		default:
			return 0U;
	}
}

in_addr CConnectData::getYourAddress() const
{
	return m_yourAddress;
}

unsigned int CConnectData::getYourPort() const
{
	return m_yourPort;
}

unsigned int CConnectData::getMyPort() const
{
	return m_myPort;
}

wxString CConnectData::getRepeater() const
{
	return m_repeater;
}

wxString CConnectData::getReflector() const
{
	return m_reflector;
}

CD_TYPE CConnectData::getType() const
{
	return m_type;
}

void CConnectData::setLocator(const wxString& locator)
{
	m_locator = locator;
}
