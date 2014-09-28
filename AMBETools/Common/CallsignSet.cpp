/*
 *   Copyright (C) 2010,2014 by Jonathan Naylor G4KLX
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

#include "DStarDefines.h"
#include "CallsignSet.h"

#if defined(__WINDOWS__)
const unsigned int LONG_CALLSIGN_WIDTH   = 80U;
const unsigned int SHORT_CALLSIGN_WIDTH  = 50U;
#else
const unsigned int LONG_CALLSIGN_WIDTH   = 100U;
const unsigned int SHORT_CALLSIGN_WIDTH  = 60U;
#endif

const unsigned int BORDER_SIZE   = 5U;

CCallsignSet::CCallsignSet(wxWindow* parent, int id, const wxString& title, const wxString& callsign1, const wxString& callsign2) :
wxPanel(parent, id),
m_title(title),
m_callsign1(NULL),
m_callsign2(NULL)
{
	wxFlexGridSizer* sizer = new wxFlexGridSizer(4);

	wxStaticText* callsignLabel = new wxStaticText(this, -1, _("Callsign"));
	sizer->Add(callsignLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_callsign1 = new CCallsignTextCtrl(this, -1, callsign1, wxDefaultPosition, wxSize(LONG_CALLSIGN_WIDTH, -1));
	m_callsign1->SetMaxLength(LONG_CALLSIGN_LENGTH);
	sizer->Add(m_callsign1, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* slashLabel = new wxStaticText(this, -1, wxT("/"));
	sizer->Add(slashLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_callsign2 = new CCallsignTextCtrl(this, -1, callsign2, wxDefaultPosition, wxSize(SHORT_CALLSIGN_WIDTH, -1));
	m_callsign2->SetMaxLength(SHORT_CALLSIGN_LENGTH);
	sizer->Add(m_callsign2, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	SetAutoLayout(true);

	sizer->Fit(this);
	sizer->SetSizeHints(this);

	SetSizer(sizer);
}


CCallsignSet::~CCallsignSet()
{
}

bool CCallsignSet::Validate()
{
	bool res = !getCallsign1().IsEmpty();
	if (!res) {
		wxMessageDialog dialog(this, _("The Callsign may not be empty"), m_title + _(" Error"), wxICON_ERROR);
		dialog.ShowModal();
	}

	return res;
}

wxString CCallsignSet::getCallsign1() const
{
	return m_callsign1->GetValue().MakeUpper();
}

wxString CCallsignSet::getCallsign2() const
{
	return m_callsign2->GetValue().MakeUpper();
}
