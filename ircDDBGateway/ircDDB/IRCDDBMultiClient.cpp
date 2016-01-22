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
#include <wx/arrimpl.cpp>
WX_DEFINE_OBJARRAY(CIRCDDBMultiClientEntry_Array);

CIRCDDBMultiClient::CIRCDDBMultiClient(CIRCDDB * * clients, unsigned int count) :
m_clients(new CIRCDDB*[count]),
m_clientCount(count),
m_queriesLock(),
m_responseQueueLock()
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
	for (unsigned int i = 0; i < m_clientCount; i++) {
		delete m_clients[i];
	}

	delete[] m_clients;

	while (m_responseQueue.Count() > 0) {
		delete m_responseQueue[0];
		m_responseQueue.RemoveAt(0);
	}

	for (CIRCDDBMultiClientEntry_HashMap::iterator it = m_userQueries.begin(); it != m_userQueries.end(); it++)
		delete it->second;
	m_userQueries.clear();

	for (CIRCDDBMultiClientEntry_HashMap::iterator it = m_repeaterQueries.begin(); it != m_repeaterQueries.end(); it++)
		delete it->second;
	m_repeaterQueries.clear();

	for (CIRCDDBMultiClientEntry_HashMap::iterator it = m_gatewayQueries.begin(); it != m_gatewayQueries.end(); it++)
		delete it->second;
	m_gatewayQueries.clear();
}

bool CIRCDDBMultiClient::open()
{
	bool result = true;

	for (unsigned int i = 0; i < m_clientCount; i++) {
		result = m_clients[i]->open() && result;
	}

	if (!result) close();

	return result;
}

void CIRCDDBMultiClient::rptrQTH(const wxString & callsign, double latitude, double longitude, const wxString & desc1, const wxString & desc2, const wxString & infoURL)
{
	for (unsigned int i = 0; i < m_clientCount; i++) {
		m_clients[i]->rptrQTH(callsign, latitude, longitude, desc1, desc2, infoURL);
	}
}

void CIRCDDBMultiClient::rptrQRG(const wxString & callsign, double txFrequency, double duplexShift, double range, double agl)
{
	for (unsigned int i = 0; i < m_clientCount; i++) {
		m_clients[i]->rptrQRG(callsign, txFrequency, duplexShift, range, agl);
	}
}

void CIRCDDBMultiClient::kickWatchdog(const wxString & callsign, const wxString & wdInfo)
{
	for (unsigned int i = 0; i < m_clientCount; i++) {
		m_clients[i]->kickWatchdog(callsign, wdInfo);
	}
}

int CIRCDDBMultiClient::getConnectionState()
{
	for (unsigned int i = 0; i < m_clientCount; i++) {
		int state = m_clients[i]->getConnectionState();
		if (state != 7)
			return state;
	}

	return 7;
}

bool CIRCDDBMultiClient::sendHeard(const wxString & myCall, const wxString & myCallExt, const wxString & yourCall, const wxString & rpt1, const wxString & rpt2, unsigned char flag1, unsigned char flag2, unsigned char flag3)
{
	bool result = true;

	for (unsigned int i = 0; i < m_clientCount; i++) {
		result = m_clients[i]->sendHeard(myCall, myCallExt, yourCall, rpt1, rpt2, flag1, flag2, flag3) && result;
	}

	return result;
}

bool CIRCDDBMultiClient::sendHeardWithTXMsg(const wxString & myCall, const wxString & myCallExt, const wxString & yourCall, const wxString & rpt1, const wxString & rpt2, unsigned char flag1, unsigned char flag2, unsigned char flag3, const wxString & network_destination, const wxString & tx_message)
{
	bool result = true;

	for (unsigned int i = 0; i < m_clientCount; i++) {
		result = m_clients[i]->sendHeardWithTXMsg(myCall, myCallExt, yourCall, rpt1, rpt2, flag1, flag2, flag3, network_destination, tx_message) && result;
	}

	return result;
}

bool CIRCDDBMultiClient::sendHeardWithTXStats(const wxString & myCall, const wxString & myCallExt, const wxString & yourCall, const wxString & rpt1, const wxString & rpt2, unsigned char flag1, unsigned char flag2, unsigned char flag3, int num_dv_frames, int num_dv_silent_frames, int num_bit_errors)
{
	bool result = true;

	for (unsigned int i = 0; i < m_clientCount; i++) {
		result = m_clients[i]->sendHeardWithTXStats(myCall, myCallExt, yourCall, rpt1, rpt2, flag1, flag2, flag3, num_dv_frames, num_dv_silent_frames, num_bit_errors) && result;
	}

	return result;
}

bool CIRCDDBMultiClient::findGateway(const wxString & gatewayCallsign)
{
	pushQuery(m_gatewayQueries, gatewayCallsign, new CIRCDDBMultiClientEntry(wxEmptyString, wxEmptyString, gatewayCallsign, wxEmptyString, wxEmptyString, IDRT_GATEWAY));
	bool result = true;
	for (unsigned int i = 0; i < m_clientCount; i++) {
		result = m_clients[i]->findGateway(gatewayCallsign) && result;
	}

	return result;
}

bool CIRCDDBMultiClient::findRepeater(const wxString & repeaterCallsign)
{
	pushQuery(m_repeaterQueries, repeaterCallsign, new CIRCDDBMultiClientEntry(wxEmptyString, repeaterCallsign, wxEmptyString, wxEmptyString, wxEmptyString, IDRT_REPEATER));
	bool result = true;
	for (unsigned int i = 0; i < m_clientCount; i++) {
		result = m_clients[i]->findRepeater(repeaterCallsign) && result;
	}

	return result;
}

bool CIRCDDBMultiClient::findUser(const wxString & userCallsign)
{
	pushQuery(m_userQueries, userCallsign, new CIRCDDBMultiClientEntry(userCallsign, wxEmptyString, wxEmptyString, wxEmptyString, wxEmptyString, IDRT_USER));
	bool result = true;
	for (unsigned int i = 0; i < m_clientCount; i++) {
		result = m_clients[i]->findUser(userCallsign) && result;
	}

	return result;
}

IRCDDB_RESPONSE_TYPE CIRCDDBMultiClient::getMessageType()
{
	//procees the inner clients at each call
	for (unsigned int i = 0; i < m_clientCount; i++) {
		IRCDDB_RESPONSE_TYPE type = m_clients[i]->getMessageType();

		switch (type) {
			case IDRT_USER: {
				wxString user, repeater, gateway, address, timestamp;
				if (!m_clients[i]->receiveUser(user, repeater, gateway, address, timestamp))
					break;

				wxMutexLocker locker(m_queriesLock);
				bool canAddToQueue = false;
				bool wasQuery = false;
				CIRCDDBMultiClientEntry * item = popQuery(m_userQueries, user);
				if (item != NULL) {//is this a response to a query we've sent ?
					item->Update(user, repeater, gateway, address, timestamp);//update item (if needed)
					canAddToQueue = (item->incrementResponseCount() >= m_clientCount); //did all the clients respond ?
					wasQuery = true;
				}
				else {
					item = new CIRCDDBMultiClientEntry(user, repeater, gateway, address, timestamp, type);
					canAddToQueue = true;
				}
				
				if(canAddToQueue){
					wxMutexLocker locker(m_responseQueueLock);
					m_responseQueue.Add(item);
				}
				else if(wasQuery)
					pushQuery(m_userQueries, user, item);
				break;
			}
			case IDRT_GATEWAY: {
				wxString gateway, address;
				if (!m_clients[i]->receiveGateway(gateway, address))
					break;

				wxMutexLocker locker(m_queriesLock);
				bool canAddToQueue = false;
				bool wasQuery = false;
				CIRCDDBMultiClientEntry * item = popQuery(m_gatewayQueries, gateway);
				if (item != NULL) {//is this a response to a query we've sent ?
					item->Update(wxEmptyString, wxEmptyString, gateway, address, wxEmptyString);//update item (if needed)
					canAddToQueue = (item->incrementResponseCount() >= m_clientCount); //did all the clients respond ?
					wasQuery = true;
				}
				else {
					item = new CIRCDDBMultiClientEntry(wxEmptyString, wxEmptyString, gateway, address, wxEmptyString, type);
					canAddToQueue = true;
				}

				if (canAddToQueue) {
					wxMutexLocker locker(m_responseQueueLock);
					m_responseQueue.Add(item);
				}
				else if (wasQuery)
					pushQuery(m_gatewayQueries, gateway, item);
				break;
			}
			case IDRT_REPEATER: {
				wxString repeater, gateway, address;
				if (!m_clients[i]->receiveRepeater(repeater, gateway, address))
					break;

				wxMutexLocker locker(m_queriesLock);
				bool canAddToQueue = false;
				bool wasQuery = false;
				CIRCDDBMultiClientEntry * item = popQuery(m_repeaterQueries, repeater);
				if (item != NULL) {//is this a response to a query we've sent ?
					item->Update(wxEmptyString, repeater, gateway, address, wxEmptyString);//update item (if needed)
					canAddToQueue = (item->incrementResponseCount() >= m_clientCount); //did all the clients respond ?
					wasQuery = true;
				}
				else {
					item = new CIRCDDBMultiClientEntry(wxEmptyString, repeater, gateway, address, wxEmptyString, type);
					canAddToQueue = true;
				}

				if (canAddToQueue) {
					wxMutexLocker locker(m_responseQueueLock);
					m_responseQueue.Add(item);
				}
				else if (wasQuery)
					pushQuery(m_repeaterQueries, repeater, item);
				break;
			}
			case IDRT_NONE: {
			default:
				break;
			}
		}
	}

	wxMutexLocker locker(m_responseQueueLock);

	if (m_responseQueue.Count() == 0)
		return IDRT_NONE;

	return m_responseQueue[0]->getType();
}

bool CIRCDDBMultiClient::receiveRepeater(wxString & repeaterCallsign, wxString & gatewayCallsign, wxString & address)
{
	CIRCDDBMultiClientEntry * item = checkAndGetNextResponse(IDRT_REPEATER, wxT("CIRCDDBMultiClient::receiveRepeater: unexpected response type"));
	if (item == NULL)
		return false;

	repeaterCallsign = item->getRepeater();
	gatewayCallsign = item->getGateway();
	address = item->getAddress();
	delete item;
	return true;
}

bool CIRCDDBMultiClient::receiveGateway(wxString & gatewayCallsign, wxString & address)
{
	CIRCDDBMultiClientEntry * item = checkAndGetNextResponse(IDRT_GATEWAY, wxT("CIRCDDBMultiClient::receiveGateway: unexpected response type"));
	if (item == NULL)
		return false;

	gatewayCallsign = item->getGateway();
	address = item->getAddress();
	delete item;
	return true;
}

bool CIRCDDBMultiClient::receiveUser(wxString & userCallsign, wxString & repeaterCallsign, wxString & gatewayCallsign, wxString & address)
{
	wxString dummy;
	return receiveUser(userCallsign, repeaterCallsign, gatewayCallsign, address, dummy);
}

bool CIRCDDBMultiClient::receiveUser(wxString & userCallsign, wxString & repeaterCallsign, wxString & gatewayCallsign, wxString & address, wxString & timeStamp)
{
	CIRCDDBMultiClientEntry * item = checkAndGetNextResponse(IDRT_USER, wxT("CIRCDDBMultiClient::receiveUser: unexpected response type"));
	if (item == NULL)
		return false;

	userCallsign = item->getUser();
	repeaterCallsign = item->getRepeater();
	gatewayCallsign = item->getGateway();
	address = item->getAddress();
	timeStamp = item->getTimestamp();
	delete item;
	return true;
}

void CIRCDDBMultiClient::close()
{
	for (unsigned int i = 0; i < m_clientCount; i++) {
		m_clients[i]->close();
	}
}

CIRCDDBMultiClientEntry * CIRCDDBMultiClient::checkAndGetNextResponse(IRCDDB_RESPONSE_TYPE expectedType, wxString errorMessage)
{
	CIRCDDBMultiClientEntry * item = NULL;
	wxMutexLocker locker(m_responseQueueLock);

	if (m_responseQueue.Count() == 0 || m_responseQueue[0]->getType() != IDRT_REPEATER)	{
		wxLogError(errorMessage);
	}
	else {
		item = m_responseQueue[0];
		m_responseQueue.RemoveAt(0);
	}

	return item;
}

void CIRCDDBMultiClient::pushQuery(CIRCDDBMultiClientEntry_HashMap queries, const wxString& key, CIRCDDBMultiClientEntry * query)
{
	wxMutexLocker locker(m_queriesLock);
	if (m_userQueries[key] == NULL)
		m_userQueries[key] = query;
	else
		delete query;
}


CIRCDDBMultiClientEntry * CIRCDDBMultiClient::popQuery(CIRCDDBMultiClientEntry_HashMap queries, const wxString & key)
{
	CIRCDDBMultiClientEntry * item = queries[key];

	if (item != NULL)
		queries.erase(key);
	
	return item;
}


