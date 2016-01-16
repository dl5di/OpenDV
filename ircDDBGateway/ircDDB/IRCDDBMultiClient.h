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
#include <wx/thread.h>

class CIRCDDBMultiClient : public CIRCDDB
{
public:
	CIRCDDBMultiClient(CIRCDDB * * clients, unsigned int count);
	~CIRCDDBMultiClient();

	// Inherited via CIRCDDB
	virtual bool open() override;
	virtual void rptrQTH(const wxString & callsign, double latitude, double longitude, const wxString & desc1, const wxString & desc2, const wxString & infoURL) override;
	virtual void rptrQRG(const wxString & callsign, double txFrequency, double duplexShift, double range, double agl) override;
	virtual void kickWatchdog(const wxString & callsign, const wxString & wdInfo) override;
	virtual int getConnectionState() override;
	virtual bool sendHeard(const wxString & myCall, const wxString & myCallExt, const wxString & yourCall, const wxString & rpt1, const wxString & rpt2, unsigned char flag1, unsigned char flag2, unsigned char flag3) override;
	virtual bool sendHeardWithTXMsg(const wxString & myCall, const wxString & myCallExt, const wxString & yourCall, const wxString & rpt1, const wxString & rpt2, unsigned char flag1, unsigned char flag2, unsigned char flag3, const wxString & network_destination, const wxString & tx_message) override;
	virtual bool sendHeardWithTXStats(const wxString & myCall, const wxString & myCallExt, const wxString & yourCall, const wxString & rpt1, const wxString & rpt2, unsigned char flag1, unsigned char flag2, unsigned char flag3, int num_dv_frames, int num_dv_silent_frames, int num_bit_errors) override;
	virtual bool findGateway(const wxString & gatewayCallsign) override;
	virtual bool findRepeater(const wxString & repeaterCallsign) override;
	virtual bool findUser(const wxString & userCallsign) override;
	virtual IRCDDB_RESPONSE_TYPE getMessageType() override;
	virtual bool receiveRepeater(wxString & repeaterCallsign, wxString & gatewayCallsign, wxString & address) override;
	virtual bool receiveGateway(wxString & gatewayCallsign, wxString & address) override;
	virtual bool receiveUser(wxString & userCallsign, wxString & repeaterCallsign, wxString & gatewayCallsign, wxString & address) override;
	virtual bool receiveUser(wxString & userCallsign, wxString & repeaterCallsign, wxString & gatewayCallsign, wxString & address, wxString & timeStamp) override;
	virtual void close() override;

private :
	CIRCDDB * * m_clients;
	unsigned int m_clientCount;
	unsigned int m_currentClient;
	wxMutex m_currentClientLock;

	void LockCurrentClient();
	void ReleaseCurrentClient();
};
#endif

