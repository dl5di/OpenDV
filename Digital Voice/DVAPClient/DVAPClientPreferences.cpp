/*
 *   Copyright (C) 2010,2012,2013 by Jonathan Naylor G4KLX
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

#include "DVAPClientPreferences.h"

#include "DVAPClientDefs.h"

CDVAPClientPreferences::CDVAPClientPreferences(wxWindow* parent, int id, const wxString& callsign1,
								   const wxString& callsign2, const wxString& readDevice,
								   const wxString& writeDevice, const wxString& port, unsigned int frequency,
								   int power, int squelch, const wxString& dvdDevice,
								   const wxString& message, bool bleep) :
wxDialog(parent, id, wxString(_("DVAP Client Preferences")), wxDefaultPosition, wxDefaultSize, wxRESIZE_BORDER | wxDEFAULT_DIALOG_STYLE),
m_callsign(NULL),
m_soundcard(NULL),
m_modem(NULL),
m_dongle(NULL),
m_message(NULL),
m_bleep(NULL)
{
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	wxNotebook* noteBook = new wxNotebook(this, -1);

	m_callsign = new CDVAPClientCallsignSet(noteBook, -1, APPLICATION_NAME, callsign1, callsign2);
	noteBook->AddPage(m_callsign, _("Callsign"), true);

	m_soundcard = new CSoundcardSet(noteBook, -1, APPLICATION_NAME, readDevice, writeDevice);
	noteBook->AddPage(m_soundcard, _("Sound Card"), false);

	m_modem = new CDVAPClientModemSet(noteBook, -1, APPLICATION_NAME, port, frequency, power, squelch);
	noteBook->AddPage(m_modem, _("DVAP"), false);

	m_dongle = new CDVDongleSet(noteBook, -1, APPLICATION_NAME, dvdDevice);
	noteBook->AddPage(m_dongle, _("DV-Dongle"), false);

	m_message = new CMessageSet(noteBook, -1, APPLICATION_NAME, message);
	noteBook->AddPage(m_message, _("Message"), false);

	m_bleep = new CBleepSet(noteBook, -1, APPLICATION_NAME, bleep);
	noteBook->AddPage(m_bleep, _("Bleep"), false);

	mainSizer->Add(noteBook, 1, wxALL | wxGROW, BORDER_SIZE);

	mainSizer->Add(CreateButtonSizer(wxOK | wxCANCEL), 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	SetAutoLayout(true);
	Layout();

	mainSizer->Fit(this);
	mainSizer->SetSizeHints(this);

	SetSizer(mainSizer);
}

CDVAPClientPreferences::~CDVAPClientPreferences()
{
}

bool CDVAPClientPreferences::Validate()
{
	if (!m_callsign->Validate())
		return false;

	if (!m_soundcard->Validate())
		return false;

	if (!m_modem->Validate())
		return false;

	if (!m_dongle->Validate())
		return false;

	if (!m_message->Validate())
		return false;

	return m_bleep->Validate();
}

wxString CDVAPClientPreferences::getCallsign1() const
{
	return m_callsign->getCallsign1();
}

wxString CDVAPClientPreferences::getCallsign2() const
{
	return m_callsign->getCallsign2();
}

wxString CDVAPClientPreferences::getSoundcardReadDevice() const
{
	return m_soundcard->getReadDevice();
}

wxString CDVAPClientPreferences::getSoundcardWriteDevice() const
{
	return m_soundcard->getWriteDevice();
}

wxString CDVAPClientPreferences::getPort() const
{
	return m_modem->getPort();
}

unsigned int CDVAPClientPreferences::getFrequency() const
{
	return m_modem->getFrequency();
}

int CDVAPClientPreferences::getPower() const
{
	return m_modem->getPower();
}

int CDVAPClientPreferences::getSquelch() const
{
	return m_modem->getSquelch();
}

wxString CDVAPClientPreferences::getDVDDevice() const
{
	return m_dongle->getDevice();
}

wxString CDVAPClientPreferences::getMessage() const
{
	return m_message->getMessage();
}

bool CDVAPClientPreferences::getBleep() const
{
	return m_bleep->getBleep();
}
