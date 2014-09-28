/*
 *   Copyright (C) 2011,2012,2013 by Jonathan Naylor G4KLX
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

#ifndef	DStarRepeaterConfigDVRPTR3Set_H
#define	DStarRepeaterConfigDVRPTR3Set_H

#include "DStarRepeaterConfigDefs.h"
#include "AddressTextCtrl.h"
#include "DStarDefines.h"
#include "PortTextCtrl.h"

#include <wx/wx.h>

class CDStarRepeaterConfigDVRPTR3Set : public wxDialog {
public:
	CDStarRepeaterConfigDVRPTR3Set(wxWindow* parent, int id, CONNECTION_TYPE connectionType, const wxString& usbPort, const wxString& address, unsigned int port, bool txInvert, unsigned int modLevel, unsigned int txDelay);
	virtual ~CDStarRepeaterConfigDVRPTR3Set();

	virtual bool Validate();

	virtual CONNECTION_TYPE getConnectionType() const;
	virtual wxString        getUSBPort() const;
	virtual wxString        getAddress() const;
	virtual unsigned int    getPort() const;
	virtual bool            getTXInvert() const;
	virtual unsigned int    getModLevel() const;
	virtual unsigned int    getTXDelay() const;

	virtual void onConnectionType(wxCommandEvent& event);

private:
	wxChoice*         m_connectionType;
	wxChoice*         m_usbPort;
	CAddressTextCtrl* m_address;
	CPortTextCtrl*    m_port;
	wxChoice*         m_txInvert;
	wxSlider*         m_modLevel;
	wxSlider*         m_txDelay;

	DECLARE_EVENT_TABLE()
};

#endif
