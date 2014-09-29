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

#include "XReflectorDPlusSet.h"
#include "DStarDefines.h"

const unsigned int CONTROL_WIDTH = 130U;

const unsigned int BORDER_SIZE = 5U;

CXReflectorDPlusSet::CXReflectorDPlusSet(wxWindow* parent, int id, const wxString& title, bool enabled) :
wxPanel(parent, id),
m_title(title),
m_enabled(NULL)
{
	wxFlexGridSizer* sizer = new wxFlexGridSizer(2);

	wxStaticText* dplusEnabledLabel = new wxStaticText(this, -1, _("DPlus"));
	sizer->Add(dplusEnabledLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_enabled = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	m_enabled->Append(_("Disabled"));
	m_enabled->Append(_("Enabled"));
	sizer->Add(m_enabled, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_enabled->SetSelection(enabled ? 1 : 0);

	SetAutoLayout(true);

	SetSizer(sizer);
}


CXReflectorDPlusSet::~CXReflectorDPlusSet()
{
}

bool CXReflectorDPlusSet::Validate()
{
	int n = m_enabled->GetCurrentSelection();
	if (n == wxNOT_FOUND)
		return false;

	return true;
}

bool CXReflectorDPlusSet::getEnabled() const
{
	int c = m_enabled->GetCurrentSelection();
	if (c == wxNOT_FOUND)
		return false;

	return c == 1;
}
