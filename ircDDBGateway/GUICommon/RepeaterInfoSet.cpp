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

#include "RepeaterInfoSet.h"
#include "DStarDefines.h"

const unsigned int CONTROL_WIDTH1 = 130U;
const unsigned int CONTROL_WIDTH2 = 80U;
const unsigned int CONTROL_WIDTH3 = 40U;

const unsigned int DESCRIPTION_LENGTH = 20U;
const unsigned int FREQUENCY_LENGTH   = 9U;
const unsigned int OFFSET_LENGTH      = 6U;
const unsigned int PORT_LENGTH        = 5U;
const unsigned int URL_LENGTH         = 40U;

const unsigned int BORDER_SIZE = 5U;


CRepeaterInfoSet::CRepeaterInfoSet(wxWindow* parent, int id, const wxString& title, double frequency, double offset, double range, double latitude, double longitude, double agl, const wxString& description1, const wxString& description2, const wxString& url) :
wxPanel(parent, id),
m_title(title),
m_frequency(NULL),
m_offset(NULL),
m_range(NULL),
m_latitude(NULL),
m_longitude(NULL),
m_agl(NULL),
m_description1(NULL),
m_description2(NULL),
m_url(NULL)
{
	wxFlexGridSizer* sizer = new wxFlexGridSizer(2);

	wxStaticText* frequencyLabel = new wxStaticText(this, -1, _("Frequency (MHz)"));
	sizer->Add(frequencyLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxString buffer;
	buffer.Printf(wxT("%.5lf"), frequency);

	m_frequency = new wxTextCtrl(this, -1, buffer, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	m_frequency->SetMaxLength(FREQUENCY_LENGTH);
	sizer->Add(m_frequency, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* offsetLabel = new wxStaticText(this, -1, _("Offset (MHz)"));
	sizer->Add(offsetLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	buffer.Printf(wxT("%.4lf"), offset);

	m_offset = new wxTextCtrl(this, -1, buffer, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	m_offset->SetMaxLength(OFFSET_LENGTH);
	sizer->Add(m_offset, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* rangeLabel = new wxStaticText(this, -1, _("Range (kms)"));
	sizer->Add(rangeLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	buffer.Printf(wxT("%.0lf"), range);

	m_range = new wxTextCtrl(this, -1, buffer, wxDefaultPosition, wxSize(CONTROL_WIDTH2, -1));
	m_range->SetMaxLength(PORT_LENGTH);
	sizer->Add(m_range, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* latitudeLabel = new wxStaticText(this, -1, _("Latitude"));
	sizer->Add(latitudeLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

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

	buffer.Printf(wxT("%.0lf"), agl);

	m_agl = new wxTextCtrl(this, -1, buffer, wxDefaultPosition, wxSize(CONTROL_WIDTH2, -1));
	m_agl->SetMaxLength(PORT_LENGTH);
	sizer->Add(m_agl, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* descriptionLabel = new wxStaticText(this, -1, _("QTH"));
	sizer->Add(descriptionLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_description1 = new CDescriptionTextCtrl(this, -1, description1, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	m_description1->SetMaxLength(DESCRIPTION_LENGTH);
	sizer->Add(m_description1, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* dummyLabel = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummyLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_description2 = new CDescriptionTextCtrl(this, -1, description2, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	m_description2->SetMaxLength(DESCRIPTION_LENGTH);
	sizer->Add(m_description2, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* urlLabel = new wxStaticText(this, -1, _("URL"));
	sizer->Add(urlLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_url = new CDescriptionTextCtrl(this, -1, url, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	m_url->SetMaxLength(URL_LENGTH);
	sizer->Add(m_url, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	SetAutoLayout(true);

	SetSizer(sizer);
}


CRepeaterInfoSet::~CRepeaterInfoSet()
{
}

bool CRepeaterInfoSet::Validate()
{
	double latitude = getLatitude();

	if (latitude < -90.0 || latitude > 90.0) {
		wxMessageDialog dialog(this, _("The Latitude is invalid"), m_title + _(" Error"), wxICON_ERROR);
		dialog.ShowModal();
		return false;
	}

	double longitude = getLongitude();

	if (longitude < -180.0 || longitude > 180.0) {
		wxMessageDialog dialog(this, _("The Longitude is invalid"), m_title + _(" Error"), wxICON_ERROR);
		dialog.ShowModal();
		return false;
	}

	return true;
}

double CRepeaterInfoSet::getFrequency() const
{
	double n;
	m_frequency->GetValue().ToDouble(&n);

	return n;
}

double CRepeaterInfoSet::getOffset() const
{
	double n;
	m_offset->GetValue().ToDouble(&n);

	return n;
}

double CRepeaterInfoSet::getRange() const
{
	double n;
	m_range->GetValue().ToDouble(&n);

	return n;
}

double CRepeaterInfoSet::getLatitude() const
{
	double val;
	
	m_latitude->GetValue().ToDouble(&val);

	return val;
}

double CRepeaterInfoSet::getLongitude() const
{
	double val;
	
	m_longitude->GetValue().ToDouble(&val);

	return val;
}

wxString CRepeaterInfoSet::getDescription1() const
{
	return m_description1->GetValue();
}

wxString CRepeaterInfoSet::getDescription2() const
{
	return m_description2->GetValue();
}

wxString CRepeaterInfoSet::getURL() const
{
	return m_url->GetValue();
}

double CRepeaterInfoSet::getAGL() const
{
	double n;
	m_agl->GetValue().ToDouble(&n);

	return n;
}
