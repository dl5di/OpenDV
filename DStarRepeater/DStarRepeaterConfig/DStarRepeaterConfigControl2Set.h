/*
 *   Copyright (C) 2009,2010,2012,2014 by Jonathan Naylor G4KLX
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

#ifndef	DStarRepeaterConfigControl2Set_H
#define	DStarRepeaterConfigControl2Set_H

#include "CallsignTextCtrl.h"

#include <wx/wx.h>

class CDStarRepeaterConfigControl2Set : public wxPanel {
public:
	CDStarRepeaterConfigControl2Set(wxWindow* parent, int id, const wxString& title, const wxString& command1,
		const wxString& command1Line, const wxString& command2, const wxString& command2Line,
		const wxString& command3, const wxString& command3Line, const wxString& command4,
		const wxString& command4Line, const wxString& command5, const wxString& command5Line,
		const wxString& command6, const wxString& command6Line);
	virtual ~CDStarRepeaterConfigControl2Set();

	virtual bool Validate();

	virtual wxString getCommand1() const;
	virtual wxString getCommand1Line() const;

	virtual wxString getCommand2() const;
	virtual wxString getCommand2Line() const;

	virtual wxString getCommand3() const;
	virtual wxString getCommand3Line() const;

	virtual wxString getCommand4() const;
	virtual wxString getCommand4Line() const;

	virtual wxString getCommand5() const;
	virtual wxString getCommand5Line() const;

	virtual wxString getCommand6() const;
	virtual wxString getCommand6Line() const;

private:
	wxString           m_title;
	CCallsignTextCtrl* m_command1;
	wxTextCtrl*        m_command1Line;
	CCallsignTextCtrl* m_command2;
	wxTextCtrl*        m_command2Line;
	CCallsignTextCtrl* m_command3;
	wxTextCtrl*        m_command3Line;
	CCallsignTextCtrl* m_command4;
	wxTextCtrl*        m_command4Line;
	CCallsignTextCtrl* m_command5;
	wxTextCtrl*        m_command5Line;
	CCallsignTextCtrl* m_command6;
	wxTextCtrl*        m_command6Line;
};

#endif
