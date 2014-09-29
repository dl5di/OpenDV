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

#include "XReflectorMiscellaneousSet.h"
#include "DStarDefines.h"

const unsigned int CONTROL_WIDTH = 130U;

const unsigned int BORDER_SIZE = 5U;

CXReflectorMiscellaneousSet::CXReflectorMiscellaneousSet(wxWindow* parent, int id, const wxString& title, bool logEnabled) :
wxPanel(parent, id),
m_title(title),
m_logEnabled(NULL)
{
	wxFlexGridSizer* sizer = new wxFlexGridSizer(2);

	wxStaticText* logEnabledLabel = new wxStaticText(this, -1, _("GUI Log"));
	sizer->Add(logEnabledLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_logEnabled = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	m_logEnabled->Append(_("Disabled"));
	m_logEnabled->Append(_("Enabled"));
	sizer->Add(m_logEnabled, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_logEnabled->SetSelection(logEnabled ? 1 : 0);

	SetAutoLayout(true);

	SetSizer(sizer);
}


CXReflectorMiscellaneousSet::~CXReflectorMiscellaneousSet()
{
}

bool CXReflectorMiscellaneousSet::Validate()
{
	return m_logEnabled->GetCurrentSelection() != wxNOT_FOUND;
}

bool CXReflectorMiscellaneousSet::getLogEnabled() const
{
	int c = m_logEnabled->GetCurrentSelection();
	if (c == wxNOT_FOUND)
		return false;

	return c == 1;
}
