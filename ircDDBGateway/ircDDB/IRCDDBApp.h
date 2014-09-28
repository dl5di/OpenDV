/*

CIRCDDB - ircDDB client library in C++

Copyright (C) 2010-2011	Michael Dirska, DL1BFF (dl1bff@mdx.de)
Copyright (C) 2012,2014	Jonathan Naylor, G4KLX

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

#if !defined(_IRCDDBAPP_H)
#define _IRCDDBAPP_H

#include "IRCMessageQueue.h"
#include "IRCApplication.h"
#include "IRCMessage.h"
#include "Timer.h"
#include "Defs.h"

#include <wx/wx.h>
#include <wx/regex.h>
#include <wx/datetime.h>

class IRCDDBAppUserObject
{
public:
	IRCDDBAppUserObject(const wxString& n, const wxString& nm, const wxString& h);
	IRCDDBAppUserObject();

	wxString     m_nick;
	wxString     m_name;
	wxString     m_host;
	bool         m_op;
	unsigned int m_usn;

	static unsigned int counter;
};

class IRCDDBAppRptrObject
{
public:
	IRCDDBAppRptrObject(wxDateTime& dt, wxString& repeaterCallsign, wxString& gatewayCallsign);
	IRCDDBAppRptrObject();

	wxString   m_arearp_cs;
	wxDateTime m_lastChanged;
	wxString   m_zonerp_cs;

	static wxDateTime maxTime;
}; 

WX_DECLARE_STRING_HASH_MAP(IRCDDBAppUserObject, IRCDDBAppUserMap);
WX_DECLARE_STRING_HASH_MAP(IRCDDBAppRptrObject, IRCDDBAppRptrMap);


class IRCDDBApp : public IRCApplication, wxThread
{
public:
	IRCDDBApp(const wxString& update_channel);
	virtual ~IRCDDBApp();

	virtual void userJoin(const wxString& nick, const wxString& name, const wxString& host);

	virtual void userLeave(const wxString& nick);

	virtual void userChanOp(const wxString& nick, bool op);
	virtual void userListReset();

	virtual void msgChannel(IRCMessage* m);
	virtual void msgQuery(IRCMessage* m);

	virtual void setCurrentNick(const wxString& nick);
	virtual void setTopic(const wxString& topic);

	virtual void setBestServer(const wxString& ircUser);

	virtual void setSendQ(IRCMessageQueue* s);
	virtual IRCMessageQueue* getSendQ();

	void startWork();
	void stopWork();

	IRCDDB_RESPONSE_TYPE getReplyMessageType();

	IRCMessage* getReplyMessage();

	bool findUser(const wxString& s);
	bool findRepeater(const wxString& s);
	bool findGateway(const wxString& s);

	bool sendHeard(const wxString& myCall, const wxString& myCallExt, const wxString& yourCall, const wxString& rpt1, const wxString& rpt2, unsigned char flag1, unsigned char flag2, unsigned char flag3, const wxString& destination, const wxString& tx_msg, const wxString& tx_stats);

	int getConnectionState();

	void rptrQRG(const wxString& callsign, double txFrequency, double duplexShift, double range, double agl);

	void rptrQTH(const wxString& callsign, double latitude, double longitude, const wxString& desc1, const wxString& desc2, const wxString& infoURL);

	void kickWatchdog(const wxString& callsign, const wxString& wdInfo);

	virtual wxThread::ExitCode Entry();

private:
	IRCMessageQueue*   m_sendQ;

	IRCDDBAppUserMap   m_user;
	wxMutex            m_userMapMutex;

	wxString           m_currentServer;
	wxString           m_myNick;

	wxRegEx            m_tablePattern;
	wxRegEx            m_datePattern;
	wxRegEx            m_timePattern;
	wxRegEx            m_dbPattern;

	int                m_state;
	CTimer             m_timer;

	wxString           m_updateChannel;
	wxString           m_channelTopic;
	wxString           m_bestServer;

	bool               m_initReady;

	bool               m_stopped;

	IRCDDBAppRptrMap   m_rptrMap;
	wxMutex            m_rptrMapMutex;

	IRCMessageQueue    m_replyQ;

	wxArrayString      m_moduleData;
	wxMutex            m_moduleDataMutex;
	CTimer             m_moduleDataTimer;

	wxArrayString      m_moduleWD;
	wxMutex            m_moduleWDMutex;
	CTimer             m_moduleWDTimer;

	void     doUpdate(wxString& msg);
	void     doNotFound(wxString& msg, wxString& retval);
	wxString getIPAddress(wxString& zonerp_cs);
	bool     findServerUser();
	bool     needsDatabaseUpdate(long tableID);
	wxString getTableIDString(long tableID, bool spaceBeforeNumber);
	wxString getLastEntryTime(long tableID);
};

#endif
