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

#ifndef	DExtraGatewayPreferences_H
#define	DExtraGatewayPreferences_H

#include <wx/wx.h>
#include <wx/notebook.h>

#include "DExtraGatewayReflectorSet.h"
#include "DExtraGatewayRepeaterSet.h"
#include "DStarDefines.h"

class CDExtraGatewayPreferences : public wxDialog {
public:
	CDExtraGatewayPreferences(wxWindow* parent, int id, const wxString& callsign, const wxString& reflector,
		bool atStartup, RECONNECT reconnect, TEXT_LANG language, const wxString& repeaterCallsign,
		const wxString& repeaterAddress, unsigned int repeaterPort, const wxString& localAddress,
		unsigned int localPort);
	virtual ~CDExtraGatewayPreferences();

	virtual bool Validate();

	virtual wxString     getCallsign() const;
	virtual wxString     getReflector() const;
	virtual bool         atStartup() const;
	virtual RECONNECT    getReconnect() const;
	virtual TEXT_LANG    getLanguage() const;

	virtual wxString     getRepeaterCallsign() const;
	virtual wxString     getRepeaterAddress() const;
	virtual unsigned int getRepeaterPort() const;
	virtual wxString     getLocalAddress() const;
	virtual unsigned int getLocalPort() const;

private:
	CDExtraGatewayReflectorSet* m_reflector;
	CDExtraGatewayRepeaterSet*  m_repeater;
};

#endif
