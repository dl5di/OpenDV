/*
 *   Copyright (C) 2010,2011,2012 by Jonathan Naylor G4KLX
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

#include "DExtraGatewayReflectorSet.h"
#include "DExtraGatewayHostFile.h"

#include <wx/filename.h>

const unsigned int BORDER_SIZE    = 5U;
const unsigned int CONTROL_WIDTH1 = 130U;
const unsigned int CONTROL_WIDTH2 = 60U;

CDExtraGatewayReflectorSet::CDExtraGatewayReflectorSet(wxWindow* parent, int id, const wxString& title, const wxString& callsign, const wxString& reflector, bool atStartup, RECONNECT reconnect, TEXT_LANG language) :
wxPanel(parent, id),
m_title(title),
m_callsign(NULL),
m_reflector(NULL),
m_channel(NULL),
m_startup(NULL),
m_reconnect(NULL),
m_language(NULL)
{
	wxFlexGridSizer* sizer = new wxFlexGridSizer(3);

	wxStaticText* callsignLabel = new wxStaticText(this, -1, _("Callsign"));
	sizer->Add(callsignLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_callsign = new CCallsignTextCtrl(this, -1, callsign, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	m_callsign->SetMaxLength(LONG_CALLSIGN_LENGTH);
	sizer->Add(m_callsign, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* dummy1Label = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy1Label, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* reflectorLabel = new wxStaticText(this, -1, _("Reflector"));
	sizer->Add(reflectorLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxFileName fileName(wxFileName::GetHomeDir(), HOSTS_FILE_NAME);

	if (!fileName.IsFileReadable()) {
		wxLogMessage(wxT("File %s not readable"), fileName.GetFullPath().c_str());
#if defined(__WINDOWS__)
		fileName.Assign(::wxGetCwd(), HOSTS_FILE_NAME);
#else
		fileName.Assign(wxT(DATA_DIR), HOSTS_FILE_NAME);
#endif
		if (!fileName.IsFileReadable())
			wxLogMessage(wxT("File %s not readable"), fileName.GetFullPath().c_str());
	}

	CDExtraGatewayHostFile file(fileName.GetFullPath());
	unsigned int hostCount = file.getCount();

	m_reflector = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	m_reflector->Append(_("None"));
	for (unsigned int i = 0U; i < hostCount; i++) {
		wxString ref = file.getName(i);
		ref.Append(wxT("        "));
		ref.Truncate(LONG_CALLSIGN_LENGTH - 1U);
		m_reflector->Append(ref);
	}
	sizer->Add(m_reflector, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	if (reflector.IsEmpty()) {
		m_reflector->SetSelection(0);
	} else {
		bool res = m_reflector->SetStringSelection(reflector.Left(LONG_CALLSIGN_LENGTH - 1U));
		if (!res)
			m_reflector->SetSelection(0);
	}

	m_channel = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH2, -1));
	m_channel->Append(wxT("A"));
	m_channel->Append(wxT("B"));
	m_channel->Append(wxT("C"));
	m_channel->Append(wxT("D"));
	m_channel->Append(wxT("E"));
	sizer->Add(m_channel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	bool res = m_channel->SetStringSelection(reflector.Right(1U));
	if (!res)
		m_channel->SetSelection(0);

	wxStaticText* startupLabel = new wxStaticText(this, -1, _("Startup"));
	sizer->Add(startupLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_startup = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	m_startup->Append(_("No"));
	m_startup->Append(_("Yes"));
	sizer->Add(m_startup, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_startup->SetSelection(atStartup ? 1 : 0);

	wxStaticText* dummy2Label = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy2Label, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* reconnectLabel = new wxStaticText(this, -1, _("Reconnect"));
	sizer->Add(reconnectLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_reconnect = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	m_reconnect->Append(_("Fixed"));
	m_reconnect->Append(_("5 minutes"));
	m_reconnect->Append(_("10 minutes"));
	m_reconnect->Append(_("15 minutes"));
	m_reconnect->Append(_("20 minutes"));
	m_reconnect->Append(_("25 minutes"));
	m_reconnect->Append(_("30 minutes"));
	m_reconnect->Append(_("Never"));
	sizer->Add(m_reconnect, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_reconnect->SetSelection(int(reconnect));

	wxStaticText* dummy3Label = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy3Label, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* languageLabel = new wxStaticText(this, -1, _("Language"));
	sizer->Add(languageLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_language = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	m_language->Append(wxT("English (UK)"));
	m_language->Append(wxT("Deutsch"));
	m_language->Append(wxT("Dansk"));
	m_language->Append(wxT("Francais"));
	m_language->Append(wxT("Italiano"));
	m_language->Append(wxT("Polski"));
	m_language->Append(wxT("Espanol"));
	m_language->Append(wxT("Svenska"));
	m_language->Append(wxT("Nederlands"));
	m_language->Append(wxT("English (US)"));
	m_language->Append(wxT("Norsk"));
	sizer->Add(m_language, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_language->SetSelection(int(language));

	SetAutoLayout(true);

	SetSizer(sizer);
}


CDExtraGatewayReflectorSet::~CDExtraGatewayReflectorSet()
{
}

bool CDExtraGatewayReflectorSet::Validate()
{
	bool res = getCallsign().IsEmpty();
	if (res) {
		wxMessageDialog dialog(this, _("The Callsign may not be empty"), m_title + _(" Error"), wxICON_ERROR);
		dialog.ShowModal();
		return false;
	}

	if (m_reflector->GetCurrentSelection() == wxNOT_FOUND)
		return false;

	if (m_channel->GetCurrentSelection() == wxNOT_FOUND)
		return false;

	if (m_startup->GetCurrentSelection() == wxNOT_FOUND)
		return false;

	if (m_reconnect->GetCurrentSelection() == wxNOT_FOUND)
		return false;

	return m_language->GetCurrentSelection() != wxNOT_FOUND;
}

wxString CDExtraGatewayReflectorSet::getCallsign() const
{
	return m_callsign->GetValue();
}

wxString CDExtraGatewayReflectorSet::getReflector() const
{
	int n = m_reflector->GetCurrentSelection();
	int c = m_channel->GetCurrentSelection();

	if (n == 0)
		return wxEmptyString;

	wxString reflector = m_reflector->GetStringSelection();

	reflector.Append(wxT("        "));
	reflector.Truncate(LONG_CALLSIGN_LENGTH - 1U);
	reflector.Append(wxT('A') + c);

	return reflector;
}

bool CDExtraGatewayReflectorSet::atStartup() const
{
	int n = m_startup->GetCurrentSelection();

	return n == 1;
}

RECONNECT CDExtraGatewayReflectorSet::getReconnect() const
{
	int n = m_reconnect->GetCurrentSelection();

	return RECONNECT(n);
}

TEXT_LANG CDExtraGatewayReflectorSet::getLanguage() const
{
	int n = m_language->GetCurrentSelection();
	if (n == wxNOT_FOUND)
		return TL_ENGLISH_UK;

	return TEXT_LANG(n);
}
