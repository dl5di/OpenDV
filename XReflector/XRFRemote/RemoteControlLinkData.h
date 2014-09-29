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

#ifndef	RemoteControlLinkData_H
#define	RemoteControlLinkData_H

#include "Defs.h"

#include <wx/wx.h>

class CRemoteControlLinkData {
public:
	CRemoteControlLinkData(const wxString& callsign, wxInt32 protocol, wxInt32 linked, wxInt32 direction, wxInt32 dongle);
	~CRemoteControlLinkData();

	wxString  getCallsign() const;
	PROTOCOL  getProtocol() const;
	bool      isLinked() const;
	DIRECTION getDirection() const;
	bool      isDongle() const;

private:
	wxString  m_callsign;
	PROTOCOL  m_protocol;
	bool      m_linked;
	DIRECTION m_direction;
	bool      m_dongle;
};

#endif
