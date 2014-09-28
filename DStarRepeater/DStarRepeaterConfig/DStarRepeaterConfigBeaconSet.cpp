/*
 *   Copyright (C) 2012,2014 by Jonathan Naylor G4KLX
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

#include "DStarRepeaterConfigBeaconSet.h"

const unsigned int MAX_MESSAGE_LENGTH = 20U;

const unsigned int CONTROL_SIZE1 = 200U;
const unsigned int CONTROL_SIZE2 = 100U;
const unsigned int TIMES_WIDTH   = 300U;
const unsigned int BORDER_SIZE   = 5U;

CDStarRepeaterConfigBeaconSet::CDStarRepeaterConfigBeaconSet(wxWindow* parent, int id, const wxString& title, unsigned int time, const wxString& text, bool voice, TEXT_LANG language) :
wxPanel(parent, id),
m_title(title),
m_time(NULL),
m_text(NULL),
m_voice(NULL),
m_language(NULL)
{
	wxFlexGridSizer* sizer = new wxFlexGridSizer(2);

	wxStaticText* timeLabel = new wxStaticText(this, -1, _("Time (mins)"));
	sizer->Add(timeLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_time = new wxSlider(this, -1, time / 60U, 0, 30, wxDefaultPosition, wxSize(TIMES_WIDTH, -1), wxSL_HORIZONTAL | wxSL_LABELS);
	sizer->Add(m_time, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* messageText = new wxStaticText(this, -1, _("Message"));
	sizer->Add(messageText, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_text = new CMessageTextCtrl(this, -1, text, wxDefaultPosition, wxSize(CONTROL_SIZE1, -1));
	m_text->SetMaxLength(MAX_MESSAGE_LENGTH);
	sizer->Add(m_text, 0, wxALL, BORDER_SIZE);

	wxStaticText* voiceText = new wxStaticText(this, -1, _("Voice"));
	sizer->Add(voiceText, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_voice = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_SIZE2, -1));
	m_voice->Append(_("Disabled"));
	m_voice->Append(_("Enabled"));
	sizer->Add(m_voice, 0, wxALL, BORDER_SIZE);
	m_voice->SetSelection(voice ? 1 : 0);

	wxStaticText* languageLabel = new wxStaticText(this, -1, _("Language"));
	sizer->Add(languageLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_language = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_SIZE2, -1));
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

	sizer->Fit(this);
	sizer->SetSizeHints(this);

	SetSizer(sizer);
}


CDStarRepeaterConfigBeaconSet::~CDStarRepeaterConfigBeaconSet()
{
}

bool CDStarRepeaterConfigBeaconSet::Validate()
{
	if (m_voice->GetCurrentSelection() == wxNOT_FOUND)
		return false;

	return m_language->GetCurrentSelection() != wxNOT_FOUND;
}

unsigned int CDStarRepeaterConfigBeaconSet::getTime() const
{
	return m_time->GetValue() * 60U;
}

wxString CDStarRepeaterConfigBeaconSet::getText() const
{
	return m_text->GetValue();
}

bool CDStarRepeaterConfigBeaconSet::getVoice() const
{
	int n = m_voice->GetSelection();

	return n == 1;
}

TEXT_LANG CDStarRepeaterConfigBeaconSet::getLanguage() const
{
	int n = m_language->GetCurrentSelection();
	if (n == wxNOT_FOUND)
		return TL_ENGLISH_UK;

	return TEXT_LANG(n);
}
