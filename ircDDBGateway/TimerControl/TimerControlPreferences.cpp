/*
 *   Copyright (C) 2011,2012 by Jonathan Naylor G4KLX
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

#include "TimerControlPreferences.h"
#include "TimerControlDefs.h"

const unsigned int BORDER_SIZE = 5U;

CTimerControlPreferences::CTimerControlPreferences(wxWindow* parent, int id, const wxString& address, unsigned int port, const wxString& password, bool delay) :
wxDialog(parent, id, wxString(_("Timer Control Preferences"))),
m_remote(NULL)
{
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	wxNotebook* noteBook = new wxNotebook(this, -1);

	m_remote = new CTimerControlRemoteSet(noteBook, -1, APPLICATION_NAME, address, port, password, delay);
	noteBook->AddPage(m_remote, _("Gateway"), true);

	mainSizer->Add(noteBook, 1, wxALL | wxGROW, BORDER_SIZE);

	mainSizer->Add(CreateButtonSizer(wxOK | wxCANCEL), 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	SetAutoLayout(true);
	Layout();

	mainSizer->Fit(this);
	mainSizer->SetSizeHints(this);

	SetSizer(mainSizer);
}

CTimerControlPreferences::~CTimerControlPreferences()
{
}

bool CTimerControlPreferences::Validate()
{
	return m_remote->Validate();
}

wxString CTimerControlPreferences::getAddress() const
{
	return m_remote->getAddress();
}

unsigned int CTimerControlPreferences::getPort() const
{
	return m_remote->getPort();
}

wxString CTimerControlPreferences::getPassword() const
{
	return m_remote->getPassword();
}

bool CTimerControlPreferences::getDelay() const
{
	return m_remote->getDelay();
}
