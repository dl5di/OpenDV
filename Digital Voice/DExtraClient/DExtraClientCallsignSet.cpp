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

#include "DExtraClientCallsignSet.h"

const unsigned int LONG_CALLSIGN_WIDTH   = 80U;
const unsigned int LONG_CALLSIGN_LENGTH  = 8U;

const unsigned int BORDER_SIZE   = 5U;

CDExtraClientCallsignSet::CDExtraClientCallsignSet(wxWindow* parent, int id, const wxString& title, const wxString& callsign) :
wxPanel(parent, id),
m_title(title),
m_callsign(NULL)
{
	wxFlexGridSizer* sizer = new wxFlexGridSizer(2);

	wxStaticText* callsignText = new wxStaticText(this, -1, _("Callsign"));
	sizer->Add(callsignText, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_callsign = new CCallsignTextCtrl(this, -1, callsign, wxDefaultPosition, wxSize(LONG_CALLSIGN_WIDTH, -1));
	m_callsign->SetMaxLength(LONG_CALLSIGN_LENGTH);
	sizer->Add(m_callsign, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	SetAutoLayout(true);

	sizer->Fit(this);
	sizer->SetSizeHints(this);

	SetSizer(sizer);
}


CDExtraClientCallsignSet::~CDExtraClientCallsignSet()
{
}

bool CDExtraClientCallsignSet::Validate()
{
	bool res = !getCallsign().IsEmpty();
	if (!res) {
		wxMessageDialog dialog(this, _("The callsign may not be empty"), m_title + _(" Error"), wxICON_ERROR);
		dialog.ShowModal();
	}

	return res;
}

wxString CDExtraClientCallsignSet::getCallsign() const
{
	return m_callsign->GetValue().MakeUpper();
}
