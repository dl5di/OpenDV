/*
 *   Copyright (C) 2010,2011,2012,2014 by Jonathan Naylor G4KLX
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

#include "DummyRepeaterPreferences.h"

#include "DummyRepeaterDefs.h"

CDummyRepeaterPreferences::CDummyRepeaterPreferences(wxWindow* parent, int id, const wxString& callsign1,
								   const wxString& callsign2, const wxString& readDevice,
								   const wxString& writeDevice, DONGLE_TYPE dongleType, const wxString& dongleDevice,
								   const wxString& dongleAddress, unsigned int donglePort,
								   const wxString& gwyAddress, unsigned int gwyPort,
								   const wxString& localAddress, unsigned int localPort, const wxString& type,
								   unsigned int config, bool pttInvert, bool squelchInvert, unsigned int timeout,
								   const wxString& message, bool bleep) :
wxDialog(parent, id, wxString(_("Dummy Repeater Preferences")), wxDefaultPosition, wxDefaultSize, wxRESIZE_BORDER | wxDEFAULT_DIALOG_STYLE),
m_callsign(NULL),
m_soundcard(NULL),
m_dongle(NULL),
m_network(NULL),
m_controller(NULL),
m_timeout(NULL),
m_message(NULL),
m_bleep(NULL)
{
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	wxNotebook* noteBook = new wxNotebook(this, -1);

	m_callsign = new CDummyRepeaterCallsignSet(noteBook, -1, APPLICATION_NAME, callsign1, callsign2);
	noteBook->AddPage(m_callsign, _("Callsign"), true);

	m_soundcard = new CDummyRepeaterSoundcardSet(noteBook, -1, APPLICATION_NAME, readDevice, writeDevice);
	noteBook->AddPage(m_soundcard, _("Sound Card"), false);

	m_dongle = new CDummyRepeaterDongleSet(noteBook, -1, APPLICATION_NAME, dongleType, dongleDevice, dongleAddress, donglePort);
	noteBook->AddPage(m_dongle, _("Dongle"), false);

	m_network = new CDummyRepeaterNetworkSet(noteBook, -1, APPLICATION_NAME, gwyAddress, gwyPort, localAddress, localPort);
	noteBook->AddPage(m_network, _("Network"), false);

	m_controller = new CDummyRepeaterControllerSet(noteBook, -1, APPLICATION_NAME, type, config, pttInvert, squelchInvert);
	noteBook->AddPage(m_controller, _("Controller"), false);

	m_timeout = new CDummyRepeaterTimeoutSet(noteBook, -1, APPLICATION_NAME, timeout);
	noteBook->AddPage(m_timeout, _("Timeout"), false);

	m_message = new CDummyRepeaterMessageSet(noteBook, -1, APPLICATION_NAME, message);
	noteBook->AddPage(m_message, _("Message"), false);

	m_bleep = new CDummyRepeaterBleepSet(noteBook, -1, APPLICATION_NAME, bleep);
	noteBook->AddPage(m_bleep, _("Bleep"), false);

	mainSizer->Add(noteBook, 1, wxALL | wxGROW, BORDER_SIZE);

	mainSizer->Add(CreateButtonSizer(wxOK | wxCANCEL), 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	SetAutoLayout(true);
	Layout();

	mainSizer->Fit(this);
	mainSizer->SetSizeHints(this);

	SetSizer(mainSizer);
}

CDummyRepeaterPreferences::~CDummyRepeaterPreferences()
{
}

bool CDummyRepeaterPreferences::Validate()
{
	if (!m_callsign->Validate())
		return false;

	if (!m_soundcard->Validate())
		return false;

	if (!m_dongle->Validate())
		return false;

	if (!m_network->Validate())
		return false;

	if (!m_controller->Validate())
		return false;

	if (!m_timeout->Validate())
		return false;

	if (!m_message->Validate())
		return false;

	return m_bleep->Validate();
}

wxString CDummyRepeaterPreferences::getCallsign1() const
{
	return m_callsign->getCallsign1();
}

wxString CDummyRepeaterPreferences::getCallsign2() const
{
	return m_callsign->getCallsign2();
}

wxString CDummyRepeaterPreferences::getSoundcardReadDevice() const
{
	return m_soundcard->getReadDevice();
}

wxString CDummyRepeaterPreferences::getSoundcardWriteDevice() const
{
	return m_soundcard->getWriteDevice();
}

DONGLE_TYPE CDummyRepeaterPreferences::getDongleType() const
{
	return m_dongle->getType();
}

wxString CDummyRepeaterPreferences::getDongleDevice() const
{
	return m_dongle->getDevice();
}

wxString CDummyRepeaterPreferences::getDongleAddress() const
{
	return m_dongle->getAddress();
}

unsigned int CDummyRepeaterPreferences::getDonglePort() const
{
	return m_dongle->getPort();
}

wxString CDummyRepeaterPreferences::getGatewayAddress() const
{
	return m_network->getGatewayAddress();
}

unsigned int CDummyRepeaterPreferences::getGatewayPort() const
{
	return m_network->getGatewayPort();
}

wxString CDummyRepeaterPreferences::getLocalAddress() const
{
	return m_network->getLocalAddress();
}

unsigned int CDummyRepeaterPreferences::getLocalPort() const
{
	return m_network->getLocalPort();
}

wxString CDummyRepeaterPreferences::getType() const
{
	return m_controller->getType();
}

unsigned int CDummyRepeaterPreferences::getConfig() const
{
	return m_controller->getConfig();
}

bool CDummyRepeaterPreferences::getPTTInvert() const
{
	return m_controller->getPTTInvert();
}

bool CDummyRepeaterPreferences::getSquelchInvert() const
{
	return m_controller->getSquelchInvert();
}

unsigned int CDummyRepeaterPreferences::getTimeout() const
{
	return m_timeout->getTimeout();
}

wxString CDummyRepeaterPreferences::getMessage() const
{
	return m_message->getMessage();
}

bool CDummyRepeaterPreferences::getBleep() const
{
	return m_bleep->getBleep();
}
