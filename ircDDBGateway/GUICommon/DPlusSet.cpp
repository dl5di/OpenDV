/*
 *   Copyright (C) 2010-2013 by Jonathan Naylor G4KLX
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
#include "DPlusSet.h"

const unsigned int CONTROL_WIDTH = 130U;

const unsigned int BORDER_SIZE = 5U;

CDPlusSet::CDPlusSet(wxWindow* parent, int id, const wxString& title, bool enabled, unsigned int maxDongles, unsigned int maxLinks, const wxString& login) :
wxPanel(parent, id),
m_title(title),
m_enabled(NULL),
m_maxDongles(NULL),
m_login(NULL)
{
	wxFlexGridSizer* sizer = new wxFlexGridSizer(2);

	wxStaticText* enabledLabel = new wxStaticText(this, -1, _("D-Plus"));
	sizer->Add(enabledLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_enabled = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	m_enabled->Append(_("Disabled"));
	m_enabled->Append(_("Enabled"));
	sizer->Add(m_enabled, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_enabled->SetSelection(enabled ? 1 : 0);

	wxStaticText* maxDonglesLabel = new wxStaticText(this, -1, _("Max. Dongles"));
	sizer->Add(maxDonglesLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_maxDongles = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	for (unsigned int i = 0U; i <= maxLinks; i++) {
		wxString text;
		text.Printf(wxT("%u"), i);
		m_maxDongles->Append(text);
	}
	sizer->Add(m_maxDongles, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_maxDongles->SetSelection(maxDongles);

	wxStaticText* loginLabel = new wxStaticText(this, -1, _("Login"));
	sizer->Add(loginLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_login = new CCallsignTextCtrl(this, -1, login, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	m_login->SetMaxLength(LONG_CALLSIGN_LENGTH);
	sizer->Add(m_login, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	SetAutoLayout(true);

	SetSizer(sizer);
}


CDPlusSet::~CDPlusSet()
{
}

bool CDPlusSet::Validate()
{
	int n = m_enabled->GetCurrentSelection();
	if (n == wxNOT_FOUND)
		return false;

	if (n == 1) {
		wxString login = m_login->GetValue();
		if (login.IsEmpty() || login.IsSameAs(wxT("        "))) {
			wxMessageDialog dialog(this, _("The D-Plus Login is blank"), m_title + _(" Error"), wxICON_ERROR);
			dialog.ShowModal();
			return false;
		}
	}

	n = m_maxDongles->GetCurrentSelection();
	if (n == wxNOT_FOUND)
		return false;

	return true;
}

bool CDPlusSet::getEnabled() const
{
	int c = m_enabled->GetCurrentSelection();
	if (c == wxNOT_FOUND)
		return false;

	return c == 1;
}

unsigned int CDPlusSet::getMaxDongles() const
{
	int c = m_maxDongles->GetCurrentSelection();
	if (c == wxNOT_FOUND)
		return 0U;

	return (unsigned int)c;
}

wxString CDPlusSet::getLogin() const
{
	wxString callsign = m_login->GetValue();

	callsign.MakeUpper();
	callsign.Append(wxT("        "));
	callsign.Truncate(LONG_CALLSIGN_LENGTH);

	return callsign;
}
