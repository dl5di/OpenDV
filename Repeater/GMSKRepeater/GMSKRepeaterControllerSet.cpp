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

#include "GMSKRepeaterControllerSet.h"
#include "SerialDataController.h"

const unsigned int CONTROL_WIDTH = 300U;

const unsigned int BORDER_SIZE = 5U;

CGMSKRepeaterControllerSet::CGMSKRepeaterControllerSet(wxWindow* parent, int id, const wxString& title, const wxString& type, unsigned int time) :
wxPanel(parent, id),
m_title(title),
m_type(NULL),
m_time(NULL)
{
	wxFlexGridSizer* sizer = new wxFlexGridSizer(2);

	wxStaticText* typeLabel = new wxStaticText(this, -1, _("Type"));
	sizer->Add(typeLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_type = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));

	m_type->Append(_("None"));

// XXX ???
#if defined(RASPBERRY_PI)
	m_type->Append(wxT("Raspberry Pi"));
#endif

	m_type->Append(wxT("Velleman K8055 - 0"));
	m_type->Append(wxT("Velleman K8055 - 1"));
	m_type->Append(wxT("Velleman K8055 - 2"));
	m_type->Append(wxT("Velleman K8055 - 3"));

	// Add the Arduino ports
	wxArrayString serialDevs = CSerialDataController::getDevices();
	for (size_t i = 0U; i < serialDevs.GetCount(); i++)
		m_type->Append(wxT("Arduino - ") + serialDevs.Item(i));

	sizer->Add(m_type, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	if (type.IsEmpty())
		m_type->SetSelection(0);
	else
		m_type->SetStringSelection(type);

	wxStaticText* timeLabel = new wxStaticText(this, -1, _("Time (secs)"));
	sizer->Add(timeLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_time = new wxSlider(this, -1, time, 0, 300, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1), wxSL_HORIZONTAL | wxSL_LABELS);
	sizer->Add(m_time, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	SetAutoLayout(true);

	sizer->Fit(this);
	sizer->SetSizeHints(this);

	SetSizer(sizer);
}


CGMSKRepeaterControllerSet::~CGMSKRepeaterControllerSet()
{
}

bool CGMSKRepeaterControllerSet::Validate()
{
	if (m_type->GetCurrentSelection() == wxNOT_FOUND)
		return false;

	return true;
}

wxString CGMSKRepeaterControllerSet::getType() const
{
	wxString type = m_type->GetStringSelection();

	if (type.IsSameAs(_("None")))
		return wxEmptyString;
	else
		return type;
}

unsigned int CGMSKRepeaterControllerSet::getTime() const
{
	return m_time->GetValue();
}
