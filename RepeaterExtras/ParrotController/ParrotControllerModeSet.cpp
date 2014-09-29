/*
 *   Copyright (C) 2009 by Jonathan Naylor G4KLX
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

#include "ParrotControllerModeSet.h"

const unsigned int CONTROL_WIDTH = 125U;

const unsigned int BORDER_SIZE   = 5U;

CParrotControllerModeSet::CParrotControllerModeSet(wxWindow* parent, int id, const wxString& title, PARROT_MODE mode) :
wxPanel(parent, id),
m_title(title),
m_mode(NULL)
{
	wxFlexGridSizer* sizer = new wxFlexGridSizer(2);

	wxStaticText* modeLabel = new wxStaticText(this, -1, _("Mode"));
	sizer->Add(modeLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_mode = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	m_mode->Append(_("Parrot"));
	m_mode->Append(_("Beacon"));
	sizer->Add(m_mode, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_mode->SetSelection(int(mode));

	SetAutoLayout(true);

	sizer->Fit(this);
	sizer->SetSizeHints(this);

	SetSizer(sizer);
}


CParrotControllerModeSet::~CParrotControllerModeSet()
{
}

bool CParrotControllerModeSet::Validate()
{
	return m_mode->GetCurrentSelection() != wxNOT_FOUND;
}

PARROT_MODE CParrotControllerModeSet::getMode() const
{
	return PARROT_MODE(m_mode->GetCurrentSelection());
}
