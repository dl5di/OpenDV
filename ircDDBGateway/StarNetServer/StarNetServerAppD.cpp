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

#include "IcomRepeaterProtocolHandler.h"
#include "HBRepeaterProtocolHandler.h"
#include "StarNetServerConfig.h"
#include "StarNetServerAppD.h"
#include "StarNetServerDefs.h"
#include "APRSWriter.h"
#include "Version.h"
#include "Logger.h"
#include "IRCDDB.h"

#include <wx/cmdline.h>
#include <wx/wfstream.h>
#include <wx/fileconf.h>
#include <wx/filename.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

const wxChar* NOLOGGING_SWITCH = wxT("nolog");
const wxChar*    LOGDIR_OPTION = wxT("logdir");
const wxChar*   CONFDIR_OPTION = wxT("confdir");
const wxChar*    DAEMON_SWITCH = wxT("daemon");


int main(int argc, char** argv)
{
	bool res = ::wxInitialize();
	if (!res) {
		::fprintf(stderr, "starnetserverd: failed to initialise the wxWidgets library, exiting\n");
		return -1;
	}

	wxCmdLineParser parser(argc, argv);
	parser.AddSwitch(NOLOGGING_SWITCH, wxEmptyString, wxEmptyString, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddSwitch(DAEMON_SWITCH,    wxEmptyString, wxEmptyString, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddOption(LOGDIR_OPTION,    wxEmptyString, wxEmptyString, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddOption(CONFDIR_OPTION,   wxEmptyString, wxEmptyString, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);

	int cmd = parser.Parse();
	if (cmd != 0) {
		::wxUninitialize();
		return 0;
	}

	bool  nolog = parser.Found(NOLOGGING_SWITCH);
	bool daemon = parser.Found(DAEMON_SWITCH);

	wxString logDir;
	bool found = parser.Found(LOGDIR_OPTION, &logDir);
	if (!found)
		logDir.Clear();

	wxString confDir;
	found = parser.Found(CONFDIR_OPTION, &confDir);
	if (!found)
		confDir = wxT(CONF_DIR);

	if (daemon) {
		pid_t pid = ::fork();

		if (pid < 0) {
			::fprintf(stderr, "starnetserverd: error in fork(), exiting\n");
			::wxUninitialize();
			return 1;
		}

		// If this is the parent, exit
		if (pid > 0)
			return 0;

		// We are the child from here onwards
		::setsid();

		::chdir("/");

		::umask(0);
	}

	CStarNetServerAppD gateway(nolog, logDir, confDir);

	if (!gateway.init()) {
		::wxUninitialize();
		return 1;
	}

	gateway.run();

	::wxUninitialize();
	return 0;
}

CStarNetServerAppD::CStarNetServerAppD(bool nolog, const wxString& logDir, const wxString& confDir) :
m_nolog(nolog),
m_logDir(logDir),
m_confDir(confDir),
m_thread(NULL)
{
}

CStarNetServerAppD::~CStarNetServerAppD()
{
}

bool CStarNetServerAppD::init()
{
	if (!m_nolog) {
		if (m_logDir.IsEmpty())
			m_logDir = wxT(LOG_DIR);

		wxLog* log = new CLogger(m_logDir, LOG_BASE_NAME);
		wxLog::SetActiveTarget(log);
	} else {
		new wxLogNull;
	}

	wxLogInfo(wxT("Starting ") + APPLICATION_NAME + wxT(" daemon - ") + VERSION);

	// Log the SVN revsion and the version of wxWidgets and the Operating System
	wxLogInfo(SVNREV);
	wxLogInfo(wxT("Using wxWidgets %d.%d.%d on %s"), wxMAJOR_VERSION, wxMINOR_VERSION, wxRELEASE_NUMBER, ::wxGetOsDescription().c_str());

	return createThread();
}

void CStarNetServerAppD::run()
{
	m_thread->run();

	wxLogInfo(APPLICATION_NAME + wxT(" is exiting"));
}

bool CStarNetServerAppD::createThread()
{
	CStarNetServerConfig config(m_confDir);

	m_thread = new CStarNetServerThread(m_nolog, m_logDir);

	wxString callsign, address;
	config.getGateway(callsign, address);

	callsign.Append(wxT("        "));
	callsign.Truncate(LONG_CALLSIGN_LENGTH - 1U);
	callsign.Append(wxT("G"));

	wxLogInfo(wxT("Gateway callsign set to %s, local address set to %s"), callsign.c_str(), address.c_str());

	bool logEnabled;
	config.getMiscellaneous(logEnabled);
	wxLogInfo(wxT("Log enabled set to %d"), int(logEnabled));

	wxString hostname, username, password;
	config.getIrcDDB(hostname, username, password);
	wxLogInfo(wxT("ircDDB host set to %s, username set to %s"), hostname.c_str(), username.c_str());

	if (!hostname.IsEmpty() && !username.IsEmpty() && !password.IsEmpty()) {
#if defined(__WINDOWS__)
		CIRCDDB* ircDDB = new CIRCDDB(hostname, 9007U, username, password, wxT("win_") + LOG_BASE_NAME + wxT("-") + VERSION, address); 
#else
		CIRCDDB* ircDDB = new CIRCDDB(hostname, 9007U, username, password, wxT("linux_") + LOG_BASE_NAME + wxT("-") + VERSION, address); 
#endif
		bool res = ircDDB->open();
		if (!res) {
			wxLogError(wxT("Cannot initialise the ircDDB protocol handler"));
			return false;
		}

		m_thread->setIRC(ircDDB);
	}

	wxString starNetBand1, starNetCallsign1, starNetLogoff1, starNetInfo1, starNetPermanent1, starNetLink1;		// DEXTRA_LINK || DCS_LINK
	unsigned int starNetUserTimeout1, starNetGroupTimeout1;
	STARNET_CALLSIGN_SWITCH starNetCallsignSwitch1;
	bool starNetTXMsgSwitch1;
	config.getStarNet1(starNetBand1, starNetCallsign1, starNetLogoff1, starNetInfo1, starNetPermanent1, starNetUserTimeout1, starNetGroupTimeout1, starNetCallsignSwitch1, starNetTXMsgSwitch1
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
		,starNetLink1
#endif
);

	if (!starNetCallsign1.IsEmpty() && !starNetCallsign1.IsSameAs(wxT("        "))) {
		wxString repeater = callsign;
		repeater.Truncate(LONG_CALLSIGN_LENGTH - 1U);
		repeater.Append(starNetBand1);

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
		m_thread->addStarNet(starNetCallsign1, starNetLogoff1, repeater, starNetInfo1, starNetPermanent1, starNetUserTimeout1, starNetGroupTimeout1, starNetCallsignSwitch1, starNetTXMsgSwitch1, starNetLink1);
		wxLogInfo(wxT("StarNet 1 set to %s/%s on repeater %s, info: \"%s\", permanent: %s, user: %u mins, group: %u mins, callsign switch: %d, tx msg switch: %d, reflector: %s"), starNetCallsign1.c_str(), starNetLogoff1.c_str(), repeater.c_str(), starNetInfo1.c_str(), starNetPermanent1.c_str(), starNetUserTimeout1, starNetGroupTimeout1, int(starNetCallsignSwitch1), int(starNetTXMsgSwitch1), starNetLink1.c_str());
#else
		m_thread->addStarNet(starNetCallsign1, starNetLogoff1, repeater, starNetInfo1, starNetPermanent1, starNetUserTimeout1, starNetGroupTimeout1, starNetCallsignSwitch1, starNetTXMsgSwitch1);
		wxLogInfo(wxT("StarNet 1 set to %s/%s on repeater %s, info: \"%s\", permanent: %s, user: %u mins, group: %u mins, callsign switch: %d, tx msg switch: %d"), starNetCallsign1.c_str(), starNetLogoff1.c_str(), repeater.c_str(), starNetInfo1.c_str(), starNetPermanent1.c_str(), starNetUserTimeout1, starNetGroupTimeout1, int(starNetCallsignSwitch1), int(starNetTXMsgSwitch1));
#endif
	}

	wxString starNetBand2, starNetCallsign2, starNetLogoff2, starNetInfo2, starNetPermanent2, starNetLink2;		// DEXTRA_LINK || DCS_LINK
	unsigned int starNetUserTimeout2, starNetGroupTimeout2;
	STARNET_CALLSIGN_SWITCH starNetCallsignSwitch2;
	bool starNetTXMsgSwitch2;
	config.getStarNet2(starNetBand2, starNetCallsign2, starNetLogoff2, starNetInfo2, starNetPermanent2, starNetUserTimeout2, starNetGroupTimeout2, starNetCallsignSwitch2, starNetTXMsgSwitch2
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
		,starNetLink2
#endif
);

	if (!starNetCallsign2.IsEmpty() && !starNetCallsign2.IsSameAs(wxT("        "))) {
		wxString repeater = callsign;
		repeater.Truncate(LONG_CALLSIGN_LENGTH - 1U);
		repeater.Append(starNetBand2);

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
		m_thread->addStarNet(starNetCallsign2, starNetLogoff2, repeater, starNetInfo2, starNetPermanent2, starNetUserTimeout2, starNetGroupTimeout2, starNetCallsignSwitch2, starNetTXMsgSwitch2, starNetLink2);
		wxLogInfo(wxT("StarNet 2 set to %s/%s on repeater %s, info: \"%s\", permanent: %s, user: %u mins, group: %u mins, callsign switch: %d, tx msg switch: %d, reflector: %s"), starNetCallsign2.c_str(), starNetLogoff2.c_str(), repeater.c_str(), starNetInfo2.c_str(), starNetPermanent2.c_str(), starNetUserTimeout2, starNetGroupTimeout2, int(starNetCallsignSwitch2), int(starNetTXMsgSwitch2), starNetLink2.c_str());
#else
		m_thread->addStarNet(starNetCallsign2, starNetLogoff2, repeater, starNetInfo2, starNetPermanent2, starNetUserTimeout2, starNetGroupTimeout2, starNetCallsignSwitch2, starNetTXMsgSwitch2);
		wxLogInfo(wxT("StarNet 2 set to %s/%s on repeater %s, info: \"%s\", permanent: %s, user: %u mins, group: %u mins, callsign switch: %d, tx msg switch: %d"), starNetCallsign2.c_str(), starNetLogoff2.c_str(), repeater.c_str(), starNetInfo2.c_str(), starNetPermanent2.c_str(), starNetUserTimeout2, starNetGroupTimeout2, int(starNetCallsignSwitch2), int(starNetTXMsgSwitch2));
#endif
	}

	wxString starNetBand3, starNetCallsign3, starNetLogoff3, starNetInfo3, starNetPermanent3, starNetLink3;		// DEXTRA_LINK || DCS_LINK
	unsigned int starNetUserTimeout3, starNetGroupTimeout3;
	STARNET_CALLSIGN_SWITCH starNetCallsignSwitch3;
	bool starNetTXMsgSwitch3;
	config.getStarNet3(starNetBand3, starNetCallsign3, starNetLogoff3, starNetInfo3, starNetPermanent3, starNetUserTimeout3, starNetGroupTimeout3, starNetCallsignSwitch3, starNetTXMsgSwitch3
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
		,starNetLink3
#endif
);

	if (!starNetCallsign3.IsEmpty() && !starNetCallsign3.IsSameAs(wxT("        "))) {
		wxString repeater = callsign;
		repeater.Truncate(LONG_CALLSIGN_LENGTH - 1U);
		repeater.Append(starNetBand3);

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
		m_thread->addStarNet(starNetCallsign3, starNetLogoff3, repeater, starNetInfo3, starNetPermanent3, starNetUserTimeout3, starNetGroupTimeout3, starNetCallsignSwitch3, starNetTXMsgSwitch3, starNetLink3);
		wxLogInfo(wxT("StarNet 3 set to %s/%s on repeater %s, info: \"%s\", permanent: %s, user: %u mins, group: %u mins, callsign switch: %d, tx msg switch: %d, reflector: %s"), starNetCallsign3.c_str(), starNetLogoff3.c_str(), repeater.c_str(), starNetInfo3.c_str(), starNetPermanent3.c_str(), starNetUserTimeout3, starNetGroupTimeout3, int(starNetCallsignSwitch3), int(starNetTXMsgSwitch3), starNetLink3.c_str());
#else
		m_thread->addStarNet(starNetCallsign3, starNetLogoff3, repeater, starNetInfo3, starNetPermanent3, starNetUserTimeout3, starNetGroupTimeout3, starNetCallsignSwitch3, starNetTXMsgSwitch3);
		wxLogInfo(wxT("StarNet 3 set to %s/%s on repeater %s, info: \"%s\", permanent: %s, user: %u mins, group: %u mins, callsign switch: %d, tx msg switch: %d"), starNetCallsign3.c_str(), starNetLogoff3.c_str(), repeater.c_str(), starNetInfo3.c_str(), starNetPermanent3.c_str(), starNetUserTimeout3, starNetGroupTimeout3, int(starNetCallsignSwitch3), int(starNetTXMsgSwitch3));
#endif
	}

	wxString starNetBand4, starNetCallsign4, starNetLogoff4, starNetInfo4, starNetPermanent4, starNetLink4;		// DEXTRA_LINK || DCS_LINK
	unsigned int starNetUserTimeout4, starNetGroupTimeout4;
	STARNET_CALLSIGN_SWITCH starNetCallsignSwitch4;
	bool starNetTXMsgSwitch4;
	config.getStarNet4(starNetBand4, starNetCallsign4, starNetLogoff4, starNetInfo4, starNetPermanent4, starNetUserTimeout4, starNetGroupTimeout4, starNetCallsignSwitch4, starNetTXMsgSwitch4
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
		,starNetLink4
#endif
);

	if (!starNetCallsign4.IsEmpty() && !starNetCallsign4.IsSameAs(wxT("        "))) {
		wxString repeater = callsign;
		repeater.Truncate(LONG_CALLSIGN_LENGTH - 1U);
		repeater.Append(starNetBand4);

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
		m_thread->addStarNet(starNetCallsign4, starNetLogoff4, repeater, starNetInfo4, starNetPermanent4, starNetUserTimeout4, starNetGroupTimeout4, starNetCallsignSwitch4, starNetTXMsgSwitch4, starNetLink4);
		wxLogInfo(wxT("StarNet 4 set to %s/%s on repeater %s, info: \"%s\", permanent: %s, user: %u mins, group: %u mins, callsign switch: %d, tx msg switch: %d, reflector: %s"), starNetCallsign4.c_str(), starNetLogoff4.c_str(), repeater.c_str(), starNetInfo4.c_str(), starNetPermanent4.c_str(), starNetUserTimeout4, starNetGroupTimeout4, int(starNetCallsignSwitch4), int(starNetTXMsgSwitch4), starNetLink4.c_str());
#else
		m_thread->addStarNet(starNetCallsign4, starNetLogoff4, repeater, starNetInfo4, starNetPermanent4, starNetUserTimeout4, starNetGroupTimeout4, starNetCallsignSwitch4, starNetTXMsgSwitch4);
		wxLogInfo(wxT("StarNet 4 set to %s/%s on repeater %s, info: \"%s\", permanent: %s, user: %u mins, group: %u mins, callsign switch: %d, tx msg switch: %d"), starNetCallsign4.c_str(), starNetLogoff4.c_str(), repeater.c_str(), starNetInfo4.c_str(), starNetPermanent4.c_str(), starNetUserTimeout4, starNetGroupTimeout4, int(starNetCallsignSwitch4), int(starNetTXMsgSwitch4));
#endif
	}

	wxString starNetBand5, starNetCallsign5, starNetLogoff5, starNetInfo5, starNetPermanent5, starNetLink5;		// DEXTRA_LINK || DCS_LINK
	unsigned int starNetUserTimeout5, starNetGroupTimeout5;
	STARNET_CALLSIGN_SWITCH starNetCallsignSwitch5;
	bool starNetTXMsgSwitch5;
	config.getStarNet5(starNetBand5, starNetCallsign5, starNetLogoff5, starNetInfo5, starNetPermanent5, starNetUserTimeout5, starNetGroupTimeout5, starNetCallsignSwitch5, starNetTXMsgSwitch5
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
		,starNetLink5
#endif
);

	if (!starNetCallsign5.IsEmpty() && !starNetCallsign5.IsSameAs(wxT("        "))) {
		wxString repeater = callsign;
		repeater.Truncate(LONG_CALLSIGN_LENGTH - 1U);
		repeater.Append(starNetBand5);

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
		m_thread->addStarNet(starNetCallsign5, starNetLogoff5, repeater, starNetInfo5, starNetPermanent5, starNetUserTimeout5, starNetGroupTimeout5, starNetCallsignSwitch5, starNetTXMsgSwitch5, starNetLink5);
		wxLogInfo(wxT("StarNet 5 set to %s/%s on repeater %s, info: \"%s\", permanent: %s, user: %u mins, group: %u mins, callsign switch: %d, tx msg switch: %d, reflector: %s"), starNetCallsign5.c_str(), starNetLogoff5.c_str(), repeater.c_str(), starNetInfo5.c_str(), starNetPermanent5.c_str(), starNetUserTimeout5, starNetGroupTimeout5, int(starNetCallsignSwitch5), int(starNetTXMsgSwitch5), starNetLink5.c_str());
#else
		m_thread->addStarNet(starNetCallsign5, starNetLogoff5, repeater, starNetInfo5, starNetPermanent5, starNetUserTimeout5, starNetGroupTimeout5, starNetCallsignSwitch5, starNetTXMsgSwitch5);
		wxLogInfo(wxT("StarNet 5 set to %s/%s on repeater %s, info: \"%s\", permanent: %s, user: %u mins, group: %u mins, callsign switch: %d, tx msg switch: %d"), starNetCallsign5.c_str(), starNetLogoff5.c_str(), repeater.c_str(), starNetInfo5.c_str(), starNetPermanent5.c_str(), starNetUserTimeout5, starNetGroupTimeout5, int(starNetCallsignSwitch5), int(starNetTXMsgSwitch5));
#endif
	}

	wxString starNetBand6, starNetCallsign6, starNetLogoff6, starNetInfo6, starNetPermanent6, starNetLink6;		// DEXTRA_LINK || DCS_LINK
	unsigned int starNetUserTimeout6, starNetGroupTimeout6;
	STARNET_CALLSIGN_SWITCH starNetCallsignSwitch6;
	bool starNetTXMsgSwitch6;
	config.getStarNet6(starNetBand6, starNetCallsign6, starNetLogoff6, starNetInfo6, starNetPermanent6, starNetUserTimeout6, starNetGroupTimeout6, starNetCallsignSwitch6, starNetTXMsgSwitch6
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
		,starNetLink6
#endif
);

	if (!starNetCallsign6.IsEmpty() && !starNetCallsign6.IsSameAs(wxT("        "))) {
		wxString repeater = callsign;
		repeater.Truncate(LONG_CALLSIGN_LENGTH - 1U);
		repeater.Append(starNetBand6);

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
		m_thread->addStarNet(starNetCallsign6, starNetLogoff6, repeater, starNetInfo6, starNetPermanent6, starNetUserTimeout6, starNetGroupTimeout6, starNetCallsignSwitch6, starNetTXMsgSwitch6, starNetLink6);
		wxLogInfo(wxT("StarNet 6 set to %s/%s on repeater %s, info: \"%s\", permanent: %s, user: %u mins, group: %u mins, callsign switch: %d, tx msg switch: %d, reflector: %s"), starNetCallsign6.c_str(), starNetLogoff6.c_str(), repeater.c_str(), starNetInfo6.c_str(), starNetPermanent6.c_str(), starNetUserTimeout6, starNetGroupTimeout6, int(starNetCallsignSwitch6), int(starNetTXMsgSwitch6), starNetLink6.c_str());
#else
		m_thread->addStarNet(starNetCallsign6, starNetLogoff6, repeater, starNetInfo6, starNetPermanent6, starNetUserTimeout6, starNetGroupTimeout6, starNetCallsignSwitch6, starNetTXMsgSwitch6);
		wxLogInfo(wxT("StarNet 6 set to %s/%s on repeater %s, info: \"%s\", permanent: %s, user: %u mins, group: %u mins, callsign switch: %d, tx msg switch: %d"), starNetCallsign6.c_str(), starNetLogoff6.c_str(), repeater.c_str(), starNetInfo6.c_str(), starNetPermanent6.c_str(), starNetUserTimeout6, starNetGroupTimeout6, int(starNetCallsignSwitch6), int(starNetTXMsgSwitch6));
#endif
	}

	wxString starNetBand7, starNetCallsign7, starNetLogoff7, starNetInfo7, starNetPermanent7, starNetLink7;		// DEXTRA_LINK || DCS_LINK
	unsigned int starNetUserTimeout7, starNetGroupTimeout7;
	STARNET_CALLSIGN_SWITCH starNetCallsignSwitch7;
	bool starNetTXMsgSwitch7;
	config.getStarNet7(starNetBand7, starNetCallsign7, starNetLogoff7, starNetInfo7, starNetPermanent7, starNetUserTimeout7, starNetGroupTimeout7, starNetCallsignSwitch7, starNetTXMsgSwitch7
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
		,starNetLink7
#endif
);

	if (!starNetCallsign7.IsEmpty() && !starNetCallsign7.IsSameAs(wxT("        "))) {
		wxString repeater = callsign;
		repeater.Truncate(LONG_CALLSIGN_LENGTH - 1U);
		repeater.Append(starNetBand7);

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
		m_thread->addStarNet(starNetCallsign7, starNetLogoff7, repeater, starNetInfo7, starNetPermanent7, starNetUserTimeout7, starNetGroupTimeout7, starNetCallsignSwitch7, starNetTXMsgSwitch7, starNetLink7);
		wxLogInfo(wxT("StarNet 7 set to %s/%s on repeater %s, info: \"%s\", permanent: %s, user: %u mins, group: %u mins, callsign switch: %d, tx msg switch: %d, reflector: %s"), starNetCallsign7.c_str(), starNetLogoff7.c_str(), repeater.c_str(), starNetInfo7.c_str(), starNetPermanent7.c_str(), starNetUserTimeout7, starNetGroupTimeout7, int(starNetCallsignSwitch7), int(starNetTXMsgSwitch7), starNetLink7.c_str());
#else
		m_thread->addStarNet(starNetCallsign7, starNetLogoff7, repeater, starNetInfo7, starNetPermanent7, starNetUserTimeout7, starNetGroupTimeout7, starNetCallsignSwitch7, starNetTXMsgSwitch7);
		wxLogInfo(wxT("StarNet 7 set to %s/%s on repeater %s, info: \"%s\", permanent: %s, user: %u mins, group: %u mins, callsign switch: %d, tx msg switch: %d"), starNetCallsign7.c_str(), starNetLogoff7.c_str(), repeater.c_str(), starNetInfo7.c_str(), starNetPermanent7.c_str(), starNetUserTimeout7, starNetGroupTimeout7, int(starNetCallsignSwitch7), int(starNetTXMsgSwitch7));
#endif
	}

	wxString starNetBand8, starNetCallsign8, starNetLogoff8, starNetInfo8, starNetPermanent8, starNetLink8;		// DEXTRA_LINK || DCS_LINK
	unsigned int starNetUserTimeout8, starNetGroupTimeout8;
	STARNET_CALLSIGN_SWITCH starNetCallsignSwitch8;
	bool starNetTXMsgSwitch8;
	config.getStarNet8(starNetBand8, starNetCallsign8, starNetLogoff8, starNetInfo8, starNetPermanent8, starNetUserTimeout8, starNetGroupTimeout8, starNetCallsignSwitch8, starNetTXMsgSwitch8
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
		,starNetLink8
#endif
);

	if (!starNetCallsign8.IsEmpty() && !starNetCallsign8.IsSameAs(wxT("        "))) {
		wxString repeater = callsign;
		repeater.Truncate(LONG_CALLSIGN_LENGTH - 1U);
		repeater.Append(starNetBand8);

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
		m_thread->addStarNet(starNetCallsign8, starNetLogoff8, repeater, starNetInfo8, starNetPermanent8, starNetUserTimeout8, starNetGroupTimeout8, starNetCallsignSwitch8, starNetTXMsgSwitch8, starNetLink8);
		wxLogInfo(wxT("StarNet 8 set to %s/%s on repeater %s, info: \"%s\", permanent: %s, user: %u mins, group: %u mins, callsign switch: %d, tx msg switch: %d, reflector: %s"), starNetCallsign8.c_str(), starNetLogoff8.c_str(), repeater.c_str(), starNetInfo8.c_str(), starNetPermanent8.c_str(), starNetUserTimeout8, starNetGroupTimeout8, int(starNetCallsignSwitch8), int(starNetTXMsgSwitch8), starNetLink8.c_str());
#else
		m_thread->addStarNet(starNetCallsign8, starNetLogoff8, repeater, starNetInfo8, starNetPermanent8, starNetUserTimeout8, starNetGroupTimeout8, starNetCallsignSwitch8, starNetTXMsgSwitch8);
		wxLogInfo(wxT("StarNet 8 set to %s/%s on repeater %s, info: \"%s\", permanent: %s, user: %u mins, group: %u mins, callsign switch: %d, tx msg switch: %d"), starNetCallsign8.c_str(), starNetLogoff8.c_str(), repeater.c_str(), starNetInfo8.c_str(), starNetPermanent8.c_str(), starNetUserTimeout8, starNetGroupTimeout8, int(starNetCallsignSwitch8), int(starNetTXMsgSwitch8));
#endif
	}

	wxString starNetBand9, starNetCallsign9, starNetLogoff9, starNetInfo9, starNetPermanent9, starNetLink9;		// DEXTRA_LINK || DCS_LINK
	unsigned int starNetUserTimeout9, starNetGroupTimeout9;
	STARNET_CALLSIGN_SWITCH starNetCallsignSwitch9;
	bool starNetTXMsgSwitch9;
	config.getStarNet9(starNetBand9, starNetCallsign9, starNetLogoff9, starNetInfo9, starNetPermanent9, starNetUserTimeout9, starNetGroupTimeout9, starNetCallsignSwitch9, starNetTXMsgSwitch9
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
		,starNetLink9
#endif
);

	if (!starNetCallsign9.IsEmpty() && !starNetCallsign9.IsSameAs(wxT("        "))) {
		wxString repeater = callsign;
		repeater.Truncate(LONG_CALLSIGN_LENGTH - 1U);
		repeater.Append(starNetBand9);

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
		m_thread->addStarNet(starNetCallsign9, starNetLogoff9, repeater, starNetInfo9, starNetPermanent9, starNetUserTimeout9, starNetGroupTimeout9, starNetCallsignSwitch9, starNetTXMsgSwitch9, starNetLink9);
		wxLogInfo(wxT("StarNet 9 set to %s/%s on repeater %s, info: \"%s\", permanent: %s, user: %u mins, group: %u mins, callsign switch: %d, tx msg switch: %d, reflector: %s"), starNetCallsign9.c_str(), starNetLogoff9.c_str(), repeater.c_str(), starNetInfo9.c_str(), starNetPermanent9.c_str(), starNetUserTimeout9, starNetGroupTimeout9, int(starNetCallsignSwitch9), int(starNetTXMsgSwitch9), starNetLink9.c_str());
#else
		m_thread->addStarNet(starNetCallsign9, starNetLogoff9, repeater, starNetInfo9, starNetPermanent9, starNetUserTimeout9, starNetGroupTimeout9, starNetCallsignSwitch9, starNetTXMsgSwitch9);
		wxLogInfo(wxT("StarNet 9 set to %s/%s on repeater %s, info: \"%s\", permanent: %s, user: %u mins, group: %u mins, callsign switch: %d, tx msg switch: %d"), starNetCallsign9.c_str(), starNetLogoff9.c_str(), repeater.c_str(), starNetInfo9.c_str(), starNetPermanent9.c_str(), starNetUserTimeout9, starNetGroupTimeout9, int(starNetCallsignSwitch9), int(starNetTXMsgSwitch9));
#endif
	}

	wxString starNetBand10, starNetCallsign10, starNetLogoff10, starNetInfo10, starNetPermanent10, starNetLink10;		// DEXTRA_LINK || DCS_LINK
	unsigned int starNetUserTimeout10, starNetGroupTimeout10;
	STARNET_CALLSIGN_SWITCH starNetCallsignSwitch10;
	bool starNetTXMsgSwitch10;
	config.getStarNet10(starNetBand10, starNetCallsign10, starNetLogoff10, starNetInfo10, starNetPermanent10, starNetUserTimeout10, starNetGroupTimeout10, starNetCallsignSwitch10, starNetTXMsgSwitch10
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
		,starNetLink10
#endif
);

	if (!starNetCallsign10.IsEmpty() && !starNetCallsign10.IsSameAs(wxT("        "))) {
		wxString repeater = callsign;
		repeater.Truncate(LONG_CALLSIGN_LENGTH - 1U);
		repeater.Append(starNetBand10);

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
		m_thread->addStarNet(starNetCallsign10, starNetLogoff10, repeater, starNetInfo10, starNetPermanent10, starNetUserTimeout10, starNetGroupTimeout10, starNetCallsignSwitch10, starNetTXMsgSwitch10, starNetLink10);
		wxLogInfo(wxT("StarNet 10 set to %s/%s on repeater %s, info: \"%s\", permanent: %s, user: %u mins, group: %u mins, callsign switch: %d, tx msg switch: %d, reflector: %s"), starNetCallsign10.c_str(), starNetLogoff10.c_str(), repeater.c_str(), starNetInfo10.c_str(), starNetPermanent10.c_str(), starNetUserTimeout10, starNetGroupTimeout10, int(starNetCallsignSwitch10), int(starNetTXMsgSwitch10), starNetLink10.c_str());
#else
		m_thread->addStarNet(starNetCallsign10, starNetLogoff10, repeater, starNetInfo10, starNetPermanent10, starNetUserTimeout10, starNetGroupTimeout10, starNetCallsignSwitch10, starNetTXMsgSwitch10);
		wxLogInfo(wxT("StarNet 10 set to %s/%s on repeater %s, info: \"%s\", permanent: %s, user: %u mins, group: %u mins, callsign switch: %d, tx msg switch: %d"), starNetCallsign10.c_str(), starNetLogoff10.c_str(), repeater.c_str(), starNetInfo10.c_str(), starNetPermanent10.c_str(), starNetUserTimeout10, starNetGroupTimeout10, int(starNetCallsignSwitch10), int(starNetTXMsgSwitch10));
#endif
	}

	wxString starNetBand11, starNetCallsign11, starNetLogoff11, starNetInfo11, starNetPermanent11, starNetLink11;		// DEXTRA_LINK || DCS_LINK
	unsigned int starNetUserTimeout11, starNetGroupTimeout11;
	STARNET_CALLSIGN_SWITCH starNetCallsignSwitch11;
	bool starNetTXMsgSwitch11;
	config.getStarNet11(starNetBand11, starNetCallsign11, starNetLogoff11, starNetInfo11, starNetPermanent11, starNetUserTimeout11, starNetGroupTimeout11, starNetCallsignSwitch11, starNetTXMsgSwitch11
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
		,starNetLink11
#endif
);

	if (!starNetCallsign11.IsEmpty() && !starNetCallsign11.IsSameAs(wxT("        "))) {
		wxString repeater = callsign;
		repeater.Truncate(LONG_CALLSIGN_LENGTH - 1U);
		repeater.Append(starNetBand11);

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
		m_thread->addStarNet(starNetCallsign11, starNetLogoff11, repeater, starNetInfo11, starNetPermanent11, starNetUserTimeout11, starNetGroupTimeout11, starNetCallsignSwitch11, starNetTXMsgSwitch11, starNetLink11);
		wxLogInfo(wxT("StarNet 11 set to %s/%s on repeater %s, info: \"%s\", permanent: %s, user: %u mins, group: %u mins, callsign switch: %d, tx msg switch: %d, reflector: %s"), starNetCallsign11.c_str(), starNetLogoff11.c_str(), repeater.c_str(), starNetInfo11.c_str(), starNetPermanent11.c_str(), starNetUserTimeout11, starNetGroupTimeout11, int(starNetCallsignSwitch11), int(starNetTXMsgSwitch11), starNetLink11.c_str());
#else
		m_thread->addStarNet(starNetCallsign11, starNetLogoff11, repeater, starNetInfo11, starNetPermanent11, starNetUserTimeout11, starNetGroupTimeout11, starNetCallsignSwitch11, starNetTXMsgSwitch11);
		wxLogInfo(wxT("StarNet 11 set to %s/%s on repeater %s, info: \"%s\", permanent: %s, user: %u mins, group: %u mins, callsign switch: %d, tx msg switch: %d"), starNetCallsign11.c_str(), starNetLogoff11.c_str(), repeater.c_str(), starNetInfo11.c_str(), starNetPermanent11.c_str(), starNetUserTimeout11, starNetGroupTimeout11, int(starNetCallsignSwitch11), int(starNetTXMsgSwitch11));
#endif
	}

	wxString starNetBand12, starNetCallsign12, starNetLogoff12, starNetInfo12, starNetPermanent12, starNetLink12;		// DEXTRA_LINK || DCS_LINK
	unsigned int starNetUserTimeout12, starNetGroupTimeout12;
	STARNET_CALLSIGN_SWITCH starNetCallsignSwitch12;
	bool starNetTXMsgSwitch12;
	config.getStarNet12(starNetBand12, starNetCallsign12, starNetLogoff12, starNetInfo12, starNetPermanent12, starNetUserTimeout12, starNetGroupTimeout12, starNetCallsignSwitch12, starNetTXMsgSwitch12
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
		,starNetLink12
#endif
);

	if (!starNetCallsign12.IsEmpty() && !starNetCallsign12.IsSameAs(wxT("        "))) {
		wxString repeater = callsign;
		repeater.Truncate(LONG_CALLSIGN_LENGTH - 1U);
		repeater.Append(starNetBand12);

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
		m_thread->addStarNet(starNetCallsign12, starNetLogoff12, repeater, starNetInfo12, starNetPermanent12, starNetUserTimeout12, starNetGroupTimeout12, starNetCallsignSwitch12, starNetTXMsgSwitch12, starNetLink12);
		wxLogInfo(wxT("StarNet 12 set to %s/%s on repeater %s, info: \"%s\", permanent: %s, user: %u mins, group: %u mins, callsign switch: %d, tx msg switch: %d, reflector: %s"), starNetCallsign12.c_str(), starNetLogoff12.c_str(), repeater.c_str(), starNetInfo12.c_str(), starNetPermanent12.c_str(), starNetUserTimeout12, starNetGroupTimeout12, int(starNetCallsignSwitch12), int(starNetTXMsgSwitch12), starNetLink12.c_str());
#else
		m_thread->addStarNet(starNetCallsign12, starNetLogoff12, repeater, starNetInfo12, starNetPermanent12, starNetUserTimeout12, starNetGroupTimeout12, starNetCallsignSwitch12, starNetTXMsgSwitch12);
		wxLogInfo(wxT("StarNet 12 set to %s/%s on repeater %s, info: \"%s\", permanent: %s, user: %u mins, group: %u mins, callsign switch: %d, tx msg switch: %d"), starNetCallsign12.c_str(), starNetLogoff12.c_str(), repeater.c_str(), starNetInfo12.c_str(), starNetPermanent12.c_str(), starNetUserTimeout12, starNetGroupTimeout12, int(starNetCallsignSwitch12), int(starNetTXMsgSwitch12));
#endif
	}

	wxString starNetBand13, starNetCallsign13, starNetLogoff13, starNetInfo13, starNetPermanent13, starNetLink13;		// DEXTRA_LINK || DCS_LINK
	unsigned int starNetUserTimeout13, starNetGroupTimeout13;
	STARNET_CALLSIGN_SWITCH starNetCallsignSwitch13;
	bool starNetTXMsgSwitch13;
	config.getStarNet13(starNetBand13, starNetCallsign13, starNetLogoff13, starNetInfo13, starNetPermanent13, starNetUserTimeout13, starNetGroupTimeout13, starNetCallsignSwitch13, starNetTXMsgSwitch13
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
		,starNetLink13
#endif
);

	if (!starNetCallsign13.IsEmpty() && !starNetCallsign13.IsSameAs(wxT("        "))) {
		wxString repeater = callsign;
		repeater.Truncate(LONG_CALLSIGN_LENGTH - 1U);
		repeater.Append(starNetBand13);

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
		m_thread->addStarNet(starNetCallsign13, starNetLogoff13, repeater, starNetInfo13, starNetPermanent13, starNetUserTimeout13, starNetGroupTimeout13, starNetCallsignSwitch13, starNetTXMsgSwitch13, starNetLink13);
		wxLogInfo(wxT("StarNet 13 set to %s/%s on repeater %s, info: \"%s\", permanent: %s, user: %u mins, group: %u mins, callsign switch: %d, tx msg switch: %d, reflector: %s"), starNetCallsign13.c_str(), starNetLogoff13.c_str(), repeater.c_str(), starNetInfo13.c_str(), starNetPermanent13.c_str(), starNetUserTimeout13, starNetGroupTimeout13, int(starNetCallsignSwitch13), int(starNetTXMsgSwitch13), starNetLink13.c_str());
#else
		m_thread->addStarNet(starNetCallsign13, starNetLogoff13, repeater, starNetInfo13, starNetPermanent13, starNetUserTimeout13, starNetGroupTimeout13, starNetCallsignSwitch13, starNetTXMsgSwitch13);
		wxLogInfo(wxT("StarNet 13 set to %s/%s on repeater %s, info: \"%s\", permanent: %s, user: %u mins, group: %u mins, callsign switch: %d, tx msg switch: %d"), starNetCallsign13.c_str(), starNetLogoff13.c_str(), repeater.c_str(), starNetInfo13.c_str(), starNetPermanent13.c_str(), starNetUserTimeout13, starNetGroupTimeout13, int(starNetCallsignSwitch13), int(starNetTXMsgSwitch13));
#endif
	}

	wxString starNetBand14, starNetCallsign14, starNetLogoff14, starNetInfo14, starNetPermanent14, starNetLink14;		// DEXTRA_LINK || DCS_LINK
	unsigned int starNetUserTimeout14, starNetGroupTimeout14;
	STARNET_CALLSIGN_SWITCH starNetCallsignSwitch14;
	bool starNetTXMsgSwitch14;
	config.getStarNet14(starNetBand14, starNetCallsign14, starNetLogoff14, starNetInfo14, starNetPermanent14, starNetUserTimeout14, starNetGroupTimeout14, starNetCallsignSwitch14, starNetTXMsgSwitch14
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
		,starNetLink14
#endif
);

	if (!starNetCallsign14.IsEmpty() && !starNetCallsign14.IsSameAs(wxT("        "))) {
		wxString repeater = callsign;
		repeater.Truncate(LONG_CALLSIGN_LENGTH - 1U);
		repeater.Append(starNetBand14);

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
		m_thread->addStarNet(starNetCallsign14, starNetLogoff14, repeater, starNetInfo14, starNetPermanent14, starNetUserTimeout14, starNetGroupTimeout14, starNetCallsignSwitch14, starNetTXMsgSwitch14, starNetLink14);
		wxLogInfo(wxT("StarNet 14 set to %s/%s on repeater %s, info: \"%s\", permanent: %s, user: %u mins, group: %u mins, callsign switch: %d, tx msg switch: %d, reflector: %s"), starNetCallsign14.c_str(), starNetLogoff14.c_str(), repeater.c_str(), starNetInfo14.c_str(), starNetPermanent14.c_str(), starNetUserTimeout14, starNetGroupTimeout14, int(starNetCallsignSwitch14), int(starNetTXMsgSwitch14), starNetLink14.c_str());
#else
		m_thread->addStarNet(starNetCallsign14, starNetLogoff14, repeater, starNetInfo14, starNetPermanent14, starNetUserTimeout14, starNetGroupTimeout14, starNetCallsignSwitch14, starNetTXMsgSwitch14);
		wxLogInfo(wxT("StarNet 14 set to %s/%s on repeater %s, info: \"%s\", permanent: %s, user: %u mins, group: %u mins, callsign switch: %d, tx msg switch: %d"), starNetCallsign14.c_str(), starNetLogoff14.c_str(), repeater.c_str(), starNetInfo14.c_str(), starNetPermanent14.c_str(), starNetUserTimeout14, starNetGroupTimeout14, int(starNetCallsignSwitch14), int(starNetTXMsgSwitch14));
#endif
	}

	wxString starNetBand15, starNetCallsign15, starNetLogoff15, starNetInfo15, starNetPermanent15, starNetLink15;		// DEXTRA_LINK || DCS_LINK
	unsigned int starNetUserTimeout15, starNetGroupTimeout15;
	STARNET_CALLSIGN_SWITCH starNetCallsignSwitch15;
	bool starNetTXMsgSwitch15;
	config.getStarNet15(starNetBand15, starNetCallsign15, starNetLogoff15, starNetInfo15, starNetPermanent15, starNetUserTimeout15, starNetGroupTimeout15, starNetCallsignSwitch15, starNetTXMsgSwitch15
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
		,starNetLink15
#endif
);

	if (!starNetCallsign15.IsEmpty() && !starNetCallsign15.IsSameAs(wxT("        "))) {
		wxString repeater = callsign;
		repeater.Truncate(LONG_CALLSIGN_LENGTH - 1U);
		repeater.Append(starNetBand15);

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
		m_thread->addStarNet(starNetCallsign15, starNetLogoff15, repeater, starNetInfo15, starNetPermanent15, starNetUserTimeout15, starNetGroupTimeout15, starNetCallsignSwitch15, starNetTXMsgSwitch15, starNetLink15);
		wxLogInfo(wxT("StarNet 15 set to %s/%s on repeater %s, info: \"%s\", permanent: %s, user: %u mins, group: %u mins, callsign switch: %d, tx msg switch: %d, reflector: %s"), starNetCallsign15.c_str(), starNetLogoff15.c_str(), repeater.c_str(), starNetInfo15.c_str(), starNetPermanent15.c_str(), starNetUserTimeout15, starNetGroupTimeout15, int(starNetCallsignSwitch15), int(starNetTXMsgSwitch15), starNetLink15.c_str());
#else
		m_thread->addStarNet(starNetCallsign15, starNetLogoff15, repeater, starNetInfo15, starNetPermanent15, starNetUserTimeout15, starNetGroupTimeout15, starNetCallsignSwitch15, starNetTXMsgSwitch15);
		wxLogInfo(wxT("StarNet 15 set to %s/%s on repeater %s, info: \"%s\", permanent: %s, user: %u mins, group: %u mins, callsign switch: %d, tx msg switch: %d"), starNetCallsign15.c_str(), starNetLogoff15.c_str(), repeater.c_str(), starNetInfo15.c_str(), starNetPermanent15.c_str(), starNetUserTimeout15, starNetGroupTimeout15, int(starNetCallsignSwitch15), int(starNetTXMsgSwitch15));
#endif
	}

	bool remoteEnabled;
	wxString remotePassword;
	unsigned int remotePort;
	config.getRemote(remoteEnabled, remotePassword, remotePort);
	wxLogInfo(wxT("Remote enabled set to %d, port set to %u"), int(remoteEnabled), remotePort);
	m_thread->setRemote(remoteEnabled, remotePassword, remotePort);

	m_thread->setLog(logEnabled);
	m_thread->setAddress(address);
	m_thread->setCallsign(callsign);

	return true;
}

