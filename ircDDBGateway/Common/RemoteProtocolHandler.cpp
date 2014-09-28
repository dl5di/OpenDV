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

#include "RemoteProtocolHandler.h"
#include "DStarDefines.h"
#include "SHA256.h"
#include "Utils.h"

const unsigned int BUFFER_LENGTH = 2000U;

CRemoteProtocolHandler::CRemoteProtocolHandler(unsigned int port, const wxString& address) :
m_socket(address, port),
m_address(),
m_port(0U),
m_loggedIn(false),
m_type(RPHT_NONE),
m_inBuffer(NULL),
m_inLength(0U),
m_outBuffer(NULL)
{
	wxASSERT(port > 0U);

	m_inBuffer  = new unsigned char[BUFFER_LENGTH];
	m_outBuffer = new unsigned char[BUFFER_LENGTH];
}

CRemoteProtocolHandler::~CRemoteProtocolHandler()
{
	delete[] m_outBuffer;
	delete[] m_inBuffer;
}

bool CRemoteProtocolHandler::open()
{
	return m_socket.open();
}

RPH_TYPE CRemoteProtocolHandler::readType()
{
	m_type = RPHT_NONE;

	in_addr address;
	unsigned int port;

	int length = m_socket.read(m_inBuffer, BUFFER_LENGTH, address, port);
	if (length <= 0)
		return m_type;

	// CUtils::dump(wxT("Incoming"), m_inBuffer, length);

	if (::memcmp(m_inBuffer, "LIN", 3U) == 0) {
		m_loggedIn = false;
		m_address  = address;
		m_port     = port;
		m_type = RPHT_LOGIN;
		return m_type;
	}

	if (address.s_addr == inet_addr("127.0.0.1")) {
		if (::memcmp(m_inBuffer, "LKS", 3U) == 0) {
			m_inLength = length;
			m_type = RPHT_LINKSCR;
			return m_type;
		}
	}

	if (m_loggedIn) {
		if (address.s_addr != m_address.s_addr || port != m_port) {
			sendNAK(wxT("You are not logged in"));
			return m_type;
		}
	}

	m_inLength = length;

	if (::memcmp(m_inBuffer, "SHA", 3U) == 0) {
		if (m_loggedIn) {
			sendNAK(wxT("Someone is already logged in"));
			return m_type;
		}
		m_type = RPHT_HASH;
		return m_type;
	} else if (::memcmp(m_inBuffer, "GCS", 3U) == 0) {
		if (!m_loggedIn) {
			sendNAK(wxT("You are not logged in"));
			return m_type;
		}
		m_type = RPHT_CALLSIGNS;
		return m_type;
	} else if (::memcmp(m_inBuffer, "GRP", 3U) == 0) {
		if (!m_loggedIn) {
			sendNAK(wxT("You are not logged in"));
			return m_type;
		}
		m_type = RPHT_REPEATER;
		return m_type;
	} else if (::memcmp(m_inBuffer, "GSN", 3U) == 0) {
		if (!m_loggedIn) {
			sendNAK(wxT("You are not logged in"));
			return m_type;
		}
		m_type = RPHT_STARNET;
		return m_type;
	} else if (::memcmp(m_inBuffer, "LNK", 3U) == 0) {
		if (!m_loggedIn) {
			sendNAK(wxT("You are not logged in"));
			return m_type;
		}
		m_type = RPHT_LINK;
		return m_type;
	} else if (::memcmp(m_inBuffer, "UNL", 3U) == 0) {
		if (!m_loggedIn) {
			sendNAK(wxT("You are not logged in"));
			return m_type;
		}
		m_type = RPHT_UNLINK;
		return m_type;
	} else if (::memcmp(m_inBuffer, "LGO", 3U) == 0) {
		if (!m_loggedIn) {
			sendNAK(wxT("You are not logged in"));
			return m_type;
		}
		m_type = RPHT_LOGOFF;
		return m_type;
	} else if (::memcmp(m_inBuffer, "LOG", 3U) == 0) {
		if (!m_loggedIn)
			return m_type;
		m_type = RPHT_LOGOUT;
		return m_type;
	} else {
		if (!m_loggedIn) {
			sendNAK(wxT("You are not logged in"));
			return m_type;
		}
		m_type = RPHT_UNKNOWN;
		return m_type;
	}
}

bool CRemoteProtocolHandler::readHash(const wxString& password, wxUint32 random)
{
	if (m_type != RPHT_HASH)
		return false;

	unsigned char* hash = m_inBuffer + 3U;

	unsigned int len = password.Len() + sizeof(wxUint32);
	unsigned char*  in = new unsigned char[len];
	unsigned char* out = new unsigned char[32U];

	::memcpy(in, &random, sizeof(wxUint32));
	for (unsigned int i = 0U; i < password.Len(); i++)
		in[i + sizeof(unsigned int)] = password.GetChar(i);

	CSHA256 sha256;
	sha256.buffer(in, len, out);

	bool res = ::memcmp(out, hash, 32U) == 0;

	delete[] in;
	delete[] out;

	return res;
}

wxString CRemoteProtocolHandler::readRepeater()
{
	if (m_type != RPHT_REPEATER)
		return wxEmptyString;

	wxString callsign((char*)(m_inBuffer + 3U), wxConvLocal, LONG_CALLSIGN_LENGTH);

	return callsign;
}

wxString CRemoteProtocolHandler::readStarNetGroup()
{
	if (m_type != RPHT_STARNET)
		return wxEmptyString;

	wxString callsign((char*)(m_inBuffer + 3U), wxConvLocal, LONG_CALLSIGN_LENGTH);

	return callsign;
}

bool CRemoteProtocolHandler::readLogoff(wxString& callsign, wxString& user)
{
	if (m_type != RPHT_LOGOFF)
		return false;

	callsign = wxString((char*)(m_inBuffer + 3U), wxConvLocal, LONG_CALLSIGN_LENGTH);
	user = wxString((char*)(m_inBuffer + 3U + LONG_CALLSIGN_LENGTH), wxConvLocal, LONG_CALLSIGN_LENGTH);

	return true;
}


bool CRemoteProtocolHandler::readLink(wxString& callsign, RECONNECT& reconnect, wxString& reflector)
{
	if (m_type != RPHT_LINK)
		return false;

	callsign = wxString((char*)(m_inBuffer + 3U), wxConvLocal, LONG_CALLSIGN_LENGTH);

	wxInt32 temp;
	::memcpy(&temp, m_inBuffer + 3U + LONG_CALLSIGN_LENGTH, sizeof(wxInt32));
	reconnect = RECONNECT(wxINT32_SWAP_ON_BE(temp));

	reflector = wxString((char*)(m_inBuffer + 3U + LONG_CALLSIGN_LENGTH + sizeof(wxInt32)), wxConvLocal, LONG_CALLSIGN_LENGTH);

	if (reflector.IsSameAs(wxT("        ")))
		reflector.Clear();

	return true;
}

bool CRemoteProtocolHandler::readUnlink(wxString& callsign, PROTOCOL& protocol, wxString& reflector)
{
	if (m_type != RPHT_UNLINK)
		return false;

	callsign = wxString((char*)(m_inBuffer + 3U), wxConvLocal, LONG_CALLSIGN_LENGTH);

	wxInt32 temp;
	::memcpy(&temp, m_inBuffer + 3U + LONG_CALLSIGN_LENGTH, sizeof(wxInt32));
	protocol = PROTOCOL(wxINT32_SWAP_ON_BE(temp));

	reflector = wxString((char*)(m_inBuffer + 3U + LONG_CALLSIGN_LENGTH + sizeof(wxInt32)), wxConvLocal, LONG_CALLSIGN_LENGTH);

	return true;
}

bool CRemoteProtocolHandler::readLinkScr(wxString& callsign, RECONNECT& reconnect, wxString& reflector)
{
	if (m_type != RPHT_LINKSCR)
		return false;

	callsign = wxString((char*)(m_inBuffer + 3U), wxConvLocal, LONG_CALLSIGN_LENGTH);

	reflector = wxString((char*)(m_inBuffer + 3U + LONG_CALLSIGN_LENGTH), wxConvLocal, LONG_CALLSIGN_LENGTH);

	wxString rec = wxString((char*)(m_inBuffer + 3U + 2U * LONG_CALLSIGN_LENGTH), wxConvLocal, 1U);

	unsigned long val;
	rec.ToULong(&val);

	reconnect = RECONNECT(val);

	if (reflector.IsSameAs(wxT("        ")))
		reflector.Clear();

	return true;
}

bool CRemoteProtocolHandler::sendCallsigns(const wxArrayString& repeaters, const wxArrayString& starNets)
{
	unsigned char* p = m_outBuffer;

	::memcpy(p, "CAL", 3U);
	p += 3U;

	for (unsigned int n = 0U; n < repeaters.GetCount(); n++) {
		*p++ = 'R';

		::memset(p, ' ' , LONG_CALLSIGN_LENGTH);
		for (unsigned int i = 0U; i < repeaters.Item(n).Len(); i++)
			p[i] = repeaters.Item(n).GetChar(i);
		p += LONG_CALLSIGN_LENGTH;
	}

	for (unsigned int n = 0U; n < starNets.GetCount(); n++) {
		*p++ = 'S';

		::memset(p, ' ' , LONG_CALLSIGN_LENGTH);
		for (unsigned int i = 0U; i < starNets.Item(n).Len(); i++)
			p[i] = starNets.Item(n).GetChar(i);
		p += LONG_CALLSIGN_LENGTH;
	}

	// CUtils::dump(wxT("Outgoing"), m_outBuffer, p - m_outBuffer);

	return m_socket.write(m_outBuffer, p - m_outBuffer, m_address, m_port);
}

bool CRemoteProtocolHandler::sendRepeater(const CRemoteRepeaterData& data)
{
	unsigned char* p = m_outBuffer;

	::memcpy(p, "RPT", 3U);
	p += 3U;

	::memset(p, ' ', LONG_CALLSIGN_LENGTH);
	for (unsigned int i = 0U; i < data.getCallsign().Len(); i++)
		p[i] = data.getCallsign().GetChar(i);
	p += LONG_CALLSIGN_LENGTH;

	wxInt32 reconnect = wxINT32_SWAP_ON_BE(data.getReconnect());
	::memcpy(p, &reconnect, sizeof(wxInt32));
	p += sizeof(wxInt32);

	::memset(p, ' ', LONG_CALLSIGN_LENGTH);
	for (unsigned int i = 0U; i < data.getReflector().Len(); i++)
		p[i] = data.getReflector().GetChar(i);
	p += LONG_CALLSIGN_LENGTH;

	for (unsigned int n = 0U; n < data.getLinkCount(); n++) {
		CRemoteLinkData& link = data.getLink(n);

		::memset(p, ' ', LONG_CALLSIGN_LENGTH);
		for (unsigned int i = 0U; i < link.getCallsign().Len(); i++)
			p[i] = link.getCallsign().GetChar(i);
		p += LONG_CALLSIGN_LENGTH;

		wxInt32 protocol = wxINT32_SWAP_ON_BE(link.getProtocol());
		::memcpy(p, &protocol, sizeof(wxInt32));
		p += sizeof(wxInt32);

		wxInt32 linked = wxINT32_SWAP_ON_BE(link.isLinked());
		::memcpy(p, &linked, sizeof(wxInt32));
		p += sizeof(wxInt32);

		wxInt32 direction = wxINT32_SWAP_ON_BE(link.getDirection());
		::memcpy(p, &direction, sizeof(wxInt32));
		p += sizeof(wxInt32);

		wxInt32 dongle = wxINT32_SWAP_ON_BE(link.isDongle());
		::memcpy(p, &dongle, sizeof(wxInt32));
		p += sizeof(wxInt32);
	}

	// CUtils::dump(wxT("Outgoing"), m_outBuffer, p - m_outBuffer);

	return m_socket.write(m_outBuffer, p - m_outBuffer, m_address, m_port);
}

bool CRemoteProtocolHandler::sendStarNetGroup(const CRemoteStarNetGroup& data)
{
	unsigned char* p = m_outBuffer;

	::memcpy(p, "SNT", 3U);
	p += 3U;

	::memset(p, ' ', LONG_CALLSIGN_LENGTH);
	for (unsigned int i = 0U; i < data.getCallsign().Len(); i++)
		p[i] = data.getCallsign().GetChar(i);
	p += LONG_CALLSIGN_LENGTH;

	::memset(p, ' ', LONG_CALLSIGN_LENGTH);
	for (unsigned int i = 0U; i < data.getLogoff().Len(); i++)
		p[i] = data.getLogoff().GetChar(i);
	p += LONG_CALLSIGN_LENGTH;

	wxUint32 timer = wxUINT32_SWAP_ON_BE(data.getTimer());
	::memcpy(p, &timer, sizeof(wxUint32));
	p += sizeof(wxUint32);

	wxUint32 timeout = wxUINT32_SWAP_ON_BE(data.getTimeout());
	::memcpy(p, &timeout, sizeof(wxUint32));
	p += sizeof(wxUint32);

	for (unsigned int n = 0U; n < data.getUserCount(); n++) {
		CRemoteStarNetUser& user = data.getUser(n);

		::memset(p, ' ', LONG_CALLSIGN_LENGTH);
		for (unsigned int i = 0U; i < user.getCallsign().Len(); i++)
			p[i] = user.getCallsign().GetChar(i);
		p += LONG_CALLSIGN_LENGTH;

		wxUint32 timer = wxUINT32_SWAP_ON_BE(user.getTimer());
		::memcpy(p, &timer, sizeof(wxUint32));
		p += sizeof(wxUint32);

		wxUint32 timeout = wxUINT32_SWAP_ON_BE(user.getTimeout());
		::memcpy(p, &timeout, sizeof(wxUint32));
		p += sizeof(wxUint32);
	}

	// CUtils::dump(wxT("Outgoing"), m_outBuffer, p - m_outBuffer);

	return m_socket.write(m_outBuffer, p - m_outBuffer, m_address, m_port);
}

void CRemoteProtocolHandler::setLoggedIn(bool set)
{
	m_loggedIn = set;
}

void CRemoteProtocolHandler::close()
{
	m_socket.close();
}

bool CRemoteProtocolHandler::sendACK()
{
	::memcpy(m_outBuffer + 0U, "ACK", 3U);

	// CUtils::dump(wxT("Outgoing"), m_outBuffer, 3U);

	return m_socket.write(m_outBuffer, 3U, m_address, m_port);
}

bool CRemoteProtocolHandler::sendNAK(const wxString& text)
{
	::memcpy(m_outBuffer + 0U, "NAK", 3U);

	::memset(m_outBuffer + 3U, 0x00U, text.Len() + 1U);

	for (unsigned int i = 0U; i < text.Len(); i++)
		m_outBuffer[i + 3U] = text.GetChar(i);

	// CUtils::dump(wxT("Outgoing"), m_outBuffer, 3U + text.Len() + 1U);

	return m_socket.write(m_outBuffer, 3U + text.Len() + 1U, m_address, m_port);
}

bool CRemoteProtocolHandler::sendRandom(wxUint32 random)
{
	::memcpy(m_outBuffer + 0U, "RND", 3U);

	wxUint32 temp = wxUINT32_SWAP_ON_BE(random);
	::memcpy(m_outBuffer + 3U, &temp, sizeof(wxUint32));

	// CUtils::dump(wxT("Outgoing"), m_outBuffer, 3U + sizeof(wxUint32));

	return m_socket.write(m_outBuffer, 3U + sizeof(wxUint32), m_address, m_port);
}
