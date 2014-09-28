/*
 *   Copyright (C) 2014 by Jonathan Naylor G4KLX
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

#include "DStarRepeaterConfigSplitSet.h"
#include "DStarDefines.h"

const unsigned int BORDER_SIZE     = 5U;

const unsigned int ADDRESS_WIDTH   = 120U;
const unsigned int PORT_WIDTH      = 80U;

const unsigned int ADDRESS_LENGTH  = 15U;
const unsigned int PORT_LENGTH     = 5U;

const unsigned int SLIDER_WIDTH    = 200U;

CDStarRepeaterConfigSplitSet::CDStarRepeaterConfigSplitSet(wxWindow* parent, int id, const wxString& localAddress, unsigned int localPort, const wxArrayString& transmitterNames, const wxArrayString& receiverNames, unsigned int timeout) :
wxDialog(parent, id, wxString(_("Split Settings"))),
m_localAddress(NULL),
m_localPort(NULL),
m_transmitterNames(NULL),
m_receiverNames(NULL),
m_timeout(NULL),
m_txCount(0U),
m_rxCount(0U)
{
	m_txCount = transmitterNames.GetCount();
	m_rxCount = receiverNames.GetCount();

	if (m_txCount > SPLIT_TX_GUI_COUNT)
		m_txCount = SPLIT_TX_GUI_COUNT;

	if (m_rxCount > SPLIT_RX_GUI_COUNT)
		m_rxCount = SPLIT_RX_GUI_COUNT;

	m_transmitterNames = new wxTextCtrl*[m_txCount];
	m_receiverNames    = new wxTextCtrl*[m_rxCount];

	wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);

	wxFlexGridSizer* sizer = new wxFlexGridSizer(2);

	wxStaticText* localLabel = new wxStaticText(this, -1, _("Local Addr/Port"));
	sizer->Add(localLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxBoxSizer* localSizer = new wxBoxSizer(wxHORIZONTAL);

	m_localAddress = new CAddressTextCtrl(this, -1, localAddress, wxDefaultPosition, wxSize(ADDRESS_WIDTH, -1));
	m_localAddress->SetMaxLength(ADDRESS_LENGTH);
	localSizer->Add(m_localAddress, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxString buffer;
	buffer.Printf(wxT("%u"), localPort);

	m_localPort = new CPortTextCtrl(this, -1, buffer, wxDefaultPosition, wxSize(PORT_WIDTH, -1));
	m_localPort->SetMaxLength(PORT_LENGTH);
	localSizer->Add(m_localPort, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	sizer->Add(localSizer);

	for (unsigned int i = 0U; i < m_txCount; i++) {
		wxString text;
		text.Printf(_("TX %u Name"), i + 1U);

		wxStaticText* transmitterLabel = new wxStaticText(this, -1, text);
		sizer->Add(transmitterLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

		m_transmitterNames[i] = new wxTextCtrl(this, -1, transmitterNames.Item(i), wxDefaultPosition, wxSize(ADDRESS_WIDTH, -1));
		sizer->Add(m_transmitterNames[i], 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	}

	for (unsigned int i = 0U; i < m_rxCount; i++) {
		wxString text;
		text.Printf(_("RX %u Name"), i + 1U);

		wxStaticText* receiverLabel = new wxStaticText(this, -1, text);
		sizer->Add(receiverLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

		m_receiverNames[i] = new wxTextCtrl(this, -1, receiverNames.Item(i), wxDefaultPosition, wxSize(ADDRESS_WIDTH, -1));
		sizer->Add(m_receiverNames[i], 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	}

	wxStaticText* timeoutLabel = new wxStaticText(this, -1, _("Packet timeout (ms)"));
	sizer->Add(timeoutLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_timeout = new wxSlider(this, -1, timeout, 10, 300, wxDefaultPosition, wxSize(SLIDER_WIDTH, -1), wxSL_HORIZONTAL | wxSL_LABELS);
	sizer->Add(m_timeout, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	topSizer->Add(sizer);

	topSizer->Add(CreateButtonSizer(wxOK | wxCANCEL), 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	SetAutoLayout(true);

	topSizer->Fit(this);
	topSizer->SetSizeHints(this);

	SetSizer(topSizer);
}

CDStarRepeaterConfigSplitSet::~CDStarRepeaterConfigSplitSet()
{
}

bool CDStarRepeaterConfigSplitSet::Validate()
{
	return true;
}

wxString CDStarRepeaterConfigSplitSet::getLocalAddress() const
{
	return m_localAddress->GetValue();
}

unsigned int CDStarRepeaterConfigSplitSet::getLocalPort() const
{
	unsigned long n;

	m_localPort->GetValue().ToULong(&n);

	return n;
}

wxArrayString CDStarRepeaterConfigSplitSet::getTransmitterNames() const
{
	wxArrayString names;

	for (unsigned int i = 0U; i < m_txCount; i++)
		names.Add(m_transmitterNames[i]->GetValue());

	return names;
}

wxArrayString CDStarRepeaterConfigSplitSet::getReceiverNames() const
{
	wxArrayString names;

	for (unsigned int i = 0U; i < m_rxCount; i++)
		names.Add(m_receiverNames[i]->GetValue());

	return names;
}

unsigned int CDStarRepeaterConfigSplitSet::getTimeout() const
{
	return (unsigned int)m_timeout->GetValue();
}
