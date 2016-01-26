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

CIRCDDBMultiClient::CIRCDDBMultiClient(const CIRCDDB_Array& clients) :
m_clients(),
m_queriesLock(),
m_responseQueueLock()
{
	for (unsigned int i = 0; i < clients.Count(); i++)	{
		if (clients[i] != NULL)
			m_clients.Add(clients[i]);
	}
}

CIRCDDBMultiClient::~CIRCDDBMultiClient()
{
	for (unsigned int i = 0; i < m_clients.Count(); i++) {
		delete m_clients[i];
	}

	while (m_responseQueue.Count() > 0) {
		delete m_responseQueue[0];
		m_responseQueue.RemoveAt(0);
	}

	for (CIRCDDBMultiClientQuery_HashMap::iterator it = m_userQueries.begin(); it != m_userQueries.end(); it++)
		delete it->second;
	m_userQueries.clear();

	for (CIRCDDBMultiClientQuery_HashMap::iterator it = m_repeaterQueries.begin(); it != m_repeaterQueries.end(); it++)
		delete it->second;
	m_repeaterQueries.clear();

	for (CIRCDDBMultiClientQuery_HashMap::iterator it = m_gatewayQueries.begin(); it != m_gatewayQueries.end(); it++)
		delete it->second;
	m_gatewayQueries.clear();
}

bool CIRCDDBMultiClient::open()
{
	bool result = true;

	for (unsigned int i = 0; i < m_clients.Count(); i++) {
		result = m_clients[i]->open() && result;
	}

	if (!result) close();

	return result;
}

void CIRCDDBMultiClient::rptrQTH(const wxString & callsign, double latitude, double longitude, const wxString & desc1, const wxString & desc2, const wxString & infoURL)
{
	for (unsigned int i = 0; i < m_clients.Count(); i++) {
		m_clients[i]->rptrQTH(callsign, latitude, longitude, desc1, desc2, infoURL);
	}
}

void CIRCDDBMultiClient::rptrQRG(const wxString & callsign, double txFrequency, double duplexShift, double range, double agl)
{
	for (unsigned int i = 0; i < m_clients.Count(); i++) {
		m_clients[i]->rptrQRG(callsign, txFrequency, duplexShift, range, agl);
	}
}

void CIRCDDBMultiClient::kickWatchdog(const wxString & callsign, const wxString & wdInfo)
{
	for (unsigned int i = 0; i < m_clients.Count(); i++) {
		m_clients[i]->kickWatchdog(callsign, wdInfo);
	}
}

int CIRCDDBMultiClient::getConnectionState()
{
	for (unsigned int i = 0; i < m_clients.Count(); i++) {
		int state = m_clients[i]->getConnectionState();
		if (state != 7)
			return state;
	}

	return 7;
}

bool CIRCDDBMultiClient::sendHeard(const wxString & myCall, const wxString & myCallExt, const wxString & yourCall, const wxString & rpt1, const wxString & rpt2, unsigned char flag1, unsigned char flag2, unsigned char flag3)
{
	bool result = true;

	for (unsigned int i = 0; i < m_clients.Count(); i++) {
		result = m_clients[i]->sendHeard(myCall, myCallExt, yourCall, rpt1, rpt2, flag1, flag2, flag3) && result;
	}

	return result;
}

bool CIRCDDBMultiClient::sendHeardWithTXMsg(const wxString & myCall, const wxString & myCallExt, const wxString & yourCall, const wxString & rpt1, const wxString & rpt2, unsigned char flag1, unsigned char flag2, unsigned char flag3, const wxString & network_destination, const wxString & tx_message)
{
	bool result = true;

	for (unsigned int i = 0; i < m_clients.Count(); i++) {
		result = m_clients[i]->sendHeardWithTXMsg(myCall, myCallExt, yourCall, rpt1, rpt2, flag1, flag2, flag3, network_destination, tx_message) && result;
	}

	return result;
}

bool CIRCDDBMultiClient::sendHeardWithTXStats(const wxString & myCall, const wxString & myCallExt, const wxString & yourCall, const wxString & rpt1, const wxString & rpt2, unsigned char flag1, unsigned char flag2, unsigned char flag3, int num_dv_frames, int num_dv_silent_frames, int num_bit_errors)
{
	bool result = true;

	for (unsigned int i = 0; i < m_clients.Count(); i++) {
		result = m_clients[i]->sendHeardWithTXStats(myCall, myCallExt, yourCall, rpt1, rpt2, flag1, flag2, flag3, num_dv_frames, num_dv_silent_frames, num_bit_errors) && result;
	}

	return result;
}

bool CIRCDDBMultiClient::findGateway(const wxString & gatewayCallsign)
{
	pushQuery(IDRT_GATEWAY, gatewayCallsign, new CIRCDDBMultiClientQuery(wxEmptyString, wxEmptyString, gatewayCallsign, wxEmptyString, wxEmptyString, IDRT_GATEWAY));
	bool result = true;
	for (unsigned int i = 0; i < m_clients.Count(); i++) {
		result = m_clients[i]->findGateway(gatewayCallsign) && result;
	}

	return result;
}

bool CIRCDDBMultiClient::findRepeater(const wxString & repeaterCallsign)
{
	pushQuery(IDRT_REPEATER, repeaterCallsign, new CIRCDDBMultiClientQuery(wxEmptyString, repeaterCallsign, wxEmptyString, wxEmptyString, wxEmptyString, IDRT_REPEATER));
	bool result = true;
	for (unsigned int i = 0; i < m_clients.Count(); i++) {
		result = m_clients[i]->findRepeater(repeaterCallsign) && result;
	}

	return result;
}

bool CIRCDDBMultiClient::findUser(const wxString & userCallsign)
{
	pushQuery(IDRT_USER, userCallsign, new CIRCDDBMultiClientQuery(userCallsign, wxEmptyString, wxEmptyString, wxEmptyString, wxEmptyString, IDRT_USER));
	bool result = true;
	for (unsigned int i = 0; i < m_clients.Count(); i++) {
		result = m_clients[i]->findUser(userCallsign) && result;
	}

	return result;
}

IRCDDB_RESPONSE_TYPE CIRCDDBMultiClient::getMessageType()
{
	//procees the inner clients at each call
	for (unsigned int i = 0; i < m_clients.Count(); i++) {
		wxString user = wxEmptyString, repeater = wxEmptyString, gateway = wxEmptyString, address = wxEmptyString, timestamp = wxEmptyString, key = wxEmptyString;
		IRCDDB_RESPONSE_TYPE type = m_clients[i]->getMessageType();

		switch (type) {
			case IDRT_USER: {
				if (!m_clients[i]->receiveUser(user, repeater, gateway, address, timestamp))
					type = IDRT_NONE;
				key = user;
				//wxLogMessage(wxT("After receive user : %s %s %s %s %s client idx %d"), user, repeater, gateway, address, timestamp, i);
				break;
			}
			case IDRT_GATEWAY: {
				if (!m_clients[i]->receiveGateway(gateway, address))
					type = IDRT_NONE;
				key = gateway;
				break;
			}
			case IDRT_REPEATER: {
				if (!m_clients[i]->receiveRepeater(repeater, gateway, address))
					type = IDRT_NONE;
				key = repeater;
				break;
			}
			case IDRT_NONE: {
			default:
				break;
			}
		}

		if (type != IDRT_NONE)
		{
			wxMutexLocker locker(m_queriesLock);
			bool canAddToQueue = false;
			bool wasQuery = false;
			CIRCDDBMultiClientQuery * item = popQuery(type, key);
			if (item != NULL) {//is this a response to a query we've sent ?
				item->Update(user, repeater, gateway, address, timestamp);//update item (if needed)
				canAddToQueue = (item->incrementResponseCount() >= m_clients.Count()); //did all the clients respond or did we have an answer ?
				wasQuery = true;
			}
			else {
				item = new CIRCDDBMultiClientQuery(user, repeater, gateway, address, timestamp, type);
				canAddToQueue = true;
			}

			//wxLogMessage(wxT("After process : %s %s %s %s %s canAdd %d wasQuery %d"), user, repeater, gateway, address, timestamp, (int)canAddToQueue, (int)wasQuery);
			if (canAddToQueue) {
				wxMutexLocker responselocker(m_responseQueueLock);
				m_responseQueue.Add(item);
			}
			else if (wasQuery)
				pushQuery(type, key, item);
		}
	}

	{   //this is an artificial scope, I'm not sure if compiler optimization would move instantiation of locker or not hence this
		//finally send the first item we queued
		wxMutexLocker locker(m_responseQueueLock);
		if (m_responseQueue.Count() == 0)
			return IDRT_NONE;

		return m_responseQueue[0]->getType();
	}
}

bool CIRCDDBMultiClient::receiveRepeater(wxString & repeaterCallsign, wxString & gatewayCallsign, wxString & address)
{
	CIRCDDBMultiClientQuery * item = checkAndGetNextResponse(IDRT_REPEATER, wxT("CIRCDDBMultiClient::receiveRepeater: unexpected response type"));
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
	CIRCDDBMultiClientQuery * item = checkAndGetNextResponse(IDRT_GATEWAY, wxT("CIRCDDBMultiClient::receiveGateway: unexpected response type"));
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
	CIRCDDBMultiClientQuery * item = checkAndGetNextResponse(IDRT_USER, wxT("CIRCDDBMultiClient::receiveUser: unexpected response type"));
	if (item == NULL) {
		//wxLogMessage(wxT("CIRCDDBMultiClient::receiveUser NO USER IN QUEUE"));
		return false;
	}

	//wxLogMessage(wxT("CIRCDDBMultiClient::receiveUser : %s"), item->toString());

	userCallsign = item->getUser().Clone();
	repeaterCallsign = item->getRepeater().Clone();
	gatewayCallsign = item->getGateway().Clone();
	address = item->getAddress().Clone();
	timeStamp = item->getTimestamp().Clone();
	delete item;
	return true;
}

void CIRCDDBMultiClient::close()
{
	for (unsigned int i = 0; i < m_clients.Count(); i++) {
		m_clients[i]->close();
	}
}

CIRCDDBMultiClientQuery * CIRCDDBMultiClient::checkAndGetNextResponse(IRCDDB_RESPONSE_TYPE expectedType, wxString errorMessage)
{
	CIRCDDBMultiClientQuery * item = NULL;
	wxMutexLocker locker(m_responseQueueLock);

	if (m_responseQueue.Count() == 0 || m_responseQueue[0]->getType() != expectedType)	{
		wxLogError(errorMessage);
	}
	else {
		item = m_responseQueue[0];
		m_responseQueue.RemoveAt(0);
	}

	return item;
}

void CIRCDDBMultiClient::pushQuery(IRCDDB_RESPONSE_TYPE type, const wxString& key, CIRCDDBMultiClientQuery * query)
{
	CIRCDDBMultiClientQuery_HashMap * queries = getQueriesHashMap(type);
	wxMutexLocker locker(m_queriesLock);
	if (queries != NULL && (*queries)[key] == NULL)
		(*queries)[key] = query;
	else
		delete query;
}


CIRCDDBMultiClientQuery * CIRCDDBMultiClient::popQuery(IRCDDB_RESPONSE_TYPE type, const wxString & key)
{
	CIRCDDBMultiClientQuery_HashMap * queries = getQueriesHashMap(type);
	wxMutexLocker locker(m_queriesLock);

	CIRCDDBMultiClientQuery * item = NULL;

	if (queries != NULL && (item = (*queries)[key]) != NULL)
		queries->erase(key);
	
	return item;
}

CIRCDDBMultiClientQuery_HashMap * CIRCDDBMultiClient::getQueriesHashMap(IRCDDB_RESPONSE_TYPE type)
{
	switch (type)
	{
	case IDRT_USER:
		return &m_userQueries;
	case IDRT_GATEWAY:
		return &m_gatewayQueries;
	case IDRT_REPEATER:
		return &m_repeaterQueries;
	case IDRT_NONE:
	default:
		return NULL;
	}
}


