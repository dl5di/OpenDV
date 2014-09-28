/*
 *   Copyright (C) 2009,2010 by Jonathan Naylor G4KLX
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

#include "AnalogueRepeaterDTMF2Set.h"

const unsigned int DTMF_WIDTH1 = 125U;
const unsigned int DTMF_WIDTH2 = 125U;
const unsigned int DTMF_WIDTH3 = 300U;
const unsigned int DTMF_WIDTH4 = 200U;

const unsigned int BORDER_SIZE = 5U;

CAnalogueRepeaterDTMF2Set::CAnalogueRepeaterDTMF2Set(wxWindow* parent, int id, const wxString& title,
															   const wxString& command1, const wxString& command1Line,
															   const wxString& command2, const wxString& command2Line) :
wxPanel(parent, id),
m_title(title),
m_command1(NULL),
m_command1Line(NULL),
m_command2(NULL),
m_command2Line(NULL)
{
	wxFlexGridSizer* sizer = new wxFlexGridSizer(3);

	wxStaticText* command1Label = new wxStaticText(this, -1, _("Command 1"));
	sizer->Add(command1Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_command1 = new CDTMFTextCtrl(this, -1, command1, wxDefaultPosition, wxSize(DTMF_WIDTH2, -1));
	m_command1->SetMaxLength(10U);
	sizer->Add(m_command1, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_command1Line = new wxTextCtrl(this, -1, command1Line, wxDefaultPosition, wxSize(DTMF_WIDTH4, -1));
	sizer->Add(m_command1Line, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* command2Label = new wxStaticText(this, -1, _("Command 2"));
	sizer->Add(command2Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_command2 = new CDTMFTextCtrl(this, -1, command2, wxDefaultPosition, wxSize(DTMF_WIDTH2, -1));
	m_command2->SetMaxLength(10U);
	sizer->Add(m_command2, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_command2Line = new wxTextCtrl(this, -1, command2Line, wxDefaultPosition, wxSize(DTMF_WIDTH4, -1));
	sizer->Add(m_command2Line, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	SetAutoLayout(true);

	sizer->Fit(this);
	sizer->SetSizeHints(this);

	SetSizer(sizer);
}


CAnalogueRepeaterDTMF2Set::~CAnalogueRepeaterDTMF2Set()
{
}

bool CAnalogueRepeaterDTMF2Set::Validate()
{
	return true;
}

wxString CAnalogueRepeaterDTMF2Set::getCommand1() const
{
	return m_command1->GetValue().MakeUpper();
}

wxString CAnalogueRepeaterDTMF2Set::getCommand1Line() const
{
	return m_command1Line->GetValue();
}

wxString CAnalogueRepeaterDTMF2Set::getCommand2() const
{
	return m_command2->GetValue().MakeUpper();
}

wxString CAnalogueRepeaterDTMF2Set::getCommand2Line() const
{
	return m_command2Line->GetValue();
}
