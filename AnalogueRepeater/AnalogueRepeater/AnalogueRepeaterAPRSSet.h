/*
 *   Copyright (C) 2013,2014 by Jonathan Naylor G4KLX
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

#ifndef	AnalogueRepeaterAPRSSet_H
#define	AnalogueRepeaterAPRSSet_H

#include "CallsignTextCtrl.h"

#include <wx/wx.h>

class CAnalogueRepeaterAPRSSet : public wxPanel {
public:
	CAnalogueRepeaterAPRSSet(wxWindow* parent, int id, const wxString& title, bool txEnabled, const wxString& callsign, wxFloat32 latitude, wxFloat32 longitude, int height, const wxString& description);
	virtual ~CAnalogueRepeaterAPRSSet();

	virtual bool Validate();

	virtual bool      getTXEnabled() const;
	virtual wxString  getCallsign() const;
	virtual wxFloat32 getLatitude() const;
	virtual wxFloat32 getLongitude() const;
	virtual int       getHeight() const;
	virtual wxString  getDescription() const;

private:
	wxString           m_title;
	wxChoice*          m_txEnabled;
	CCallsignTextCtrl* m_callsign;
	wxTextCtrl*        m_latitude;
	wxTextCtrl*        m_longitude;
	wxTextCtrl*        m_height;
	wxTextCtrl*        m_description;
};

#endif
