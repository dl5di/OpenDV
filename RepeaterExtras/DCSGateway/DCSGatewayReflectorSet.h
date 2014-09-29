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

#ifndef	DCSGatewayReflectorSet_H
#define	DCSGatewayReflectorSet_H

#include "CallsignTextCtrl.h"
#include "DCSGatewayDefs.h"
#include "DStarDefines.h"

#include <wx/wx.h>

class CDCSGatewayReflectorSet : public wxPanel {
public:
	CDCSGatewayReflectorSet(wxWindow* parent, int id, const wxString& title, const wxString& callsign, const wxString& reflector, bool atStartup, RECONNECT reconnect, TEXT_LANG language);
	virtual ~CDCSGatewayReflectorSet();

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
