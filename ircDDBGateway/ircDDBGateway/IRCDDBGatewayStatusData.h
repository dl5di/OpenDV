/*
 *   Copyright (C) 2010,2011,2013 by Jonathan Naylor G4KLX
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

#ifndef	IRCDDBGatewayStatusData_H
#define	IRCDDBGatewayStatusData_H

#include "Defs.h"

#include <wx/wx.h>

class CIRCDDBGatewayStatusData {
public:
	CIRCDDBGatewayStatusData(IRCDDB_STATUS ircDDBStatus, bool dprsStatus);
	~CIRCDDBGatewayStatusData();

	void setRepeater(unsigned int n, const wxString& callsign, LINK_STATUS linkStatus, const wxString& linkCallsign, const wxString& incoming);

	void setDongles(const wxString& dongles);

	IRCDDB_STATUS getIrcDDBStatus() const;
	bool          getDPRSStatus() const;

	wxString      getCallsign(unsigned int n) const;
	LINK_STATUS   getLinkStatus(unsigned int n) const;
	wxString      getLinkCallsign(unsigned int n) const;
	wxString      getIncoming(unsigned int n) const;

	wxString      getDongles() const;

private:
	IRCDDB_STATUS  m_ircDDBStatus;
	bool           m_dprsStatus;
	wxString       m_callsign[4];
	LINK_STATUS    m_linkStatus[4];
	wxString       m_linkCallsign[4];
	wxString       m_incoming[4];
	wxString       m_dongles;
};

#endif
