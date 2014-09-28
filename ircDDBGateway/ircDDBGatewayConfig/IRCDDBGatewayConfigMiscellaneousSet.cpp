/*
 *   Copyright (C) 2010-2013 by Jonathan Naylor G4KLX
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

#include "IRCDDBGatewayConfigMiscellaneousSet.h"
#include "DStarDefines.h"

const unsigned int CONTROL_WIDTH = 130U;

const unsigned int BORDER_SIZE = 5U;

CIRCDDBGatewayConfigMiscellaneousSet::CIRCDDBGatewayConfigMiscellaneousSet(wxWindow* parent, int id, const wxString& title, TEXT_LANG language, bool infoEnabled, bool echoEnabled, bool logEnabled, bool dratsEnabled, bool dtmfEnabled) :
wxPanel(parent, id),
m_title(title),
m_language(NULL),
m_infoEnabled(NULL),
m_echoEnabled(NULL),
m_logEnabled(NULL),
m_dratsEnabled(NULL),
m_dtmfEnabled(NULL)
{
	wxFlexGridSizer* sizer = new wxFlexGridSizer(2);

	wxStaticText* languageLabel = new wxStaticText(this, -1, _("Language"));
	sizer->Add(languageLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_language = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	m_language->Append(wxT("English (UK)"));
	m_language->Append(wxT("Deutsch"));
	m_language->Append(wxT("Dansk"));
	m_language->Append(wxT("Francais"));
	m_language->Append(wxT("Italiano"));
	m_language->Append(wxT("Polski"));
	m_language->Append(wxT("English (US)"));
	m_language->Append(wxT("Espanol"));
	m_language->Append(wxT("Svenska"));
	m_language->Append(wxT("Nederlands (NL)"));
	m_language->Append(wxT("Nederlands (BE)"));
	m_language->Append(wxT("Norsk"));
	m_language->Append(wxT("Portugues"));
	sizer->Add(m_language, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_language->SetSelection(int(language));

	wxStaticText* infoEnabledLabel = new wxStaticText(this, -1, _("Info Command"));
	sizer->Add(infoEnabledLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_infoEnabled = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	m_infoEnabled->Append(_("Disabled"));
	m_infoEnabled->Append(_("Enabled"));
	sizer->Add(m_infoEnabled, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_infoEnabled->SetSelection(infoEnabled ? 1 : 0);

	wxStaticText* echoEnabledLabel = new wxStaticText(this, -1, _("Echo Command"));
	sizer->Add(echoEnabledLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_echoEnabled = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	m_echoEnabled->Append(_("Disabled"));
	m_echoEnabled->Append(_("Enabled"));
	sizer->Add(m_echoEnabled, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_echoEnabled->SetSelection(echoEnabled ? 1 : 0);

	wxStaticText* logEnabledLabel = new wxStaticText(this, -1, _("GUI Log"));
	sizer->Add(logEnabledLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_logEnabled = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	m_logEnabled->Append(_("Disabled"));
	m_logEnabled->Append(_("Enabled"));
	sizer->Add(m_logEnabled, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_logEnabled->SetSelection(logEnabled ? 1 : 0);

	wxStaticText* dratsEnabledLabel = new wxStaticText(this, -1, wxT("D-RATS"));
	sizer->Add(dratsEnabledLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_dratsEnabled = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	m_dratsEnabled->Append(_("Disabled"));
	m_dratsEnabled->Append(_("Enabled"));
	sizer->Add(m_dratsEnabled, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_dratsEnabled->SetSelection(dratsEnabled ? 1 : 0);

	wxStaticText* dtmfEnabledLabel = new wxStaticText(this, -1, wxT("DTMF Control"));
	sizer->Add(dtmfEnabledLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_dtmfEnabled = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	m_dtmfEnabled->Append(_("Disabled"));
	m_dtmfEnabled->Append(_("Enabled"));
	sizer->Add(m_dtmfEnabled, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_dtmfEnabled->SetSelection(dtmfEnabled ? 1 : 0);

	SetAutoLayout(true);

	SetSizer(sizer);
}


CIRCDDBGatewayConfigMiscellaneousSet::~CIRCDDBGatewayConfigMiscellaneousSet()
{
}

bool CIRCDDBGatewayConfigMiscellaneousSet::Validate()
{
	if (m_language->GetCurrentSelection() == wxNOT_FOUND)
		return false;

	if (m_logEnabled->GetCurrentSelection() == wxNOT_FOUND)
		return false;

	if (m_infoEnabled->GetCurrentSelection() == wxNOT_FOUND)
		return false;

	if (m_echoEnabled->GetCurrentSelection() == wxNOT_FOUND)
		return false;

	if (m_dratsEnabled->GetCurrentSelection() == wxNOT_FOUND)
		return false;

	return m_dtmfEnabled->GetCurrentSelection() != wxNOT_FOUND;
}

TEXT_LANG CIRCDDBGatewayConfigMiscellaneousSet::getLanguage() const
{
	int c = m_language->GetCurrentSelection();
	if (c == wxNOT_FOUND)
		return TL_ENGLISH_UK;

	return TEXT_LANG(c);
}

bool CIRCDDBGatewayConfigMiscellaneousSet::getInfoEnabled() const
{
	int c = m_infoEnabled->GetCurrentSelection();
	if (c == wxNOT_FOUND)
		return false;

	return c == 1;
}

bool CIRCDDBGatewayConfigMiscellaneousSet::getEchoEnabled() const
{
	int c = m_echoEnabled->GetCurrentSelection();
	if (c == wxNOT_FOUND)
		return false;

	return c == 1;
}

bool CIRCDDBGatewayConfigMiscellaneousSet::getLogEnabled() const
{
	int c = m_logEnabled->GetCurrentSelection();
	if (c == wxNOT_FOUND)
		return false;

	return c == 1;
}

bool CIRCDDBGatewayConfigMiscellaneousSet::getDRATSEnabled() const
{
	int c = m_dratsEnabled->GetCurrentSelection();
	if (c == wxNOT_FOUND)
		return false;

	return c == 1;
}

bool CIRCDDBGatewayConfigMiscellaneousSet::getDTMFEnabled() const
{
	int c = m_dtmfEnabled->GetCurrentSelection();
	if (c == wxNOT_FOUND)
		return false;

	return c == 1;
}
