/*
 *   Copyright (C) 2010,2011,2014 by Jonathan Naylor G4KLX
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

#include "DummyRepeaterNetworkSet.h"

const unsigned int ADDRESS_WIDTH   = 120U;
const unsigned int PORT_WIDTH      = 80U;

const unsigned int ADDRESS_LENGTH  = 15U;
const unsigned int PORT_LENGTH     = 5U;

const unsigned int BORDER_SIZE = 5U;

CDummyRepeaterNetworkSet::CDummyRepeaterNetworkSet(wxWindow* parent, int id, const wxString& title, const wxString& gatewayAddress, unsigned int gatewayPort, const wxString& localAddress, unsigned int localPort) :
wxPanel(parent, id),
m_title(title),
m_gatewayAddress(NULL),
m_gatewayPort(NULL),
m_localAddress(NULL),
m_localPort(NULL)
{
	wxFlexGridSizer* sizer = new wxFlexGridSizer(2);

	wxStaticText* gatewayAddressLabel = new wxStaticText(this, -1, _("Gateway Address"));
	sizer->Add(gatewayAddressLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_gatewayAddress = new CAddressTextCtrl(this, -1, gatewayAddress, wxDefaultPosition, wxSize(ADDRESS_WIDTH, -1));
	m_gatewayAddress->SetMaxLength(ADDRESS_LENGTH);
	sizer->Add(m_gatewayAddress, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* gatewayPortLabel = new wxStaticText(this, -1, _("Gateway Port"));
	sizer->Add(gatewayPortLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxString buffer;
	buffer.Printf(wxT("%u"), gatewayPort);

	m_gatewayPort = new CPortTextCtrl(this, -1, buffer, wxDefaultPosition, wxSize(PORT_WIDTH, -1));
	m_gatewayPort->SetMaxLength(PORT_LENGTH);
	sizer->Add(m_gatewayPort, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* localAddressLabel = new wxStaticText(this, -1, _("Local Address"));
	sizer->Add(localAddressLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_localAddress = new CAddressTextCtrl(this, -1, localAddress, wxDefaultPosition, wxSize(ADDRESS_WIDTH, -1));
	m_localAddress->SetMaxLength(ADDRESS_LENGTH);
	sizer->Add(m_localAddress, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* localPortLabel = new wxStaticText(this, -1, _("Local Port"));
	sizer->Add(localPortLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	buffer.Printf(wxT("%u"), localPort);

	m_localPort = new CPortTextCtrl(this, -1, buffer, wxDefaultPosition, wxSize(PORT_WIDTH, -1));
	m_localPort->SetMaxLength(PORT_LENGTH);
	sizer->Add(m_localPort, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	SetAutoLayout(true);

	sizer->Fit(this);
	sizer->SetSizeHints(this);

	SetSizer(sizer);
}


CDummyRepeaterNetworkSet::~CDummyRepeaterNetworkSet()
{
}

bool CDummyRepeaterNetworkSet::Validate()
{
	unsigned int port = getGatewayPort();

	if (port == 0U || port > 65535U) {
		wxMessageDialog dialog(this, _("The Gateway Port is not valid"), m_title + _(" Error"), wxICON_ERROR);
		dialog.ShowModal();
		return false;
	}

	port = getLocalPort();

	if (port == 0U || port > 65535U) {
		wxMessageDialog dialog(this, _("The Local Port is not valid"), m_title + _(" Error"), wxICON_ERROR);
		dialog.ShowModal();
		return false;
	}

	return true;
}

wxString CDummyRepeaterNetworkSet::getGatewayAddress() const
{
	return m_gatewayAddress->GetValue();
}

unsigned int CDummyRepeaterNetworkSet::getGatewayPort() const
{
	unsigned long n;

	m_gatewayPort->GetValue().ToULong(&n);

	return n;
}

wxString CDummyRepeaterNetworkSet::getLocalAddress() const
{
	return m_localAddress->GetValue();
}

unsigned int CDummyRepeaterNetworkSet::getLocalPort() const
{
	unsigned long n;

	m_localPort->GetValue().ToULong(&n);

	return n;
}
