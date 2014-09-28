/*
 *   Copyright (C) 2010,2011 by Jonathan Naylor G4KLX
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

#include "StarNetServerCallsignSet.h"
#include "DStarDefines.h"
#include <cctype>

const unsigned int CALLSIGN_WIDTH = 120U;
const unsigned int ADDRESS_WIDTH  = 120U;

const unsigned int ADDRESS_LENGTH = 15U;

const unsigned int BORDER_SIZE = 5U;

CStarNetServerCallsignSet::CStarNetServerCallsignSet(wxWindow* parent, int id, const wxString& title, const wxString& callsign, const wxString& address) :
wxPanel(parent, id),
m_title(title),
m_callsign(NULL),
m_address(NULL)
{
	wxFlexGridSizer* sizer = new wxFlexGridSizer(3);

	wxStaticText* callsignLabel = new wxStaticText(this, -1, _("Callsign"));
	sizer->Add(callsignLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxString call = callsign;
	call.Truncate(LONG_CALLSIGN_LENGTH - 1U);
	call.Trim();

	m_callsign = new CCallsignTextCtrl(this, -1, call, wxDefaultPosition, wxSize(CALLSIGN_WIDTH, -1));
	m_callsign->SetMaxLength(LONG_CALLSIGN_LENGTH);
	sizer->Add(m_callsign, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* gLabel = new wxStaticText(this, -1, wxT("G"));
	sizer->Add(gLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxStaticText* addressLabel = new wxStaticText(this, -1, _("Address"));
	sizer->Add(addressLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_address = new CAddressTextCtrl(this, -1, address, wxDefaultPosition, wxSize(ADDRESS_WIDTH, -1));
	m_address->SetMaxLength(ADDRESS_LENGTH);
	sizer->Add(m_address, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	SetAutoLayout(true);

	SetSizer(sizer);
}


CStarNetServerCallsignSet::~CStarNetServerCallsignSet()
{
}

bool CStarNetServerCallsignSet::Validate()
{
	wxString callsign = getCallsign();

	if (callsign.IsEmpty()) {
		wxMessageDialog dialog(this, _("The Callsign Callsign is not valid"), m_title + _(" Error"), wxICON_ERROR);
		dialog.ShowModal();
		return false;
	}

	return true;
}

wxString CStarNetServerCallsignSet::getCallsign() const
{
	wxString callsign = m_callsign->GetValue();

	callsign.MakeUpper();

	return callsign;
}

wxString CStarNetServerCallsignSet::getAddress() const
{
	return m_address->GetValue();
}
