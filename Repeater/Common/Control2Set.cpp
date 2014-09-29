/*
 *   Copyright (C) 2009,2010,2012 by Jonathan Naylor G4KLX
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

#include "Control2Set.h"
#include "DStarDefines.h"

const unsigned int BORDER_SIZE    = 5U;
const unsigned int CONTROL_WIDTH1 = 120U;
const unsigned int CONTROL_WIDTH2 = 200U;

CControl2Set::CControl2Set(wxWindow* parent, int id, const wxString& title, const wxString& command1, const wxString& command1Line, const wxString& command2, const wxString& command2Line, const wxString& command3, const wxString& command3Line, const wxString& command4, const wxString& command4Line) :
wxPanel(parent, id),
m_title(title),
m_command1(NULL),
m_command1Line(NULL),
m_command2(NULL),
m_command2Line(NULL),
m_command3(NULL),
m_command3Line(NULL),
m_command4(NULL),
m_command4Line(NULL)
{
	wxFlexGridSizer* sizer = new wxFlexGridSizer(3);

	wxStaticText* command1Label = new wxStaticText(this, -1, _("Command 1"));
	sizer->Add(command1Label, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_command1 = new CCallsignTextCtrl(this, -1, command1, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	m_command1->SetMaxLength(LONG_CALLSIGN_LENGTH);
	sizer->Add(m_command1, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_command1Line = new wxTextCtrl(this, -1, command1Line, wxDefaultPosition, wxSize(CONTROL_WIDTH2, -1));
	sizer->Add(m_command1Line, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* command2Label = new wxStaticText(this, -1, _("Command 2"));
	sizer->Add(command2Label, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_command2 = new CCallsignTextCtrl(this, -1, command2, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	m_command2->SetMaxLength(LONG_CALLSIGN_LENGTH);
	sizer->Add(m_command2, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_command2Line = new wxTextCtrl(this, -1, command2Line, wxDefaultPosition, wxSize(CONTROL_WIDTH2, -1));
	sizer->Add(m_command2Line, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* command3Label = new wxStaticText(this, -1, _("Command 3"));
	sizer->Add(command3Label, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_command3 = new CCallsignTextCtrl(this, -1, command3, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	m_command3->SetMaxLength(LONG_CALLSIGN_LENGTH);
	sizer->Add(m_command3, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_command3Line = new wxTextCtrl(this, -1, command3Line, wxDefaultPosition, wxSize(CONTROL_WIDTH2, -1));
	sizer->Add(m_command3Line, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* command4Label = new wxStaticText(this, -1, _("Command 4"));
	sizer->Add(command4Label, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_command4 = new CCallsignTextCtrl(this, -1, command4, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	m_command4->SetMaxLength(LONG_CALLSIGN_LENGTH);
	sizer->Add(m_command4, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_command4Line = new wxTextCtrl(this, -1, command4Line, wxDefaultPosition, wxSize(CONTROL_WIDTH2, -1));
	sizer->Add(m_command4Line, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	SetAutoLayout(true);

	sizer->Fit(this);
	sizer->SetSizeHints(this);

	SetSizer(sizer);
}


CControl2Set::~CControl2Set()
{
}

bool CControl2Set::Validate()
{
	return true;
}

wxString CControl2Set::getCommand1() const
{
	return m_command1->GetValue().MakeUpper();
}

wxString CControl2Set::getCommand1Line() const
{
	return m_command1Line->GetValue();
}

wxString CControl2Set::getCommand2() const
{
	return m_command2->GetValue().MakeUpper();
}

wxString CControl2Set::getCommand2Line() const
{
	return m_command2Line->GetValue();
}

wxString CControl2Set::getCommand3() const
{
	return m_command3->GetValue().MakeUpper();
}

wxString CControl2Set::getCommand3Line() const
{
	return m_command3Line->GetValue();
}

wxString CControl2Set::getCommand4() const
{
	return m_command4->GetValue().MakeUpper();
}

wxString CControl2Set::getCommand4Line() const
{
	return m_command4Line->GetValue();
}
