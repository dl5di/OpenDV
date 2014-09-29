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

#include "DExtraGatewayPreferences.h"

#include "DExtraGatewayDefs.h"

const unsigned int BORDER_SIZE = 5U;

CDExtraGatewayPreferences::CDExtraGatewayPreferences(wxWindow* parent, int id, const wxString& callsign,
													 const wxString& reflector, bool atStartup,
													 RECONNECT reconnect, TEXT_LANG language,
													 const wxString& repeaterCallsign,
													 const wxString& repeaterAddress, unsigned int repeaterPort,
													 const wxString& localAddress, unsigned int localPort) :
wxDialog(parent, id, wxString(_("DExtra GatewayPreferences"))),
m_reflector(NULL),
m_repeater(NULL)
{
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	wxNotebook* noteBook = new wxNotebook(this, -1);

	m_reflector = new CDExtraGatewayReflectorSet(noteBook, -1, APPLICATION_NAME, callsign, reflector, atStartup, reconnect, language);
	noteBook->AddPage(m_reflector, _("Reflector"), true);

	m_repeater = new CDExtraGatewayRepeaterSet(noteBook, -1, APPLICATION_NAME, repeaterCallsign, repeaterAddress, repeaterPort, localAddress, localPort);
	noteBook->AddPage(m_repeater, _("Repeater"), false);

	mainSizer->Add(noteBook, 1, wxALL | wxGROW, BORDER_SIZE);

	mainSizer->Add(CreateButtonSizer(wxOK | wxCANCEL), 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	SetAutoLayout(true);
	Layout();

	mainSizer->Fit(this);
	mainSizer->SetSizeHints(this);

	SetSizer(mainSizer);
}

CDExtraGatewayPreferences::~CDExtraGatewayPreferences()
{
}

bool CDExtraGatewayPreferences::Validate()
{
	if (!m_reflector->Validate())
		return false;

	return m_repeater->Validate();
}

wxString CDExtraGatewayPreferences::getCallsign() const
{
	return m_reflector->getCallsign();
}

wxString CDExtraGatewayPreferences::getReflector() const
{
	return m_reflector->getReflector();
}

bool CDExtraGatewayPreferences::atStartup() const
{
	return m_reflector->atStartup();
}

RECONNECT CDExtraGatewayPreferences::getReconnect() const
{
	return m_reflector->getReconnect();
}

TEXT_LANG CDExtraGatewayPreferences::getLanguage() const
{
	return m_reflector->getLanguage();
}

wxString CDExtraGatewayPreferences::getRepeaterCallsign() const
{
	return m_repeater->getRepeaterCallsign();
}

wxString CDExtraGatewayPreferences::getRepeaterAddress() const
{
	return m_repeater->getRepeaterAddress();
}

unsigned int CDExtraGatewayPreferences::getRepeaterPort() const
{
	return m_repeater->getRepeaterPort();
}

wxString CDExtraGatewayPreferences::getLocalAddress() const
{
	return m_repeater->getLocalAddress();
}

unsigned int CDExtraGatewayPreferences::getLocalPort() const
{
	return m_repeater->getLocalPort();
}
