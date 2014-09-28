/*
 *   Copyright (C) 2012,2013 by Jonathan Naylor G4KLX
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

#include "TimeServerAnnouncementsSet.h"
#include "DStarDefines.h"

const unsigned int BORDER_SIZE   = 5U;
const unsigned int CONTROL_WIDTH = 150U;

CTimeServerAnnouncementsSet::CTimeServerAnnouncementsSet(wxWindow* parent, int id, const wxString& title, LANGUAGE language, FORMAT format, INTERVAL interval) :
wxPanel(parent, id),
m_title(title),
m_language(NULL),
m_format(NULL),
m_interval(NULL)
{
	wxFlexGridSizer* sizer = new wxFlexGridSizer(2);

	wxStaticText* languageLabel = new wxStaticText(this, -1, _("Language"));
	sizer->Add(languageLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_language = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	m_language->Append(wxT("English 1 (UK)"));
	m_language->Append(wxT("English 2 (UK)"));
	m_language->Append(wxT("English 1 (US)"));
	m_language->Append(wxT("English 2 (US)"));
	m_language->Append(wxT("Deutsch 1"));
	m_language->Append(wxT("Deutsch 2"));
	m_language->Append(wxT("Francais"));
	m_language->Append(wxT("Nederlands"));
	m_language->Append(wxT("Svenska"));
	m_language->Append(wxT("Espanol"));
	m_language->Append(wxT("Norsk"));
	m_language->Append(wxT("Portugues"));
	sizer->Add(m_language, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_language->SetSelection(int(language));

	wxStaticText* formatLabel = new wxStaticText(this, -1, _("Format"));
	sizer->Add(formatLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_format = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	m_format->Append(_("Voice, time only"));
	m_format->Append(_("Voice, callsign + time"));
	m_format->Append(_("Text, time only"));
	sizer->Add(m_format, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_format->SetSelection(int(format));

	wxStaticText* intervalLabel = new wxStaticText(this, -1, _("Interval"));
	sizer->Add(intervalLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_interval = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	m_interval->Append(_("Every 15 minutes"));
	m_interval->Append(_("Every 30 minutes"));
	m_interval->Append(_("Every hour"));
	sizer->Add(m_interval, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_interval->SetSelection(int(interval));

	SetAutoLayout(true);

	SetSizer(sizer);
}


CTimeServerAnnouncementsSet::~CTimeServerAnnouncementsSet()
{
}

bool CTimeServerAnnouncementsSet::Validate()
{
	if (m_language->GetCurrentSelection() == wxNOT_FOUND)
		return false;

	return m_interval->GetCurrentSelection() != wxNOT_FOUND;
}

LANGUAGE CTimeServerAnnouncementsSet::getLanguage() const
{
	int n = m_language->GetCurrentSelection();
	if (n == wxNOT_FOUND)
		return LANG_ENGLISH_UK_1;

	return LANGUAGE(n);
}

FORMAT CTimeServerAnnouncementsSet::getFormat() const
{
	int n = m_format->GetCurrentSelection();
	if (n == wxNOT_FOUND)
		return FORMAT_VOICE_TIME;

	return FORMAT(n);
}

INTERVAL CTimeServerAnnouncementsSet::getInterval() const
{
	int n = m_interval->GetCurrentSelection();
	if (n == wxNOT_FOUND)
		return INTERVAL_15MINS;

	return INTERVAL(n);
}
