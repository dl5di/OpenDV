/*
 *   Copyright (C) 2009-2012,2014 by Jonathan Naylor G4KLX
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

#ifndef	DStarRepeaterConfigControl1Set_H
#define	DStarRepeaterConfigControl1Set_H

#include "CallsignTextCtrl.h"

#include <wx/wx.h>

class CDStarRepeaterConfigControl1Set : public wxPanel {
public:
	CDStarRepeaterConfigControl1Set(wxWindow* parent, int id, const wxString& title, bool enabled,
		const wxString& rpt1Callsign, const wxString& rpt2Callsign, const wxString& shutdown,
		const wxString& startup, const wxString& status1, const wxString& status2, const wxString& status3,
		const wxString& status4, const wxString& status5, const wxString& output1, const wxString& output2,
		const wxString& output3, const wxString& output4);
	virtual ~CDStarRepeaterConfigControl1Set();

	virtual bool Validate();

	virtual bool     getEnabled() const;

	virtual wxString getRPT1Callsign() const;
	virtual wxString getRPT2Callsign() const;

	virtual wxString getShutdown() const;
	virtual wxString getStartup() const;

	virtual wxString getStatus1() const;
	virtual wxString getStatus2() const;
	virtual wxString getStatus3() const;
	virtual wxString getStatus4() const;
	virtual wxString getStatus5() const;

	virtual wxString getOutput1() const;
	virtual wxString getOutput2() const;
	virtual wxString getOutput3() const;
	virtual wxString getOutput4() const;

private:
	wxString           m_title;
	wxChoice*          m_enabled;
	CCallsignTextCtrl* m_rpt1Callsign;
	CCallsignTextCtrl* m_rpt2Callsign;
	CCallsignTextCtrl* m_shutdown;
	CCallsignTextCtrl* m_startup;
	CCallsignTextCtrl* m_status1;
	CCallsignTextCtrl* m_status2;
	CCallsignTextCtrl* m_status3;
	CCallsignTextCtrl* m_status4;
	CCallsignTextCtrl* m_status5;
	CCallsignTextCtrl* m_output1;
	CCallsignTextCtrl* m_output2;
	CCallsignTextCtrl* m_output3;
	CCallsignTextCtrl* m_output4;
};

#endif
