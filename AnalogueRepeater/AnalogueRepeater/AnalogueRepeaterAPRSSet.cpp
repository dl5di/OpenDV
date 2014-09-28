/*
 *   Copyright (C) 2013,2014 by Jonathan Naylor G4KLX
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

#include "AnalogueRepeaterAPRSSet.h"

const unsigned int BORDER_SIZE    = 5U;
const unsigned int CONTROL_WIDTH1 = 80U;
const unsigned int CONTROL_WIDTH2 = 200U;

CAnalogueRepeaterAPRSSet::CAnalogueRepeaterAPRSSet(wxWindow* parent, int id, const wxString& title, bool txEnabled, const wxString& callsign, wxFloat32 latitude, wxFloat32 longitude, int height, const wxString& description) :
wxPanel(parent, id),
m_title(title),
m_txEnabled(NULL),
m_callsign(NULL),
m_latitude(NULL),
m_longitude(NULL),
m_height(NULL),
m_description(NULL)
{
	wxFlexGridSizer* sizer = new wxFlexGridSizer(2);

	wxStaticText* txEnabledLabel = new wxStaticText(this, -1, _("Transmit"));
	sizer->Add(txEnabledLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_txEnabled = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	m_txEnabled->Append(_("Disabled"));
	m_txEnabled->Append(_("Enabled"));
	sizer->Add(m_txEnabled, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_txEnabled->SetSelection(txEnabled ? 1 : 0);

	wxStaticText* callsignLabel = new wxStaticText(this, -1, _("Callsign"));
	sizer->Add(callsignLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxString call = callsign;
	call.Truncate(6U);
	call.Trim();

	m_callsign = new CCallsignTextCtrl(this, -1, call, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	m_callsign->SetMaxLength(6U);
	sizer->Add(m_callsign, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* latitudeLabel = new wxStaticText(this, -1, _("Latitude"));
	sizer->Add(latitudeLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxString buffer;
	buffer.Printf(wxT("%lf"), latitude);

	m_latitude = new wxTextCtrl(this, -1, buffer, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	sizer->Add(m_latitude, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* longitudeLabel = new wxStaticText(this, -1, _("Longitude"));
	sizer->Add(longitudeLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	buffer.Printf(wxT("%lf"), longitude);

	m_longitude = new wxTextCtrl(this, -1, buffer, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	sizer->Add(m_longitude, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* aglLabel = new wxStaticText(this, -1, _("AGL (m)"));
	sizer->Add(aglLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	buffer.Printf(wxT("%d"), height);

	m_height = new wxTextCtrl(this, -1, buffer, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	sizer->Add(m_height, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* descriptionLabel = new wxStaticText(this, -1, _("Description"));
	sizer->Add(descriptionLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_description = new wxTextCtrl(this, -1, description, wxDefaultPosition, wxSize(CONTROL_WIDTH2, -1));
	m_description->SetMaxLength(100U);
	sizer->Add(m_description, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	SetAutoLayout(true);

	SetSizer(sizer);
}


CAnalogueRepeaterAPRSSet::~CAnalogueRepeaterAPRSSet()
{
}

bool CAnalogueRepeaterAPRSSet::Validate()
{
	int n = m_txEnabled->GetCurrentSelection();
	if (n == wxNOT_FOUND)
		return false;

	if (n == 0)
		return true;

	wxString callsign = m_callsign->GetValue();
	callsign.Trim();
	if (callsign.IsEmpty())
		return false;

	wxFloat32 lat = getLatitude();
	wxFloat32 lon = getLongitude();
	if (lat == 0.0F && lon == 0.0F)
		return false;

	return true;
}

bool CAnalogueRepeaterAPRSSet::getTXEnabled() const
{
	int c = m_txEnabled->GetCurrentSelection();
	if (c == wxNOT_FOUND)
		return false;

	return c == 1;
}

wxString CAnalogueRepeaterAPRSSet::getCallsign() const
{
	wxString callsign = m_callsign->GetValue();

	callsign.MakeUpper();

	return callsign;
}

wxFloat32 CAnalogueRepeaterAPRSSet::getLatitude() const
{
	wxString text = m_latitude->GetValue();

	double val;
	text.ToDouble(&val);

	return wxFloat32(val);
}

wxFloat32 CAnalogueRepeaterAPRSSet::getLongitude() const
{
	wxString text = m_longitude->GetValue();

	double val;
	text.ToDouble(&val);

	return wxFloat32(val);
}

int CAnalogueRepeaterAPRSSet::getHeight() const
{
	wxString text = m_height->GetValue();

	long val;
	text.ToLong(&val);

	return int(val);
}

wxString CAnalogueRepeaterAPRSSet::getDescription() const
{
	return m_description->GetValue();
}
