/*
 *   Copyright (C) 2010 by Jonathan Naylor G4KLX
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

#include "DPRSSet.h"

const unsigned int BORDER_SIZE    = 5U;
const unsigned int CONTROL_WIDTH1 = 200U;
const unsigned int CONTROL_WIDTH2 = 80U;

const unsigned int PORT_LENGTH     = 5U;
const unsigned int PASSWORD_LENGTH = 5U;

CDPRSSet::CDPRSSet(wxWindow* parent, int id, const wxString& title, bool enabled, const wxString& hostname, unsigned int port) :
wxPanel(parent, id),
m_title(title),
m_enabled(NULL),
m_hostname(NULL),
m_port(NULL)
{
	wxFlexGridSizer* sizer = new wxFlexGridSizer(2);

	wxStaticText* enabledLabel = new wxStaticText(this, -1, _("D-PRS"));
	sizer->Add(enabledLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_enabled = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	m_enabled->Append(_("Disabled"));
	m_enabled->Append(_("Enabled"));
	sizer->Add(m_enabled, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_enabled->SetSelection(enabled ? 1 : 0);

	wxStaticText* hostnameLabel = new wxStaticText(this, -1, _("Hostname"));
	sizer->Add(hostnameLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_hostname = new wxTextCtrl(this, -1, hostname, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	sizer->Add(m_hostname, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* portLabel = new wxStaticText(this, -1, _("Port"));
	sizer->Add(portLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxString buffer;
	buffer.Printf(wxT("%u"), port);

	m_port = new CPortTextCtrl(this, -1, buffer, wxDefaultPosition, wxSize(CONTROL_WIDTH2, -1));
	m_port->SetMaxLength(PORT_LENGTH);
	sizer->Add(m_port, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	SetAutoLayout(true);

	SetSizer(sizer);
}


CDPRSSet::~CDPRSSet()
{
}

bool CDPRSSet::Validate()
{
	int n = m_enabled->GetCurrentSelection();
	if (n == wxNOT_FOUND)
		return false;

	wxString hostname = m_hostname->GetValue();
	if (hostname.IsEmpty())
		return true;

	unsigned int port = getPort();

	if (port == 0U || port > 65535U) {
		wxMessageDialog dialog(this, _("The Port is not valid"), m_title + _(" Error"), wxICON_ERROR);
		dialog.ShowModal();
		return false;
	}

	return true;
}

bool CDPRSSet::getEnabled() const
{
	int c = m_enabled->GetCurrentSelection();
	if (c == wxNOT_FOUND)
		return false;

	return c == 1;
}

wxString CDPRSSet::getHostname() const
{
	return m_hostname->GetValue();
}

unsigned int CDPRSSet::getPort() const
{
	unsigned long n;

	m_port->GetValue().ToULong(&n);

	return n;
}
