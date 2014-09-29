/*
 *   Copyright (C) 2011,2012,2013 by Jonathan Naylor G4KLX
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

#include "DVRPTRRepeaterPreferences.h"
#include "DStarDefines.h"

const unsigned int BORDER_SIZE   = 5U;

#include <wx/gbsizer.h>
#include <wx/notebook.h>

CDVRPTRRepeaterPreferences::CDVRPTRRepeaterPreferences(wxWindow* parent, int id, const wxString& callsign,
	const wxString& gateway, DSTAR_MODE mode, ACK_TYPE ack, bool restriction, bool rpt1Validation, bool dtmfBlanking,
	const wxString& gatewayAddress, unsigned int gatewayPort, const wxString& localAddress,
	unsigned int localPort, unsigned int timeout, unsigned int ackTime, unsigned int beaconTime,
	const wxString& beaconText, bool beaconVoice, TEXT_LANG language, bool announcementEnabled,
	unsigned int announcementTime, const wxString& announcementRecordRPT1, const wxString& announcementRecordRPT2,
	const wxString& announcementDeleteRPT1, const wxString& announcementDeleteRPT2, DVRPTR_VERSION version,
	CONNECTION_TYPE connectionType, const wxString& usbPort, const wxString& address, unsigned int port, 
	bool rxInvert, bool txInvert, bool channel, unsigned int modLevel, unsigned int txDelay, bool enabled,
	const wxString& rpt1Callsign, const wxString& rpt2Callsign, const wxString& shutdown,
	const wxString& startup, const wxString& status1, const wxString& status2, const wxString& status3,
	const wxString& status4, const wxString& status5, const wxString& command1, const wxString& command1Line,
	const wxString& command2, const wxString& command2Line, const wxString& command3,
	const wxString& command3Line, const wxString& command4, const wxString& command4Line,
	const wxString& output1, const wxString& output2, const wxString& output3, const wxString& output4,
	const wxString& controllerType,
	unsigned int activeHangTime) :
wxDialog(parent, id, wxString(_("DV-RPTR Repeater Preferences"))),
m_callsign(NULL),
m_network(NULL),
m_times(NULL),
m_beacon(NULL),
m_announcement(NULL),
m_modem(NULL),
m_control1(NULL),
m_control2(NULL),
m_controller(NULL)
{
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	wxNotebook* noteBook = new wxNotebook(this, -1);

	m_callsign = new CDVRPTRRepeaterCallsignSet(noteBook, -1, APPLICATION_NAME, callsign, gateway, mode, ack, restriction, rpt1Validation, dtmfBlanking);
	noteBook->AddPage(m_callsign, _("Callsign"), true);

	m_network = new CNetworkSet(noteBook, -1, APPLICATION_NAME, gatewayAddress, gatewayPort, localAddress, localPort);
	noteBook->AddPage(m_network, _("Network"), false);

	m_times = new CDVRPTRRepeaterTimesSet(noteBook, -1, APPLICATION_NAME, timeout, ackTime);
	noteBook->AddPage(m_times, _("Timers"), false);

	m_announcement = new CAnnouncementSet(noteBook, -1, APPLICATION_NAME, announcementEnabled, announcementTime, announcementRecordRPT1, announcementRecordRPT2, announcementDeleteRPT1, announcementDeleteRPT2);
	noteBook->AddPage(m_announcement, _("Announcement"), false);

	m_beacon = new CBeaconSet(noteBook, -1, APPLICATION_NAME, beaconTime, beaconText, beaconVoice, language);
	noteBook->AddPage(m_beacon, _("Beacon"), false);

	m_modem = new CDVRPTRRepeaterModemSet(noteBook, -1, APPLICATION_NAME, version, connectionType, usbPort, address, port, rxInvert, txInvert, channel, modLevel, txDelay);
	noteBook->AddPage(m_modem, _("DV-RPTR"), false);

	m_control1 = new CControl1Set(noteBook, -1, APPLICATION_NAME, enabled, rpt1Callsign, rpt2Callsign, shutdown, startup, status1, status2, status3, status4, status5, output1, output2, output3, output4);
	noteBook->AddPage(m_control1, _("Control 1"), false);

	m_control2 = new CControl2Set(noteBook, -1, APPLICATION_NAME, command1, command1Line, command2, command2Line, command3, command3Line, command4, command4Line);
	noteBook->AddPage(m_control2, _("Control 2"), false);

	m_controller = new CDVRPTRRepeaterControllerSet(noteBook, -1, APPLICATION_NAME, controllerType, activeHangTime);
	noteBook->AddPage(m_controller, _("Controller"), false);

	mainSizer->Add(noteBook, 1, wxALL | wxGROW, BORDER_SIZE);

	mainSizer->Add(CreateButtonSizer(wxOK | wxCANCEL), 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	SetAutoLayout(true);
	Layout();

	mainSizer->Fit(this);
	mainSizer->SetSizeHints(this);

	SetSizer(mainSizer);
}

CDVRPTRRepeaterPreferences::~CDVRPTRRepeaterPreferences()
{
}

bool CDVRPTRRepeaterPreferences::Validate()
{
	if (!m_callsign->Validate() || !m_network->Validate() || !m_times->Validate() || !m_beacon->Validate() || !m_announcement->Validate() || !m_modem->Validate() || !m_control1->Validate() || !m_control2->Validate() || !m_controller->Validate())
		return false;

	return true;	
}

wxString CDVRPTRRepeaterPreferences::getCallsign() const
{
	return m_callsign->getCallsign();
}

wxString CDVRPTRRepeaterPreferences::getGateway() const
{
	return m_callsign->getGateway();
}

DSTAR_MODE CDVRPTRRepeaterPreferences::getMode() const
{
	return m_callsign->getMode();
}

ACK_TYPE CDVRPTRRepeaterPreferences::getAck() const
{
	return m_callsign->getAck();
}

bool CDVRPTRRepeaterPreferences::getRestriction() const
{
	return m_callsign->getRestriction();
}

bool CDVRPTRRepeaterPreferences::getRPT1Validation() const
{
	return m_callsign->getRPT1Validation();
}

bool CDVRPTRRepeaterPreferences::getDTMFBlanking() const
{
	return m_callsign->getDTMFBlanking();
}

wxString CDVRPTRRepeaterPreferences::getGatewayAddress() const
{
	return m_network->getGatewayAddress();
}

unsigned int CDVRPTRRepeaterPreferences::getGatewayPort() const
{
	return m_network->getGatewayPort();
}

wxString CDVRPTRRepeaterPreferences::getLocalAddress() const
{
	return m_network->getLocalAddress();
}

unsigned int CDVRPTRRepeaterPreferences::getLocalPort() const
{
	return m_network->getLocalPort();
}

unsigned int CDVRPTRRepeaterPreferences::getTimeout() const
{
	return m_times->getTimeout();
}

unsigned int CDVRPTRRepeaterPreferences::getAckTime() const
{
	return m_times->getAckTime();
}

unsigned int CDVRPTRRepeaterPreferences::getBeaconTime() const
{
	return m_beacon->getTime();
}

wxString CDVRPTRRepeaterPreferences::getBeaconText() const
{
	return m_beacon->getText();
}

bool CDVRPTRRepeaterPreferences::getBeaconVoice() const
{
	return m_beacon->getVoice();
}

TEXT_LANG CDVRPTRRepeaterPreferences::getLanguage() const
{
	return m_beacon->getLanguage();
}

bool CDVRPTRRepeaterPreferences::getAnnouncementEnabled() const
{
	return m_announcement->getEnabled();
}

unsigned int CDVRPTRRepeaterPreferences::getAnnouncementTime() const
{
	return m_announcement->getTime();
}

wxString CDVRPTRRepeaterPreferences::getAnnouncementRecordRPT1() const
{
	return m_announcement->getRecordRPT1();
}

wxString CDVRPTRRepeaterPreferences::getAnnouncementRecordRPT2() const
{
	return m_announcement->getRecordRPT2();
}

wxString CDVRPTRRepeaterPreferences::getAnnouncementDeleteRPT1() const
{
	return m_announcement->getDeleteRPT1();
}

wxString CDVRPTRRepeaterPreferences::getAnnouncementDeleteRPT2() const
{
	return m_announcement->getDeleteRPT2();
}

DVRPTR_VERSION CDVRPTRRepeaterPreferences::getVersion() const
{
	return m_modem->getVersion();
}

CONNECTION_TYPE CDVRPTRRepeaterPreferences::getConnectionType() const
{
	return m_modem->getConnectionType();
}

wxString CDVRPTRRepeaterPreferences::getUSBPort() const
{
	return m_modem->getUSBPort();
}

wxString CDVRPTRRepeaterPreferences::getAddress() const
{
	return m_modem->getAddress();
}

unsigned int CDVRPTRRepeaterPreferences::getPort() const
{
	return m_modem->getPort();
}

bool CDVRPTRRepeaterPreferences::getRXInvert() const
{
	return m_modem->getRXInvert();
}

bool CDVRPTRRepeaterPreferences::getTXInvert() const
{
	return m_modem->getTXInvert();
}

bool CDVRPTRRepeaterPreferences::getChannel() const
{
	return m_modem->getChannel();
}

unsigned int CDVRPTRRepeaterPreferences::getModLevel() const
{
	return m_modem->getModLevel();
}

unsigned int CDVRPTRRepeaterPreferences::getTXDelay() const
{
	return m_modem->getTXDelay();
}

bool CDVRPTRRepeaterPreferences::getEnabled() const
{
	return m_control1->getEnabled();
}

wxString CDVRPTRRepeaterPreferences::getRPT1Callsign() const
{
	return m_control1->getRPT1Callsign();
}

wxString CDVRPTRRepeaterPreferences::getRPT2Callsign() const
{
	return m_control1->getRPT2Callsign();
}

wxString CDVRPTRRepeaterPreferences::getShutdown() const
{
	return m_control1->getShutdown();
}

wxString CDVRPTRRepeaterPreferences::getStartup() const
{
	return m_control1->getStartup();
}

wxString CDVRPTRRepeaterPreferences::getStatus1() const
{
	return m_control1->getStatus1();
}

wxString CDVRPTRRepeaterPreferences::getStatus2() const
{
	return m_control1->getStatus2();
}

wxString CDVRPTRRepeaterPreferences::getStatus3() const
{
	return m_control1->getStatus3();
}

wxString CDVRPTRRepeaterPreferences::getStatus4() const
{
	return m_control1->getStatus4();
}

wxString CDVRPTRRepeaterPreferences::getStatus5() const
{
	return m_control1->getStatus5();
}

wxString CDVRPTRRepeaterPreferences::getOutput1() const
{
	return m_control1->getOutput1();
}

wxString CDVRPTRRepeaterPreferences::getOutput2() const
{
	return m_control1->getOutput2();
}

wxString CDVRPTRRepeaterPreferences::getOutput3() const
{
	return m_control1->getOutput3();
}

wxString CDVRPTRRepeaterPreferences::getOutput4() const
{
	return m_control1->getOutput4();
}

wxString CDVRPTRRepeaterPreferences::getCommand1() const
{
	return m_control2->getCommand1();
}

wxString CDVRPTRRepeaterPreferences::getCommand1Line() const
{
	return m_control2->getCommand1Line();
}

wxString CDVRPTRRepeaterPreferences::getCommand2() const
{
	return m_control2->getCommand2();
}

wxString CDVRPTRRepeaterPreferences::getCommand2Line() const
{
	return m_control2->getCommand2Line();
}

wxString CDVRPTRRepeaterPreferences::getCommand3() const
{
	return m_control2->getCommand3();
}

wxString CDVRPTRRepeaterPreferences::getCommand3Line() const
{
	return m_control2->getCommand3Line();
}

wxString CDVRPTRRepeaterPreferences::getCommand4() const
{
	return m_control2->getCommand4();
}

wxString CDVRPTRRepeaterPreferences::getCommand4Line() const
{
	return m_control2->getCommand4Line();
}

wxString CDVRPTRRepeaterPreferences::getControllerType() const
{
	return m_controller->getType();
}

unsigned int CDVRPTRRepeaterPreferences::getActiveHangTime() const
{
	return m_controller->getTime();
}
