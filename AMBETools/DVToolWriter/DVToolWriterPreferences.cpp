/*
 *   Copyright (C) 2009,2010,2014 by Jonathan Naylor G4KLX
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

#include "DVToolWriterPreferences.h"
#include "DVToolWriterDefs.h"

#include <wx/notebook.h>

CDVToolWriterPreferences::CDVToolWriterPreferences(wxWindow* parent, int id, const wxString& callsign1, const wxString& callsign2, DONGLE_TYPE type, const wxString& device, const wxString& address, unsigned int port, const wxString& soundcard) :
wxDialog(parent, id, wxString(_("DVTool Writer Preferences")), wxDefaultPosition, wxDefaultSize, wxRESIZE_BORDER | wxDEFAULT_DIALOG_STYLE),
m_callsign(NULL),
m_soundcard(NULL),
m_dongle(NULL)
{
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	wxNotebook* noteBook = new wxNotebook(this, -1);

	m_callsign = new CCallsignSet(noteBook, -1, APPLICATION_NAME, callsign1, callsign2);
	noteBook->AddPage(m_callsign, _("Callsign"), true);

	m_soundcard = new CSoundcardSet(noteBook, -1, APPLICATION_NAME, soundcard, true, false);
	noteBook->AddPage(m_soundcard, _("Sound Card"), false);

	m_dongle = new CDongleSet(noteBook, -1, APPLICATION_NAME, type, device, address, port);
	noteBook->AddPage(m_dongle, _("Dongle"), false);

	mainSizer->Add(noteBook, 1, wxALL | wxGROW, BORDER_SIZE);

	mainSizer->Add(CreateButtonSizer(wxOK | wxCANCEL), 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	SetAutoLayout(true);
	Layout();

	mainSizer->Fit(this);
	mainSizer->SetSizeHints(this);

	SetSizer(mainSizer);
}

CDVToolWriterPreferences::~CDVToolWriterPreferences()
{
}

bool CDVToolWriterPreferences::Validate()
{
	if (!m_callsign->Validate())
		return false;

	if (!m_soundcard->Validate())
		return false;

	return m_dongle->Validate();
}

wxString CDVToolWriterPreferences::getCallsign1() const
{
	return m_callsign->getCallsign1();
}

wxString CDVToolWriterPreferences::getCallsign2() const
{
	return m_callsign->getCallsign2();
}

DONGLE_TYPE CDVToolWriterPreferences::getType() const
{
	return m_dongle->getType();
}

wxString CDVToolWriterPreferences::getDevice() const
{
	return m_dongle->getDevice();
}

wxString CDVToolWriterPreferences::getAddress() const
{
	return m_dongle->getAddress();
}

unsigned int CDVToolWriterPreferences::getPort() const
{
	return m_dongle->getPort();
}

wxString CDVToolWriterPreferences::getSoundcard() const
{
	return m_soundcard->getReadDevice();
}
