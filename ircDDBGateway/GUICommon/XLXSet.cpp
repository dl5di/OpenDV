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

#include "HostFile.h"
#include "XLXSet.h"
#include "Defs.h"

#include <wx/url.h>

const unsigned int CONTROL_WIDTH = 130U;

const unsigned int BORDER_SIZE = 5U;

const int CHOICE_ENABLED = 8788;

BEGIN_EVENT_TABLE(CXLXSet, wxPanel)
	EVT_CHOICE(CHOICE_ENABLED, CXLXSet::onEnabled)
END_EVENT_TABLE()


CXLXSet::CXLXSet(wxWindow* parent, int id, const wxString& title, bool xlxEnabled, const wxString& xlxHostsFileUrl) :
wxPanel(parent, id),
m_title(title),
m_xlxEnabled(NULL),
m_xlxHostsFileUrl(NULL)
{
	wxFlexGridSizer* sizer = new wxFlexGridSizer(2);

	wxStaticText* xlxEnabledLabel = new wxStaticText(this, -1, wxT("XLX"));
	sizer->Add(xlxEnabledLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_xlxEnabled = new wxChoice(this, CHOICE_ENABLED, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	m_xlxEnabled->Append(_("Disabled"));
	m_xlxEnabled->Append(_("Enabled"));
	sizer->Add(m_xlxEnabled, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_xlxEnabled->SetSelection(xlxEnabled ? 1 : 0);

	wxStaticText* xlxHostsFileUrlLabel = new wxStaticText(this, -1, _("Hosts file URL"));
	sizer->Add(xlxHostsFileUrlLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_xlxHostsFileUrl  = new wxTextCtrl(this, -1, xlxHostsFileUrl, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	sizer->Add(m_xlxHostsFileUrl, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);


	if (xlxEnabled)
		m_xlxHostsFileUrl->Enable();
	else
		m_xlxHostsFileUrl->Disable();

	SetAutoLayout(true);

	SetSizer(sizer);
}


CXLXSet::~CXLXSet()
{
}

bool CXLXSet::Validate()
{
	int n = m_xlxEnabled->GetCurrentSelection();
	if (n == wxNOT_FOUND)
		return false;

	wxString value = m_xlxHostsFileUrl->GetValue();
	wxURL url(value);
	if (url.GetError() != wxURL_NOERR)
		return false;

	return true;
}

bool CXLXSet::getXLXEnabled() const
{
	int c = m_xlxEnabled->GetCurrentSelection();
	if (c == wxNOT_FOUND)
		return false;

	return c == 1;
}

wxString CXLXSet::getXLXHostsFileUrl() const
{
	wxString value = m_xlxHostsFileUrl->GetValue();
	wxURL url(value);
	if (url.GetError() == wxURL_NOERR)
		return value;
		
	return wxEmptyString;
}

void CXLXSet::onEnabled(wxCommandEvent &event)
{
	int n = m_xlxEnabled->GetCurrentSelection();
	if (n != 1)
		m_xlxHostsFileUrl->Disable();
	else
		m_xlxHostsFileUrl->Enable();
}
