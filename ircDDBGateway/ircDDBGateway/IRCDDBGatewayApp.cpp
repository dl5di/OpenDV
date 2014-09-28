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

#include "DummyRepeaterProtocolHandler.h"
#include "IcomRepeaterProtocolHandler.h"
#include "HBRepeaterProtocolHandler.h"
#include "IRCDDBGatewayLogRedirect.h"
#include "IRCDDBGatewayThread.h"
#include "IRCDDBGatewayDefs.h"
#include "IRCDDBGatewayApp.h"
#include "CallsignList.h"
#include "APRSWriter.h"
#include "Version.h"
#include "Logger.h"
#include "IRCDDB.h"
#include "Utils.h"

#include <wx/config.h>
#include <wx/cmdline.h>
#include <wx/filename.h>

IMPLEMENT_APP(CIRCDDBGatewayApp)

const wxChar*       NAME_PARAM = wxT("Gateway Name");
const wxChar* NOLOGGING_SWITCH = wxT("nolog");
const wxChar*       GUI_SWITCH = wxT("gui");
const wxChar*    LOGDIR_OPTION = wxT("logdir");
const wxChar*   CONFDIR_OPTION = wxT("confdir");

CIRCDDBGatewayApp::CIRCDDBGatewayApp() :
wxApp(),
m_name(),
m_nolog(false),
m_gui(false),
m_logDir(),
m_confDir(),
m_frame(NULL),
m_thread(NULL),
m_config(NULL),
m_checker(NULL),
m_logChain(NULL)
{
}

CIRCDDBGatewayApp::~CIRCDDBGatewayApp()
{
}

bool CIRCDDBGatewayApp::OnInit()
{
	SetVendorName(VENDOR_NAME);

	if (!wxApp::OnInit())
		return false;

	if (!m_nolog) {
		wxString logBaseName = LOG_BASE_NAME;
		if (!m_name.IsEmpty()) {
			logBaseName.Append(wxT("_"));
			logBaseName.Append(m_name);
		}

#if defined(__WINDOWS__)
		if (m_logDir.IsEmpty())
			m_logDir = ::wxGetHomeDir();
#else
		if (m_logDir.IsEmpty())
			m_logDir = wxT(LOG_DIR);
#endif

		wxLog* log = new CLogger(m_logDir, logBaseName);
		wxLog::SetActiveTarget(log);
	} else {
		new wxLogNull;
	}

	m_logChain = new wxLogChain(new CIRCDDBGatewayLogRedirect);

	wxString appName;
	if (!m_name.IsEmpty())
		appName = APPLICATION_NAME + wxT(" ") + m_name;
	else
		appName = APPLICATION_NAME;

#if !defined(__WINDOWS__)
	appName.Replace(wxT(" "), wxT("_"));
	m_checker = new wxSingleInstanceChecker(appName, wxT("/tmp"));
#else
	m_checker = new wxSingleInstanceChecker(appName);
#endif

	bool ret = m_checker->IsAnotherRunning();
	if (ret) {
		wxLogError(wxT("Another copy of the ircDDB Gateway is running, exiting"));
		return false;
	}

#if defined(__WINDOWS__)
	if (m_confDir.IsEmpty())
		m_confDir = ::wxGetHomeDir();

	m_config = new CIRCDDBGatewayConfig(new wxConfig(APPLICATION_NAME), m_confDir, CONFIG_FILE_NAME, m_name);
#else
	if (m_confDir.IsEmpty())
		m_confDir = wxT(CONF_DIR);

	m_config = new CIRCDDBGatewayConfig(m_confDir, CONFIG_FILE_NAME, m_name);
#endif

	wxString frameName = APPLICATION_NAME + wxT(" - ");
	if (!m_name.IsEmpty()) {
		frameName.Append(m_name);
		frameName.Append(wxT(" - "));
	}
	frameName.Append(VERSION);

	wxPoint position = wxDefaultPosition;

	int x, y;
	m_config->getPosition(x, y);
	if (x >= 0 && y >= 0)
		position = wxPoint(x, y);

	m_frame = new CIRCDDBGatewayFrame(frameName, position, m_gui);
	m_frame->Show();

	SetTopWindow(m_frame);

	wxLogInfo(wxT("Starting ") + APPLICATION_NAME + wxT(" - ") + VERSION);

	// Log the SVN revsion and the version of wxWidgets and the Operating System
	wxLogInfo(SVNREV);
	wxLogInfo(wxT("Using wxWidgets %d.%d.%d on %s"), wxMAJOR_VERSION, wxMINOR_VERSION, wxRELEASE_NUMBER, ::wxGetOsDescription().c_str());

	createThread();

	return true;
}

int CIRCDDBGatewayApp::OnExit()
{
	m_logChain->SetLog(NULL);

	wxLogInfo(APPLICATION_NAME + wxT(" is exiting"));

	m_thread->kill();

	delete m_config;

	delete m_checker;

	return 0;
}

void CIRCDDBGatewayApp::OnInitCmdLine(wxCmdLineParser& parser)
{
	parser.AddSwitch(NOLOGGING_SWITCH, wxEmptyString, wxEmptyString, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddSwitch(GUI_SWITCH,       wxEmptyString, wxEmptyString, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddOption(LOGDIR_OPTION,    wxEmptyString, wxEmptyString, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddOption(CONFDIR_OPTION,   wxEmptyString, wxEmptyString, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddParam(NAME_PARAM, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);

	wxApp::OnInitCmdLine(parser);
}

bool CIRCDDBGatewayApp::OnCmdLineParsed(wxCmdLineParser& parser)
{
	if (!wxApp::OnCmdLineParsed(parser))
		return false;

	m_nolog  = parser.Found(NOLOGGING_SWITCH);
	m_gui    = parser.Found(GUI_SWITCH);

	wxString logDir;
	bool found = parser.Found(LOGDIR_OPTION, &logDir);
	if (found)
		m_logDir = logDir;

	wxString confDir;
	found = parser.Found(CONFDIR_OPTION, &confDir);
	if (found)
		m_confDir = confDir;

	if (parser.GetParamCount() > 0U)
		m_name = parser.GetParam(0U);

	return true;
}

#if defined(__WXDEBUG__)
void CIRCDDBGatewayApp::OnAssertFailure(const wxChar* file, int line, const wxChar* func, const wxChar* cond, const wxChar* msg)
{
	wxLogFatalError(wxT("Assertion failed on line %d in file %s and function %s: %s %s"), line, file, func, cond, msg);
}
#endif

void CIRCDDBGatewayApp::showLog(const wxString& text)
{
	m_frame->showLog(text);
}

CIRCDDBGatewayStatusData* CIRCDDBGatewayApp::getStatus() const
{
	return m_thread->getStatus();
}

void CIRCDDBGatewayApp::setPosition(int x, int y)
{
	m_config->setPosition(x, y);
	m_config->write();
}

void CIRCDDBGatewayApp::createThread()
{
	wxASSERT(m_config != NULL);

	CIRCDDBGatewayThread* thread = new CIRCDDBGatewayThread(m_logDir, m_name);

	GATEWAY_TYPE gatewayType;
	wxString gatewayCallsign, gatewayAddress, icomAddress, hbAddress, description1, description2, url;
	unsigned int icomPort, hbPort;
	double latitude, longitude;
	m_config->getGateway(gatewayType, gatewayCallsign, gatewayAddress, icomAddress, icomPort, hbAddress, hbPort, latitude, longitude, description1, description2, url);

	gatewayCallsign.MakeUpper();
	gatewayCallsign.Append(wxT("        "));
	gatewayCallsign.Truncate(LONG_CALLSIGN_LENGTH - 1U);

	wxString callsign = gatewayCallsign;
	callsign.Append(wxT(" "));

	gatewayCallsign.Append(wxT("G"));

	wxLogInfo(wxT("Gateway type: %d, callsign: \"%s\", address: %s, Icom address: %s:%u, homebrew address: %s:%u, latitude: %lf, longitude: %lf, description: \"%s %s\", URL: \"%s\""), int(gatewayType), gatewayCallsign.c_str(), gatewayAddress.c_str(), icomAddress.c_str(), icomPort, hbAddress.c_str(), hbPort, latitude, longitude, description1.c_str(), description2.c_str(), url.c_str());

	thread->setGateway(gatewayType, gatewayCallsign, gatewayAddress);

	wxString aprsHostname;
	unsigned int aprsPort;
	bool aprsEnabled;
	m_config->getDPRS(aprsEnabled, aprsHostname, aprsPort);
	wxLogInfo(wxT("APRS enabled: %d, host: %s:%u"), int(aprsEnabled), aprsHostname.c_str(), aprsPort);

	CAPRSWriter* aprs = NULL;
	if (aprsEnabled && !gatewayCallsign.IsEmpty() && !aprsHostname.IsEmpty() && aprsPort != 0U) {
		aprs = new CAPRSWriter(aprsHostname, aprsPort, gatewayCallsign, gatewayAddress);

		bool res = aprs->open();
		if (!res)
			wxLogError(wxT("Cannot initialise the APRS data writer"));
		else
			thread->setAPRSWriter(aprs);
	}

	TEXT_LANG language;
	bool infoEnabled, echoEnabled, logEnabled, dratsEnabled, dtmfEnabled;
	m_config->getMiscellaneous(language, infoEnabled, echoEnabled, logEnabled, dratsEnabled, dtmfEnabled);
	wxLogInfo(wxT("Language: %d, info enabled: %d, echo enabled: %d, log enabled : %d, D-RATS enabled: %d, DTMF control enabled: %d"), int(language), int(infoEnabled), int(echoEnabled), int(logEnabled), int(dratsEnabled), int(dtmfEnabled));

	CIcomRepeaterProtocolHandler* icomRepeaterHandler = NULL;
	CHBRepeaterProtocolHandler* hbRepeaterHandler = NULL;
	CDummyRepeaterProtocolHandler* dummyRepeaterHandler = NULL;

	unsigned int icomCount = 0U;

	wxString repeaterCall1, repeaterBand1, repeaterAddress1, reflector1, description11, description12, url1;
	double frequency1, offset1, range1, latitude1, longitude1, agl1;
	unsigned char band11, band12, band13;
	unsigned int repeaterPort1;
	HW_TYPE repeaterType1;
	bool atStartup1;
	RECONNECT reconnect1;
	m_config->getRepeater1(repeaterCall1, repeaterBand1, repeaterType1, repeaterAddress1, repeaterPort1, band11, band12, band13, reflector1, atStartup1, reconnect1, frequency1, offset1, range1, latitude1, longitude1, agl1, description11, description12, url1);

	CUtils::clean(description11, wxT("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 .,&*()-+=@/?:;"));
	CUtils::clean(description12, wxT("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 .,&*()-+=@/?:;"));
	CUtils::clean(url1, wxT("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 .,&*()-+=@/?:;"));

	wxString callsign1 = callsign;
	if (!repeaterBand1.IsSameAs(wxT(" "))) {
		if (!repeaterCall1.IsEmpty()) {
			callsign1 = repeaterCall1;
			callsign1.Append(wxT("        "));
			callsign1.Truncate(LONG_CALLSIGN_LENGTH);
		}

		wxLogInfo(wxT("Repeater 1 callsign: \"%.7s%s\", hardware type: %d, address: %s:%u"), callsign1.c_str(), repeaterBand1.c_str(), int(repeaterType1), repeaterAddress1.c_str(), repeaterPort1);
		wxLogInfo(wxT("Repeater 1 reflector: %s, at startup: %d, reconnect: %d"), reflector1.c_str(), atStartup1, reconnect1);
		wxLogInfo(wxT("Repeater 1 latitude: %lf, longitude: %lf, range: %.0lf kms, height: %.0lf m, frequency: %.4lf MHz, offset: %.4lf MHz"), latitude1, longitude1, range1, agl1, frequency1, offset1);
		wxLogInfo(wxT("Repeater 1 description: \"%s %s\", URL: \"%s\""), description11.c_str(), description12.c_str(), url1.c_str());

		if (repeaterType1 == HW_ICOM && !icomAddress.IsEmpty()) {
			icomRepeaterHandler = new CIcomRepeaterProtocolHandler(icomAddress, icomPort, repeaterAddress1, repeaterPort1);
			bool res = icomRepeaterHandler->open();
			if (!res) {
				wxLogError(wxT("Cannot open the Icom repeater protocol handler"));
				delete icomRepeaterHandler;
				icomRepeaterHandler = NULL;
			}
		} else if (repeaterType1 == HW_HOMEBREW && !hbAddress.IsEmpty()) {
			hbRepeaterHandler = new CHBRepeaterProtocolHandler(hbAddress, hbPort);
			bool res = hbRepeaterHandler->open();
			if (!res) {
				wxLogError(wxT("Cannot open the Homebrew repeater protocol handler"));
				delete hbRepeaterHandler;
				hbRepeaterHandler = NULL;
			}
		} else if (repeaterType1 == HW_DUMMY && !hbAddress.IsEmpty()) {
			dummyRepeaterHandler = new CDummyRepeaterProtocolHandler;
			bool res = dummyRepeaterHandler->open();
			if (!res) {
				wxLogError(wxT("Cannot open the Dummy repeater protocol handler"));
				delete dummyRepeaterHandler;
				dummyRepeaterHandler = NULL;
			}
		}

		if (latitude1 == 0.0 && longitude1 == 0.0) {
			latitude1  = latitude;
			longitude1 = longitude;
		}

		if (description11.IsEmpty())
			description11 = description1;
		if (description12.IsEmpty())
			description12 = description2;

		if (url1.IsEmpty())
			url1 = url;

		if (repeaterType1 == HW_ICOM && icomRepeaterHandler != NULL) {
			wxLogInfo(wxT("Repeater 1 bands: %u %u %u"), band11, band12, band13);
			thread->addRepeater(callsign1, repeaterBand1, repeaterAddress1, repeaterPort1, repeaterType1, reflector1, atStartup1, reconnect1, dratsEnabled, frequency1, offset1, range1, latitude1, longitude1, agl1, description11, description12, url1, icomRepeaterHandler, band11, band12, band13);

			if (aprs != NULL)
				aprs->setPort(callsign1, repeaterBand1, frequency1, offset1, range1, latitude1, longitude1, agl1);

			icomCount++;
		} else if (repeaterType1 == HW_HOMEBREW && hbRepeaterHandler != NULL) {
			thread->addRepeater(callsign1, repeaterBand1, repeaterAddress1, repeaterPort1, repeaterType1, reflector1, atStartup1, reconnect1, dratsEnabled, frequency1, offset1, range1, latitude1, longitude1, agl1, description11, description12, url1, hbRepeaterHandler);

			if (aprs != NULL)
				aprs->setPort(callsign1, repeaterBand1, frequency1, offset1, range1, latitude1, longitude1, agl1);
		} else if (repeaterType1 == HW_DUMMY && dummyRepeaterHandler != NULL) {
			thread->addRepeater(callsign1, repeaterBand1, repeaterAddress1, repeaterPort1, repeaterType1, reflector1, atStartup1, reconnect1, dratsEnabled, frequency1, offset1, range1, latitude1, longitude1, agl1, description11, description12, url1, dummyRepeaterHandler);
		}
	}

	wxString repeaterCall2, repeaterBand2, repeaterAddress2, reflector2, description21, description22, url2;
	double frequency2, offset2, range2, latitude2, longitude2, agl2;
	unsigned char band21, band22, band23;
	unsigned int repeaterPort2;
	HW_TYPE repeaterType2;
	bool atStartup2;
	RECONNECT reconnect2;
	m_config->getRepeater2(repeaterCall2, repeaterBand2, repeaterType2, repeaterAddress2, repeaterPort2, band21, band22, band23, reflector2, atStartup2, reconnect2, frequency2, offset2, range2, latitude2, longitude2, agl2, description21, description22, url2);

	CUtils::clean(description21, wxT("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 .,&*()-+=@/?:;"));
	CUtils::clean(description22, wxT("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 .,&*()-+=@/?:;"));
	CUtils::clean(url2, wxT("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 .,&*()-+=@/?:;"));

	wxString callsign2 = callsign;
	if (!repeaterBand2.IsSameAs(wxT(" "))) {
		if (!repeaterCall2.IsEmpty()) {
			callsign2 = repeaterCall2;
			callsign2.Append(wxT("        "));
			callsign2.Truncate(LONG_CALLSIGN_LENGTH);
		}

		wxLogInfo(wxT("Repeater 2 callsign: \"%.7s%s\", hardware type: %d, address: %s:%u"), callsign2.c_str(), repeaterBand2.c_str(), int(repeaterType2), repeaterAddress2.c_str(), repeaterPort2);
		wxLogInfo(wxT("Repeater 2 reflector: %s, at startup: %d, reconnect: %d"), reflector2.c_str(), atStartup2, reconnect2);
		wxLogInfo(wxT("Repeater 2 latitude: %lf, longitude: %lf, range: %.0lf kms, height: %.0lf m, frequency: %.4lf MHz, offset: %.4lf MHz"), latitude2, longitude2, range2, agl2, frequency2, offset2);
		wxLogInfo(wxT("Repeater 2 description: \"%s %s\", URL: \"%s\""), description21.c_str(), description22.c_str(), url2.c_str());

		if (callsign1.IsSameAs(callsign2) && repeaterBand1.IsSameAs(repeaterBand2)) {
			wxLogError(wxT("Repeater 2 has the same callsign and module as repeater 1, exiting"));
			return;
		}

		if (repeaterType2 == HW_ICOM && !icomAddress.IsEmpty() && icomRepeaterHandler == NULL) {
			icomRepeaterHandler = new CIcomRepeaterProtocolHandler(icomAddress, icomPort, repeaterAddress2, repeaterPort2);
			bool res = icomRepeaterHandler->open();
			if (!res) {
				wxLogError(wxT("Cannot open the Icom repeater protocol handler"));
				delete icomRepeaterHandler;
				icomRepeaterHandler = NULL;
			}
		} else if (repeaterType2 == HW_HOMEBREW && !hbAddress.IsEmpty() && hbRepeaterHandler == NULL) {
			hbRepeaterHandler = new CHBRepeaterProtocolHandler(hbAddress, hbPort);
			bool res = hbRepeaterHandler->open();
			if (!res) {
				wxLogError(wxT("Cannot open the Homebrew repeater protocol handler"));
				delete hbRepeaterHandler;
				hbRepeaterHandler = NULL;
			}
		} else if (repeaterType2 == HW_DUMMY && !hbAddress.IsEmpty() && dummyRepeaterHandler == NULL) {
			dummyRepeaterHandler = new CDummyRepeaterProtocolHandler;
			bool res = dummyRepeaterHandler->open();
			if (!res) {
				wxLogError(wxT("Cannot open the Dummy repeater protocol handler"));
				delete dummyRepeaterHandler;
				dummyRepeaterHandler = NULL;
			}
		}

		if (latitude2 == 0.0 && longitude2 == 0.0) {
			latitude2  = latitude;
			longitude2 = longitude;
		}

		if (description21.IsEmpty())
			description21 = description1;
		if (description22.IsEmpty())
			description22 = description2;

		if (url2.IsEmpty())
			url2 = url;

		if (repeaterType2 == HW_ICOM && icomRepeaterHandler != NULL) {
			wxLogInfo(wxT("Repeater 2 bands: %u %u %u"), band21, band22, band23);
			thread->addRepeater(callsign2, repeaterBand2, repeaterAddress2, repeaterPort2, repeaterType2, reflector2, atStartup2, reconnect2, dratsEnabled, frequency2, offset2, range2, latitude2, longitude2, agl2, description21, description22, url2, icomRepeaterHandler, band21, band22, band23);

			if (aprs != NULL)
				aprs->setPort(callsign2, repeaterBand2, frequency2, offset2, range2, latitude2, longitude2, agl2);

			icomCount++;
		} else if (repeaterType2 == HW_HOMEBREW && hbRepeaterHandler != NULL) {
			thread->addRepeater(callsign2, repeaterBand2, repeaterAddress2, repeaterPort2, repeaterType2, reflector2, atStartup2, reconnect2, dratsEnabled, frequency2, offset2, range2, latitude2, longitude2, agl2, description21, description22, url2, hbRepeaterHandler);

			if (aprs != NULL)
				aprs->setPort(callsign2, repeaterBand2, frequency2, offset2, range2, latitude2, longitude2, agl2);
		} else if (repeaterType2 == HW_DUMMY && dummyRepeaterHandler != NULL) {
			thread->addRepeater(callsign2, repeaterBand2, repeaterAddress2, repeaterPort2, repeaterType2, reflector2, atStartup2, reconnect2, dratsEnabled, frequency2, offset2, range2, latitude2, longitude2, agl2, description21, description22, url2, dummyRepeaterHandler);
		}
	}

	wxString repeaterCall3, repeaterBand3, repeaterAddress3, reflector3, description31, description32, url3;
	double frequency3, offset3, range3, latitude3, longitude3, agl3;
	unsigned char band31, band32, band33;
	unsigned int repeaterPort3;
	HW_TYPE repeaterType3;
	bool atStartup3;
	RECONNECT reconnect3;
	m_config->getRepeater3(repeaterCall3, repeaterBand3, repeaterType3, repeaterAddress3, repeaterPort3, band31, band32, band33, reflector3, atStartup3, reconnect3, frequency3, offset3, range3, latitude3, longitude3, agl3, description31, description32, url3);

	CUtils::clean(description31, wxT("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 .,&*()-+=@/?:;"));
	CUtils::clean(description32, wxT("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 .,&*()-+=@/?:;"));
	CUtils::clean(url3, wxT("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 .,&*()-+=@/?:;"));

	wxString callsign3 = callsign;
	if (!repeaterBand3.IsSameAs(wxT(" "))) {
		if (!repeaterCall3.IsEmpty()) {
			callsign3 = repeaterCall3;
			callsign3.Append(wxT("        "));
			callsign3.Truncate(LONG_CALLSIGN_LENGTH);
		}

		wxLogInfo(wxT("Repeater 3 callsign: \"%.7s%s\", hardware type: %d, address: %s:%u"), callsign3.c_str(), repeaterBand3.c_str(), int(repeaterType3), repeaterAddress3.c_str(), repeaterPort3);
		wxLogInfo(wxT("Repeater 3 reflector: %s, at startup: %d, reconnect: %d"), reflector3.c_str(), atStartup3, reconnect3);
		wxLogInfo(wxT("Repeater 3 latitude: %lf, longitude: %lf, range: %.0lf kms, height: %.0lf m, frequency: %.4lf MHz, offset: %.4lf MHz"), latitude3, longitude3, range3, agl3, frequency3, offset3);
		wxLogInfo(wxT("Repeater 3 description: \"%s %s\", URL: \"%s\""), description31.c_str(), description32.c_str(), url3.c_str());

		if (callsign1.IsSameAs(callsign3) && repeaterBand1.IsSameAs(repeaterBand3)) {
			wxLogError(wxT("Repeater 3 has the same callsign and module as repeater 1, exiting"));
			return;
		}
		if (callsign2.IsSameAs(callsign3) && repeaterBand2.IsSameAs(repeaterBand3)) {
			wxLogError(wxT("Repeater 3 has the same callsign and module as repeater 2, exiting"));
			return;
		}

		if (repeaterType3 == HW_ICOM && !icomAddress.IsEmpty() && icomRepeaterHandler == NULL) {
			icomRepeaterHandler = new CIcomRepeaterProtocolHandler(icomAddress, icomPort, repeaterAddress3, repeaterPort3);
			bool res = icomRepeaterHandler->open();
			if (!res) {
				wxLogError(wxT("Cannot open the Icom repeater protocol handler"));
				delete icomRepeaterHandler;
				icomRepeaterHandler = NULL;
			}
		} else if (repeaterType3 == HW_HOMEBREW && !hbAddress.IsEmpty() && hbRepeaterHandler == NULL) {
			hbRepeaterHandler = new CHBRepeaterProtocolHandler(hbAddress, hbPort);
			bool res = hbRepeaterHandler->open();
			if (!res) {
				wxLogError(wxT("Cannot open the Homebrew repeater protocol handler"));
				delete hbRepeaterHandler;
				hbRepeaterHandler = NULL;
			}
		} else if (repeaterType3 == HW_DUMMY && !hbAddress.IsEmpty() && dummyRepeaterHandler == NULL) {
			dummyRepeaterHandler = new CDummyRepeaterProtocolHandler;
			bool res = dummyRepeaterHandler->open();
			if (!res) {
				wxLogError(wxT("Cannot open the Dummy repeater protocol handler"));
				delete dummyRepeaterHandler;
				dummyRepeaterHandler = NULL;
			}
		}

		if (latitude3 == 0.0 && longitude3 == 0.0) {
			latitude3  = latitude;
			longitude3 = longitude;
		}

		if (description31.IsEmpty())
			description31 = description1;
		if (description32.IsEmpty())
			description32 = description2;

		if (url3.IsEmpty())
			url3 = url;

		if (repeaterType3 == HW_ICOM && icomRepeaterHandler != NULL) {
			wxLogInfo(wxT("Repeater 3 bands: %u %u %u"), band31, band32, band33);
			thread->addRepeater(callsign3, repeaterBand3, repeaterAddress3, repeaterPort3, repeaterType3, reflector3, atStartup3, reconnect3, dratsEnabled, frequency3, offset3, range3, latitude3, longitude3, agl3, description31, description32, url3, icomRepeaterHandler, band31, band32, band33);

			if (aprs != NULL)
				aprs->setPort(callsign3, repeaterBand3, frequency3, offset3, range3, latitude3, longitude3, agl3);

			icomCount++;
		} else if (repeaterType3 == HW_HOMEBREW && hbRepeaterHandler != NULL) {
			thread->addRepeater(callsign3, repeaterBand3, repeaterAddress3, repeaterPort3, repeaterType3, reflector3, atStartup3, reconnect3, dratsEnabled, frequency3, offset3, range3, latitude3, longitude3, agl3, description31, description32, url3, hbRepeaterHandler);

			if (aprs != NULL)
				aprs->setPort(callsign3, repeaterBand3, frequency3, offset3, range3, latitude3, longitude3, agl3);
		} else if (repeaterType3 == HW_DUMMY && dummyRepeaterHandler != NULL) {
			thread->addRepeater(callsign3, repeaterBand3, repeaterAddress3, repeaterPort3, repeaterType3, reflector3, atStartup3, reconnect3, dratsEnabled, frequency3, offset3, range3, latitude3, longitude3, agl3, description31, description32, url3, dummyRepeaterHandler);
		}
	}

	wxString repeaterCall4, repeaterBand4, repeaterAddress4, reflector4, description41, description42, url4;
	double frequency4, offset4, range4, latitude4, longitude4, agl4;
	unsigned char band41, band42, band43;
	unsigned int repeaterPort4;
	HW_TYPE repeaterType4;
	bool atStartup4;
	RECONNECT reconnect4;
	m_config->getRepeater4(repeaterCall4, repeaterBand4, repeaterType4, repeaterAddress4, repeaterPort4, band41, band42, band43, reflector4, atStartup4, reconnect4, frequency4, offset4, range4, latitude4, longitude4, agl4, description41, description42, url4);

	CUtils::clean(description41, wxT("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 .,&*()-+=@/?:;"));
	CUtils::clean(description42, wxT("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 .,&*()-+=@/?:;"));
	CUtils::clean(url4, wxT("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 .,&*()-+=@/?:;"));

	wxString callsign4 = callsign;
	if (!repeaterBand4.IsSameAs(wxT(" "))) {
		if (!repeaterCall4.IsEmpty()) {
			callsign4 = repeaterCall4;
			callsign4.Append(wxT("        "));
			callsign4.Truncate(LONG_CALLSIGN_LENGTH);
		}

		wxLogInfo(wxT("Repeater 4 callsign: \"%.7s%s\", hardware type: %d, address: %s:%u"), callsign4.c_str(), repeaterBand4.c_str(), int(repeaterType4), repeaterAddress4.c_str(), repeaterPort4);
		wxLogInfo(wxT("Repeater 4 reflector: %s, at startup: %d, reconnect: %d"), reflector4.c_str(), atStartup4, reconnect4);
		wxLogInfo(wxT("Repeater 4 latitude: %lf, longitude: %lf, range: %.0lf kms, height: %.0lf m, frequency: %.4lf MHz, offset: %.4lf MHz"), latitude4, longitude4, range4, agl4, frequency4, offset4);
		wxLogInfo(wxT("Repeater 4 description: \"%s %s\", URL: \"%s\""), description41.c_str(), description42.c_str(), url4.c_str());

		if (callsign1.IsSameAs(callsign4) && repeaterBand1.IsSameAs(repeaterBand4)) {
			wxLogError(wxT("Repeater 4 has the same callsign and module as repeater 1, exiting"));
			return;
		}
		if (callsign2.IsSameAs(callsign4) && repeaterBand2.IsSameAs(repeaterBand4)) {
			wxLogError(wxT("Repeater 4 has the same callsign and module as repeater 2, exiting"));
			return;
		}
		if (callsign3.IsSameAs(callsign4) && repeaterBand3.IsSameAs(repeaterBand4)) {
			wxLogError(wxT("Repeater 4 has the same callsign and module as repeater 3, exiting"));
			return;
		}

		if (repeaterType4 == HW_ICOM && !icomAddress.IsEmpty() && icomRepeaterHandler == NULL) {
			icomRepeaterHandler = new CIcomRepeaterProtocolHandler(icomAddress, icomPort, repeaterAddress4, repeaterPort4);
			bool res = icomRepeaterHandler->open();
			if (!res) {
				wxLogError(wxT("Cannot open the Icom repeater protocol handler"));
				delete icomRepeaterHandler;
				icomRepeaterHandler = NULL;
			}
		} else if (repeaterType4 == HW_HOMEBREW && !hbAddress.IsEmpty() && hbRepeaterHandler == NULL) {
			hbRepeaterHandler = new CHBRepeaterProtocolHandler(hbAddress, hbPort);
			bool res = hbRepeaterHandler->open();
			if (!res) {
				wxLogError(wxT("Cannot open the Homebrew repeater protocol handler"));
				delete hbRepeaterHandler;
				hbRepeaterHandler = NULL;
			}
		} else if (repeaterType4 == HW_DUMMY && !hbAddress.IsEmpty() && dummyRepeaterHandler == NULL) {
			dummyRepeaterHandler = new CDummyRepeaterProtocolHandler;
			bool res = dummyRepeaterHandler->open();
			if (!res) {
				wxLogError(wxT("Cannot open the Dummy repeater protocol handler"));
				delete dummyRepeaterHandler;
				dummyRepeaterHandler = NULL;
			}
		}

		if (latitude4 == 0.0 && longitude4 == 0.0) {
			latitude4  = latitude;
			longitude4 = longitude;
		}

		if (description41.IsEmpty())
			description41 = description1;
		if (description42.IsEmpty())
			description42 = description2;

		if (url4.IsEmpty())
			url4 = url;

		if (repeaterType4 == HW_ICOM && icomRepeaterHandler != NULL) {
			wxLogInfo(wxT("Repeater 4 bands: %u %u %u"), band41, band42, band43);
			thread->addRepeater(callsign4, repeaterBand4, repeaterAddress4, repeaterPort4, repeaterType4, reflector4, atStartup4, reconnect4, dratsEnabled, frequency4, offset4, range4, latitude4, longitude4, agl4, description41, description42, url4, icomRepeaterHandler, band41, band42, band43);

			if (aprs != NULL)
				aprs->setPort(callsign4, repeaterBand4, frequency4, offset4, range4, latitude4, longitude4, agl4);

			icomCount++;
		} else if (repeaterType4 == HW_HOMEBREW && hbRepeaterHandler != NULL) {
			thread->addRepeater(callsign4, repeaterBand4, repeaterAddress4, repeaterPort4, repeaterType4, reflector4, atStartup4, reconnect4, dratsEnabled, frequency4, offset4, range4, latitude4, longitude4, agl4, description41, description42, url4, hbRepeaterHandler);

			if (aprs != NULL)
				aprs->setPort(callsign4, repeaterBand4, frequency4, offset4, range4, latitude4, longitude4, agl4);
		} else if (repeaterType4 == HW_DUMMY && dummyRepeaterHandler != NULL) {
			thread->addRepeater(callsign4, repeaterBand4, repeaterAddress4, repeaterPort4, repeaterType4, reflector4, atStartup4, reconnect4, dratsEnabled, frequency4, offset4, range4, latitude4, longitude4, agl4, description41, description42, url4, dummyRepeaterHandler);
		}
	}

	if (icomRepeaterHandler != NULL)
		icomRepeaterHandler->setCount(icomCount);

	bool ircDDBEnabled;
	wxString ircDDBHostname, ircDDBUsername, ircDDBPassword;
	m_config->getIrcDDB(ircDDBEnabled, ircDDBHostname, ircDDBUsername, ircDDBPassword);
	wxLogInfo(wxT("ircDDB enabled: %d, host: %s, username: %s"), int(ircDDBEnabled), ircDDBHostname.c_str(), ircDDBUsername.c_str());

	if (ircDDBEnabled) {
#if defined(__WINDOWS__)
		CIRCDDB* ircDDB = new CIRCDDB(ircDDBHostname, 9007U, ircDDBUsername, ircDDBPassword, wxT("win_") + LOG_BASE_NAME + wxT("-") + VERSION, gatewayAddress); 
#else
		CIRCDDB* ircDDB = new CIRCDDB(ircDDBHostname, 9007U, ircDDBUsername, ircDDBPassword, wxT("linux_") + LOG_BASE_NAME + wxT("-") + VERSION, gatewayAddress); 
#endif
		bool res = ircDDB->open();
		if (!res) {
			wxLogError(wxT("Cannot initialise the ircDDB protocol handler"));
			ircDDBEnabled = false;
		} else {
			thread->setIRC(ircDDB);
		}
	}

	if (ircDDBEnabled) {
		wxString starNetBand1, starNetCallsign1, starNetLogoff1, starNetInfo1, starNetPermanent1, link1;		// DEXTRA_LINK || DCS_LINK
		unsigned int starNetUserTimeout1, starNetGroupTimeout1;
		STARNET_CALLSIGN_SWITCH starNetCallsignSwitch1;
		bool starNetTXMsgSwitch1;
		m_config->getStarNet1(starNetBand1, starNetCallsign1, starNetLogoff1, starNetInfo1, starNetPermanent1, starNetUserTimeout1, starNetGroupTimeout1, starNetCallsignSwitch1, starNetTXMsgSwitch1
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
				,link1
#endif
			);

		if (!starNetCallsign1.IsEmpty() && !starNetCallsign1.IsSameAs(wxT("        "))) {
			wxString repeater = gatewayCallsign;
			repeater.Truncate(LONG_CALLSIGN_LENGTH - 1U);
			repeater.Append(starNetBand1);

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
			thread->addStarNet(starNetCallsign1, starNetLogoff1, repeater, starNetInfo1, starNetPermanent1, starNetUserTimeout1, starNetGroupTimeout1, starNetCallsignSwitch1, starNetTXMsgSwitch1, link1);
			wxLogInfo(wxT("STARnet group 1 set to %s/%s on repeater %s, info: \"%s\", permanent: %s, user: %u mins, group: %u mins, callsign switch: %d, tx msg switch: %d, reflector: %s"), starNetCallsign1.c_str(), starNetLogoff1.c_str(), repeater.c_str(), starNetInfo1.c_str(), starNetPermanent1.c_str(), starNetUserTimeout1, starNetGroupTimeout1, int(starNetCallsignSwitch1), int(starNetTXMsgSwitch1), link1.c_str());
#else
			thread->addStarNet(starNetCallsign1, starNetLogoff1, repeater, starNetInfo1, starNetPermanent1, starNetUserTimeout1, starNetGroupTimeout1, starNetCallsignSwitch1, starNetTXMsgSwitch1);
			wxLogInfo(wxT("STARnet group 1 set to %s/%s on repeater %s, info: \"%s\", permanent: %s, user: %u mins, group: %u mins, callsign switch: %d, tx msg switch: %d"), starNetCallsign1.c_str(), starNetLogoff1.c_str(), repeater.c_str(), starNetInfo1.c_str(), starNetPermanent1.c_str(), starNetUserTimeout1, starNetGroupTimeout1, int(starNetCallsignSwitch1), int(starNetTXMsgSwitch1));
#endif
		}

		wxString starNetBand2, starNetCallsign2, starNetLogoff2, starNetInfo2, starNetPermanent2, link2;		// DEXTRA_LINK || DCS_LINK
		unsigned int starNetUserTimeout2, starNetGroupTimeout2;
		STARNET_CALLSIGN_SWITCH starNetCallsignSwitch2;
		bool starNetTXMsgSwitch2;
		m_config->getStarNet2(starNetBand2, starNetCallsign2, starNetLogoff2, starNetInfo2, starNetPermanent2, starNetUserTimeout2, starNetGroupTimeout2, starNetCallsignSwitch2, starNetTXMsgSwitch2
#if	defined(DEXTRA_LINK) || defined(DCS_LINK)
			,link2
#endif
			);

		if (!starNetCallsign2.IsEmpty() && !starNetCallsign2.IsSameAs(wxT("        "))) {
			wxString repeater = gatewayCallsign;
			repeater.Truncate(LONG_CALLSIGN_LENGTH - 1U);
			repeater.Append(starNetBand2);

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
			thread->addStarNet(starNetCallsign2, starNetLogoff2, repeater, starNetInfo2, starNetPermanent2, starNetUserTimeout2, starNetGroupTimeout2, starNetCallsignSwitch2, starNetTXMsgSwitch2, link2);
			wxLogInfo(wxT("STARnet group 2 set to %s/%s on repeater %s, info: \"%s\", permanent: %s, user: %u mins, group: %u mins, callsign switch: %d, tx msg switch: %d, reflector: %s"), starNetCallsign2.c_str(), starNetLogoff2.c_str(), repeater.c_str(), starNetInfo2.c_str(), starNetPermanent2.c_str(), starNetUserTimeout2, starNetGroupTimeout2, int(starNetCallsignSwitch2), int(starNetTXMsgSwitch2), link2.c_str());
#else
			thread->addStarNet(starNetCallsign2, starNetLogoff2, repeater, starNetInfo2, starNetPermanent2, starNetUserTimeout2, starNetGroupTimeout2, starNetCallsignSwitch2, starNetTXMsgSwitch2);
			wxLogInfo(wxT("STARnet group 2 set to %s/%s on repeater %s, info: \"%s\", permanent: %s, user: %u mins, group: %u mins, callsign switch: %d, tx msg switch: %d"), starNetCallsign2.c_str(), starNetLogoff2.c_str(), repeater.c_str(), starNetInfo2.c_str(), starNetPermanent2.c_str(), starNetUserTimeout2, starNetGroupTimeout2, int(starNetCallsignSwitch2), int(starNetTXMsgSwitch2));
#endif
		}

		wxString starNetBand3, starNetCallsign3, starNetLogoff3, starNetInfo3, starNetPermanent3, link3;		// DEXTRA_LINK || DCS_LINK
		unsigned int starNetUserTimeout3, starNetGroupTimeout3;
		STARNET_CALLSIGN_SWITCH starNetCallsignSwitch3;
		bool starNetTXMsgSwitch3;
		m_config->getStarNet3(starNetBand3, starNetCallsign3, starNetLogoff3, starNetInfo3, starNetPermanent3, starNetUserTimeout3, starNetGroupTimeout3, starNetCallsignSwitch3, starNetTXMsgSwitch3
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
				,link3
#endif
			);

		if (!starNetCallsign3.IsEmpty() && !starNetCallsign3.IsSameAs(wxT("        "))) {
			wxString repeater = gatewayCallsign;
			repeater.Truncate(LONG_CALLSIGN_LENGTH - 1U);
			repeater.Append(starNetBand3);

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
			thread->addStarNet(starNetCallsign3, starNetLogoff3, repeater, starNetInfo3, starNetPermanent3, starNetUserTimeout3, starNetGroupTimeout3, starNetCallsignSwitch3, starNetTXMsgSwitch3, link3);
			wxLogInfo(wxT("STARnet group 3 set to %s/%s on repeater %s, info: \"%s\", permanent: %s, user: %u mins, group: %u mins, callsign switch: %d, tx msg switch: %d, reflector: %s"), starNetCallsign3.c_str(), starNetLogoff3.c_str(), repeater.c_str(), starNetInfo3.c_str(), starNetPermanent3.c_str(), starNetUserTimeout3, starNetGroupTimeout3, int(starNetCallsignSwitch3), int(starNetTXMsgSwitch3), link3.c_str());
#else
			thread->addStarNet(starNetCallsign3, starNetLogoff3, repeater, starNetInfo3, starNetPermanent3, starNetUserTimeout3, starNetGroupTimeout3, starNetCallsignSwitch3, starNetTXMsgSwitch3);
			wxLogInfo(wxT("STARnet group 3 set to %s/%s on repeater %s, info: \"%s\", permanent: %s, user: %u mins, group: %u mins, callsign switch: %d, tx msg switch: %d"), starNetCallsign3.c_str(), starNetLogoff3.c_str(), repeater.c_str(), starNetInfo3.c_str(), starNetPermanent3.c_str(), starNetUserTimeout3, starNetGroupTimeout3, int(starNetCallsignSwitch3), int(starNetTXMsgSwitch3));
#endif
		}

		wxString starNetBand4, starNetCallsign4, starNetLogoff4, starNetInfo4, starNetPermanent4, link4;		// DEXTRA_LINK || DCS_LINK
		unsigned int starNetUserTimeout4, starNetGroupTimeout4;
		STARNET_CALLSIGN_SWITCH starNetCallsignSwitch4;
		bool starNetTXMsgSwitch4;
		m_config->getStarNet4(starNetBand4, starNetCallsign4, starNetLogoff4, starNetInfo4, starNetPermanent4, starNetUserTimeout4, starNetGroupTimeout4, starNetCallsignSwitch4, starNetTXMsgSwitch4
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
				,link4
#endif
			);

		if (!starNetCallsign4.IsEmpty() && !starNetCallsign4.IsSameAs(wxT("        "))) {
			wxString repeater = gatewayCallsign;
			repeater.Truncate(LONG_CALLSIGN_LENGTH - 1U);
			repeater.Append(starNetBand4);

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
			thread->addStarNet(starNetCallsign4, starNetLogoff4, repeater, starNetInfo4, starNetPermanent4, starNetUserTimeout4, starNetGroupTimeout4, starNetCallsignSwitch4, starNetTXMsgSwitch4, link4);
			wxLogInfo(wxT("STARnet group 4 set to %s/%s on repeater %s, info: \"%s\", permanent: %s, user: %u mins, group: %u mins, callsign switch: %d, tx msg switch: %d, reflector: %s"), starNetCallsign4.c_str(), starNetLogoff4.c_str(), repeater.c_str(), starNetInfo4.c_str(), starNetPermanent4.c_str(), starNetUserTimeout4, starNetGroupTimeout4, int(starNetCallsignSwitch4), int(starNetTXMsgSwitch4), link4.c_str());
#else
			thread->addStarNet(starNetCallsign4, starNetLogoff4, repeater, starNetInfo4, starNetPermanent4, starNetUserTimeout4, starNetGroupTimeout4, starNetCallsignSwitch4, starNetTXMsgSwitch4);
			wxLogInfo(wxT("STARnet group 4 set to %s/%s on repeater %s, info: \"%s\", permanent: %s, user: %u mins, group: %u mins, callsign switch: %d, tx msg switch: %d"), starNetCallsign4.c_str(), starNetLogoff4.c_str(), repeater.c_str(), starNetInfo4.c_str(), starNetPermanent4.c_str(), starNetUserTimeout4, starNetGroupTimeout4, int(starNetCallsignSwitch4), int(starNetTXMsgSwitch4));
#endif
		}

		wxString starNetBand5, starNetCallsign5, starNetLogoff5, starNetInfo5, starNetPermanent5, link5;		// DEXTRA_LINK || DCS_LINK
		unsigned int starNetUserTimeout5, starNetGroupTimeout5;
		STARNET_CALLSIGN_SWITCH starNetCallsignSwitch5;
		bool starNetTXMsgSwitch5;
		m_config->getStarNet5(starNetBand5, starNetCallsign5, starNetLogoff5, starNetInfo5, starNetPermanent5, starNetUserTimeout5, starNetGroupTimeout5, starNetCallsignSwitch5, starNetTXMsgSwitch5
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
				,link5
#endif
			);

		if (!starNetCallsign5.IsEmpty() && !starNetCallsign5.IsSameAs(wxT("        "))) {
			wxString repeater = gatewayCallsign;
			repeater.Truncate(LONG_CALLSIGN_LENGTH - 1U);
			repeater.Append(starNetBand5);

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
			thread->addStarNet(starNetCallsign5, starNetLogoff5, repeater, starNetInfo5, starNetPermanent5, starNetUserTimeout5, starNetGroupTimeout5, starNetCallsignSwitch5, starNetTXMsgSwitch5, link5);
			wxLogInfo(wxT("STARnet group 5 set to %s/%s on repeater %s, info: \"%s\", permanent: %s, user: %u mins, group: %u mins, callsign switch: %d, tx msg switch: %d, reflector: %s"), starNetCallsign5.c_str(), starNetLogoff5.c_str(), repeater.c_str(), starNetInfo5.c_str(), starNetPermanent5.c_str(), starNetUserTimeout5, starNetGroupTimeout5, int(starNetCallsignSwitch5), int(starNetTXMsgSwitch5), link5.c_str());
#else
			thread->addStarNet(starNetCallsign5, starNetLogoff5, repeater, starNetInfo5, starNetPermanent5, starNetUserTimeout5, starNetGroupTimeout5, starNetCallsignSwitch5, starNetTXMsgSwitch5);
			wxLogInfo(wxT("STARnet group 5 set to %s/%s on repeater %s, info: \"%s\", permanent: %s, user: %u mins, group: %u mins, callsign switch: %d, tx msg switch: %d"), starNetCallsign5.c_str(), starNetLogoff5.c_str(), repeater.c_str(), starNetInfo5.c_str(), starNetPermanent5.c_str(), starNetUserTimeout5, starNetGroupTimeout5, int(starNetCallsignSwitch5), int(starNetTXMsgSwitch5));
#endif
		}
	}

	bool dextraEnabled;
	unsigned int dextraMaxDongles;
	m_config->getDExtra(dextraEnabled, dextraMaxDongles);
	wxLogInfo(wxT("DExtra enabled: %d, max. dongles: %u"), int(dextraEnabled), dextraMaxDongles);

	bool remoteEnabled;
	wxString remotePassword;
	unsigned int remotePort;
	m_config->getRemote(remoteEnabled, remotePassword, remotePort);
	wxLogInfo(wxT("Remote enabled: %d, port: %u"), int(remoteEnabled), remotePort);
	thread->setRemote(remoteEnabled, remotePassword, remotePort);

	wxString dplusLogin;
	unsigned int dplusMaxDongles;
	bool dplusEnabled;
	m_config->getDPlus(dplusEnabled, dplusMaxDongles, dplusLogin);
	wxLogInfo(wxT("D-Plus enabled: %d, max. dongles; %u, login: %s"), int(dplusEnabled), dplusMaxDongles, dplusLogin.c_str());

	bool dcsEnabled, ccsEnabled;
	wxString ccsHost;
	m_config->getDCS(dcsEnabled, ccsEnabled, ccsHost);
	wxLogInfo(wxT("DCS enabled: %d, CCS enabled: %d, server: %s"), int(dcsEnabled), int(ccsEnabled), ccsHost.c_str());

	if (repeaterBand1.Len() > 1U || repeaterBand2.Len() > 1U ||
		repeaterBand3.Len() > 1U || repeaterBand4.Len() > 1U) {
		wxLogInfo(wxT("DD mode enabled"));
		thread->setDDModeEnabled(true);
	}

	wxFileName filename(wxFileName::GetHomeDir(), RESTRICT_FILE_NAME);
	bool exists = filename.FileExists();
	if (exists) {
		CCallsignList* list = new CCallsignList(filename.GetFullPath());
		bool res = list->load();
		if (!res) {
			wxLogError(wxT("Unable to open the restrict list file - %s"), filename.GetFullPath().c_str());
			delete list;
		} else {
			wxLogInfo(wxT("%u callsigns loaded into the restrict list"), list->getCount());
			thread->setRestrictList(list);
		}
	}

	thread->setIcomRepeaterHandler(icomRepeaterHandler);
	thread->setHBRepeaterHandler(hbRepeaterHandler);
	thread->setDummyRepeaterHandler(dummyRepeaterHandler);
	thread->setLanguage(language);
	thread->setDPlus(dplusEnabled, dplusMaxDongles, dplusLogin);
	thread->setDExtra(dextraEnabled, dextraMaxDongles);
	thread->setDCS(dcsEnabled);
	thread->setCCS(ccsEnabled, ccsHost);
	thread->setInfoEnabled(infoEnabled);
	thread->setEchoEnabled(echoEnabled);
	thread->setDTMFEnabled(dtmfEnabled);
	thread->setLog(logEnabled);
	thread->setLocation(latitude, longitude);

	// Convert the worker class into a thread
	m_thread = new CIRCDDBGatewayThreadHelper(thread);
	m_thread->start();
}
