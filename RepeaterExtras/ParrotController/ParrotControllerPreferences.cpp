/*
 *   Copyright (C) 2009 by Jonathan Naylor G4KLX
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

#include "ParrotControllerPreferences.h"

#include "ParrotControllerDefs.h"

const unsigned int BORDER_SIZE = 5U;

CParrotControllerPreferences::CParrotControllerPreferences(wxWindow* parent, int id, PARROT_MODE mode,
														   unsigned int beaconTime, const wxString& beaconFileName,
														   unsigned int turnaroundTime, bool keepFile,
														   const wxString& repeaterAddress, unsigned int repeaterPort,
														   const wxString& localAddress, unsigned int localPort) :
wxDialog(parent, id, wxString(_("Parrot Controller Preferences"))),
m_mode(NULL),
m_parrot(NULL),
m_beacon(NULL),
m_network(NULL)
{
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	wxNotebook* noteBook = new wxNotebook(this, -1);

	m_mode = new CParrotControllerModeSet(noteBook, -1, APPLICATION_NAME, mode);
	noteBook->AddPage(m_mode, _("Mode"), true);

	m_parrot = new CParrotControllerParrotSet(noteBook, -1, APPLICATION_NAME, turnaroundTime, keepFile);
	noteBook->AddPage(m_parrot, _("Parrot"), false);

	m_beacon = new CParrotControllerBeaconSet(noteBook, -1, APPLICATION_NAME, beaconTime, beaconFileName);
	noteBook->AddPage(m_beacon, _("Beacon"), false);

	m_network = new CParrotControllerNetworkSet(noteBook, -1, APPLICATION_NAME, repeaterAddress, repeaterPort, localAddress, localPort);
	noteBook->AddPage(m_network, _("Network"), false);

	mainSizer->Add(noteBook, 1, wxALL | wxGROW, BORDER_SIZE);

	mainSizer->Add(CreateButtonSizer(wxOK | wxCANCEL), 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	SetAutoLayout(true);
	Layout();

	mainSizer->Fit(this);
	mainSizer->SetSizeHints(this);

	SetSizer(mainSizer);
}

CParrotControllerPreferences::~CParrotControllerPreferences()
{
}

bool CParrotControllerPreferences::Validate()
{
	if (!m_mode->Validate())
		return false;

	if (!m_parrot->Validate())
		return false;

	if (!m_beacon->Validate())
		return false;

	return m_network->Validate();
}

PARROT_MODE CParrotControllerPreferences::getMode() const
{
	return m_mode->getMode();
}


unsigned int CParrotControllerPreferences::getBeaconTime() const
{
	return m_beacon->getBeaconTime();
}

wxString CParrotControllerPreferences::getBeaconFileName() const
{
	return m_beacon->getBeaconFileName();
}

unsigned int CParrotControllerPreferences::getTurnaroundTime() const
{
	return m_parrot->getTurnaroundTime();
}

bool CParrotControllerPreferences::getKeepFile() const
{
	return m_parrot->getKeepFile();
}

wxString CParrotControllerPreferences::getRepeaterAddress() const
{
	return m_network->getRepeaterAddress();
}

unsigned int CParrotControllerPreferences::getRepeaterPort() const
{
	return m_network->getRepeaterPort();
}

wxString CParrotControllerPreferences::getLocalAddress() const
{
	return m_network->getLocalAddress();
}

unsigned int CParrotControllerPreferences::getLocalPort() const
{
	return m_network->getLocalPort();
}
