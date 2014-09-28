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

#include "RemoteControlRemoteSet.h"
#include "DStarDefines.h"

const unsigned int PASSWORD_WIDTH = 120U;
const unsigned int PORT_WIDTH     = 80U;

const unsigned int PORT_LENGTH    = 5U;

const unsigned int BORDER_SIZE = 5U;

CRemoteControlRemoteSet::CRemoteControlRemoteSet(wxWindow* parent, int id, const wxString& title, const wxString& address, unsigned int port, const wxString& password) :
wxPanel(parent, id),
m_title(title),
m_address(NULL),
m_port(NULL),
m_password(NULL)
{
	wxFlexGridSizer* sizer = new wxFlexGridSizer(2);

	wxStaticText* addressLabel = new wxStaticText(this, -1, _("Address"));
	sizer->Add(addressLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_address = new wxTextCtrl(this, -1, address, wxDefaultPosition, wxSize(PASSWORD_WIDTH, -1));
	sizer->Add(m_address, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* portLabel = new wxStaticText(this, -1, _("Port"));
	sizer->Add(portLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxString buffer;
	buffer.Printf(wxT("%u"), port);

	m_port = new CPortTextCtrl(this, -1, buffer, wxDefaultPosition, wxSize(PORT_WIDTH, -1));
	m_port->SetMaxLength(PORT_LENGTH);
	sizer->Add(m_port, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* passwordLabel = new wxStaticText(this, -1, _("Password"));
	sizer->Add(passwordLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_password = new wxTextCtrl(this, -1, password, wxDefaultPosition, wxSize(PASSWORD_WIDTH, -1), wxTE_PASSWORD);
	sizer->Add(m_password, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	SetAutoLayout(true);

	SetSizer(sizer);
}


CRemoteControlRemoteSet::~CRemoteControlRemoteSet()
{
}

bool CRemoteControlRemoteSet::Validate()
{
	wxString address = getAddress();
	if (address.IsEmpty()) {
		wxMessageDialog dialog(this, _("The Address is empty"), m_title + _(" Error"), wxICON_ERROR);
		dialog.ShowModal();
		return false;
	}

	unsigned int port = getPort();
	if (port == 0U || port > 65535U) {
		wxMessageDialog dialog(this, _("The Port is not valid"), m_title + _(" Error"), wxICON_ERROR);
		dialog.ShowModal();
		return false;
	}

	wxString password = getPassword();
	if (password.IsEmpty()) {
		wxMessageDialog dialog(this, _("The Password is empty"), m_title + _(" Error"), wxICON_ERROR);
		dialog.ShowModal();
		return false;
	}

	return true;
}

wxString CRemoteControlRemoteSet::getAddress() const
{
	return m_address->GetValue();
}

unsigned int CRemoteControlRemoteSet::getPort() const
{
	unsigned long n;
	m_port->GetValue().ToULong(&n);

	return n;
}

wxString CRemoteControlRemoteSet::getPassword() const
{
	return m_password->GetValue();
}
