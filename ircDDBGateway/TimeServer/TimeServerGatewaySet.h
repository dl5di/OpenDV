/*
 *   Copyright (C) 2012 by Jonathan Naylor G4KLX
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

#ifndef	TimeServerGatewaySet_H
#define	TimeServerGatewaySet_H

#include "CallsignTextCtrl.h"
#include "AddressTextCtrl.h"

#include <wx/wx.h>

class CTimeServerGatewaySet : public wxPanel {
public:
	CTimeServerGatewaySet(wxWindow* parent, int id, const wxString& title, const wxString& callsign, bool sendA, bool sendB,
		bool sendC, bool sendD, bool sendE, const wxString& address);
	virtual ~CTimeServerGatewaySet();

	virtual bool Validate();

	virtual wxString getCallsign() const;

	virtual wxString getAddress() const;

	virtual bool     getSendA() const;
	virtual bool     getSendB() const;
	virtual bool     getSendC() const;
	virtual bool     getSendD() const;
	virtual bool     getSendE() const;

private:
	wxString           m_title;
	CCallsignTextCtrl* m_callsign;
	CAddressTextCtrl*  m_address;
	wxChoice*          m_sendA;
	wxChoice*          m_sendB;
	wxChoice*          m_sendC;
	wxChoice*          m_sendD;
	wxChoice*          m_sendE;
};

#endif
