/*
 *   Copyright (C) 2009 by Jonathan Naylor G4KLX
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

#ifndef	ParrotControllerNetworkSet_H
#define	ParrotControllerNetworkSet_H

#include "AddressTextCtrl.h"
#include "PortTextCtrl.h"

#include <wx/wx.h>

class CParrotControllerNetworkSet : public wxPanel {
public:
	CParrotControllerNetworkSet(wxWindow* parent, int id, const wxString& title, const wxString& repeaterAddress, unsigned int repeaterPort, const wxString& localAddress, unsigned int localPort);
	virtual ~CParrotControllerNetworkSet();

	virtual bool Validate();

	virtual wxString     getRepeaterAddress() const;
	virtual unsigned int getRepeaterPort() const;
	virtual wxString     getLocalAddress() const;
	virtual unsigned int getLocalPort() const;

private:
	wxString           m_title;
	CAddressTextCtrl*  m_repeaterAddress;
	CPortTextCtrl*     m_repeaterPort;
	CAddressTextCtrl*  m_localAddress;
	CPortTextCtrl*     m_localPort;
};

#endif
