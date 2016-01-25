/*

CIRCDDB - ircDDB client library in C++

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


#if !defined(_IRCDDBMULTICLIENT_H)
#define _IRCDDBMULTICLIENT_H

#include "IRCDDB.h"
#include <wx/wx.h>
#include <wx/thread.h>

//Small data container to keep track of queries with sent to the inner clients
class CIRCDDBMultiClientQuery
{
public:
	CIRCDDBMultiClientQuery(const wxString& user,
							const wxString& repeater,
							const wxString& gateway,
							const wxString& address,
							const wxString& timestamp,
							IRCDDB_RESPONSE_TYPE type) :
		m_user(user.Clone()),
		m_repeater(repeater.Clone()),
		m_gateway(gateway.Clone()),
		m_address(address.Clone()),
		m_timestamp(timestamp.Clone()),
		m_type(type),
		m_responseCount(0)
	{

	}

	wxString getUser() const
	{
		return m_user;
	}

	wxString getRepeater() const
	{
		return m_repeater;
	}

	wxString getGateway() const
	{
		return m_gateway;
	}

	wxString getAddress() const
	{
		return m_address;
	}

	wxString getTimestamp() const
	{
		return m_timestamp;
	}

	unsigned int getResponseCount()
	{
		return m_responseCount;
	}

	unsigned int incrementResponseCount()
	{
		++m_responseCount;
		//wxLogMessage(wxT("Resp Count : %s %d"), toString(), m_responseCount);
		return m_responseCount;
	}

	/*
		Updates the entry, but only if the timestamp is newer. if an address was already specified it is kept.
	*/
	void Update(const wxString& user, const wxString& repeater, const wxString& gateway, const wxString& address, const wxString& timestamp)
	{
		//wxLogMessage(wxT("Before : %s"), toString());
		if (timestamp.IsEmpty() || timestamp.Cmp(m_timestamp) >= 0) {
			m_user = user.Clone();
			m_repeater = repeater.Clone();
			m_gateway = gateway.Clone();
			m_timestamp = timestamp.Clone();

			if(m_address.IsEmpty() && !address.IsEmpty())
				m_address = address.Clone();
		}
		//wxLogMessage(wxT("After : %s"), toString());
	}

	IRCDDB_RESPONSE_TYPE getType()
	{
		return m_type;
	}

	wxString toString()
	{
		return wxString::Format(wxT("%s %s %s %s %s"), m_user, m_repeater, m_gateway, m_address, m_timestamp);
	}

private:
	wxString m_user;
	wxString m_repeater;
	wxString m_gateway;
	wxString m_address;
	wxString m_timestamp;
	IRCDDB_RESPONSE_TYPE m_type;
	unsigned int m_responseCount;
};

WX_DECLARE_STRING_HASH_MAP(CIRCDDBMultiClientQuery*, CIRCDDBMultiClientQuery_HashMap);
WX_DEFINE_ARRAY_PTR(CIRCDDBMultiClientQuery*, CIRCDDBMultiClientQuery_Array);

class CIRCDDBMultiClient : public CIRCDDB
{
public:
	CIRCDDBMultiClient(const CIRCDDB_Array& clients);
	~CIRCDDBMultiClient();

	// Inherited via CIRCDDB
	virtual bool open();
	virtual void rptrQTH(const wxString & callsign, double latitude, double longitude, const wxString & desc1, const wxString & desc2, const wxString & infoURL);
	virtual void rptrQRG(const wxString & callsign, double txFrequency, double duplexShift, double range, double agl);
	virtual void kickWatchdog(const wxString & callsign, const wxString & wdInfo);
	virtual int getConnectionState() ;
	virtual bool sendHeard(const wxString & myCall, const wxString & myCallExt, const wxString & yourCall, const wxString & rpt1, const wxString & rpt2, unsigned char flag1, unsigned char flag2, unsigned char flag3);
	virtual bool sendHeardWithTXMsg(const wxString & myCall, const wxString & myCallExt, const wxString & yourCall, const wxString & rpt1, const wxString & rpt2, unsigned char flag1, unsigned char flag2, unsigned char flag3, const wxString & network_destination, const wxString & tx_message);
	virtual bool sendHeardWithTXStats(const wxString & myCall, const wxString & myCallExt, const wxString & yourCall, const wxString & rpt1, const wxString & rpt2, unsigned char flag1, unsigned char flag2, unsigned char flag3, int num_dv_frames, int num_dv_silent_frames, int num_bit_errors);
	virtual bool findGateway(const wxString & gatewayCallsign);
	virtual bool findRepeater(const wxString & repeaterCallsign);
	virtual bool findUser(const wxString & userCallsign);
	virtual IRCDDB_RESPONSE_TYPE getMessageType();
	virtual bool receiveRepeater(wxString & repeaterCallsign, wxString & gatewayCallsign, wxString & address);
	virtual bool receiveGateway(wxString & gatewayCallsign, wxString & address);
	virtual bool receiveUser(wxString & userCallsign, wxString & repeaterCallsign, wxString & gatewayCallsign, wxString & address);
	virtual bool receiveUser(wxString & userCallsign, wxString & repeaterCallsign, wxString & gatewayCallsign, wxString & address, wxString & timeStamp);
	virtual void close();

	//

private :
	CIRCDDB_Array m_clients;
	wxMutex m_queriesLock, m_responseQueueLock;

	CIRCDDBMultiClientQuery_HashMap m_userQueries;
	CIRCDDBMultiClientQuery_HashMap m_repeaterQueries;
	CIRCDDBMultiClientQuery_HashMap m_gatewayQueries;
	CIRCDDBMultiClientQuery_Array m_responseQueue;

	CIRCDDBMultiClientQuery * checkAndGetNextResponse(IRCDDB_RESPONSE_TYPE expectedType, wxString errorMessage);
	void pushQuery(IRCDDB_RESPONSE_TYPE type, const wxString& key,  CIRCDDBMultiClientQuery * query);
	CIRCDDBMultiClientQuery * popQuery(IRCDDB_RESPONSE_TYPE type, const wxString& key);
	CIRCDDBMultiClientQuery_HashMap * getQueriesHashMap(IRCDDB_RESPONSE_TYPE type);
};
#endif

