/*

CIRCDDB - ircDDB client library in C++

Copyright (C) 2010-2011			Michael Dirska, DL1BFF (dl1bff@mdx.de)
Copyright (C) 2011,2012,2014	Jonathan Naylor, G4KLX

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/


#include "IRCDDB.h"

#include "DStarDefines.h"
#include "IRCClient.h"
#include "IRCDDBApp.h"

#include <wx/wx.h>

CIRCDDB::CIRCDDB(const wxString& hostName, unsigned int port, const wxString& callsign, const wxString& password, const wxString& versionInfo, const wxString& localAddr) :
m_client(NULL),
m_app(NULL)
{
	wxString update_channel = wxT("#dstar");

	m_app = new IRCDDBApp(update_channel);

	m_client = new IRCClient(m_app, update_channel, hostName, port, callsign, password, versionInfo, localAddr);
}

CIRCDDB::~CIRCDDB()
{
}

// A false return implies a network error, or unable to log in
bool CIRCDDB::open()
{
	m_client->startWork();
	m_app->startWork();

	return true;
}

int CIRCDDB::getConnectionState()
{
	return m_app->getConnectionState();
}

void CIRCDDB::rptrQTH(const wxString& callsign, double latitude, double longitude, const wxString& desc1, const wxString& desc2, const wxString& infoURL)
{
	m_app->rptrQTH(callsign, latitude, longitude, desc1, desc2, infoURL);
}

void CIRCDDB::rptrQRG(const wxString& callsign, double txFrequency, double duplexShift, double range, double agl)
{
	m_app->rptrQRG(callsign, txFrequency, duplexShift, range, agl);
}

void CIRCDDB::kickWatchdog(const wxString& callsign, const wxString& wdInfo)
{
	m_app->kickWatchdog(callsign, wdInfo);
}

// Send heard data, a false return implies a network error
bool CIRCDDB::sendHeard(const wxString& myCall, const wxString& myCallExt, const wxString& yourCall, const wxString& rpt1, const wxString& rpt2, unsigned char flag1, unsigned char flag2, unsigned char flag3)
{
	if (myCall.Len() != LONG_CALLSIGN_LENGTH)
		return false;

	if (myCallExt.Len() != SHORT_CALLSIGN_LENGTH)
		return false;

	if (yourCall.Len() != LONG_CALLSIGN_LENGTH)
		return false;

	if (rpt1.Len() != LONG_CALLSIGN_LENGTH)
		return false;

	if (rpt2.Len() != LONG_CALLSIGN_LENGTH)
		return false;

	return m_app->sendHeard( myCall, myCallExt, yourCall, rpt1, rpt2, flag1, flag2, flag3, wxT("        "), wxEmptyString, wxEmptyString);
}

// Send heard data, a false return implies a network error
bool CIRCDDB::sendHeardWithTXMsg( const wxString& myCall, const wxString& myCallExt, const wxString& yourCall, const wxString& rpt1, const wxString& rpt2, unsigned char flag1, unsigned char flag2, unsigned char flag3, const wxString& network_destination, const wxString& tx_message)
{
	if (myCall.Len() != LONG_CALLSIGN_LENGTH)
		return false;

	if (myCallExt.Len() != SHORT_CALLSIGN_LENGTH)
		return false;

	if (yourCall.Len() != LONG_CALLSIGN_LENGTH)
		return false;

	if (rpt1.Len() != LONG_CALLSIGN_LENGTH)
		return false;

	if (rpt2.Len() != LONG_CALLSIGN_LENGTH)
		return false;

	wxString dest = network_destination;

	if (dest.Len() == 0)
		dest = wxT("        ");

	if (dest.Len() != LONG_CALLSIGN_LENGTH)
		return false;

	wxString msg;

	if (tx_message.Len() == 20) {
		for (unsigned int i = 0U; i < tx_message.Len(); i++) {
			wxChar ch = tx_message.GetChar(i);

			if ((ch > 32) && (ch < 127))
				msg.Append(ch);
			else
				msg.Append(wxT('_'));
		}
	}

	return m_app->sendHeard(myCall, myCallExt, yourCall, rpt1, rpt2, flag1, flag2, flag3, dest, msg, wxEmptyString);
}

bool CIRCDDB::sendHeardWithTXStats( const wxString& myCall, const wxString& myCallExt, const wxString& yourCall, const wxString& rpt1, const wxString& rpt2, unsigned char flag1, unsigned char flag2, unsigned char flag3, int num_dv_frames, int num_dv_silent_frames, int num_bit_errors)
{
	if ((num_dv_frames <= 0) || (num_dv_frames > 65535))
		return false;

	if (num_dv_silent_frames > num_dv_frames)
		return false;

	if (num_bit_errors > (4*num_dv_frames)) // max 4 bit errors per frame
		return false;

	if (myCall.Len() != LONG_CALLSIGN_LENGTH)
		return false;

	if (myCallExt.Len() != SHORT_CALLSIGN_LENGTH)
		return false;

	if (yourCall.Len() != LONG_CALLSIGN_LENGTH)
		return false;

	if (rpt1.Len() != LONG_CALLSIGN_LENGTH)
		return false;

	if (rpt2.Len() != LONG_CALLSIGN_LENGTH)
		return false;

	wxString stats = wxString::Format(wxT("%04x"), num_dv_frames);
	wxString s;

	if (num_dv_silent_frames >= 0) {
		s.Printf(wxT("%02x"), (num_dv_silent_frames * 100) / num_dv_frames);
		stats.Append(s);

		if (num_bit_errors >= 0) {
			s.Printf(wxT("%02x"), (num_bit_errors * 125) / (num_dv_frames * 3));
			stats.Append(s);
		} else {
			stats.Append(wxT("__"));
		}
	} else {
		stats.Append(wxT("____"));
	}

	stats.Append(wxT("____________"));  // stats string should have 20 chars

	return m_app->sendHeard(myCall, myCallExt, yourCall, rpt1, rpt2, flag1, flag2, flag3, wxT("        "), wxEmptyString, stats);
}

// Send query for a gateway/reflector, a false return implies a network error
bool CIRCDDB::findGateway(const wxString& gatewayCallsign)
{
	if (gatewayCallsign.Len() != LONG_CALLSIGN_LENGTH)
		return false;

	return m_app->findGateway(gatewayCallsign.Upper());
}

bool CIRCDDB::findRepeater(const wxString& repeaterCallsign)
{
	if (repeaterCallsign.Len() != LONG_CALLSIGN_LENGTH)
		return false;

	return m_app->findRepeater(repeaterCallsign.Upper());
}

// Send query for a user, a false return implies a network error
bool CIRCDDB::findUser(const wxString& userCallsign)
{
	if (userCallsign.Len() != LONG_CALLSIGN_LENGTH)
		return false;

	return m_app->findUser(userCallsign.Upper());
}

// The following functions are for processing received messages

// Get the waiting message type
IRCDDB_RESPONSE_TYPE CIRCDDB::getMessageType()
{
	return m_app->getReplyMessageType();
}

// Get a gateway message, as a result of IDRT_REPEATER returned from getMessageType()
// A false return implies a network error
bool CIRCDDB::receiveRepeater(wxString& repeaterCallsign, wxString& gatewayCallsign, wxString& address)
{
	IRCDDB_RESPONSE_TYPE rt = m_app->getReplyMessageType();
	if (rt != IDRT_REPEATER) {
		wxLogError(wxT("CIRCDDB::receiveRepeater: unexpected response type"));
		return false;
	}

	IRCMessage* m = m_app->getReplyMessage();
	if (m == NULL) {
		wxLogError(wxT("CIRCDDB::receiveRepeater: no message"));
		return false;
	}

	if (!m->m_command.IsSameAs(wxT("IDRT_REPEATER"))) {
		wxLogError(wxT("CIRCDDB::receiveRepeater: wrong message type"));
		delete m;
		return false;
	}

	if (m->m_params.GetCount() != 3) {
		wxLogError(wxT("CIRCDDB::receiveRepeater: unexpected number of message parameters"));
		delete m;
		return false;
	}

	repeaterCallsign = m->m_params.Item(0);
	gatewayCallsign  = m->m_params.Item(1);
	address          = m->m_params.Item(2);

	delete m;

	return true;
}

// Get a gateway message, as a result of IDRT_GATEWAY returned from getMessageType()
// A false return implies a network error
bool CIRCDDB::receiveGateway(wxString& gatewayCallsign, wxString& address)
{
	IRCDDB_RESPONSE_TYPE rt = m_app->getReplyMessageType();
	if (rt != IDRT_GATEWAY) {
		wxLogError(wxT("CIRCDDB::receiveGateway: unexpected response type"));
		return false;
	}

	IRCMessage* m = m_app->getReplyMessage();
	if (m == NULL) {
		wxLogError(wxT("CIRCDDB::receiveGateway: no message"));
		return false;
	}

	if (!m->m_command.IsSameAs(wxT("IDRT_GATEWAY"))) {
		wxLogError(wxT("CIRCDDB::receiveGateway: wrong message type"));
		delete m;
		return false;
	}

	if (m->m_params.GetCount() != 2U) {
		wxLogError(wxT("CIRCDDB::receiveGateway: unexpected number of message parameters"));
		delete m;
		return false;
	}

	gatewayCallsign = m->m_params.Item(0);
	address         = m->m_params.Item(1);

	delete m;

	return true;
}

// Get a user message, as a result of IDRT_USER returned from getMessageType()
// A false return implies a network error
bool CIRCDDB::receiveUser(wxString& userCallsign, wxString& repeaterCallsign, wxString& gatewayCallsign, wxString& address)
{
	wxString dummy;
	return receiveUser(userCallsign, repeaterCallsign, gatewayCallsign, address, dummy);
}

bool CIRCDDB::receiveUser(wxString& userCallsign, wxString& repeaterCallsign, wxString& gatewayCallsign, wxString& address, wxString& timeStamp)
{
	IRCDDB_RESPONSE_TYPE rt = m_app->getReplyMessageType();
	if (rt != IDRT_USER) {
		wxLogError(wxT("CIRCDDB::receiveUser: unexpected response type"));
		return false;
	}

	IRCMessage* m = m_app->getReplyMessage();
	if (m == NULL) {
		wxLogError(wxT("CIRCDDB::receiveUser: no message"));
		return false;
	}

	if (!m->m_command.IsSameAs(wxT("IDRT_USER"))) {
		wxLogError(wxT("CIRCDDB::receiveUser: wrong message type"));
		delete m;
		return false;
	}

	if (m->m_params.GetCount() != 5) {
		wxLogError(wxT("CIRCDDB::receiveUser: unexpected number of message parameters"));
		delete m;
		return false;
	}

	userCallsign     = m->m_params.Item(0);
	repeaterCallsign = m->m_params.Item(1);
	gatewayCallsign  = m->m_params.Item(2);
	address          = m->m_params.Item(3);
	timeStamp        = m->m_params.Item(4);

	delete m;

	return true;
}

void CIRCDDB::close()		// Implictely kills any threads in the IRC code
{
	m_client->stopWork();
	m_app->stopWork();
}
