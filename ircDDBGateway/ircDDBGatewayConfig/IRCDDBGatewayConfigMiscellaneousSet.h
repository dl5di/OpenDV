/*
 *   Copyright (C) 2010,2011,2012 by Jonathan Naylor G4KLX
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

#ifndef	IRCDDBGatewayConfigMiscellaneousSet_H
#define	IRCDDBGatewayConfigMiscellaneousSet_H

#include "Defs.h"

#include <wx/wx.h>

class CIRCDDBGatewayConfigMiscellaneousSet : public wxPanel {
public:
	CIRCDDBGatewayConfigMiscellaneousSet(wxWindow* parent, int id, const wxString& title, TEXT_LANG language, bool infoEnabled, bool echoEnabled, bool logEnabled, bool dratsEnabled, bool dtmfEnabled);
	virtual ~CIRCDDBGatewayConfigMiscellaneousSet();

	virtual bool Validate();

	virtual TEXT_LANG getLanguage() const;

	virtual bool      getInfoEnabled() const;

	virtual bool      getEchoEnabled() const;

	virtual bool      getLogEnabled() const;

	virtual bool      getDRATSEnabled() const;

	virtual bool      getDTMFEnabled() const;

private:
	wxString  m_title;
	wxChoice* m_language;
	wxChoice* m_infoEnabled;
	wxChoice* m_echoEnabled;
	wxChoice* m_logEnabled;
	wxChoice* m_dratsEnabled;
	wxChoice* m_dtmfEnabled;
};

#endif
