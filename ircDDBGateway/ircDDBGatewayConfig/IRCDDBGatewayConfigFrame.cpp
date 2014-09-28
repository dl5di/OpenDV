/*
 *   Copyright (C) 2010-2014 by Jonathan Naylor G4KLX
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

#include "IRCDDBGatewayConfigFrame.h"
#include "IRCDDBGatewayConfigDefs.h"
#include "Version.h"

const unsigned int BORDER_SIZE = 5U;

#include <wx/gbsizer.h>
#include <wx/aboutdlg.h>
#include <wx/notebook.h>

enum {
	Menu_File_Save = 6000
};

BEGIN_EVENT_TABLE(CIRCDDBGatewayConfigFrame, wxFrame)
	EVT_MENU(wxID_EXIT,      CIRCDDBGatewayConfigFrame::onQuit)
	EVT_MENU(Menu_File_Save, CIRCDDBGatewayConfigFrame::onSave)
	EVT_MENU(wxID_ABOUT,     CIRCDDBGatewayConfigFrame::onAbout)

	EVT_CLOSE(CIRCDDBGatewayConfigFrame::onClose)
END_EVENT_TABLE()

CIRCDDBGatewayConfigFrame::CIRCDDBGatewayConfigFrame(const wxString& title, const wxString& confDir, const wxString& name) :
wxFrame(NULL, -1, title),
m_config(NULL),
m_gateway(NULL),
m_repeaterData1(NULL),
m_repeaterInfo1(NULL),
m_repeaterData2(NULL),
m_repeaterInfo2(NULL),
m_repeaterData3(NULL),
m_repeaterInfo3(NULL),
m_repeaterData4(NULL),
m_repeaterInfo4(NULL),
m_ircDDB(NULL),
m_dprs(NULL),
m_dextra(NULL),
m_dplus(NULL),
m_dcs(NULL),
m_starNet1(NULL),
m_starNet2(NULL),
m_starNet3(NULL),
m_starNet4(NULL),
m_starNet5(NULL),
m_remote(NULL),
m_miscellaneous(NULL)
{
	SetMenuBar(createMenuBar());

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	wxPanel* panel = new wxPanel(this, -1);

#if defined(__WINDOWS__)
	if (confDir.IsEmpty())
		m_config = new CIRCDDBGatewayConfig(new wxConfig(APPLICATION_NAME), ::wxGetHomeDir(), CONFIG_FILE_NAME, name);
	else
		m_config = new CIRCDDBGatewayConfig(new wxConfig(APPLICATION_NAME), confDir, CONFIG_FILE_NAME, name);
#else
	if (confDir.IsEmpty())
		m_config = new CIRCDDBGatewayConfig(wxT(CONF_DIR), CONFIG_FILE_NAME, name);
	else
		m_config = new CIRCDDBGatewayConfig(confDir, CONFIG_FILE_NAME, name);
#endif

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	wxNotebook* noteBook = new wxNotebook(panel, -1);

	bool dextraEnabled;
	unsigned int maxDExtraDongles;
	m_config->getDExtra(dextraEnabled, maxDExtraDongles);

	wxString dplusLogin;
	unsigned int maxDPlusDongles;
	bool dplusEnabled;
	m_config->getDPlus(dplusEnabled, maxDPlusDongles, dplusLogin);

	bool dcsEnabled, ccsEnabled;
	wxString ccsHost;
	m_config->getDCS(dcsEnabled, ccsEnabled, ccsHost);

	GATEWAY_TYPE gatewayType;
	wxString gatewayCallsign, gatewayAddress, icomAddress, hbAddress, description1, description2, url;
	unsigned int icomPort, hbPort;
	double latitude, longitude;
	m_config->getGateway(gatewayType, gatewayCallsign, gatewayAddress, icomAddress, icomPort, hbAddress, hbPort, latitude, longitude, description1, description2, url);

	m_gateway = new CIRCDDBGatewayConfigGatewaySet(noteBook, -1, APPLICATION_NAME, gatewayType, gatewayCallsign, gatewayAddress, icomAddress, icomPort, hbAddress, hbPort, latitude, longitude, description1, description2, url);
	noteBook->AddPage(m_gateway, _("Gateway"), true);

	wxString repeaterCall1, repeaterBand1, repeaterAddress1, reflector1, description11, description12, url1;
	double frequency1, offset1, range1, latitude1, longitude1, agl1;
	unsigned char band11, band12, band13;
	unsigned int repeaterPort1;
	HW_TYPE repeaterType1;
	bool atStartup1;
	RECONNECT reconnect1;
	m_config->getRepeater1(repeaterCall1, repeaterBand1, repeaterType1, repeaterAddress1, repeaterPort1, band11, band12, band13, reflector1, atStartup1, reconnect1, frequency1, offset1, range1, latitude1, longitude1, agl1, description11, description12, url1);

	m_repeaterData1 = new CRepeaterDataSet(noteBook, -1, APPLICATION_NAME, repeaterBand1, repeaterType1, repeaterAddress1, repeaterPort1, band11, band12, band13, dplusEnabled, dextraEnabled, dcsEnabled, reflector1, atStartup1, reconnect1);
	noteBook->AddPage(m_repeaterData1, _("Repeater 1"), false);

	m_repeaterInfo1 = new CRepeaterInfoSet(noteBook, -1, APPLICATION_NAME, frequency1, offset1, range1, latitude1, longitude1, agl1, description11, description12, url1);
	noteBook->AddPage(m_repeaterInfo1, _("Repeater 1"), false);

	wxString repeaterCall2, repeaterBand2, repeaterAddress2, reflector2, description21, description22, url2;
	double frequency2, offset2, range2, latitude2, longitude2, agl2;
	unsigned char band21, band22, band23;
	unsigned int repeaterPort2;
	HW_TYPE repeaterType2;
	bool atStartup2;
	RECONNECT reconnect2;
	m_config->getRepeater2(repeaterCall2, repeaterBand2, repeaterType2, repeaterAddress2, repeaterPort2, band21, band22, band23, reflector2, atStartup2, reconnect2, frequency2, offset2, range2, latitude2, longitude2, agl2, description21, description22, url2);

	m_repeaterData2 = new CRepeaterDataSet(noteBook, -1, APPLICATION_NAME, repeaterBand2, repeaterType2, repeaterAddress2, repeaterPort2, band21, band22, band23, dplusEnabled, dextraEnabled, dcsEnabled, reflector2, atStartup2, reconnect2);
	noteBook->AddPage(m_repeaterData2, _("Repeater 2"), false);

	m_repeaterInfo2 = new CRepeaterInfoSet(noteBook, -1, APPLICATION_NAME, frequency2, offset2, range2, latitude2, longitude2, agl2, description21, description22, url2);
	noteBook->AddPage(m_repeaterInfo2, _("Repeater 2"), false);

	wxString repeaterCall3, repeaterBand3, repeaterAddress3, reflector3, description31, description32, url3;
	double frequency3, offset3, range3, latitude3, longitude3, agl3;
	unsigned char band31, band32, band33;
	unsigned int repeaterPort3;
	HW_TYPE repeaterType3;
	bool atStartup3;
	RECONNECT reconnect3;
	m_config->getRepeater3(repeaterCall3, repeaterBand3, repeaterType3, repeaterAddress3, repeaterPort3, band31, band32, band33, reflector3, atStartup3, reconnect3, frequency3, offset3, range3, latitude3, longitude3, agl3, description31, description32, url3);

	m_repeaterData3 = new CRepeaterDataSet(noteBook, -1, APPLICATION_NAME, repeaterBand3, repeaterType3, repeaterAddress3, repeaterPort3, band31, band32, band33, dplusEnabled, dextraEnabled, dcsEnabled, reflector3, atStartup3, reconnect3);
	noteBook->AddPage(m_repeaterData3, _("Repeater 3"), false);

	m_repeaterInfo3 = new CRepeaterInfoSet(noteBook, -1, APPLICATION_NAME, frequency3, offset3, range3, latitude3, longitude3, agl3, description31, description32, url3);
	noteBook->AddPage(m_repeaterInfo3, _("Repeater 3"), false);

	wxString repeaterCall4, repeaterBand4, repeaterAddress4, reflector4, description41, description42, url4;
	double frequency4, offset4, range4, latitude4, longitude4, agl4;
	unsigned char band41, band42, band43;
	unsigned int repeaterPort4;
	HW_TYPE repeaterType4;
	bool atStartup4;
	RECONNECT reconnect4;
	m_config->getRepeater4(repeaterCall4, repeaterBand4, repeaterType4, repeaterAddress4, repeaterPort4, band41, band42, band43, reflector4, atStartup4, reconnect4, frequency4, offset4, range4, latitude4, longitude4, agl4, description41, description42, url4);

	m_repeaterData4 = new CRepeaterDataSet(noteBook, -1, APPLICATION_NAME, repeaterBand4, repeaterType4, repeaterAddress4, repeaterPort4, band41, band42, band43, dplusEnabled, dextraEnabled, dcsEnabled, reflector4, atStartup4, reconnect4);
	noteBook->AddPage(m_repeaterData4, _("Repeater 4"), false);

	m_repeaterInfo4 = new CRepeaterInfoSet(noteBook, -1, APPLICATION_NAME, frequency4, offset4, range4, latitude4, longitude4, agl4, description41, description42, url4);
	noteBook->AddPage(m_repeaterInfo4, _("Repeater 4"), false);

	bool ircDDBEnabled;
	wxString ircDDBHostname, ircDDBUsername, ircDDBPassword;
	m_config->getIrcDDB(ircDDBEnabled, ircDDBHostname, ircDDBUsername, ircDDBPassword);

	m_ircDDB = new CIRCDDBGatewayConfigIrcDDBSet(noteBook, -1, APPLICATION_NAME, ircDDBEnabled, ircDDBHostname, ircDDBUsername, ircDDBPassword);
	noteBook->AddPage(m_ircDDB, wxT("ircDDB"), false);

	wxString aprsHostname;
	unsigned int aprsPort;
	bool aprsEnabled;
	m_config->getDPRS(aprsEnabled, aprsHostname, aprsPort);

	m_dprs = new CDPRSSet(noteBook, -1, APPLICATION_NAME, aprsEnabled, aprsHostname, aprsPort);
	noteBook->AddPage(m_dprs, wxT("D-PRS"), false);

	m_dextra = new CDExtraSet(noteBook, -1, APPLICATION_NAME, dextraEnabled, maxDExtraDongles, MAX_DEXTRA_LINKS);
	noteBook->AddPage(m_dextra, wxT("DExtra"), false);

	m_dplus = new CDPlusSet(noteBook, -1, APPLICATION_NAME, dplusEnabled, maxDPlusDongles, MAX_DPLUS_LINKS, dplusLogin);
	noteBook->AddPage(m_dplus, wxT("D-Plus"), false);

	m_dcs = new CDCSSet(noteBook, -1, APPLICATION_NAME, dcsEnabled, ccsEnabled, ccsHost);
	noteBook->AddPage(m_dcs, _("DCS and CCS"), false);

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	wxString starNetBand1, starNetCallsign1, starNetLogoff1, starNetInfo1, starNetLink1, starNetPermanent1;
	unsigned int starNetUserTimeout1, starNetGroupTimeout1;
	STARNET_CALLSIGN_SWITCH starNetCallsignSwitch1;
	bool starNetTXMsgSwitch1;
	m_config->getStarNet1(starNetBand1, starNetCallsign1, starNetLogoff1, starNetInfo1, starNetPermanent1, starNetUserTimeout1, starNetGroupTimeout1, starNetCallsignSwitch1, starNetTXMsgSwitch1, starNetLink1);

	m_starNet1 = new CStarNetSet(noteBook, -1, APPLICATION_NAME, starNetBand1, starNetCallsign1, starNetLogoff1, starNetInfo1, starNetPermanent1, starNetUserTimeout1, starNetGroupTimeout1, starNetCallsignSwitch1, starNetTXMsgSwitch1, starNetLink1);
	noteBook->AddPage(m_starNet1, wxT("StarNet 1"), false);

	wxString starNetBand2, starNetCallsign2, starNetLogoff2, starNetInfo2, starNetLink2, starNetPermanent2;
	unsigned int starNetUserTimeout2, starNetGroupTimeout2;
	STARNET_CALLSIGN_SWITCH starNetCallsignSwitch2;
	bool starNetTXMsgSwitch2;
	m_config->getStarNet2(starNetBand2, starNetCallsign2, starNetLogoff2, starNetInfo2, starNetPermanent2, starNetUserTimeout2, starNetGroupTimeout2, starNetCallsignSwitch2, starNetTXMsgSwitch2, starNetLink2);

	m_starNet2 = new CStarNetSet(noteBook, -1, APPLICATION_NAME, starNetBand2, starNetCallsign2, starNetLogoff2, starNetInfo2, starNetPermanent2, starNetUserTimeout2, starNetGroupTimeout2, starNetCallsignSwitch2, starNetTXMsgSwitch2, starNetLink2);
	noteBook->AddPage(m_starNet2, wxT("StarNet 2"), false);

	wxString starNetBand3, starNetCallsign3, starNetLogoff3, starNetInfo3, starNetLink3, starNetPermanent3;
	unsigned int starNetUserTimeout3, starNetGroupTimeout3;
	STARNET_CALLSIGN_SWITCH starNetCallsignSwitch3;
	bool starNetTXMsgSwitch3;
	m_config->getStarNet3(starNetBand3, starNetCallsign3, starNetLogoff3, starNetInfo3, starNetPermanent3, starNetUserTimeout3, starNetGroupTimeout3, starNetCallsignSwitch3, starNetTXMsgSwitch3, starNetLink3);

	m_starNet3 = new CStarNetSet(noteBook, -1, APPLICATION_NAME, starNetBand3, starNetCallsign3, starNetLogoff3, starNetInfo3, starNetPermanent3, starNetUserTimeout3, starNetGroupTimeout3, starNetCallsignSwitch3, starNetTXMsgSwitch3, starNetLink3);
	noteBook->AddPage(m_starNet3, wxT("StarNet 3"), false);

	wxString starNetBand4, starNetCallsign4, starNetLogoff4, starNetInfo4, starNetLink4, starNetPermanent4;
	unsigned int starNetUserTimeout4, starNetGroupTimeout4;
	STARNET_CALLSIGN_SWITCH starNetCallsignSwitch4;
	bool starNetTXMsgSwitch4;
	m_config->getStarNet4(starNetBand4, starNetCallsign4, starNetLogoff4, starNetInfo4, starNetPermanent4, starNetUserTimeout4, starNetGroupTimeout4, starNetCallsignSwitch4, starNetTXMsgSwitch4, starNetLink4);

	m_starNet4 = new CStarNetSet(noteBook, -1, APPLICATION_NAME, starNetBand4, starNetCallsign4, starNetLogoff4, starNetInfo4, starNetPermanent4, starNetUserTimeout4, starNetGroupTimeout4, starNetCallsignSwitch4, starNetTXMsgSwitch4, starNetLink4);
	noteBook->AddPage(m_starNet4, wxT("StarNet 4"), false);

	wxString starNetBand5, starNetCallsign5, starNetLogoff5, starNetInfo5, starNetLink5, starNetPermanent5;
	unsigned int starNetUserTimeout5, starNetGroupTimeout5;
	STARNET_CALLSIGN_SWITCH starNetCallsignSwitch5;
	bool starNetTXMsgSwitch5;
	m_config->getStarNet5(starNetBand5, starNetCallsign5, starNetLogoff5, starNetInfo5, starNetPermanent5, starNetUserTimeout5, starNetGroupTimeout5, starNetCallsignSwitch5, starNetTXMsgSwitch5, starNetLink5);

	m_starNet5 = new CStarNetSet(noteBook, -1, APPLICATION_NAME, starNetBand5, starNetCallsign5, starNetLogoff5, starNetInfo5, starNetPermanent5, starNetUserTimeout5, starNetGroupTimeout5, starNetCallsignSwitch5, starNetTXMsgSwitch5, starNetLink5);
	noteBook->AddPage(m_starNet5, wxT("StarNet 5"), false);
#else
	wxString starNetBand1, starNetCallsign1, starNetLogoff1, starNetInfo1, starNetPermanent1;
	unsigned int starNetUserTimeout1, starNetGroupTimeout1;
	STARNET_CALLSIGN_SWITCH starNetCallsignSwitch1;
	bool starNetTXMsgSwitch1;
	m_config->getStarNet1(starNetBand1, starNetCallsign1, starNetLogoff1, starNetInfo1, starNetPermanent1, starNetUserTimeout1, starNetGroupTimeout1, starNetCallsignSwitch1, starNetTXMsgSwitch1);

	m_starNet1 = new CStarNetSet(noteBook, -1, APPLICATION_NAME, starNetBand1, starNetCallsign1, starNetLogoff1, starNetInfo1, starNetPermanent1, starNetUserTimeout1, starNetGroupTimeout1, starNetCallsignSwitch1, starNetTXMsgSwitch1);
	noteBook->AddPage(m_starNet1, wxT("StarNet 1"), false);

	wxString starNetBand2, starNetCallsign2, starNetLogoff2, starNetInfo2, starNetPermanent2;
	unsigned int starNetUserTimeout2, starNetGroupTimeout2;
	STARNET_CALLSIGN_SWITCH starNetCallsignSwitch2;
	bool starNetTXMsgSwitch2;
	m_config->getStarNet2(starNetBand2, starNetCallsign2, starNetLogoff2, starNetInfo2, starNetPermanent2, starNetUserTimeout2, starNetGroupTimeout2, starNetCallsignSwitch2, starNetTXMsgSwitch2);

	m_starNet2 = new CStarNetSet(noteBook, -1, APPLICATION_NAME, starNetBand2, starNetCallsign2, starNetLogoff2, starNetInfo2, starNetPermanent2, starNetUserTimeout2, starNetGroupTimeout2, starNetCallsignSwitch2, starNetTXMsgSwitch2);
	noteBook->AddPage(m_starNet2, wxT("StarNet 2"), false);

	wxString starNetBand3, starNetCallsign3, starNetLogoff3, starNetInfo3, starNetPermanent3;
	unsigned int starNetUserTimeout3, starNetGroupTimeout3;
	STARNET_CALLSIGN_SWITCH starNetCallsignSwitch3;
	bool starNetTXMsgSwitch3;
	m_config->getStarNet3(starNetBand3, starNetCallsign3, starNetLogoff3, starNetInfo3, starNetPermanent3, starNetUserTimeout3, starNetGroupTimeout3, starNetCallsignSwitch3, starNetTXMsgSwitch3);

	m_starNet3 = new CStarNetSet(noteBook, -1, APPLICATION_NAME, starNetBand3, starNetCallsign3, starNetLogoff3, starNetInfo3, starNetPermanent3, starNetUserTimeout3, starNetGroupTimeout3, starNetCallsignSwitch3, starNetTXMsgSwitch3);
	noteBook->AddPage(m_starNet3, wxT("StarNet 3"), false);

	wxString starNetBand4, starNetCallsign4, starNetLogoff4, starNetInfo4, starNetPermanent4;
	unsigned int starNetUserTimeout4, starNetGroupTimeout4;
	STARNET_CALLSIGN_SWITCH starNetCallsignSwitch4;
	bool starNetTXMsgSwitch4;
	m_config->getStarNet4(starNetBand4, starNetCallsign4, starNetLogoff4, starNetInfo4, starNetPermanent4, starNetUserTimeout4, starNetGroupTimeout4, starNetCallsignSwitch4, starNetTXMsgSwitch4);

	m_starNet4 = new CStarNetSet(noteBook, -1, APPLICATION_NAME, starNetBand4, starNetCallsign4, starNetLogoff4, starNetInfo4, starNetPermanent4, starNetUserTimeout4, starNetGroupTimeout4, starNetCallsignSwitch4, starNetTXMsgSwitch4);
	noteBook->AddPage(m_starNet4, wxT("StarNet 4"), false);

	wxString starNetBand5, starNetCallsign5, starNetLogoff5, starNetInfo5, starNetPermanent5;
	unsigned int starNetUserTimeout5, starNetGroupTimeout5;
	STARNET_CALLSIGN_SWITCH starNetCallsignSwitch5;
	bool starNetTXMsgSwitch5;
	m_config->getStarNet5(starNetBand5, starNetCallsign5, starNetLogoff5, starNetInfo5, starNetPermanent5, starNetUserTimeout5, starNetGroupTimeout5, starNetCallsignSwitch5, starNetTXMsgSwitch5);

	m_starNet5 = new CStarNetSet(noteBook, -1, APPLICATION_NAME, starNetBand5, starNetCallsign5, starNetLogoff5, starNetInfo5, starNetPermanent5, starNetUserTimeout5, starNetGroupTimeout5, starNetCallsignSwitch5, starNetTXMsgSwitch5);
	noteBook->AddPage(m_starNet5, wxT("StarNet 5"), false);
#endif

	unsigned int remotePort;
	wxString remotePassword;
	bool remoteEnabled;
	m_config->getRemote(remoteEnabled, remotePassword, remotePort);

	m_remote = new CRemoteSet(noteBook, -1, APPLICATION_NAME, remoteEnabled, remotePassword, remotePort);
	noteBook->AddPage(m_remote, wxT("Remote"), false);

	TEXT_LANG language;
	bool infoEnabled, echoEnabled, logEnabled, dratsEnabled, dtmfEnabled;
	m_config->getMiscellaneous(language, infoEnabled, echoEnabled, logEnabled, dratsEnabled, dtmfEnabled);

	m_miscellaneous = new CIRCDDBGatewayConfigMiscellaneousSet(noteBook, -1, APPLICATION_NAME, language, infoEnabled, echoEnabled, logEnabled, dratsEnabled, dtmfEnabled);
	noteBook->AddPage(m_miscellaneous, wxT("Misc"), false);

	sizer->Add(noteBook, 1, wxALL | wxGROW, BORDER_SIZE);

	panel->SetSizer(sizer);

	mainSizer->Add(panel);

	SetSizer(mainSizer);

	mainSizer->SetSizeHints(this);
}

CIRCDDBGatewayConfigFrame::~CIRCDDBGatewayConfigFrame()
{
	delete m_config;
}

wxMenuBar* CIRCDDBGatewayConfigFrame::createMenuBar()
{
	wxMenu* fileMenu = new wxMenu();
	fileMenu->Append(Menu_File_Save, _("Save"));
	fileMenu->AppendSeparator();
	fileMenu->Append(wxID_EXIT,      _("Exit"));

	wxMenu* helpMenu = new wxMenu();
	helpMenu->Append(wxID_ABOUT, _("About ircDDB Gateway Config"));

	wxMenuBar* menuBar = new wxMenuBar();
	menuBar->Append(fileMenu, _("File"));
	menuBar->Append(helpMenu, _("Help"));

	return menuBar;
}

void CIRCDDBGatewayConfigFrame::onQuit(wxCommandEvent&)
{
	Close(false);
}

void CIRCDDBGatewayConfigFrame::onClose(wxCloseEvent&)
{
	Destroy();
}

void CIRCDDBGatewayConfigFrame::onSave(wxCommandEvent&)
{
	if (!m_gateway->Validate() || !m_repeaterData1->Validate() || !m_repeaterInfo1->Validate() || !m_repeaterData2->Validate() ||
		!m_repeaterInfo2->Validate() || !m_repeaterData3->Validate() || !m_repeaterInfo3->Validate() || !m_repeaterData4->Validate() ||
		!m_repeaterInfo4->Validate() || !m_ircDDB->Validate() || !m_dprs->Validate() || !m_dplus->Validate() || !m_dcs->Validate() ||
		!m_starNet1->Validate() || !m_starNet2->Validate() || !m_starNet3->Validate() || !m_starNet4->Validate() ||
		!m_starNet5->Validate() || !m_remote->Validate() || !m_miscellaneous->Validate())
		return;

	GATEWAY_TYPE gatewayType = m_gateway->getType();
	wxString gatewayCallsign = m_gateway->getCallsign();
	wxString gatewayAddress  = m_gateway->getAddress();
	wxString icomAddress     = m_gateway->getIcomAddress();
	unsigned int icomPort    = m_gateway->getIcomPort();
	wxString hbAddress       = m_gateway->getHBAddress();
	unsigned int hbPort      = m_gateway->getHBPort();
	double latitude          = m_gateway->getLatitude();
	double longitude         = m_gateway->getLongitude();
	wxString description1    = m_gateway->getDescription1();
	wxString description2    = m_gateway->getDescription2();
	wxString url             = m_gateway->getURL();
	m_config->setGateway(gatewayType, gatewayCallsign, gatewayAddress, icomAddress, icomPort, hbAddress, hbPort, latitude, longitude, description1, description2, url);

	wxString repeaterBand1     = m_repeaterData1->getBand();
	HW_TYPE repeaterType1      = m_repeaterData1->getType();
	wxString repeaterAddress1  = m_repeaterData1->getAddress();
	unsigned int repeaterPort1 = m_repeaterData1->getPort();
	unsigned char band11       = m_repeaterData1->getBand1();
	unsigned char band12       = m_repeaterData1->getBand2();
	unsigned char band13       = m_repeaterData1->getBand3();
	wxString reflector1        = m_repeaterData1->getReflector();
	bool atStartup1            = m_repeaterData1->atStartup();
	RECONNECT reconnect1       = m_repeaterData1->getReconnect();
	double frequency1          = m_repeaterInfo1->getFrequency();
	double offset1             = m_repeaterInfo1->getOffset();
	double range1              = m_repeaterInfo1->getRange();
	double latitude1           = m_repeaterInfo1->getLatitude();
	double longitude1          = m_repeaterInfo1->getLongitude();
	double agl1                = m_repeaterInfo1->getAGL();
	wxString description11     = m_repeaterInfo1->getDescription1();
	wxString description12     = m_repeaterInfo1->getDescription2();
	wxString url1              = m_repeaterInfo1->getURL();
	m_config->setRepeater1(repeaterBand1, repeaterType1, repeaterAddress1, repeaterPort1, band11, band12, band13, reflector1, atStartup1, reconnect1, frequency1, offset1, range1, latitude1, longitude1, agl1, description11, description12, url1);

	wxString repeaterBand2     = m_repeaterData2->getBand();
	HW_TYPE repeaterType2      = m_repeaterData2->getType();
	wxString repeaterAddress2  = m_repeaterData2->getAddress();
	unsigned int repeaterPort2 = m_repeaterData2->getPort();
	unsigned char band21       = m_repeaterData2->getBand1();
	unsigned char band22       = m_repeaterData2->getBand2();
	unsigned char band23       = m_repeaterData2->getBand3();
	wxString reflector2        = m_repeaterData2->getReflector();
	bool atStartup2            = m_repeaterData2->atStartup();
	RECONNECT reconnect2       = m_repeaterData2->getReconnect();
	double frequency2          = m_repeaterInfo2->getFrequency();
	double offset2             = m_repeaterInfo2->getOffset();
	double range2              = m_repeaterInfo2->getRange();
	double latitude2           = m_repeaterInfo2->getLatitude();
	double longitude2          = m_repeaterInfo2->getLongitude();
	double agl2                = m_repeaterInfo2->getAGL();
	wxString description21     = m_repeaterInfo2->getDescription1();
	wxString description22     = m_repeaterInfo2->getDescription2();
	wxString url2              = m_repeaterInfo2->getURL();
	m_config->setRepeater2(repeaterBand2, repeaterType2, repeaterAddress2, repeaterPort2, band21, band22, band23, reflector2, atStartup2, reconnect2, frequency2, offset2, range2, latitude2, longitude2, agl2, description21, description22, url2);

	wxString repeaterBand3     = m_repeaterData3->getBand();
	HW_TYPE repeaterType3      = m_repeaterData3->getType();
	wxString repeaterAddress3  = m_repeaterData3->getAddress();
	unsigned int repeaterPort3 = m_repeaterData3->getPort();
	unsigned char band31       = m_repeaterData3->getBand1();
	unsigned char band32       = m_repeaterData3->getBand2();
	unsigned char band33       = m_repeaterData3->getBand3();
	wxString reflector3        = m_repeaterData3->getReflector();
	bool atStartup3            = m_repeaterData3->atStartup();
	RECONNECT reconnect3       = m_repeaterData3->getReconnect();
	double frequency3          = m_repeaterInfo3->getFrequency();
	double offset3             = m_repeaterInfo3->getOffset();
	double range3              = m_repeaterInfo3->getRange();
	double latitude3           = m_repeaterInfo3->getLatitude();
	double longitude3          = m_repeaterInfo3->getLongitude();
	double agl3                = m_repeaterInfo3->getAGL();
	wxString description31     = m_repeaterInfo3->getDescription1();
	wxString description32     = m_repeaterInfo3->getDescription2();
	wxString url3              = m_repeaterInfo3->getURL();
	m_config->setRepeater3(repeaterBand3, repeaterType3, repeaterAddress3, repeaterPort3, band31, band32, band33, reflector3, atStartup3, reconnect3, frequency3, offset3, range3, latitude3, longitude3, agl3, description31, description32, url3);

	wxString repeaterBand4     = m_repeaterData4->getBand();
	HW_TYPE repeaterType4      = m_repeaterData4->getType();
	wxString repeaterAddress4  = m_repeaterData4->getAddress();
	unsigned int repeaterPort4 = m_repeaterData4->getPort();
	unsigned char band41       = m_repeaterData4->getBand1();
	unsigned char band42       = m_repeaterData4->getBand2();
	unsigned char band43       = m_repeaterData4->getBand3();
	wxString reflector4        = m_repeaterData4->getReflector();
	bool atStartup4            = m_repeaterData4->atStartup();
	RECONNECT reconnect4       = m_repeaterData4->getReconnect();
	double frequency4          = m_repeaterInfo4->getFrequency();
	double offset4             = m_repeaterInfo4->getOffset();
	double range4              = m_repeaterInfo4->getRange();
	double latitude4           = m_repeaterInfo4->getLatitude();
	double longitude4          = m_repeaterInfo4->getLongitude();
	double agl4                = m_repeaterInfo4->getAGL();
	wxString description41     = m_repeaterInfo4->getDescription1();
	wxString description42     = m_repeaterInfo4->getDescription2();
	wxString url4              = m_repeaterInfo4->getURL();
	m_config->setRepeater4(repeaterBand4, repeaterType4, repeaterAddress4, repeaterPort4, band41, band42, band43, reflector4, atStartup4, reconnect4, frequency4, offset4, range4, latitude4, longitude4, agl4, description41, description42, url4);

	bool ircDDBEnabled      = m_ircDDB->getEnabled();
	wxString ircDDBHostname = m_ircDDB->getHostname();
	wxString ircDDBUsername = m_ircDDB->getUsername();
	wxString ircDDBPassword = m_ircDDB->getPassword();
	m_config->setIrcDDB(ircDDBEnabled, ircDDBHostname, ircDDBUsername, ircDDBPassword);

	bool aprsEnabled      = m_dprs->getEnabled();
	wxString aprsHostname = m_dprs->getHostname();
	unsigned int aprsPort = m_dprs->getPort();
	m_config->setDPRS(aprsEnabled, aprsHostname, aprsPort);

	bool dextraEnabled            = m_dextra->getEnabled();
	unsigned int maxDExtraDongles = m_dextra->getMaxDongles();
	m_config->setDExtra(dextraEnabled, maxDExtraDongles);

	bool dplusEnabled             = m_dplus->getEnabled();
	unsigned int maxDPlusDongles  = m_dplus->getMaxDongles();
	wxString dplusLogin           = m_dplus->getLogin();
	m_config->setDPlus(dplusEnabled, maxDPlusDongles, dplusLogin);

	bool dcsEnabled  = m_dcs->getDCSEnabled();
	bool ccsEnabled  = m_dcs->getCCSEnabled();
	wxString ccsHost = m_dcs->getCCSHost();
	m_config->setDCS(dcsEnabled, ccsEnabled, ccsHost);

	wxString starNetBand1             = m_starNet1->getBand();
	wxString starNetCallsign1         = m_starNet1->getCallsign();
	wxString starNetLogoff1           = m_starNet1->getLogoff();
	wxString starNetInfo1             = m_starNet1->getInfo();
	wxString starNetPermanent1        = m_starNet1->getPermanent();
	unsigned int starNetUserTimeout1  = m_starNet1->getUserTimeout();
	unsigned int starNetGroupTimeout1 = m_starNet1->getGroupTimeout();
	STARNET_CALLSIGN_SWITCH starNetCallsignSwitch1 = m_starNet1->getCallsignSwitch();
	bool starNetTXMsgSwitch1          = m_starNet1->getTXMsgSwitch();
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	wxString starNetLink1             = m_starNet1->getReflector();
	m_config->setStarNet1(starNetBand1, starNetCallsign1, starNetLogoff1, starNetInfo1, starNetPermanent1, starNetUserTimeout1, starNetGroupTimeout1, starNetCallsignSwitch1, starNetTXMsgSwitch1, starNetLink1);
#else
	m_config->setStarNet1(starNetBand1, starNetCallsign1, starNetLogoff1, starNetInfo1, starNetPermanent1, starNetUserTimeout1, starNetGroupTimeout1, starNetCallsignSwitch1, starNetTXMsgSwitch1);
#endif

	wxString starNetBand2             = m_starNet2->getBand();
	wxString starNetCallsign2         = m_starNet2->getCallsign();
	wxString starNetLogoff2           = m_starNet2->getLogoff();
	wxString starNetInfo2             = m_starNet2->getInfo();
	wxString starNetPermanent2        = m_starNet2->getPermanent();
	unsigned int starNetUserTimeout2  = m_starNet2->getUserTimeout();
	unsigned int starNetGroupTimeout2 = m_starNet2->getGroupTimeout();
	STARNET_CALLSIGN_SWITCH starNetCallsignSwitch2 = m_starNet2->getCallsignSwitch();
	bool starNetTXMsgSwitch2          = m_starNet2->getTXMsgSwitch();
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	wxString starNetLink2             = m_starNet2->getReflector();
	m_config->setStarNet2(starNetBand2, starNetCallsign2, starNetLogoff2, starNetInfo2, starNetPermanent2, starNetUserTimeout2, starNetGroupTimeout2, starNetCallsignSwitch2, starNetTXMsgSwitch2, starNetLink2);
#else
	m_config->setStarNet2(starNetBand2, starNetCallsign2, starNetLogoff2, starNetInfo2, starNetPermanent2, starNetUserTimeout2, starNetGroupTimeout2, starNetCallsignSwitch2, starNetTXMsgSwitch2);
#endif

	wxString starNetBand3             = m_starNet3->getBand();
	wxString starNetCallsign3         = m_starNet3->getCallsign();
	wxString starNetLogoff3           = m_starNet3->getLogoff();
	wxString starNetInfo3             = m_starNet3->getInfo();
	wxString starNetPermanent3        = m_starNet3->getPermanent();
	unsigned int starNetUserTimeout3  = m_starNet3->getUserTimeout();
	unsigned int starNetGroupTimeout3 = m_starNet3->getGroupTimeout();
	STARNET_CALLSIGN_SWITCH starNetCallsignSwitch3 = m_starNet3->getCallsignSwitch();
	bool starNetTXMsgSwitch3          = m_starNet3->getTXMsgSwitch();
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	wxString starNetLink3             = m_starNet3->getReflector();
	m_config->setStarNet3(starNetBand3, starNetCallsign3, starNetLogoff3, starNetInfo3, starNetPermanent3, starNetUserTimeout3, starNetGroupTimeout3, starNetCallsignSwitch3, starNetTXMsgSwitch3, starNetLink3);
#else
	m_config->setStarNet3(starNetBand3, starNetCallsign3, starNetLogoff3, starNetInfo3, starNetPermanent3, starNetUserTimeout3, starNetGroupTimeout3, starNetCallsignSwitch3, starNetTXMsgSwitch3);
#endif

	wxString starNetBand4             = m_starNet4->getBand();
	wxString starNetCallsign4         = m_starNet4->getCallsign();
	wxString starNetLogoff4           = m_starNet4->getLogoff();
	wxString starNetInfo4             = m_starNet4->getInfo();
	wxString starNetPermanent4        = m_starNet4->getPermanent();
	unsigned int starNetUserTimeout4  = m_starNet4->getUserTimeout();
	unsigned int starNetGroupTimeout4 = m_starNet4->getGroupTimeout();
	STARNET_CALLSIGN_SWITCH starNetCallsignSwitch4 = m_starNet4->getCallsignSwitch();
	bool starNetTXMsgSwitch4          = m_starNet4->getTXMsgSwitch();
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	wxString starNetLink4             = m_starNet4->getReflector();
	m_config->setStarNet4(starNetBand4, starNetCallsign4, starNetLogoff4, starNetInfo4, starNetPermanent4, starNetUserTimeout4, starNetGroupTimeout4, starNetCallsignSwitch4, starNetTXMsgSwitch4, starNetLink4);
#else
	m_config->setStarNet4(starNetBand4, starNetCallsign4, starNetLogoff4, starNetInfo4, starNetPermanent4, starNetUserTimeout4, starNetGroupTimeout4, starNetCallsignSwitch4, starNetTXMsgSwitch4);
#endif

	wxString starNetBand5             = m_starNet5->getBand();
	wxString starNetCallsign5         = m_starNet5->getCallsign();
	wxString starNetLogoff5           = m_starNet5->getLogoff();
	wxString starNetInfo5             = m_starNet5->getInfo();
	wxString starNetPermanent5        = m_starNet5->getPermanent();
	unsigned int starNetUserTimeout5  = m_starNet5->getUserTimeout();
	unsigned int starNetGroupTimeout5 = m_starNet5->getGroupTimeout();
	STARNET_CALLSIGN_SWITCH starNetCallsignSwitch5 = m_starNet5->getCallsignSwitch();
	bool starNetTXMsgSwitch5          = m_starNet5->getTXMsgSwitch();
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	wxString starNetLink5             = m_starNet5->getReflector();
	m_config->setStarNet5(starNetBand5, starNetCallsign5, starNetLogoff5, starNetInfo5, starNetPermanent5, starNetUserTimeout5, starNetGroupTimeout5, starNetCallsignSwitch5, starNetTXMsgSwitch5, starNetLink5);
#else
	m_config->setStarNet5(starNetBand5, starNetCallsign5, starNetLogoff5, starNetInfo5, starNetPermanent5, starNetUserTimeout5, starNetGroupTimeout5, starNetCallsignSwitch5, starNetTXMsgSwitch5);
#endif

	bool remoteEnabled      = m_remote->getEnabled();
	wxString remotePassword = m_remote->getPassword();
	unsigned int remotePort = m_remote->getPort();
	m_config->setRemote(remoteEnabled, remotePassword, remotePort);

	TEXT_LANG language = m_miscellaneous->getLanguage();
	bool infoEnabled   = m_miscellaneous->getInfoEnabled();
	bool echoEnabled   = m_miscellaneous->getEchoEnabled();
	bool logEnabled    = m_miscellaneous->getLogEnabled();
	bool dratsEnabled  = m_miscellaneous->getDRATSEnabled();
	bool dtmfEnabled   = m_miscellaneous->getDTMFEnabled();
	m_config->setMiscellaneous(language, infoEnabled, echoEnabled, logEnabled, dratsEnabled, dtmfEnabled);

	bool ret = m_config->write();
	if (!ret) {
		wxMessageDialog dialog(this, _("There was an error when writing the ircDDB Gateway configuration file"), _("Error"), wxICON_ERROR);
		dialog.ShowModal();
	} else {
		wxMessageDialog dialog(this, _("The changes made will not take effect\nuntil the ircDDB Gateway is (re)started"), _("Information"), wxICON_INFORMATION);
		dialog.ShowModal();
	}
}

void CIRCDDBGatewayConfigFrame::onAbout(wxCommandEvent&)
{
	wxAboutDialogInfo info;
	info.AddDeveloper(wxT("Jonathan Naylor, G4KLX"));
	info.SetCopyright(wxT("(C) 2010-2014 using GPL v2 or later"));
	info.SetName(APPLICATION_NAME);
	info.SetVersion(VERSION);
	info.SetDescription(_("This program configures the ircDDB Gateway."));

	::wxAboutBox(info);
}
