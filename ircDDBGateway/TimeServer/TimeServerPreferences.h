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

#ifndef	TimeServerPreferences_H
#define	TimeServerPreferences_H

#include "TimeServerAnnouncementsSet.h"
#include "TimeServerGatewaySet.h"
#include "TimeServerDefs.h"

#include <wx/wx.h>
#include <wx/notebook.h>

class CTimeServerPreferences : public wxDialog {
public:
	CTimeServerPreferences(wxWindow* parent, int id, const wxString& callsign, bool sendA, bool sendB,
		bool sendC, bool sendD, bool sendE, const wxString& address, LANGUAGE language, FORMAT format, INTERVAL interval);
	virtual ~CTimeServerPreferences();

	virtual bool Validate();

	virtual wxString getCallsign() const;
	virtual wxString getAddress() const;
	virtual bool     getSendA() const;
	virtual bool     getSendB() const;
	virtual bool     getSendC() const;
	virtual bool     getSendD() const;
	virtual bool     getSendE() const;

	virtual LANGUAGE getLanguage() const;
	virtual FORMAT   getFormat() const;
	virtual INTERVAL getInterval() const;

private:
	CTimeServerGatewaySet*       m_gateway;
	CTimeServerAnnouncementsSet* m_announcements;
};

#endif
