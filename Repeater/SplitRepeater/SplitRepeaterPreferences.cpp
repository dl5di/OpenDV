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

#include "SplitRepeaterPreferences.h"
#include "DStarDefines.h"

const unsigned int BORDER_SIZE   = 5U;

#include <wx/gbsizer.h>
#include <wx/notebook.h>

CSplitRepeaterPreferences::CSplitRepeaterPreferences(wxWindow* parent, int id, const wxString& callsign,
	const wxString& gateway, DSTAR_MODE mode, ACK_TYPE ack, bool restriction, bool rpt1Validation, bool dtmfBlanking,
	const wxString& gatewayAddress, unsigned int gatewayPort, const wxString& localAddress,
	unsigned int localPort, unsigned int timeout, unsigned int ackTime, unsigned int frameWaitTime,
	unsigned int beaconTime, const wxString& beaconText, bool beaconVoice, TEXT_LANG language,bool announcementEnabled,
	unsigned int announcementTime, const wxString& announcementRecordRPT1, const wxString& announcementRecordRPT2,
	const wxString& announcementDeleteRPT1, const wxString& announcementDeleteRPT2, 
	const wxString& receiver1Address, unsigned int receiver1Port,
	const wxString& receiver2Address, unsigned int receiver2Port,
	const wxString& transmitter1Address, unsigned int transmitter1Port,
	const wxString& transmitter2Address, unsigned int transmitter2Port,
	bool enabled, const wxString& rpt1Callsign, const wxString& rpt2Callsign,
	const wxString& shutdown, const wxString& startup, const wxString& status1, const wxString& status2,
	const wxString& status3, const wxString& status4, const wxString& status5, const wxString& command1,
	const wxString& command1Line, const wxString& command2, const wxString& command2Line,
	const wxString& command3, const wxString& command3Line, const wxString& command4,
	const wxString& command4Line) :
wxDialog(parent, id, wxString(_("Split Repeater Preferences"))),
m_callsign(NULL),
m_network(NULL),
m_times(NULL),
m_beacon(NULL),
m_announcement(NULL),
m_receiver1(NULL),
m_receiver2(NULL),
m_transmitter1(NULL),
m_transmitter2(NULL),
m_control1(NULL),
m_control2(NULL)
{
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	wxNotebook* noteBook = new wxNotebook(this, -1);

	m_callsign = new CSplitRepeaterCallsignSet(noteBook, -1, APPLICATION_NAME, callsign, gateway, mode, ack, restriction, rpt1Validation, dtmfBlanking);
	noteBook->AddPage(m_callsign, _("Callsign"), true);

	m_network = new CNetworkSet(noteBook, -1, APPLICATION_NAME, gatewayAddress, gatewayPort, localAddress, localPort);
	noteBook->AddPage(m_network, _("Network"), false);

	m_times = new CSplitRepeaterTimesSet(noteBook, -1, APPLICATION_NAME, timeout, ackTime, frameWaitTime);
	noteBook->AddPage(m_times, _("Timers"), false);

	m_beacon = new CBeaconSet(noteBook, -1, APPLICATION_NAME, beaconTime, beaconText, beaconVoice, language);
	noteBook->AddPage(m_beacon, _("Beacon"), false);

	m_announcement = new CAnnouncementSet(noteBook, -1, APPLICATION_NAME, announcementEnabled, announcementTime, announcementRecordRPT1, announcementRecordRPT2, announcementDeleteRPT1, announcementDeleteRPT2);
	noteBook->AddPage(m_announcement, _("Announcement"), false);

	m_receiver1 = new CSplitRepeaterAddressSet(noteBook, -1, APPLICATION_NAME, receiver1Address, receiver1Port);
	noteBook->AddPage(m_receiver1, _("Receiver 1"), false);

	m_receiver2 = new CSplitRepeaterAddressSet(noteBook, -1, APPLICATION_NAME, receiver2Address, receiver2Port);
	noteBook->AddPage(m_receiver2, _("Receiver 2"), false);

	m_transmitter1 = new CSplitRepeaterAddressSet(noteBook, -1, APPLICATION_NAME, transmitter1Address, transmitter1Port);
	noteBook->AddPage(m_transmitter1, _("Transmitter 1"), false);

	m_transmitter2 = new CSplitRepeaterAddressSet(noteBook, -1, APPLICATION_NAME, transmitter2Address, transmitter2Port);
	noteBook->AddPage(m_transmitter2, _("Transmitter 2"), false);

	m_control1 = new CSplitRepeaterControl1Set(noteBook, -1, APPLICATION_NAME, enabled, rpt1Callsign, rpt2Callsign, shutdown, startup, status1, status2, status3, status4, status5);
	noteBook->AddPage(m_control1, _("Control 1"), false);

	m_control2 = new CSplitRepeaterControl2Set(noteBook, -1, APPLICATION_NAME, command1, command1Line, command2, command2Line, command3, command3Line, command4, command4Line);
	noteBook->AddPage(m_control2, _("Control 2"), false);

	mainSizer->Add(noteBook, 1, wxALL | wxGROW, BORDER_SIZE);

	mainSizer->Add(CreateButtonSizer(wxOK | wxCANCEL), 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	SetAutoLayout(true);
	Layout();

	mainSizer->Fit(this);
	mainSizer->SetSizeHints(this);

	SetSizer(mainSizer);
}

CSplitRepeaterPreferences::~CSplitRepeaterPreferences()
{
}

bool CSplitRepeaterPreferences::Validate()
{
	if (!m_callsign->Validate() || !m_network->Validate() || !m_times->Validate() || !m_beacon->Validate() || !m_announcement->Validate() || !m_receiver1->Validate() || !m_receiver2->Validate() || !m_transmitter1->Validate() || !m_transmitter2->Validate() || !m_control1->Validate() || !m_control2->Validate())
		return false;

	return true;	
}

wxString CSplitRepeaterPreferences::getCallsign() const
{
	return m_callsign->getCallsign();
}

wxString CSplitRepeaterPreferences::getGateway() const
{
	return m_callsign->getGateway();
}

DSTAR_MODE CSplitRepeaterPreferences::getMode() const
{
	return m_callsign->getMode();
}

ACK_TYPE CSplitRepeaterPreferences::getAck() const
{
	return m_callsign->getAck();
}

bool CSplitRepeaterPreferences::getRestriction() const
{
	return m_callsign->getRestriction();
}

bool CSplitRepeaterPreferences::getRPT1Validation() const
{
	return m_callsign->getRPT1Validation();
}

bool CSplitRepeaterPreferences::getDTMFBlanking() const
{
	return m_callsign->getDTMFBlanking();
}

wxString CSplitRepeaterPreferences::getGatewayAddress() const
{
	return m_network->getGatewayAddress();
}

unsigned int CSplitRepeaterPreferences::getGatewayPort() const
{
	return m_network->getGatewayPort();
}

wxString CSplitRepeaterPreferences::getLocalAddress() const
{
	return m_network->getLocalAddress();
}

unsigned int CSplitRepeaterPreferences::getLocalPort() const
{
	return m_network->getLocalPort();
}

unsigned int CSplitRepeaterPreferences::getTimeout() const
{
	return m_times->getTimeout();
}

unsigned int CSplitRepeaterPreferences::getAckTime() const
{
	return m_times->getAckTime();
}

unsigned int CSplitRepeaterPreferences::getFrameWaitTime() const
{
	return m_times->getFrameWaitTime();
}

unsigned int CSplitRepeaterPreferences::getBeaconTime() const
{
	return m_beacon->getTime();
}

wxString CSplitRepeaterPreferences::getBeaconText() const
{
	return m_beacon->getText();
}

bool CSplitRepeaterPreferences::getBeaconVoice() const
{
	return m_beacon->getVoice();
}

TEXT_LANG CSplitRepeaterPreferences::getLanguage() const
{
	return m_beacon->getLanguage();
}

bool CSplitRepeaterPreferences::getAnnouncementEnabled() const
{
	return m_announcement->getEnabled();
}

unsigned int CSplitRepeaterPreferences::getAnnouncementTime() const
{
	return m_announcement->getTime();
}

wxString CSplitRepeaterPreferences::getAnnouncementRecordRPT1() const
{
	return m_announcement->getRecordRPT1();
}

wxString CSplitRepeaterPreferences::getAnnouncementRecordRPT2() const
{
	return m_announcement->getRecordRPT2();
}

wxString CSplitRepeaterPreferences::getAnnouncementDeleteRPT1() const
{
	return m_announcement->getDeleteRPT1();
}

wxString CSplitRepeaterPreferences::getAnnouncementDeleteRPT2() const
{
	return m_announcement->getDeleteRPT2();
}

wxString CSplitRepeaterPreferences::getReceiver1Address() const
{
	return m_receiver1->getAddress();
}

unsigned int CSplitRepeaterPreferences::getReceiver1Port() const
{
	return m_receiver1->getPort();
}

wxString CSplitRepeaterPreferences::getReceiver2Address() const
{
	return m_receiver2->getAddress();
}

unsigned int CSplitRepeaterPreferences::getReceiver2Port() const
{
	return m_receiver2->getPort();
}

wxString CSplitRepeaterPreferences::getTransmitter1Address() const
{
	return m_transmitter1->getAddress();
}

unsigned int CSplitRepeaterPreferences::getTransmitter1Port() const
{
	return m_transmitter1->getPort();
}

wxString CSplitRepeaterPreferences::getTransmitter2Address() const
{
	return m_transmitter2->getAddress();
}

unsigned int CSplitRepeaterPreferences::getTransmitter2Port() const
{
	return m_transmitter2->getPort();
}

bool CSplitRepeaterPreferences::getEnabled() const
{
	return m_control1->getEnabled();
}

wxString CSplitRepeaterPreferences::getRPT1Callsign() const
{
	return m_control1->getRPT1Callsign();
}

wxString CSplitRepeaterPreferences::getRPT2Callsign() const
{
	return m_control1->getRPT2Callsign();
}

wxString CSplitRepeaterPreferences::getShutdown() const
{
	return m_control1->getShutdown();
}

wxString CSplitRepeaterPreferences::getStartup() const
{
	return m_control1->getStartup();
}

wxString CSplitRepeaterPreferences::getStatus1() const
{
	return m_control1->getStatus1();
}

wxString CSplitRepeaterPreferences::getStatus2() const
{
	return m_control1->getStatus2();
}

wxString CSplitRepeaterPreferences::getStatus3() const
{
	return m_control1->getStatus3();
}

wxString CSplitRepeaterPreferences::getStatus4() const
{
	return m_control1->getStatus4();
}

wxString CSplitRepeaterPreferences::getStatus5() const
{
	return m_control1->getStatus5();
}

wxString CSplitRepeaterPreferences::getCommand1() const
{
	return m_control2->getCommand1();
}

wxString CSplitRepeaterPreferences::getCommand1Line() const
{
	return m_control2->getCommand1Line();
}

wxString CSplitRepeaterPreferences::getCommand2() const
{
	return m_control2->getCommand2();
}

wxString CSplitRepeaterPreferences::getCommand2Line() const
{
	return m_control2->getCommand2Line();
}

wxString CSplitRepeaterPreferences::getCommand3() const
{
	return m_control2->getCommand3();
}

wxString CSplitRepeaterPreferences::getCommand3Line() const
{
	return m_control2->getCommand3Line();
}

wxString CSplitRepeaterPreferences::getCommand4() const
{
	return m_control2->getCommand4();
}

wxString CSplitRepeaterPreferences::getCommand4Line() const
{
	return m_control2->getCommand4Line();
}
