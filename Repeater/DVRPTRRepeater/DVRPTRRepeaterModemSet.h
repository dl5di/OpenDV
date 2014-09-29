/*
 *   Copyright (C) 2011,2012 by Jonathan Naylor G4KLX
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

#ifndef	DVRPTRRepeaterModemSet_H
#define	DVRPTRRepeaterModemSet_H

#include "DVRPTRRepeaterDefs.h"
#include "AddressTextCtrl.h"
#include "DStarDefines.h"
#include "PortTextCtrl.h"

#include <wx/wx.h>

class CDVRPTRRepeaterModemSet : public wxPanel {
public:
	CDVRPTRRepeaterModemSet(wxWindow* parent, int id, const wxString& title, DVRPTR_VERSION version, CONNECTION_TYPE connectionType, const wxString& usbPort, const wxString& address, unsigned int port, bool rxInvert, bool txInvert, bool channel, unsigned int modLevel, unsigned int txDelay);
	virtual ~CDVRPTRRepeaterModemSet();

	virtual bool Validate();

	virtual DVRPTR_VERSION  getVersion() const;
	virtual CONNECTION_TYPE getConnectionType() const;
	virtual wxString        getUSBPort() const;
	virtual wxString        getAddress() const;
	virtual unsigned int    getPort() const;
	virtual bool            getRXInvert() const;
	virtual bool            getTXInvert() const;
	virtual bool            getChannel() const;
	virtual unsigned int    getModLevel() const;
	virtual unsigned int    getTXDelay() const;

	virtual void onVersion(wxCommandEvent& event);
	virtual void onConnectionType(wxCommandEvent& event);

private:
	wxString          m_title;
	wxChoice*         m_version;
	wxChoice*         m_connectionType;
	wxChoice*         m_usbPort;
	CAddressTextCtrl* m_address;
	CPortTextCtrl*    m_port;
	wxChoice*         m_txInvert;
	wxChoice*         m_rxInvert;
	wxChoice*         m_channel;
	wxSlider*         m_modLevel;
	wxSlider*         m_txDelay;

	DECLARE_EVENT_TABLE()
};

#endif
