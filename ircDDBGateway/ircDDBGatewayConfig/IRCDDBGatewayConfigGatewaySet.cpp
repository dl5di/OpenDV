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

#include "DStarDefines.h"
#include "IRCDDBGatewayConfigGatewaySet.h"
#include <cctype>

const unsigned int DESCRIPTION_WIDTH = 120U;
const unsigned int ADDRESS_WIDTH     = 120U;
const unsigned int PORT_WIDTH        = 80U;

const unsigned int DESCRIPTION_LENGTH = 20U;
const unsigned int ADDRESS_LENGTH     = 15U;
const unsigned int PORT_LENGTH        = 5U;

const unsigned int BORDER_SIZE = 5U;

CIRCDDBGatewayConfigGatewaySet::CIRCDDBGatewayConfigGatewaySet(wxWindow* parent, int id, const wxString& title, GATEWAY_TYPE type, const wxString& callsign, const wxString& address, const wxString& icomAddress, unsigned int icomPort, const wxString& hbAddress, unsigned int hbPort, double latitude, double longitude, const wxString& description1, const wxString& description2, const wxString& url) :
wxPanel(parent, id),
m_title(title),
m_type(NULL),
m_callsign(NULL),
m_address(NULL),
m_icomAddress(NULL),
m_icomPort(NULL),
m_hbAddress(NULL),
m_hbPort(NULL),
m_latitude(NULL),
m_longitude(NULL),
m_description1(NULL),
m_description2(NULL),
m_url(NULL)
{
	wxFlexGridSizer* sizer = new wxFlexGridSizer(3);

	wxStaticText* typeLabel = new wxStaticText(this, -1, _("Type"));
	sizer->Add(typeLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_type = new wxChoice(this, -1);
	m_type->Append(_("Repeater"));
	m_type->Append(_("Hotspot"));
	m_type->Append(_("Dongle"));
	sizer->Add(m_type, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_type->SetSelection(int(type));

	wxStaticText* dummy10Label = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy10Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxStaticText* callsignLabel = new wxStaticText(this, -1, _("Callsign"));
	sizer->Add(callsignLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxString call = callsign;
	call.Truncate(LONG_CALLSIGN_LENGTH - 1U);
	call.Trim();

	m_callsign = new CCallsignTextCtrl(this, -1, call, wxDefaultPosition, wxSize(ADDRESS_WIDTH, -1));
	m_callsign->SetMaxLength(LONG_CALLSIGN_LENGTH);
	sizer->Add(m_callsign, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* gLabel = new wxStaticText(this, -1, wxT("G"));
	sizer->Add(gLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxStaticText* addressLabel = new wxStaticText(this, -1, _("Gateway Address"));
	sizer->Add(addressLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_address = new CAddressTextCtrl(this, -1, address, wxDefaultPosition, wxSize(ADDRESS_WIDTH, -1));
	m_address->SetMaxLength(ADDRESS_LENGTH);
	sizer->Add(m_address, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* dummy0Label = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy0Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxStaticText* icomAddressLabel = new wxStaticText(this, -1, _("Local Icom Address"));
	sizer->Add(icomAddressLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_icomAddress = new CAddressTextCtrl(this, -1, icomAddress, wxDefaultPosition, wxSize(ADDRESS_WIDTH, -1));
	m_icomAddress->SetMaxLength(ADDRESS_LENGTH);
	sizer->Add(m_icomAddress, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* dummy1Label = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy1Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxStaticText* icomPortLabel = new wxStaticText(this, -1, _("Local Icom Port"));
	sizer->Add(icomPortLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxString buffer;
	buffer.Printf(wxT("%u"), icomPort);

	m_icomPort = new CPortTextCtrl(this, -1, buffer, wxDefaultPosition, wxSize(PORT_WIDTH, -1));
	m_icomPort->SetMaxLength(PORT_LENGTH);
	sizer->Add(m_icomPort, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* dummy2Label = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy2Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxStaticText* hbAddressLabel = new wxStaticText(this, -1, _("Local HB Address"));
	sizer->Add(hbAddressLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_hbAddress = new CAddressTextCtrl(this, -1, hbAddress, wxDefaultPosition, wxSize(ADDRESS_WIDTH, -1));
	m_hbAddress->SetMaxLength(ADDRESS_LENGTH);
	sizer->Add(m_hbAddress, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* dummy3Label = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy3Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxStaticText* hbPortLabel = new wxStaticText(this, -1, _("Local HB Port"));
	sizer->Add(hbPortLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	buffer.Printf(wxT("%u"), hbPort);

	m_hbPort = new CPortTextCtrl(this, -1, buffer, wxDefaultPosition, wxSize(PORT_WIDTH, -1));
	m_hbPort->SetMaxLength(PORT_LENGTH);
	sizer->Add(m_hbPort, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* dummy4Label = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy4Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxStaticText* latitudeLabel = new wxStaticText(this, -1, _("Latitude"));
	sizer->Add(latitudeLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	buffer.Printf(wxT("%lf"), latitude);

	m_latitude = new wxTextCtrl(this, -1, buffer, wxDefaultPosition, wxSize(PORT_WIDTH, -1));
	sizer->Add(m_latitude, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* dummy5Label = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy5Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxStaticText* longitudeLabel = new wxStaticText(this, -1, _("Longitude"));
	sizer->Add(longitudeLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	buffer.Printf(wxT("%lf"), longitude);

	m_longitude = new wxTextCtrl(this, -1, buffer, wxDefaultPosition, wxSize(PORT_WIDTH, -1));
	sizer->Add(m_longitude, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* dummy6Label = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy6Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxStaticText* descriptionLabel = new wxStaticText(this, -1, _("QTH"));
	sizer->Add(descriptionLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_description1 = new CDescriptionTextCtrl(this, -1, description1, wxDefaultPosition, wxSize(DESCRIPTION_WIDTH, -1));
	m_description1->SetMaxLength(DESCRIPTION_LENGTH);
	sizer->Add(m_description1, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* dummy7Label = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy7Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxStaticText* dummy8Label = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy8Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_description2 = new CDescriptionTextCtrl(this, -1, description2, wxDefaultPosition, wxSize(DESCRIPTION_WIDTH, -1));
	m_description2->SetMaxLength(DESCRIPTION_LENGTH);
	sizer->Add(m_description2, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* dummy9Label = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy9Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxStaticText* urlLabel = new wxStaticText(this, -1, _("URL"));
	sizer->Add(urlLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_url = new CDescriptionTextCtrl(this, -1, url, wxDefaultPosition, wxSize(DESCRIPTION_WIDTH, -1));
	sizer->Add(m_url, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	SetAutoLayout(true);

	SetSizer(sizer);
}


CIRCDDBGatewayConfigGatewaySet::~CIRCDDBGatewayConfigGatewaySet()
{
}

bool CIRCDDBGatewayConfigGatewaySet::Validate()
{
	int n = m_type->GetCurrentSelection();
	if (n == wxNOT_FOUND)
		return false;

	wxString callsign = getCallsign();

	if (callsign.IsEmpty()) {
		wxMessageDialog dialog(this, _("The Gateway Callsign is not valid"), m_title + _(" Error"), wxICON_ERROR);
		dialog.ShowModal();
		return false;
	}

	unsigned int port = getIcomPort();

	if (port == 0U || port > 65535U) {
		wxMessageDialog dialog(this, _("The Icom Port is not valid"), m_title + _(" Error"), wxICON_ERROR);
		dialog.ShowModal();
		return false;
	}

	port = getHBPort();

	if (port == 0U || port > 65535U) {
		wxMessageDialog dialog(this, _("The Homebrew Port is not valid"), m_title + _(" Error"), wxICON_ERROR);
		dialog.ShowModal();
		return false;
	}

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

GATEWAY_TYPE CIRCDDBGatewayConfigGatewaySet::getType() const
{
	int n = m_type->GetCurrentSelection();
	if (n == wxNOT_FOUND)
		return GT_REPEATER;

	return GATEWAY_TYPE(n);
}

wxString CIRCDDBGatewayConfigGatewaySet::getCallsign() const
{
	wxString callsign = m_callsign->GetValue();

	callsign.MakeUpper();

	return callsign;
}

wxString CIRCDDBGatewayConfigGatewaySet::getAddress() const
{
	return m_address->GetValue();
}

wxString CIRCDDBGatewayConfigGatewaySet::getIcomAddress() const
{
	return m_icomAddress->GetValue();
}

unsigned int CIRCDDBGatewayConfigGatewaySet::getIcomPort() const
{
	unsigned long n;
	m_icomPort->GetValue().ToULong(&n);

	return n;
}

wxString CIRCDDBGatewayConfigGatewaySet::getHBAddress() const
{
	return m_hbAddress->GetValue();
}

unsigned int CIRCDDBGatewayConfigGatewaySet::getHBPort() const
{
	unsigned long n;
	m_hbPort->GetValue().ToULong(&n);

	return n;
}

double CIRCDDBGatewayConfigGatewaySet::getLatitude() const
{
	double val;
	
	m_latitude->GetValue().ToDouble(&val);

	return val;
}

double CIRCDDBGatewayConfigGatewaySet::getLongitude() const
{
	double val;
	
	m_longitude->GetValue().ToDouble(&val);

	return val;
}

wxString CIRCDDBGatewayConfigGatewaySet::getDescription1() const
{
	return m_description1->GetValue();
}

wxString CIRCDDBGatewayConfigGatewaySet::getDescription2() const
{
	return m_description2->GetValue();
}

wxString CIRCDDBGatewayConfigGatewaySet::getURL() const
{
	return m_url->GetValue();
}
