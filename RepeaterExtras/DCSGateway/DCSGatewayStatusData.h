/*
 *   Copyright (C) 2010 by Jonathan Naylor G4KLX
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

#ifndef	DCSGatewayStatusData_H
#define	DCSGatewayStatusData_H

#include "DCSGatewayDefs.h"

#include <wx/wx.h>

class CDCSGatewayStatusData {
public:
	CDCSGatewayStatusData(const wxString& myCall, const wxString& yourCall, const wxString& rptCall1,
		const wxString& rptCall2, LINK_STATE state, const wxString& reflector, unsigned int reconnectTimer,
		unsigned int reconnectExpiry);
	~CDCSGatewayStatusData();

	wxString      getMyCall() const;
	wxString      getYourCall() const;
	wxString      getRptCall1() const;
	wxString      getRptCall2() const;

	LINK_STATE    getState() const;
	wxString      getReflector() const;

	unsigned int  getReconnectTimer() const;
	unsigned int  getReconnectExpiry() const;

private:
	wxString     m_myCall;
	wxString     m_yourCall;
	wxString     m_rptCall1;
	wxString     m_rptCall2;
	LINK_STATE   m_state;
	wxString     m_reflector;
	unsigned int m_reconnectTimer;
	unsigned int m_reconnectExpiry;
};

#endif
