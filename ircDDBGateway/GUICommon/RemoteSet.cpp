/*
 *   Copyright (C) 2011 by Jonathan Naylor G4KLX
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
#include "RemoteSet.h"

const unsigned int PASSWORD_WIDTH = 120U;
const unsigned int PORT_WIDTH     = 80U;

const unsigned int PORT_LENGTH    = 5U;

const unsigned int BORDER_SIZE = 5U;

CRemoteSet::CRemoteSet(wxWindow* parent, int id, const wxString& title, bool enabled, const wxString& password, unsigned int port) :
wxPanel(parent, id),
m_title(title),
m_enabled(NULL),
m_password(NULL),
m_port(NULL)
{
	wxFlexGridSizer* sizer = new wxFlexGridSizer(2);

	wxStaticText* enabledLabel = new wxStaticText(this, -1, _("Remote"));
	sizer->Add(enabledLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_enabled = new wxChoice(this, -1, wxDefaultPosition, wxSize(PASSWORD_WIDTH, -1));
	m_enabled->Append(_("Disabled"));
	m_enabled->Append(_("Enabled"));
	sizer->Add(m_enabled, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_enabled->SetSelection(enabled ? 1 : 0);

	wxStaticText* passwordLabel = new wxStaticText(this, -1, _("Password"));
	sizer->Add(passwordLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_password = new wxTextCtrl(this, -1, password, wxDefaultPosition, wxSize(PASSWORD_WIDTH, -1), wxTE_PASSWORD);
	sizer->Add(m_password, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* repeaterPortLabel = new wxStaticText(this, -1, _("Port"));
	sizer->Add(repeaterPortLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxString buffer;
	buffer.Printf(wxT("%u"), port);

	m_port = new CPortTextCtrl(this, -1, buffer, wxDefaultPosition, wxSize(PORT_WIDTH, -1));
	m_port->SetMaxLength(PORT_LENGTH);
	sizer->Add(m_port, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	SetAutoLayout(true);

	SetSizer(sizer);
}


CRemoteSet::~CRemoteSet()
{
}

bool CRemoteSet::Validate()
{
	int n = m_enabled->GetCurrentSelection();
	if (n == wxNOT_FOUND) {
		wxMessageDialog dialog(this, _("Remote Enabled is not set"), m_title + _(" Error"), wxICON_ERROR);
		dialog.ShowModal();
		return false;
	}

	if (n == 0)
		return true;

	wxString password = getPassword();
	if (password.IsEmpty()) {
		wxMessageDialog dialog(this, _("The Remote Password is empty"), m_title + _(" Error"), wxICON_ERROR);
		dialog.ShowModal();
		return false;
	}

	unsigned int port = getPort();
	if (port == 0U || port > 65535U) {
		wxMessageDialog dialog(this, _("The Remote Port is not valid"), m_title + _(" Error"), wxICON_ERROR);
		dialog.ShowModal();
		return false;
	}

	return true;
}

bool CRemoteSet::getEnabled() const
{
	int n = m_enabled->GetCurrentSelection();

	return n == 1;
}

wxString CRemoteSet::getPassword() const
{
	return m_password->GetValue();
}

unsigned int CRemoteSet::getPort() const
{
	unsigned long n;
	m_port->GetValue().ToULong(&n);

	return n;
}
