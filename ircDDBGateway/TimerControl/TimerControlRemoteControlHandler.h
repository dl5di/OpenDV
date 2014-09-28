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

#ifndef	TimerControlRemoteControlHandler_H
#define	TimerControlRemoteControlHandler_H

#include "UDPReaderWriter.h"
#include "Defs.h"

#include <wx/wx.h>

enum TC_TYPE {
	TCT_NONE,
	TCT_ACK,
	TCT_NAK,
	TCT_RANDOM,
	TCT_CALLSIGNS
};

class CTimerControlRemoteControlHandler {
public:
	CTimerControlRemoteControlHandler(const wxString& address, unsigned int port);
	~CTimerControlRemoteControlHandler();

	bool open();

	TC_TYPE readType();

	wxString      readNAK();
	unsigned int  readRandom();
	wxArrayString readCallsigns();

	bool login();
	bool sendHash(const unsigned char* hash, unsigned int length);

	void setLoggedIn(bool set);

	bool getCallsigns();

	bool link(const wxString& callsign, RECONNECT reconnect, const wxString& reflector);

	bool logout();

	bool retry();

	void close();

private:
	CUDPReaderWriter  m_socket;
	in_addr           m_address;
	unsigned int      m_port;
	bool              m_loggedIn;
	unsigned int      m_retryCount;
	TC_TYPE           m_type;
	unsigned char*    m_inBuffer;
	unsigned int      m_inLength;
	unsigned char*    m_outBuffer;
	unsigned int      m_outLength;
};

#endif
