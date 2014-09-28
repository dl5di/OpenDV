/*
 *   Copyright (C) 2012,2013,2014 by Jonathan Naylor G4KLX
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

#include "DStarRepeaterConfigControllerSet.h"
#include "SerialDataController.h"
#include "SerialLineController.h"

const unsigned int CONTROL_WIDTH1 = 150U;
const unsigned int CONTROL_WIDTH2 = 300U;
const unsigned int CONTROL_WIDTH3 = 80U;

const unsigned int BORDER_SIZE = 5U;

CDStarRepeaterConfigControllerSet::CDStarRepeaterConfigControllerSet(wxWindow* parent, int id, const wxString& title, const wxString& type, unsigned int config, bool pttInvert, unsigned int time) :
wxPanel(parent, id),
m_title(title),
m_type(NULL),
m_config(NULL),
m_pttInvert(NULL),
m_time(NULL)
{
	wxFlexGridSizer* sizer = new wxFlexGridSizer(2);

	wxStaticText* typeLabel = new wxStaticText(this, -1, _("Type"));
	sizer->Add(typeLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_type = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));

	m_type->Append(_("None"));

// XXX ???
#if defined(RASPBERRY_PI)
	m_type->Append(wxT("Raspberry Pi"));
#endif

	m_type->Append(wxT("Velleman K8055 - 0"));
	m_type->Append(wxT("Velleman K8055 - 1"));
	m_type->Append(wxT("Velleman K8055 - 2"));
	m_type->Append(wxT("Velleman K8055 - 3"));

	// Add the Serial ports
	wxArrayString serialDevs = CSerialLineController::getDevices();
	for (size_t i = 0U; i < serialDevs.GetCount(); i++)
		m_type->Append(wxT("Serial - ") + serialDevs.Item(i));

	// Add the Arduino ports
	serialDevs = CSerialDataController::getDevices();
	for (size_t i = 0U; i < serialDevs.GetCount(); i++)
		m_type->Append(wxT("Arduino - ") + serialDevs.Item(i));

	sizer->Add(m_type, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	if (type.IsEmpty())
		m_type->SetSelection(0);
	else
		m_type->SetStringSelection(type);

	wxStaticText* configLabel = new wxStaticText(this, -1, _("Config"));
	sizer->Add(configLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_config = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH3, -1));
	m_config->Append(wxT("1"));
	m_config->Append(wxT("2"));
	m_config->Append(wxT("3"));
	sizer->Add(m_config, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_config->SetSelection(config - 1);

	wxStaticText* pttInvertLabel = new wxStaticText(this, -1, _("PTT Inversion"));
	sizer->Add(pttInvertLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_pttInvert = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH3, -1));
	m_pttInvert->Append(_("Off"));
	m_pttInvert->Append(_("On"));
	sizer->Add(m_pttInvert, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_pttInvert->SetSelection(pttInvert ? 1 : 0);

	wxStaticText* timeLabel = new wxStaticText(this, -1, _("Time (secs)"));
	sizer->Add(timeLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_time = new wxSlider(this, -1, time, 0, 300, wxDefaultPosition, wxSize(CONTROL_WIDTH2, -1), wxSL_HORIZONTAL | wxSL_LABELS);
	sizer->Add(m_time, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	SetAutoLayout(true);

	sizer->Fit(this);
	sizer->SetSizeHints(this);

	SetSizer(sizer);
}


CDStarRepeaterConfigControllerSet::~CDStarRepeaterConfigControllerSet()
{
}

bool CDStarRepeaterConfigControllerSet::Validate()
{
	if (m_type->GetCurrentSelection() == wxNOT_FOUND)
		return false;

	if (m_config->GetCurrentSelection() == wxNOT_FOUND)
		return false;

	if (m_pttInvert->GetCurrentSelection() == wxNOT_FOUND)
		return false;

	return true;
}

wxString CDStarRepeaterConfigControllerSet::getType() const
{
	wxString type = m_type->GetStringSelection();

	if (type.IsSameAs(_("None")))
		return wxEmptyString;
	else
		return type;
}

unsigned int CDStarRepeaterConfigControllerSet::getConfig() const
{
	int n = m_config->GetCurrentSelection();

	if (n == wxNOT_FOUND)
		return 1U;
	else
		return n + 1U;
}

bool CDStarRepeaterConfigControllerSet::getPTTInvert() const
{
	int n = m_pttInvert->GetCurrentSelection();

	return n == 1;
}

unsigned int CDStarRepeaterConfigControllerSet::getTime() const
{
	return m_time->GetValue();
}
