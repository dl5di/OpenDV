/*

CIRCDDBClient - ircDDB client library in C++

Copyright (C) 2010-2011   Michael Dirska, DL1BFF (dl1bff@mdx.de)
Copyright (C) 2011,2012   Jonathan Naylor, G4KLX

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

#include "IRCDDBMultiClient.h"
#include <wx/wx.h>

CIRCDDBMultiClient::CIRCDDBMultiClient(CIRCDDB * * clients, unsigned int count) :
m_clients(new CIRCDDB*[count]),
m_clientCount(count),
m_currentClient(0),
m_currentClientLock()
{
	wxASSERT(clients != NULL);
	::memcpy(m_clients, clients, count * sizeof(CIRCDDB *));

#if _DEBUG
	for (unsigned int i = 0; i < count; i++) {
		wxASSERT(m_clients[i] != NULL);
	}
#endif
}

CIRCDDBMultiClient::~CIRCDDBMultiClient()
{
	for (int i = 0; i < m_clientCount; i++) {
		delete m_clients[i];
	}

	delete[] m_clients;
}

bool CIRCDDBMultiClient::open()
{
	bool result = false;

	for (int i = 0; i < m_clientCount; i++) {
		result = m_clients[i]->open() && result;
	}

	if (!result) close();

	return result;
}

void CIRCDDBMultiClient::rptrQTH(const wxString & callsign, double latitude, double longitude, const wxString & desc1, const wxString & desc2, const wxString & infoURL)
{
	for (int i = 0; i < m_clientCount; i++) {
		m_clients[i]->rptrQTH(callsign, latitude, longitude, desc1, desc2, infoURL);
	}
}

void CIRCDDBMultiClient::rptrQRG(const wxString & callsign, double txFrequency, double duplexShift, double range, double agl)
{
	for (int i = 0; i < m_clientCount; i++) {
		m_clients[i]->rptrQRG(callsign, txFrequency, duplexShift, range, agl);
	}
}

void CIRCDDBMultiClient::kickWatchdog(const wxString & callsign, const wxString & wdInfo)
{
	for (int i = 0; i < m_clientCount; i++) {
		m_clients[i]->kickWatchdog(callsign, wdInfo);
	}
}

int CIRCDDBMultiClient::getConnectionState()
{
	for (int i = 0; i < m_clientCount; i++) {
		int state = m_clients[i]->getConnectionState();
		if (state != 7)
			return state;
	}

	return 7;
}

bool CIRCDDBMultiClient::sendHeard(const wxString & myCall, const wxString & myCallExt, const wxString & yourCall, const wxString & rpt1, const wxString & rpt2, unsigned char flag1, unsigned char flag2, unsigned char flag3)
{
	bool result = true;

	for (int i = 0; i < m_clientCount; i++) {
		result = m_clients[i]->sendHeard(myCall, myCallExt, yourCall, rpt1, rpt2, flag1, flag2, flag3) && result;
	}

	return result;
}

bool CIRCDDBMultiClient::sendHeardWithTXMsg(const wxString & myCall, const wxString & myCallExt, const wxString & yourCall, const wxString & rpt1, const wxString & rpt2, unsigned char flag1, unsigned char flag2, unsigned char flag3, const wxString & network_destination, const wxString & tx_message)
{
	bool result = true;

	for (int i = 0; i < m_clientCount; i++) {
		result = m_clients[i]->sendHeardWithTXMsg(myCall, myCallExt, yourCall, rpt1, rpt2, flag1, flag2, flag3, network_destination, tx_message) && result;
	}

	return result;
}

bool CIRCDDBMultiClient::sendHeardWithTXStats(const wxString & myCall, const wxString & myCallExt, const wxString & yourCall, const wxString & rpt1, const wxString & rpt2, unsigned char flag1, unsigned char flag2, unsigned char flag3, int num_dv_frames, int num_dv_silent_frames, int num_bit_errors)
{
	bool result = true;

	for (int i = 0; i < m_clientCount; i++) {
		result = m_clients[i]->sendHeardWithTXStats(myCall, myCallExt, yourCall, rpt1, rpt2, flag1, flag2, flag3, num_dv_frames, num_dv_silent_frames, num_bit_errors) && result;
	}

	return result;
}

bool CIRCDDBMultiClient::findGateway(const wxString & gatewayCallsign)
{
	bool result = true;

	for (int i = 0; i < m_clientCount; i++) {
		result = m_clients[i]->findGateway(gatewayCallsign) && result;
	}

	return result;
}

bool CIRCDDBMultiClient::findRepeater(const wxString & repeaterCallsign)
{
	bool result = true;

	for (int i = 0; i < m_clientCount; i++) {
		result = m_clients[i]->findRepeater(repeaterCallsign) && result;
	}

	return result;
}

bool CIRCDDBMultiClient::findUser(const wxString & userCallsign)
{
	bool result = true;

	for (int i = 0; i < m_clientCount; i++) {
		result = m_clients[i]->findUser(userCallsign) && result;
	}

	return result;
}

IRCDDB_RESPONSE_TYPE CIRCDDBMultiClient::getMessageType()
{
	LockCurrentClient();
	return m_clients[m_currentClient]->getMessageType();
}

bool CIRCDDBMultiClient::receiveRepeater(wxString & repeaterCallsign, wxString & gatewayCallsign, wxString & address)
{
	bool result = m_clients[m_currentClient]->receiveRepeater(repeaterCallsign, gatewayCallsign, address);
	ReleaseCurrentClient();
	return result;
}

bool CIRCDDBMultiClient::receiveGateway(wxString & gatewayCallsign, wxString & address)
{
	bool result = m_clients[m_currentClient]->receiveGateway(gatewayCallsign, address);
	ReleaseCurrentClient();
	return result;
}

bool CIRCDDBMultiClient::receiveUser(wxString & userCallsign, wxString & repeaterCallsign, wxString & gatewayCallsign, wxString & address)
{
	bool result = m_clients[m_currentClient]->receiveUser(userCallsign, repeaterCallsign, gatewayCallsign, address);
	ReleaseCurrentClient();
	return result;
}

bool CIRCDDBMultiClient::receiveUser(wxString & userCallsign, wxString & repeaterCallsign, wxString & gatewayCallsign, wxString & address, wxString & timeStamp)
{
	bool result = m_clients[m_currentClient]->receiveUser(userCallsign, repeaterCallsign, gatewayCallsign, address, timeStamp);
	ReleaseCurrentClient();
	return result;
}

void CIRCDDBMultiClient::close()
{
	for (int i = 0; i < m_clientCount; i++) {
		m_clients[i]->close();
	}
}

void CIRCDDBMultiClient::LockCurrentClient()
{
	m_currentClientLock.Lock();
}

void CIRCDDBMultiClient::ReleaseCurrentClient()
{
	m_currentClient++;
	if (m_currentClient >= m_clientCount)
		m_currentClient = 0;

	m_currentClientLock.Unlock();
}
