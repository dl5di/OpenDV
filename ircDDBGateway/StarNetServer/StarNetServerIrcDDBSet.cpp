/*
 *   Copyright (C) 2010,2012,2013,2014 by Jonathan Naylor G4KLX
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

#include "StarNetServerIrcDDBSet.h"

const unsigned int BORDER_SIZE    = 5U;
const unsigned int CONTROL_WIDTH1 = 200U;
const unsigned int CONTROL_WIDTH2 = 80U;

const unsigned int PORT_LENGTH   = 5U;

CStarNetServerIrcDDBSet::CStarNetServerIrcDDBSet(wxWindow* parent, int id, const wxString& title, const wxString& hostname, const wxString& username, const wxString& password) :
wxPanel(parent, id),
m_title(title),
m_hostname(NULL),
m_username(NULL),
m_password(NULL)
{
	wxFlexGridSizer* sizer = new wxFlexGridSizer(2);

	wxStaticText* hostnameLabel = new wxStaticText(this, -1, _("Hostname"));
	sizer->Add(hostnameLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_hostname = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	m_hostname->Append(wxT("group1-irc.ircddb.net"));
	m_hostname->Append(wxT("group2-irc.ircddb.net"));
	m_hostname->Append(wxT("irc1.openquad.net"));
	m_hostname->Append(wxT("irc2.openquad.net"));
	m_hostname->Append(wxT("irc3.openquad.net"));
	m_hostname->Append(wxT("irc4.openquad.net"));
	m_hostname->Append(wxT("irc5.openquad.net"));
	m_hostname->Append(wxT("irc6.openquad.net"));
	m_hostname->Append(wxT("server1-ik2xyp.free-dstar.org"));
	m_hostname->Append(wxT("ircddb.dstar.su"));
	sizer->Add(m_hostname, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_hostname->SetStringSelection(hostname);

	wxStaticText* usernameLabel = new wxStaticText(this, -1, _("Username"));
	sizer->Add(usernameLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_username = new wxTextCtrl(this, -1, username, wxDefaultPosition, wxSize(CONTROL_WIDTH2, -1));
	sizer->Add(m_username, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* passwordLabel = new wxStaticText(this, -1, _("Password"));
	sizer->Add(passwordLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_password = new wxTextCtrl(this, -1, password, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1), wxTE_PASSWORD);
	sizer->Add(m_password, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	SetAutoLayout(true);

	SetSizer(sizer);
}


CStarNetServerIrcDDBSet::~CStarNetServerIrcDDBSet()
{
}

bool CStarNetServerIrcDDBSet::Validate()
{
	bool res = getHostname().IsEmpty();
	if (res) {
		wxMessageDialog dialog(this, _("The Hostname may not be empty"), m_title + _(" Error"), wxICON_ERROR);
		dialog.ShowModal();
		return false;
	}

	res = getUsername().IsEmpty();
	if (res) {
		wxMessageDialog dialog(this, _("The Username may not be empty"), m_title + _(" Error"), wxICON_ERROR);
		dialog.ShowModal();
		return false;
	}

	return true;
}

wxString CStarNetServerIrcDDBSet::getHostname() const
{
	return m_hostname->GetStringSelection();
}

wxString CStarNetServerIrcDDBSet::getUsername() const
{
	return m_username->GetValue();
}

wxString CStarNetServerIrcDDBSet::getPassword() const
{
	return m_password->GetValue();
}
