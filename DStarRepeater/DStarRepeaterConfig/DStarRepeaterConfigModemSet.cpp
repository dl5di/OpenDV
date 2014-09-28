/*
 *   Copyright (C) 2011-2014 by Jonathan Naylor G4KLX
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

#include "DStarRepeaterConfigSoundCardSet.h"
#include "DStarRepeaterConfigDVRPTR1Set.h"
#include "DStarRepeaterConfigDVRPTR2Set.h"
#include "DStarRepeaterConfigDVRPTR3Set.h"
#include "DStarRepeaterConfigDVMegaSet.h"
#include "DStarRepeaterConfigModemSet.h"
#include "DStarRepeaterConfigGMSKSet.h"
#include "DStarRepeaterConfigDVAPSet.h"
#include "DStarRepeaterConfigSplitSet.h"

enum {
	Button_Configure = 7088
};

BEGIN_EVENT_TABLE(CDStarRepeaterConfigModemSet, wxPanel)
	EVT_BUTTON(Button_Configure, CDStarRepeaterConfigModemSet::onConfigure)
END_EVENT_TABLE()

const unsigned int BORDER_SIZE   = 5U;
const unsigned int CONTROL_WIDTH = 130U;

CDStarRepeaterConfigModemSet::CDStarRepeaterConfigModemSet(wxWindow* parent, int id, const wxString& title, CDStarRepeaterConfig* config, const wxString& type) :
wxPanel(parent, id),
m_config(config),
m_title(title),
m_type(NULL)
{
	wxASSERT(config != NULL);

	wxFlexGridSizer* sizer = new wxFlexGridSizer(2);

	wxStaticText* typeLabel = new wxStaticText(this, -1, _("Type"));
	sizer->Add(typeLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_type = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	m_type->Append(wxT("DVAP"));
	m_type->Append(wxT("DVMEGA"));
	m_type->Append(wxT("DV-RPTR V1"));
	m_type->Append(wxT("DV-RPTR V2"));
	m_type->Append(wxT("DV-RPTR V3"));
	m_type->Append(wxT("GMSK Modem"));
	m_type->Append(wxT("Sound Card"));
	m_type->Append(wxT("Split"));
	sizer->Add(m_type, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* dummy = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy, 0, wxALL, BORDER_SIZE);

	wxButton* configure = new wxButton(this, Button_Configure, _("Configure..."), wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	sizer->Add(configure, 0, wxALL, BORDER_SIZE);

	if (type.IsEmpty()) {
		m_type->SetSelection(0);
	} else {
		int n = m_type->SetStringSelection(type);
		if (n == wxNOT_FOUND)
			m_type->SetSelection(0);
	}

	SetAutoLayout(true);

	sizer->Fit(this);
	sizer->SetSizeHints(this);

	SetSizer(sizer);
}

CDStarRepeaterConfigModemSet::~CDStarRepeaterConfigModemSet()
{
}

bool CDStarRepeaterConfigModemSet::Validate()
{
	return m_type->GetCurrentSelection() != wxNOT_FOUND;
}

wxString CDStarRepeaterConfigModemSet::getType() const
{
	int n = m_type->GetCurrentSelection();

	if (n == wxNOT_FOUND)
		return wxT("DVAP");

	return m_type->GetStringSelection();
}

void CDStarRepeaterConfigModemSet::onConfigure(wxCommandEvent& event)
{
	int n = m_type->GetSelection();
	if (n == wxNOT_FOUND)
		return;

	wxString type = m_type->GetStringSelection();

	if (type.IsSameAs(wxT("DVAP"))) {
		wxString port;
		unsigned int frequency;
		int power, squelch;
		m_config->getDVAP(port, frequency, power, squelch);
		CDStarRepeaterConfigDVAPSet modem(this, -1, port, frequency, power, squelch);
		if (modem.ShowModal() == wxID_OK) {
			if (modem.Validate()) {
				port      = modem.getPort();
				frequency = modem.getFrequency();
				power     = modem.getPower();
				squelch   = modem.getSquelch();
				m_config->setDVAP(port, frequency, power, squelch);
			}
		}
	} else if (type.IsSameAs(wxT("GMSK Modem"))) {
		USB_INTERFACE modemType;
		unsigned int modemAddress;
		m_config->getGMSK(modemType, modemAddress);
		CDStarRepeaterConfigGMSKSet modem(this, -1, modemType, modemAddress);
		if (modem.ShowModal() == wxID_OK) {
			if (modem.Validate()) {
				modemType    = modem.getType();
				modemAddress = modem.getAddress();
				m_config->setGMSK(modemType, modemAddress);
			}
		}
	} else if (type.IsSameAs(wxT("DV-RPTR V1"))) {
		wxString port;
		bool txInvert, rxInvert, channel;
		unsigned int modLevel, txDelay;
		m_config->getDVRPTR1(port, rxInvert, txInvert, channel, modLevel, txDelay);
		CDStarRepeaterConfigDVRPTR1Set modem(this, -1, port, rxInvert, txInvert, channel, modLevel, txDelay);
		if (modem.ShowModal() == wxID_OK) {
			if (modem.Validate()) {
				port     = modem.getPort();
				rxInvert = modem.getRXInvert();
				txInvert = modem.getTXInvert();
				channel  = modem.getChannel();
				modLevel = modem.getModLevel();
				txDelay  = modem.getTXDelay();
				m_config->setDVRPTR1(port, rxInvert, txInvert, channel, modLevel, txDelay);
			}
		}
	} else if (type.IsSameAs(wxT("DV-RPTR V2"))) {
		CONNECTION_TYPE connType;
		wxString usbPort, address;
		bool txInvert;
		unsigned int port, modLevel, txDelay;
		m_config->getDVRPTR2(connType, usbPort, address, port, txInvert, modLevel, txDelay);
		CDStarRepeaterConfigDVRPTR2Set modem(this, -1, connType, usbPort, address, port, txInvert, modLevel, txDelay);
		if (modem.ShowModal() == wxID_OK) {
			if (modem.Validate()) {
				connType = modem.getConnectionType();
				usbPort  = modem.getUSBPort();
				address  = modem.getAddress();
				port     = modem.getPort();
				txInvert = modem.getTXInvert();
				modLevel = modem.getModLevel();
				txDelay  = modem.getTXDelay();
				m_config->setDVRPTR2(connType, usbPort, address, port, txInvert, modLevel, txDelay);
			}
		}
	} else if (type.IsSameAs(wxT("DV-RPTR V3"))) {
		CONNECTION_TYPE connType;
		wxString usbPort, address;
		bool txInvert;
		unsigned int port, modLevel, txDelay;
		m_config->getDVRPTR3(connType, usbPort, address, port, txInvert, modLevel, txDelay);
		CDStarRepeaterConfigDVRPTR3Set modem(this, -1, connType, usbPort, address, port, txInvert, modLevel, txDelay);
		if (modem.ShowModal() == wxID_OK) {
			if (modem.Validate()) {
				connType = modem.getConnectionType();
				usbPort  = modem.getUSBPort();
				address  = modem.getAddress();
				port     = modem.getPort();
				txInvert = modem.getTXInvert();
				modLevel = modem.getModLevel();
				txDelay  = modem.getTXDelay();
				m_config->setDVRPTR3(connType, usbPort, address, port, txInvert, modLevel, txDelay);
			}
		}
	} else if (type.IsSameAs(wxT("DVMEGA"))) {
		wxString port;
		DVMEGA_VARIANT variant;
		bool txInvert, rxInvert;
		unsigned int txDelay, frequency, power;
		m_config->getDVMEGA(port, variant, rxInvert, txInvert, txDelay, frequency, power);
		CDStarRepeaterConfigDVMegaSet modem(this, -1, port, variant, rxInvert, txInvert, txDelay, frequency, power);
		if (modem.ShowModal() == wxID_OK) {
			if (modem.Validate()) {
				port      = modem.getPort();
				variant   = modem.getVariant();
				rxInvert  = modem.getRXInvert();
				txInvert  = modem.getTXInvert();
				txDelay   = modem.getTXDelay();
				frequency = modem.getFrequency();
				power     = modem.getPower();
				m_config->setDVMEGA(port, variant, rxInvert, txInvert, txDelay, frequency, power);
			}
		}
	} else if (type.IsSameAs(wxT("Sound Card"))) {
		wxString rxDevice, txDevice;
		bool txInvert, rxInvert;
		wxFloat32 rxLevel, txLevel;
		unsigned int txDelay, txTail;
		m_config->getSoundCard(rxDevice, txDevice, rxInvert, txInvert, rxLevel, txLevel, txDelay, txTail);
		CDStarRepeaterConfigSoundCardSet modem(this, -1, rxDevice, txDevice, rxInvert, txInvert, rxLevel, txLevel, txDelay, txTail);
		if (modem.ShowModal() == wxID_OK) {
			if (modem.Validate()) {
				rxDevice = modem.getRXDevice();
				txDevice = modem.getTXDevice();
				rxInvert = modem.getRXInvert();
				txInvert = modem.getTXInvert();
				rxLevel  = modem.getRXLevel();
				txLevel  = modem.getTXLevel();
				txDelay  = modem.getTXDelay();
				txTail   = modem.getTXTail();
				m_config->setSoundCard(rxDevice, txDevice, rxInvert, txInvert, rxLevel, txLevel, txDelay, txTail);
			}
		}
	} else if (type.IsSameAs(wxT("Split"))) {
		wxString localAddress;
		unsigned int localPort, timeout;
		wxArrayString transmitterNames, receiverNames;
		m_config->getSplit(localAddress, localPort, transmitterNames, receiverNames, timeout);
		CDStarRepeaterConfigSplitSet modem(this, -1, localAddress, localPort, transmitterNames, receiverNames, timeout);
		if (modem.ShowModal() == wxID_OK) {
			if (modem.Validate()) {
				localAddress     = modem.getLocalAddress();
				localPort        = modem.getLocalPort();
				transmitterNames = modem.getTransmitterNames();
				receiverNames    = modem.getReceiverNames();
				timeout          = modem.getTimeout();
				m_config->setSplit(localAddress, localPort, transmitterNames, receiverNames, timeout);
			}
		}
	}
}
