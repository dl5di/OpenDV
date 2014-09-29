/*
 *   Copyright (C) 2011 by Jonathan Naylor G4KLX
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

#ifndef	RemoteProtocolHandler_H
#define	RemoteProtocolHandler_H

#include "RemoteStarNetGroup.h"
#include "RemoteRepeaterData.h"
#include "UDPReaderWriter.h"
#include "Defs.h"

#include <wx/wx.h>

enum RPH_TYPE {
	RPHT_NONE,
	RPHT_LOGIN,
	RPHT_HASH,
	RPHT_CALLSIGNS,
	RPHT_REPEATER,
	RPHT_STARNET,
	RPHT_LINK,
	RPHT_LINKSCR,
	RPHT_LOGOFF,
	RPHT_LOGOUT,
	RPHT_UNKNOWN
};

class CRemoteProtocolHandler {
public:
	CRemoteProtocolHandler(unsigned int port, const wxString& address = wxEmptyString);
	~CRemoteProtocolHandler();

	bool open();

	RPH_TYPE readType();

	bool     readHash(const wxString& password, wxUint32 random);
	wxString readRepeater();
	wxString readStarNetGroup();
	bool     readLink(wxString& callsign, RECONNECT& reconnect, wxString& reflector);
	bool     readLinkScr(wxString& callsign, RECONNECT& reconnect, wxString& reflector);
	bool     readLogoff(wxString& callsign, wxString& user);

	bool     sendACK();
	bool     sendNAK(const wxString& text);
	bool     sendRandom(wxUint32 random);
	bool     sendCallsigns(const wxArrayString& repeaters, const wxArrayString& starNets);
	bool     sendRepeater(const CRemoteRepeaterData& data);
	bool     sendStarNetGroup(const CRemoteStarNetGroup& data);

	void setLoggedIn(bool set);

	void close();

private:
	CUDPReaderWriter  m_socket;
	in_addr           m_address;
	unsigned int      m_port;
	bool              m_loggedIn;
	RPH_TYPE          m_type;
	unsigned char*    m_inBuffer;
	unsigned int      m_inLength;
	unsigned char*    m_outBuffer;
};

#endif
