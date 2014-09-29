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

#ifndef	DExtraGatewayReflectorSet_H
#define	DExtraGatewayReflectorSet_H

#include "DExtraGatewayDefs.h"
#include "CallsignTextCtrl.h"
#include "DStarDefines.h"

#include <wx/wx.h>

class CDExtraGatewayReflectorSet : public wxPanel {
public:
	CDExtraGatewayReflectorSet(wxWindow* parent, int id, const wxString& title, const wxString& callsign, const wxString& reflector, bool atStartup, RECONNECT reconnect, TEXT_LANG language);
	virtual ~CDExtraGatewayReflectorSet();

	virtual bool Validate();

	virtual wxString  getCallsign() const;

	virtual wxString  getReflector() const;

	virtual bool      atStartup() const;

	virtual RECONNECT getReconnect() const;

	virtual TEXT_LANG getLanguage() const;

private:
	wxString           m_title;
	CCallsignTextCtrl* m_callsign;
	wxChoice*          m_reflector;
	wxChoice*          m_channel;
	wxChoice*          m_startup;
	wxChoice*          m_reconnect;
	wxChoice*          m_language;
};

#endif
