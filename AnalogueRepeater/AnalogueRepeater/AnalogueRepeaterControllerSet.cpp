/*
 *   Copyright (C) 2009-2013 by Jonathan Naylor G4KLX
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

#include "AnalogueRepeaterControllerSet.h"

#include "SerialDataController.h"
#include "SerialLineController.h"

const unsigned int BORDER_SIZE    = 5U;
const unsigned int CONTROL_WIDTH1 = 300U;
const unsigned int CONTROL_WIDTH2 = 100U;

CAnalogueRepeaterControllerSet::CAnalogueRepeaterControllerSet(wxWindow* parent, int id, const wxString& title, const wxString& type, unsigned int config, unsigned int pttDelay, unsigned int squelchDelay, bool pttInvert, bool squelchInvert) :
wxPanel(parent, id),
m_title(title),
m_type(NULL),
m_config(NULL),
m_pttDelay(NULL),
m_squelchDelay(NULL),
m_pttInvert(NULL),
m_squelchInvert(NULL)
{
	wxFlexGridSizer* sizer = new wxFlexGridSizer(2);

	wxStaticText* typeLabel = new wxStaticText(this, -1, _("Type"));
	sizer->Add(typeLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_type = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));

	m_type->Append(_("None"));

#if defined(RASPBERRY_PI)
	m_type->Append(wxT("Raspberry Pi"));
#endif

	// Add the Velleman K8055
	m_type->Append(wxT("Velleman K8055 - 0"));
	m_type->Append(wxT("Velleman K8055 - 1"));
	m_type->Append(wxT("Velleman K8055 - 2"));
	m_type->Append(wxT("Velleman K8055 - 3"));

	// Add the serial ports
	wxArrayString serialDevs = CSerialLineController::getDevices();
	for (size_t i = 0U; i < serialDevs.GetCount(); i++)
		m_type->Append(wxT("Serial - ") + serialDevs.Item(i));

	// Add the Arduino ports
	serialDevs = CSerialDataController::getDevices();
	for (size_t i = 0U; i < serialDevs.GetCount(); i++)
		m_type->Append(wxT("Arduino - ") + serialDevs.Item(i));

	sizer->Add(m_type, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* configLabel = new wxStaticText(this, -1, _("Config"));
	sizer->Add(configLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_config = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH2, -1));
	m_config->Append(wxT("1"));
	m_config->Append(wxT("2"));
	m_config->Append(wxT("3"));
	sizer->Add(m_config, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_config->SetSelection(config - 1);

	wxStaticText* pttDelayLabel = new wxStaticText(this, -1, _("PTT Delay"));
	sizer->Add(pttDelayLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_pttDelay = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH2, -1));
	m_pttDelay->Append(_("None"));
	m_pttDelay->Append(wxT("20 ms"));
	m_pttDelay->Append(wxT("40 ms"));
	m_pttDelay->Append(wxT("60 ms"));
	m_pttDelay->Append(wxT("80 ms"));
	m_pttDelay->Append(wxT("100 ms"));
	m_pttDelay->Append(wxT("120 ms"));
	m_pttDelay->Append(wxT("140 ms"));
	m_pttDelay->Append(wxT("160 ms"));
	m_pttDelay->Append(wxT("180 ms"));
	m_pttDelay->Append(wxT("200 ms"));
	m_pttDelay->Append(wxT("220 ms"));
	m_pttDelay->Append(wxT("240 ms"));
	m_pttDelay->Append(wxT("260 ms"));
	m_pttDelay->Append(wxT("280 ms"));
	m_pttDelay->Append(wxT("300 ms"));
	m_pttDelay->Append(wxT("320 ms"));
	m_pttDelay->Append(wxT("340 ms"));
	m_pttDelay->Append(wxT("360 ms"));
	m_pttDelay->Append(wxT("380 ms"));
	m_pttDelay->Append(wxT("400 ms"));
	m_pttDelay->Append(wxT("420 ms"));
	m_pttDelay->Append(wxT("440 ms"));
	m_pttDelay->Append(wxT("460 ms"));
	m_pttDelay->Append(wxT("480 ms"));
	m_pttDelay->Append(wxT("500 ms"));
	sizer->Add(m_pttDelay, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_pttDelay->SetSelection(pttDelay);

	wxStaticText* squelchDelayLabel = new wxStaticText(this, -1, _("Squelch Delay"));
	sizer->Add(squelchDelayLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_squelchDelay = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH2, -1));
	m_squelchDelay->Append(_("None"));
	m_squelchDelay->Append(wxT("20 ms"));
	m_squelchDelay->Append(wxT("40 ms"));
	m_squelchDelay->Append(wxT("60 ms"));
	m_squelchDelay->Append(wxT("80 ms"));
	m_squelchDelay->Append(wxT("100 ms"));
	sizer->Add(m_squelchDelay, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_squelchDelay->SetSelection(squelchDelay);

	wxStaticText* pttInvertLabel = new wxStaticText(this, -1, _("PTT Inversion"));
	sizer->Add(pttInvertLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_pttInvert = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH2, -1));
	m_pttInvert->Append(_("Off"));
	m_pttInvert->Append(_("On"));
	sizer->Add(m_pttInvert, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* squelchInvertLabel = new wxStaticText(this, -1, _("Squelch Inversion"));
	sizer->Add(squelchInvertLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_squelchInvert = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH2, -1));
	m_squelchInvert->Append(_("Off"));
	m_squelchInvert->Append(_("On"));
	sizer->Add(m_squelchInvert, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	if (type.IsEmpty())
		m_type->SetSelection(0);
	else
		m_type->SetStringSelection(type);

	m_pttInvert->SetSelection(pttInvert ? 1 : 0);
	m_squelchInvert->SetSelection(squelchInvert ? 1 : 0);

	SetAutoLayout(true);

	sizer->Fit(this);
	sizer->SetSizeHints(this);

	SetSizer(sizer);
}


CAnalogueRepeaterControllerSet::~CAnalogueRepeaterControllerSet()
{
}

bool CAnalogueRepeaterControllerSet::Validate()
{
	if (m_type->GetCurrentSelection() == wxNOT_FOUND)
		return false;

	if (m_config->GetCurrentSelection() == wxNOT_FOUND)
		return false;

	if (m_pttDelay->GetCurrentSelection() == wxNOT_FOUND)
		return false;

	if (m_pttInvert->GetCurrentSelection() == wxNOT_FOUND)
		return false;

	return m_squelchInvert->GetCurrentSelection() != wxNOT_FOUND;
}

wxString CAnalogueRepeaterControllerSet::getType() const
{
	wxString type = m_type->GetStringSelection();

	if (type.IsSameAs(_("None")))
		return wxEmptyString;
	else
		return type;
}

unsigned int CAnalogueRepeaterControllerSet::getConfig() const
{
	int n = m_config->GetCurrentSelection();

	if (n == wxNOT_FOUND)
		return 1U;
	else
		return n + 1U;
}

unsigned int CAnalogueRepeaterControllerSet::getPTTDelay() const
{
	int n = m_pttDelay->GetCurrentSelection();

	if (n == wxNOT_FOUND)
		return 0U;
	else
		return n;
}

unsigned int CAnalogueRepeaterControllerSet::getSquelchDelay() const
{
	int n = m_squelchDelay->GetCurrentSelection();

	if (n == wxNOT_FOUND)
		return 0U;
	else
		return n;
}

bool CAnalogueRepeaterControllerSet::getPTTInvert() const
{
	int n = m_pttInvert->GetCurrentSelection();

	switch (n) {
		case 0:
			return false;
		case 1:
			return true;
		default:
			return false;
	}
}

bool CAnalogueRepeaterControllerSet::getSquelchInvert() const
{
	int n = m_squelchInvert->GetCurrentSelection();

	switch (n) {
		case 0:
			return false;
		case 1:
			return true;
		default:
			return false;
	}
}
