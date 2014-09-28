/*
 *   Copyright (C) 2013,2014 by Jonathan Naylor G4KLX
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

#include "RemoteControlRemoteControlHandler.h"
#include "RemoteControlConfig.h"
#include "RemoteControlDefs.h"
#include "DStarDefines.h"
#include "SHA256.h"
#include "Defs.h"

#include <wx/cmdline.h>

const wxChar* NAME_OPTION     = wxT("name");
const wxChar* REPEATER_PARAM  = wxT("Callsign");
const wxChar* ACTION_PARAM    = wxT("Action");
const wxChar* RECONNECT_PARAM = wxT("Param1");
const wxChar* REFLECTOR_PARAM = wxT("Param2");

void sendHash(CRemoteControlRemoteControlHandler* handler, const wxString& password, unsigned int rnd)
{
	wxASSERT(handler != NULL);

	unsigned int len = password.Len() + sizeof(unsigned int);
	unsigned char*  in = new unsigned char[len];
	unsigned char* out = new unsigned char[32U];

	::memcpy(in, &rnd, sizeof(unsigned int));
	for (unsigned int i = 0U; i < password.Len(); i++)
		in[i + sizeof(unsigned int)] = password.GetChar(i);

	CSHA256 sha256;
	sha256.buffer(in, len, out);

	handler->sendHash(out, 32U);

	delete[] in;
	delete[] out;
}

int main(int argc, char** argv)
{
	bool res = ::wxInitialize();
	if (!res) {
		::fprintf(stderr, "remotecontrold: failed to initialise the wxWidgets library, exiting\n");
		return -1;
	}

	wxCmdLineParser parser(argc, argv);
	parser.AddOption(NAME_OPTION, wxEmptyString, wxEmptyString, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddParam(REPEATER_PARAM,  wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddParam(ACTION_PARAM,    wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddParam(RECONNECT_PARAM, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddParam(REFLECTOR_PARAM, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);

	int cmd = parser.Parse();
	if (cmd != 0) {
		::wxUninitialize();
		return 0;
	}

	wxString name;
	parser.Found(NAME_OPTION, &name);

	if (parser.GetParamCount() < 2U) {
		::fprintf(stderr, "remotecontrold: invalid command line usage: remotecontrold [-name <name>] <repeater> link <reconnect> <reflector>\n");
		::fprintf(stderr, "                                            remotecontrold [-name <name>] <repeater> unlink\n");
		::fprintf(stderr, "                                            remotecontrold [-name <name>] <starnet> drop <user>\n");
		::fprintf(stderr, "                                            remotecontrold [-name <name>] <starnet> drop all\n");
		::wxUninitialize();
		return 1;
	}

	wxString repeater = parser.GetParam(0U);
	repeater.Replace(wxT("_"), wxT(" "));
	repeater.resize(LONG_CALLSIGN_LENGTH, wxT(' '));
	repeater.MakeUpper();

	wxString actionText = parser.GetParam(1U);

	wxString  user;								// For STARnet Digital
	wxString  reflector;						// For linking
	RECONNECT reconnect = RECONNECT_NEVER;		// For linking

	if (actionText.IsSameAs(wxT("link"), false)) {
		if (parser.GetParamCount() < 4U) {
			::fprintf(stderr, "remotecontrold: invalid command line usage: remotecontrold [-name <name>] <repeater> link <reconnect> <reflector>\n");
			::wxUninitialize();
			return 1;
		}

		wxString reconnectText = parser.GetParam(2U);

		reflector = parser.GetParam(3U);
		reflector.Replace(wxT("_"), wxT(" "));
		reflector.resize(LONG_CALLSIGN_LENGTH, wxT(' '));
		reflector.MakeUpper();

		if (reconnectText.IsSameAs(wxT("never"), false)) {
			reconnect = RECONNECT_NEVER;
		} else if (reconnectText.IsSameAs(wxT("fixed"), false)) {
			reconnect = RECONNECT_FIXED;
		} else if (reconnectText.IsSameAs(wxT("5"), false)) {
			reconnect = RECONNECT_5MINS;
		} else if (reconnectText.IsSameAs(wxT("10"), false)) {
			reconnect = RECONNECT_10MINS;
		} else if (reconnectText.IsSameAs(wxT("15"), false)) {
			reconnect = RECONNECT_15MINS;
		} else if (reconnectText.IsSameAs(wxT("20"), false)) {
			reconnect = RECONNECT_20MINS;
		} else if (reconnectText.IsSameAs(wxT("25"), false)) {
			reconnect = RECONNECT_25MINS;
		} else if (reconnectText.IsSameAs(wxT("30"), false)) {
			reconnect = RECONNECT_30MINS;
		} else if (reconnectText.IsSameAs(wxT("60"), false)) {
			reconnect = RECONNECT_60MINS;
		} else if (reconnectText.IsSameAs(wxT("90"), false)) {
			reconnect = RECONNECT_90MINS;
		} else if (reconnectText.IsSameAs(wxT("120"), false)) {
			reconnect = RECONNECT_120MINS;
		} else if (reconnectText.IsSameAs(wxT("180"), false)) {
			reconnect = RECONNECT_180MINS;
		} else {
			::fprintf(stderr, "remotecontrold: invalid reconnect value passed\n");
			::wxUninitialize();
			return 1;
		}
	} else if (actionText.IsSameAs(wxT("unlink"), false)) {
		reconnect = RECONNECT_NEVER;
		reflector.Clear();
	} else if (actionText.IsSameAs(wxT("drop"), false)) {
		if (parser.GetParamCount() < 3U) {
			::fprintf(stderr, "remotecontrold: invalid command line usage: remotecontrold [-name <name>] <starnet> drop <user>\n");
			::fprintf(stderr, "                                            remotecontrold [-name <name>] <starnet> drop all\n");
			::wxUninitialize();
			return 1;
		}

		user = parser.GetParam(2U);
		user.Replace(wxT("_"), wxT(" "));
		user.resize(LONG_CALLSIGN_LENGTH, wxT(' '));
		user.MakeUpper();
	} else {
		::fprintf(stderr, "remotecontrold: invalid action value passed, only drop, link or unlink are allowed\n");
		::wxUninitialize();
		return 1;
	}

	CRemoteControlConfig config(new wxConfig(APPLICATION_NAME), name);

	wxString address, password;
	unsigned int port;
	config.getConfig(address, port, password);

	if (address.IsEmpty() || port == 0U || password.IsEmpty()) {
		::fprintf(stderr, "remotecontrold: no address, port, or password is set\n");
		::wxUninitialize();
		return 1;
	}

	CRemoteControlRemoteControlHandler handler(address, port);

	bool ret = handler.open();
	if (!ret) {
		::fprintf(stderr, "remotecontrold: uanble to open the UDP port\n");
		::wxUninitialize();
		return 1;
	}

	ret = handler.login();
	if (!ret) {
		handler.close();
		::fprintf(stderr, "remotecontrold: uanble to login to the gateway/starnetserver\n");
		::wxUninitialize();
		return 1;
	}

	unsigned int count = 0U;
	while (count < 10U) {
		::wxMilliSleep(100UL);

		RC_TYPE type = handler.readType();
		if (type == RCT_RANDOM)
			break;

		if (type == RCT_NONE)
			handler.retry();

		count++;
	}

	if (count >= 10U) {
		handler.close();
		::fprintf(stderr, "remotecontrold: unable to get a response from the gateway/starnetserver\n");
		::wxUninitialize();
		return 1;
	}

	unsigned int rnd = handler.readRandom();
	sendHash(&handler, password, rnd);

	count = 0U;
	while (count < 10U) {
		::wxMilliSleep(100UL);

		RC_TYPE type = handler.readType();
		if (type == RCT_ACK)
			break;

		if (type == RCT_NAK) {
			handler.close();
			::fprintf(stderr, "remotecontrold: invalid password sent to the gateway/starnetserver\n");
			::wxUninitialize();
			return 1;
		}

		if (type == RCT_NONE)
			handler.retry();

		count++;
	}

	if (count >= 10U) {
		handler.close();
		::fprintf(stderr, "remotecontrold: unable to get a response from the gateway/starnetserver\n");
		::wxUninitialize();
		return 1;
	}

	handler.setLoggedIn(true);

	if (actionText.IsSameAs(wxT("drop"), false))
		handler.logoff(repeater, user);
	else
		handler.link(repeater, reconnect, reflector);

	count = 0U;
	while (count < 10U) {
		::wxMilliSleep(100UL);

		RC_TYPE type = handler.readType();
		if (type == RCT_ACK)
			break;

		if (type == RCT_NAK) {
			handler.close();
			::fprintf(stderr, "remotecontrold: drop/link/unlink command rejected by the gateway/starnetserver\n");
			::wxUninitialize();
			return 1;
		}

		if (type == RCT_NONE)
			handler.retry();

		count++;
	}

	if (count >= 10U) {
		handler.close();
		::fprintf(stderr, "remotecontrold: unable to get a response from the gateway/starnetserver\n");
		::wxUninitialize();
		return 1;
	}

	::fprintf(stdout, "remotecontrold: command accepted by the gateway/starnetserver\n");

	handler.logout();
	handler.close();

	::wxUninitialize();

	return 0;
}
