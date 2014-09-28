/*
 *   Copyright (C) 2010,2011,2014 by Jonathan Naylor G4KLX
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

#ifndef	DStarRepeaterConfigNetworkSet_H
#define	DStarRepeaterConfigNetworkSet_H

#include "AddressTextCtrl.h"
#include "PortTextCtrl.h"

#include <wx/wx.h>

class CDStarRepeaterConfigNetworkSet : public wxPanel {
public:
	CDStarRepeaterConfigNetworkSet(wxWindow* parent, int id, const wxString& title, const wxString& gatewayAddress, unsigned int gatewayPort, const wxString& localAddress, unsigned int localPort, const wxString& name);
	virtual ~CDStarRepeaterConfigNetworkSet();

	virtual bool Validate();

	virtual wxString     getGatewayAddress() const;
	virtual unsigned int getGatewayPort() const;
	virtual wxString     getLocalAddress() const;
	virtual unsigned int getLocalPort() const;
	virtual wxString     getName() const;

private:
	wxString          m_title;
	CAddressTextCtrl* m_gatewayAddress;
	CPortTextCtrl*    m_gatewayPort;
	CAddressTextCtrl* m_localAddress;
	CPortTextCtrl*    m_localPort;
	wxTextCtrl*       m_name;
};

#endif
