/*
 *   Copyright (C) 2012 by Jonathan Naylor G4KLX
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

#include "TimeServerPreferences.h"
#include "DStarDefines.h"

const unsigned int BORDER_SIZE   = 5U;
const unsigned int CONTROL_WIDTH = 150U;

CTimeServerPreferences::CTimeServerPreferences(wxWindow* parent, int id, const wxString& callsign, bool sendA, bool sendB, bool sendC, bool sendD, bool sendE, const wxString& address, LANGUAGE language, FORMAT format, INTERVAL interval) :
wxDialog(parent, id, wxString(_("Time Server Preferences"))),
m_gateway(NULL),
m_announcements(NULL)
{
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	wxNotebook* noteBook = new wxNotebook(this, -1);

	m_gateway = new CTimeServerGatewaySet(noteBook, -1, APPLICATION_NAME, callsign, sendA, sendB, sendC, sendD, sendE, address);
	noteBook->AddPage(m_gateway, _("Gateway"), true);

	m_announcements = new CTimeServerAnnouncementsSet(noteBook, -1, APPLICATION_NAME, language, format, interval);
	noteBook->AddPage(m_announcements, _("Announcements"), false);

	mainSizer->Add(noteBook, 1, wxALL | wxGROW, BORDER_SIZE);

	mainSizer->Add(CreateButtonSizer(wxOK | wxCANCEL), 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	SetAutoLayout(true);
	Layout();

	mainSizer->Fit(this);
	mainSizer->SetSizeHints(this);

	SetSizer(mainSizer);
}


CTimeServerPreferences::~CTimeServerPreferences()
{
}

bool CTimeServerPreferences::Validate()
{
	if (!m_gateway->Validate())
		return false;

	return m_announcements->Validate();
}

wxString CTimeServerPreferences::getCallsign() const
{
	return m_gateway->getCallsign();
}

wxString CTimeServerPreferences::getAddress() const
{
	return m_gateway->getAddress();
}

bool CTimeServerPreferences::getSendA() const
{
	return m_gateway->getSendA();
}

bool CTimeServerPreferences::getSendB() const
{
	return m_gateway->getSendB();
}

bool CTimeServerPreferences::getSendC() const
{
	return m_gateway->getSendC();
}

bool CTimeServerPreferences::getSendD() const
{
	return m_gateway->getSendD();
}

bool CTimeServerPreferences::getSendE() const
{
	return m_gateway->getSendE();
}

LANGUAGE CTimeServerPreferences::getLanguage() const
{
	return m_announcements->getLanguage();
}

FORMAT CTimeServerPreferences::getFormat() const
{
	return m_announcements->getFormat();
}

INTERVAL CTimeServerPreferences::getInterval() const
{
	return m_announcements->getInterval();
}
