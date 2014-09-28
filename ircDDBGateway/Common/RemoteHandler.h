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

#ifndef	RemoteHandler_H
#define	RemoteHandler_H

#include "RemoteProtocolHandler.h"
#include "Timer.h"

#include <wx/wx.h>

class CRemoteHandler {
public:
	CRemoteHandler(const wxString& password, unsigned int port, const wxString& address = wxEmptyString);
	~CRemoteHandler();

	bool open();

	void process();

	void close();

private:
	wxString               m_password;
	CRemoteProtocolHandler m_handler;
	unsigned int           m_random;

	void sendCallsigns();
	void sendRepeater(const wxString& callsign);
	void sendStarNetGroup(const wxString& callsign);
	void link(const wxString& callsign, RECONNECT reconnect, const wxString& reflector, bool respond);
	void unlink(const wxString& callsign, PROTOCOL protocol, const wxString& reflector);
	void logoff(const wxString& callsign, const wxString& user);
};

#endif
