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

#ifndef	DStarRepeaterConfigAnnouncementSet_H
#define	DStarRepeaterConfigAnnouncementSet_H

#include "CallsignTextCtrl.h"
#include "DStarDefines.h"

#include <wx/wx.h>

class CDStarRepeaterConfigAnnouncementSet : public wxPanel {
public:
	CDStarRepeaterConfigAnnouncementSet(wxWindow* parent, int id, const wxString& title, bool enabled, unsigned int time, const wxString& recordRPT1, const wxString& recordRPT2, const wxString& deleteRPT1, const wxString& deleteRPT2);
	virtual ~CDStarRepeaterConfigAnnouncementSet();

	virtual bool Validate();

	virtual bool         getEnabled() const;
	virtual unsigned int getTime() const;
	virtual wxString     getRecordRPT1() const;
	virtual wxString     getRecordRPT2() const;
	virtual wxString     getDeleteRPT1() const;
	virtual wxString     getDeleteRPT2() const;

private:
	wxString           m_title;
	wxChoice*          m_enabled;
	wxSlider*          m_time;
	CCallsignTextCtrl* m_recordRPT1;
	CCallsignTextCtrl* m_recordRPT2;
	CCallsignTextCtrl* m_deleteRPT1;
	CCallsignTextCtrl* m_deleteRPT2;
};

#endif
