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

#ifndef	RemoteControlRepeaterData_H
#define	RemoteControlRepeaterData_H

#include "RemoteControlLinkData.h"

#include <wx/wx.h>
#include <wx/dynarray.h>

WX_DECLARE_OBJARRAY(CRemoteControlLinkData, CRemoteLinkData_t);

class CRemoteControlRepeaterData {
public:
	CRemoteControlRepeaterData(const wxString& callsign, wxInt32 reconnect, const wxString& reflector);
	~CRemoteControlRepeaterData();

	void addLink(const wxString& callsign, wxInt32 protocol, wxInt32 linked, wxInt32 direction, wxInt32 dongle);

	wxString  getCallsign() const;
	RECONNECT getReconnect() const;
	wxString  getReflector() const;

	unsigned int getLinkCount() const;
	CRemoteControlLinkData& getLink(unsigned int n) const;

private:
	wxString          m_callsign;
	RECONNECT         m_reconnect;
	wxString          m_reflector;
	CRemoteLinkData_t m_links;
};

#endif
