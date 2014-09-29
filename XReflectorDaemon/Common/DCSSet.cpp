/*
 *   Copyright (C) 2012,2013 by Jonathan Naylor G4KLX
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

#include "DCSSet.h"

const unsigned int CONTROL_WIDTH = 130U;

const unsigned int BORDER_SIZE = 5U;

CDCSSet::CDCSSet(wxWindow* parent, int id, const wxString& title, bool dcsEnabled, bool ccsEnabled) :
wxPanel(parent, id),
m_title(title),
m_dcsEnabled(NULL),
m_ccsEnabled(NULL)
{
	wxFlexGridSizer* sizer = new wxFlexGridSizer(2);

	wxStaticText* dcsEnabledLabel = new wxStaticText(this, -1, wxT("DCS"));
	sizer->Add(dcsEnabledLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_dcsEnabled = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	m_dcsEnabled->Append(_("Disabled"));
	m_dcsEnabled->Append(_("Enabled"));
	sizer->Add(m_dcsEnabled, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_dcsEnabled->SetSelection(dcsEnabled ? 1 : 0);

	wxStaticText* ccsEnabledLabel = new wxStaticText(this, -1, wxT("CCS"));
	sizer->Add(ccsEnabledLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_ccsEnabled = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	m_ccsEnabled->Append(_("Disabled"));
	m_ccsEnabled->Append(_("Enabled"));
	sizer->Add(m_ccsEnabled, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_ccsEnabled->SetSelection(ccsEnabled ? 1 : 0);

	SetAutoLayout(true);

	SetSizer(sizer);
}


CDCSSet::~CDCSSet()
{
}

bool CDCSSet::Validate()
{
	int n = m_dcsEnabled->GetCurrentSelection();
	if (n == wxNOT_FOUND)
		return false;

	n = m_ccsEnabled->GetCurrentSelection();
	if (n == wxNOT_FOUND)
		return false;

	return true;
}

bool CDCSSet::getDCSEnabled() const
{
	int c = m_dcsEnabled->GetCurrentSelection();
	if (c == wxNOT_FOUND)
		return false;

	return c == 1;
}

bool CDCSSet::getCCSEnabled() const
{
	int c = m_ccsEnabled->GetCurrentSelection();
	if (c == wxNOT_FOUND)
		return false;

	return c == 1;
}
