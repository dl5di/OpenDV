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

#include "DStarRepeaterConfigFrame.h"
#include "Version.h"

#include <wx/notebook.h>
#include <wx/aboutdlg.h>

enum {
	Menu_File_Save = 6000
};

BEGIN_EVENT_TABLE(CDStarRepeaterConfigFrame, wxFrame)
	EVT_MENU(wxID_EXIT,      CDStarRepeaterConfigFrame::onQuit)
	EVT_MENU(Menu_File_Save, CDStarRepeaterConfigFrame::onSave)
	EVT_MENU(wxID_ABOUT,     CDStarRepeaterConfigFrame::onAbout)

	EVT_CLOSE(CDStarRepeaterConfigFrame::onClose)
END_EVENT_TABLE()

#if defined(__WINDOWS__)
const unsigned int BORDER_SIZE    = 5U;
const unsigned int CONTROL_WIDTH  = 100U;
const unsigned int LABEL_WIDTH    = 70U;
#else
const unsigned int BORDER_SIZE    = 5U;
const unsigned int CONTROL_WIDTH  = 150U;
const unsigned int LABEL_WIDTH    = 70U;
#endif

CDStarRepeaterConfigFrame::CDStarRepeaterConfigFrame(const wxString& title, const wxString& confDir, const wxString& name) :
wxFrame(NULL, -1, title),
m_config(NULL),
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
	SetMenuBar(createMenuBar());

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	wxPanel* panel = new wxPanel(this, -1);

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	wxNotebook* noteBook = new wxNotebook(panel, -1);

#if defined(__WINDOWS__)
	if (confDir.IsEmpty())
		m_config = new CDStarRepeaterConfig(new wxConfig(APPLICATION_NAME), ::wxGetHomeDir(), CONFIG_FILE_NAME, name);
	else
		m_config = new CDStarRepeaterConfig(new wxConfig(APPLICATION_NAME), confDir, CONFIG_FILE_NAME, name);
#else
	if (confDir.IsEmpty())
		m_config = new CDStarRepeaterConfig(wxT(CONF_DIR), CONFIG_FILE_NAME, name);
	else
		m_config = new CDStarRepeaterConfig(confDir, CONFIG_FILE_NAME, name);
#endif

	wxString callsign, gateway;
	DSTAR_MODE mode;
	ACK_TYPE ack;
	bool restriction, rpt1Validation, dtmfBlanking, errorReply;
	m_config->getCallsign(callsign, gateway, mode, ack, restriction, rpt1Validation, dtmfBlanking, errorReply);

	m_callsign = new CDStarRepeaterConfigCallsignSet(noteBook, -1, APPLICATION_NAME, callsign, gateway, mode, ack, restriction, rpt1Validation, dtmfBlanking, errorReply);
	noteBook->AddPage(m_callsign, _("Callsign"), true);

	wxString gatewayAddress, localAddress, netName;
	unsigned int gatewayPort, localPort;
	m_config->getNetwork(gatewayAddress, gatewayPort, localAddress, localPort, netName);

	m_network = new CDStarRepeaterConfigNetworkSet(noteBook, -1, APPLICATION_NAME, gatewayAddress, gatewayPort, localAddress, localPort, netName);
	noteBook->AddPage(m_network, _("Network"), false);

	unsigned int timeout, ackTime;
	m_config->getTimes(timeout, ackTime);

	m_times = new CDStarRepeaterConfigTimesSet(noteBook, -1, APPLICATION_NAME, timeout, ackTime);
	noteBook->AddPage(m_times, _("Timers"), false);

	bool announcementEnabled;
	unsigned int announcementTime;
	wxString announcementRecordRPT1, announcementRecordRPT2;
	wxString announcementDeleteRPT1, announcementDeleteRPT2;
	m_config->getAnnouncement(announcementEnabled, announcementTime, announcementRecordRPT1, announcementRecordRPT2, announcementDeleteRPT1, announcementDeleteRPT2);

	m_announcement = new CDStarRepeaterConfigAnnouncementSet(noteBook, -1, APPLICATION_NAME, announcementEnabled, announcementTime, announcementRecordRPT1, announcementRecordRPT2, announcementDeleteRPT1, announcementDeleteRPT2);
	noteBook->AddPage(m_announcement, _("Announcement"), false);

	unsigned int beaconTime;
	wxString beaconText;
	bool beaconVoice;
	TEXT_LANG language;
	m_config->getBeacon(beaconTime, beaconText, beaconVoice, language);

	m_beacon = new CDStarRepeaterConfigBeaconSet(noteBook, -1, APPLICATION_NAME, beaconTime, beaconText, beaconVoice, language);
	noteBook->AddPage(m_beacon, _("Beacon"), false);

	wxString modemType;
	m_config->getModem(modemType);

	m_modem = new CDStarRepeaterConfigModemSet(noteBook, -1, APPLICATION_NAME, m_config, modemType);
	noteBook->AddPage(m_modem, _("Modem"), false);

	bool enabled;
	wxString rpt1Callsign, rpt2Callsign;
	wxString shutdown, startup;
	wxString status1, status2, status3, status4, status5;
	wxString command1, command1Line, command2, command2Line;
	wxString command3, command3Line, command4, command4Line;
	wxString command5, command5Line, command6, command6Line;
	wxString output1, output2, output3, output4;
	m_config->getControl(enabled, rpt1Callsign, rpt2Callsign, shutdown, startup, status1, status2, status3, status4, status5, command1, command1Line, command2, command2Line, command3, command3Line, command4, command4Line, command5, command5Line, command6, command6Line, output1, output2, output3, output4);

	m_control1 = new CDStarRepeaterConfigControl1Set(noteBook, -1, APPLICATION_NAME, enabled, rpt1Callsign, rpt2Callsign, shutdown, startup, status1, status2, status3, status4, status5, output1, output2, output3, output4);
	noteBook->AddPage(m_control1, _("Control 1"), false);

	m_control2 = new CDStarRepeaterConfigControl2Set(noteBook, -1, APPLICATION_NAME, command1, command1Line, command2, command2Line, command3, command3Line, command4, command4Line, command5, command5Line, command6, command6Line);
	noteBook->AddPage(m_control2, _("Control 2"), false);

	wxString controllerType;
	unsigned int serialConfig, activeHangTime;
	bool pttInvert;
	m_config->getController(controllerType, serialConfig, pttInvert, activeHangTime);

	m_controller = new CDStarRepeaterConfigControllerSet(noteBook, -1, APPLICATION_NAME, controllerType, serialConfig, pttInvert, activeHangTime);
	noteBook->AddPage(m_controller, _("Controller"), false);

	sizer->Add(noteBook, 1, wxALL | wxGROW, BORDER_SIZE);

	panel->SetSizer(sizer);

	mainSizer->Add(panel);

	SetSizer(mainSizer);

	mainSizer->SetSizeHints(this);
}

CDStarRepeaterConfigFrame::~CDStarRepeaterConfigFrame()
{
	delete m_config;
}

wxMenuBar* CDStarRepeaterConfigFrame::createMenuBar()
{
	wxMenu* fileMenu = new wxMenu();
	fileMenu->Append(Menu_File_Save, _("Save"));
	fileMenu->AppendSeparator();
	fileMenu->Append(wxID_EXIT,      _("Exit"));

	wxMenu* helpMenu = new wxMenu();
	helpMenu->Append(wxID_ABOUT, _("About D-Star Repeater Config"));

	wxMenuBar* menuBar = new wxMenuBar();
	menuBar->Append(fileMenu,     _("File"));
	menuBar->Append(helpMenu,     _("Help"));

	return menuBar;
}

void CDStarRepeaterConfigFrame::onQuit(wxCommandEvent& event)
{
	Close(false);
}

void CDStarRepeaterConfigFrame::onClose(wxCloseEvent& event)
{
	Destroy();
}

void CDStarRepeaterConfigFrame::onAbout(wxCommandEvent& event)
{
	wxAboutDialogInfo info;
	info.AddDeveloper(wxT("Jonathan Naylor, G4KLX"));
	info.SetCopyright(wxT("(C) 2013,2014 using GPL v2 or later"));
	info.SetName(APPLICATION_NAME);
	info.SetVersion(VERSION);
	info.SetDescription(_("This program configures the D-Star Repeater."));

	::wxAboutBox(info);
}

void CDStarRepeaterConfigFrame::onSave(wxCommandEvent& event)
{
	if (!m_callsign->Validate() || !m_network->Validate() || !m_times->Validate() || !m_beacon->Validate() ||
		!m_announcement->Validate() || !m_modem->Validate() || !m_control1->Validate() || !m_control2->Validate() ||
		!m_controller->Validate())
		return;

	wxString callsign   = m_callsign->getCallsign();
	wxString gateway    = m_callsign->getGateway();
	DSTAR_MODE mode     = m_callsign->getMode();
	ACK_TYPE ack        = m_callsign->getAck();
	bool restriction    = m_callsign->getRestriction();
	bool rpt1Validation = m_callsign->getRPT1Validation();
	bool dtmfBlanking   = m_callsign->getDTMFBlanking();
	bool errorReply     = m_callsign->getErrorReply();
	m_config->setCallsign(callsign, gateway, mode, ack, restriction, rpt1Validation, dtmfBlanking, errorReply);

	wxString gatewayAddress  = m_network->getGatewayAddress();
	unsigned int gatewayPort = m_network->getGatewayPort();
	wxString localAddress    = m_network->getLocalAddress();
	unsigned int localPort   = m_network->getLocalPort();
	wxString netName         = m_network->getName();
	m_config->setNetwork(gatewayAddress, gatewayPort, localAddress, localPort, netName);

	unsigned int timeout = m_times->getTimeout();
	unsigned int ackTime = m_times->getAckTime();
	m_config->setTimes(timeout, ackTime);

	unsigned int beaconTime = m_beacon->getTime();
	wxString beaconText     = m_beacon->getText();
	bool beaconVoice        = m_beacon->getVoice();
	TEXT_LANG language      = m_beacon->getLanguage();
	m_config->setBeacon(beaconTime, beaconText, beaconVoice, language);

	bool announcementEnabled        = m_announcement->getEnabled();
	unsigned int announcementTime   = m_announcement->getTime();
	wxString announcementRecordRPT1 = m_announcement->getRecordRPT1();
	wxString announcementRecordRPT2 = m_announcement->getRecordRPT2();
	wxString announcementDeleteRPT1 = m_announcement->getDeleteRPT1();
	wxString announcementDeleteRPT2 = m_announcement->getDeleteRPT2();
	m_config->setAnnouncement(announcementEnabled, announcementTime, announcementRecordRPT1, announcementRecordRPT2, announcementDeleteRPT1, announcementDeleteRPT2);

	wxString modemType = m_modem->getType();
	m_config->setModem(modemType);

	bool enabled          = m_control1->getEnabled();
	wxString rpt1Callsign = m_control1->getRPT1Callsign();
	wxString rpt2Callsign = m_control1->getRPT2Callsign();
	wxString shutdown     = m_control1->getShutdown();
	wxString startup      = m_control1->getStartup();
	wxString status1      = m_control1->getStatus1();
	wxString status2      = m_control1->getStatus2();
	wxString status3      = m_control1->getStatus3();
	wxString status4      = m_control1->getStatus4();
	wxString status5      = m_control1->getStatus5();
	wxString output1      = m_control1->getOutput1();
	wxString output2      = m_control1->getOutput2();
	wxString output3      = m_control1->getOutput3();
	wxString output4      = m_control1->getOutput4();
	wxString command1     = m_control2->getCommand1();
	wxString command1Line = m_control2->getCommand1Line();
	wxString command2     = m_control2->getCommand2();
	wxString command2Line = m_control2->getCommand2Line();
	wxString command3     = m_control2->getCommand3();
	wxString command3Line = m_control2->getCommand3Line();
	wxString command4     = m_control2->getCommand4();
	wxString command4Line = m_control2->getCommand4Line();
	wxString command5     = m_control2->getCommand5();
	wxString command5Line = m_control2->getCommand5Line();
	wxString command6     = m_control2->getCommand6();
	wxString command6Line = m_control2->getCommand6Line();
	m_config->setControl(enabled, rpt1Callsign, rpt2Callsign, shutdown, startup, status1, status2, status3, status4, status5, command1, command1Line, command2, command2Line, command3, command3Line, command4, command4Line, command5, command5Line, command6, command6Line, output1, output2, output3, output4);

	wxString controllerType     = m_controller->getType();
	unsigned int serialConfig   = m_controller->getConfig();
	bool         pttInvert      = m_controller->getPTTInvert();
	unsigned int activeHangTime = m_controller->getTime();
	m_config->setController(controllerType, serialConfig, pttInvert, activeHangTime);

	bool ret = m_config->write();
	if (!ret) {
		wxMessageDialog dialog(this, _("There was an error when writing the D-Star Repeater configuration file"), _("Error"), wxICON_ERROR);
		dialog.ShowModal();
	} else {
		wxMessageDialog dialog(this, _("The changes made will not take effect\nuntil the D-Star Repeater is (re)started"), _("Information"), wxICON_INFORMATION);
		dialog.ShowModal();
	}
}
