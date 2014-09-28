/*

CIRCDDB - ircDDB client library in C++

Copyright (C) 2010-2011   Michael Dirska, DL1BFF (dl1bff@mdx.de)
Copyright (C) 2012,2014   Jonathan Naylor, G4KLX

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

#if defined(WIN32)
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#endif

#include "IRCDDBApp.h"
#include "IRCutils.h"

#include <wx/tokenzr.h>

IRCDDBAppUserObject::IRCDDBAppUserObject() :
m_nick(),
m_name(),
m_host(),
m_op(false),
m_usn(counter)
{
	counter++;
}

IRCDDBAppUserObject::IRCDDBAppUserObject(const wxString& n, const wxString& nm, const wxString& h) :
m_nick(n),
m_name(nm),
m_host(h),
m_op(false),
m_usn(counter)
{
	counter++;
}

unsigned int IRCDDBAppUserObject::counter = 0U;

IRCDDBAppRptrObject::IRCDDBAppRptrObject() :
m_arearp_cs(),
m_lastChanged(),
m_zonerp_cs()
{
}

IRCDDBAppRptrObject::IRCDDBAppRptrObject(wxDateTime& dt, wxString& repeaterCallsign, wxString& gatewayCallsign) :
m_arearp_cs(repeaterCallsign),
m_lastChanged(dt),
m_zonerp_cs(gatewayCallsign)
{
	if (dt.IsLaterThan(maxTime))
		maxTime = dt;
}

wxDateTime IRCDDBAppRptrObject::maxTime(time_t(950000000));  // February 2000


IRCDDBApp::IRCDDBApp(const wxString& u_chan) :
wxThread(wxTHREAD_JOINABLE),
m_sendQ(NULL),
m_user(),
m_userMapMutex(),
m_currentServer(),
m_myNick(wxT("none")),
m_tablePattern(wxT("^[0-9]$")),
m_datePattern(wxT("^20[0-9][0-9]-((1[0-2])|(0[1-9]))-((3[01])|([12][0-9])|(0[1-9]))$")),
m_timePattern(wxT("^((2[0-3])|([01][0-9])):[0-5][0-9]:[0-5][0-9]$")),
m_dbPattern(wxT("^[0-9A-Z_]{8}$")),
m_state(0),
m_timer(1U),
m_updateChannel(u_chan),
m_channelTopic(),
m_bestServer(),
m_initReady(false),
m_stopped(false),
m_rptrMap(),
m_rptrMapMutex(),
m_replyQ(),
m_moduleData(),
m_moduleDataMutex(),
m_moduleDataTimer(1U),
m_moduleWD(),
m_moduleWDMutex(),
m_moduleWDTimer(1U, 60U)
{
	userListReset();
}

IRCDDBApp::~IRCDDBApp()
{
	delete m_sendQ;
}

void IRCDDBApp::rptrQTH(const wxString& callsign, double latitude, double longitude, const wxString& desc1, const wxString& desc2, const wxString& infoURL)
{
	wxString pos;
	pos.Printf(wxT("%+09.5f %+010.5f"), latitude, longitude);

	wxString cs = callsign;
	wxString d1 = desc1;
	wxString d2 = desc2;

	d1.Append(wxT(' '), 20U);
	d2.Append(wxT(' '), 20U);

	wxRegEx nonValid(wxT("[^a-zA-Z0-9 +&(),./'-]"));
	nonValid.Replace(&d1, wxEmptyString);
	nonValid.Replace(&d2, wxEmptyString);

	pos.Replace(wxT(","), wxT("."));
	d1.Replace(wxT(" "), wxT("_"));
	d2.Replace(wxT(" "), wxT("_"));
	cs.Replace(wxT(" "), wxT("_"));

	wxMutexLocker locker(m_moduleDataMutex);

	wxString moduleQTH = wxT("IRCDDB RPTRQTH: ") + cs + wxT(" ") + pos + wxT(" ") + d1.Mid(0, 20) + wxT(" ") + d2.Mid(0, 20);
	m_moduleData.Add(moduleQTH);

	wxString url = infoURL;

	wxRegEx urlNonValid(wxT("[^[:graph:]]"));
	urlNonValid.Replace(&url, wxEmptyString);

	if (!url.IsEmpty()) {
		wxString moduleURL = wxT("IRCDDB RPTRURL: ") + cs + wxT(" ") + url;
		m_moduleData.Add(moduleURL);
	}

	m_moduleDataTimer.start(5U);	// send info in 5 seconds
}

void IRCDDBApp::rptrQRG(const wxString& callsign, double txFrequency, double duplexShift, double range, double agl)
{
	wxString cs = callsign;
	cs.Replace(wxT(" "), wxT("_"));

	wxString f;
	f.Printf(wxT("%011.5f %+010.5f %06.2f %06.1f"), txFrequency, duplexShift, range / 1609.344, agl);
	f.Replace(wxT(","), wxT("."));

	wxMutexLocker locker(m_moduleDataMutex);

	wxString moduleQRG = wxT("IRCDDB RPTRQRG: ") + cs + wxT(" ") + f;
	m_moduleData.Add(moduleQRG);

	m_moduleDataTimer.start(5U);	// send info in 5 seconds
}

void IRCDDBApp::kickWatchdog(const wxString& callsign, const wxString& s)
{
	wxString text = s;

	wxRegEx nonValid(wxT("[^[:graph:]]"));
	nonValid.Replace(&text, wxEmptyString);

	if (!text.IsEmpty()) {
		wxString cs = callsign;
		cs.Replace(wxT(" "), wxT("_"));

		wxMutexLocker locker(m_moduleWDMutex);

		wxString moduleWD = wxT("IRCDDB RPTRSW: ") + cs + wxT(" ") + text;
		m_moduleWD.Add(moduleWD);

		m_moduleWDTimer.start();
	}
}

int IRCDDBApp::getConnectionState()
{
	return m_state;
}

IRCDDB_RESPONSE_TYPE IRCDDBApp::getReplyMessageType()
{
	IRCMessage* m = m_replyQ.peekFirst();
	if (m == NULL)
		return IDRT_NONE;

	wxString msgType = m->m_command;

	if (msgType.IsSameAs(wxT("IDRT_USER")))
		return IDRT_USER;

	if (msgType.IsSameAs(wxT("IDRT_REPEATER")))
		return IDRT_REPEATER;

	if (msgType.IsSameAs(wxT("IDRT_GATEWAY")))
		return IDRT_GATEWAY;

	wxLogError(wxT("IRCDDBApp::getMessageType: unknown msg type: %s"), msgType.c_str());

	return IDRT_NONE;
}

IRCMessage* IRCDDBApp::getReplyMessage()
{
	return m_replyQ.getMessage();
}

void IRCDDBApp::startWork()
{
	m_stopped = false;

	Create();

	Run();
}

void IRCDDBApp::stopWork()
{
	m_stopped = true;

	Wait();
}


void IRCDDBApp::userJoin(const wxString& nick, const wxString& name, const wxString& host)
{
	wxMutexLocker lock(m_userMapMutex);

	wxString lnick = nick;
	lnick.MakeLower();

	IRCDDBAppUserObject u(lnick, name, host);

	m_user[lnick] = u;

	if (m_initReady) {
		int hyphenPos = nick.Find(wxT('-'));

		if ((hyphenPos >= 4) && (hyphenPos <= 6)) {
			wxString gatewayCallsign = nick.Mid(0, hyphenPos).Upper();

			while (gatewayCallsign.Length() < 7U)
				gatewayCallsign.Append(wxT(' '));

			gatewayCallsign.Append(wxT('G'));

			IRCMessage * m2 = new IRCMessage(wxT("IDRT_GATEWAY"));
			m2->m_params.Add(gatewayCallsign);
			m2->m_params.Add(host);
			m_replyQ.putMessage(m2);
		}
	}
}

void IRCDDBApp::userLeave(const wxString& nick)
{
	wxMutexLocker lock(m_userMapMutex);

	wxString lnick = nick;
	lnick.MakeLower();

	m_user.erase(lnick);

	if (m_currentServer.Len() > 0U) {
		if (m_user.count(m_myNick) != 1)
			return;

		IRCDDBAppUserObject& me = m_user[m_myNick];

		if (!me.m_op) {
			// if I am not op, then look for new server

			if (m_currentServer.IsSameAs(lnick)) {
				// currentServer = null;
				m_state = 2;  // choose new server

				m_timer.start(200U);

				m_initReady = false;
			}
		}
	}
}

void IRCDDBApp::userListReset()
{
	wxMutexLocker lock(m_userMapMutex);

	m_user.clear();
}

void IRCDDBApp::setCurrentNick(const wxString& nick)
{
	m_myNick = nick;
}

void IRCDDBApp::setBestServer(const wxString& ircUser)
{
	m_bestServer = ircUser;
}

void IRCDDBApp::setTopic(const wxString& topic)
{
	m_channelTopic = topic;
}

bool IRCDDBApp::findServerUser()
{
	wxMutexLocker lock(m_userMapMutex);

	bool found = false;

	for (IRCDDBAppUserMap::iterator it = m_user.begin(); it != m_user.end(); ++it) {
		IRCDDBAppUserObject u = it->second;

		if (u.m_nick.StartsWith(wxT("s-")) && u.m_op && !m_myNick.IsSameAs(u.m_nick) && u.m_nick.IsSameAs(m_bestServer)) {
			m_currentServer = u.m_nick;
			found = true;
			break;
		}
	}

	if (found)
		return true;

	if (m_bestServer.Len() == 8U) {
		for (IRCDDBAppUserMap::iterator it = m_user.begin(); it != m_user.end(); ++it) {
			IRCDDBAppUserObject u = it->second;

			if (u.m_nick.StartsWith(m_bestServer.Mid(0U, 7U)) && u.m_op && !m_myNick.IsSameAs(u.m_nick)) {
				m_currentServer = u.m_nick;
				found = true;
				break;
			}
		}
	}

	if (found)
		return true;

	for (IRCDDBAppUserMap::iterator it = m_user.begin(); it != m_user.end(); ++it) {
		IRCDDBAppUserObject u = it->second;

		if (u.m_nick.StartsWith(wxT("s-")) && u.m_op && !m_myNick.IsSameAs(u.m_nick)) {
			m_currentServer = u.m_nick;
			found = true;
			break;
		}
	}

	return found;
}

void IRCDDBApp::userChanOp(const wxString& nick, bool op)
{
	wxMutexLocker locker(m_userMapMutex);

	wxString lnick = nick;
	lnick.MakeLower();

	if (m_user.count(lnick) == 1)
		m_user[lnick].m_op = op;
}

const long NUMBER_OF_TABLES = 2L;

wxString IRCDDBApp::getIPAddress(wxString& zonerp_cs)
{
	wxMutexLocker locker(m_userMapMutex);

	wxString gw = zonerp_cs;

	gw.Replace(wxT("_"), wxT(" "));
	gw.MakeLower();

	unsigned int max_usn = 0U;
	wxString ipAddr;

	for (unsigned int j = 1U; j <= 4U; j++) {
		wxString ircUser = gw.Strip() + wxString::Format(wxT("-%d"), j);

		if (m_user.count(ircUser) == 1) {
			IRCDDBAppUserObject o = m_user[ircUser];

			if (o.m_usn >= max_usn) {
				max_usn = o.m_usn;
				ipAddr  = o.m_host;
			}
		}
	}

	return ipAddr;
}

bool IRCDDBApp::findGateway(const wxString& gwCall)
{
	wxString s = gwCall.Mid(0U, 6U);

	IRCMessage* m2 = new IRCMessage(wxT("IDRT_GATEWAY"));
	m2->m_params.Add(gwCall);
	m2->m_params.Add(getIPAddress(s));
	m_replyQ.putMessage(m2);

	return true;
}

bool IRCDDBApp::findRepeater(const wxString& rptrCall)
{
	wxString arearp_cs = rptrCall;
	arearp_cs.Replace(wxT(" "), wxT("_"));

	wxString zonerp_cs;

	wxMutexLocker locker(m_rptrMapMutex);

	wxString s = wxT("NONE");

	if (m_rptrMap.count(arearp_cs) == 1) {
		IRCDDBAppRptrObject o = m_rptrMap[arearp_cs];
		zonerp_cs = o.m_zonerp_cs;
		zonerp_cs.Replace(wxT("_"), wxT(" "));
		zonerp_cs.SetChar(7U, wxT('G'));
		s = o.m_zonerp_cs;
	}

	IRCMessage* m2 = new IRCMessage(wxT("IDRT_REPEATER"));
	m2->m_params.Add(rptrCall);
	m2->m_params.Add(zonerp_cs);
	m2->m_params.Add(getIPAddress(s));
	m_replyQ.putMessage(m2);

	return true;
}

bool IRCDDBApp::sendHeard(const wxString& myCall, const wxString& myCallExt, const wxString& yourCall, const wxString& rpt1, const wxString& rpt2, unsigned char flag1, unsigned char flag2, unsigned char flag3, const wxString& destination, const wxString& tx_msg, const wxString& tx_stats)
{
	wxString my = myCall;
	wxString myext = myCallExt;
	wxString ur = yourCall;
	wxString r1 = rpt1;
	wxString r2 = rpt2;
	wxString dest = destination;

	wxRegEx nonValid(wxT("[^A-Z0-9/]"));
	wxString underScore = wxT("_");

	nonValid.Replace(&my, underScore);
	nonValid.Replace(&myext, underScore);
	nonValid.Replace(&ur, underScore);
	nonValid.Replace(&r1, underScore);
	nonValid.Replace(&r2, underScore);
	nonValid.Replace(&dest, underScore);

	bool statsMsg = (tx_stats.Len() > 0);

	wxString srv = m_currentServer;
	IRCMessageQueue* q = getSendQ();

	if ((srv.Len() > 0) && (m_state >= 6) && (q != NULL)) {
		wxString cmd = wxT("UPDATE ");

		cmd.Append(IRCUtils::getCurrentTime());

		cmd.Append(wxT(" "));

		cmd.Append(my);
		cmd.Append(wxT(" "));
		cmd.Append(r1);
		cmd.Append(wxT(" "));

		if (!statsMsg)
			cmd.Append(wxT("0 "));

		cmd.Append(r2);
		cmd.Append(wxT(" "));
		cmd.Append(ur);
		cmd.Append(wxT(" "));

		wxString flags = wxString::Format(wxT("%02X %02X %02X"), flag1, flag2, flag3);

		cmd.Append(flags);
		cmd.Append(wxT(" "));
		cmd.Append(myext);

		if (statsMsg) {
			cmd.Append(wxT(" # "));
			cmd.Append(tx_stats);
		} else {
			cmd.Append(wxT(" 00 "));
			cmd.Append(dest);

			if (tx_msg.Len() == 20U) {
				cmd.Append(wxT(" "));
				cmd.Append(tx_msg);
			}
		}

		IRCMessage* m = new IRCMessage(srv, cmd);

		q->putMessage(m);

		return true;
	} else {
		return false;
	}
}

bool IRCDDBApp::findUser(const wxString& usrCall)
{
	wxString srv = m_currentServer;
	IRCMessageQueue* q = getSendQ();

	if ((srv.Len() > 0) && (m_state >= 6) && (q != NULL)) {
		wxString usr = usrCall;

		usr.Replace(wxT(" "), wxT("_"));

		IRCMessage* m = new IRCMessage(srv, wxT("FIND ") + usr);

		q->putMessage(m);
	} else {
		IRCMessage* m2 = new IRCMessage(wxT("IDRT_USER"));
		m2->m_params.Add(usrCall);
		m2->m_params.Add(wxEmptyString);
		m2->m_params.Add(wxEmptyString);
		m2->m_params.Add(wxEmptyString);
		m2->m_params.Add(wxEmptyString);
		m_replyQ.putMessage(m2);
	}

	return true;
}

void IRCDDBApp::msgChannel(IRCMessage* m)
{
	wxASSERT(m != NULL);

	if (m->getPrefixNick().StartsWith(wxT("s-")) && (m->m_params.GetCount() >= 2U))  // server msg
		doUpdate(m->m_params.Item(1));
}

void IRCDDBApp::doNotFound(wxString& msg, wxString& retval)
{
	long tableID = 0L;

	wxStringTokenizer tkz(msg);

	if (!tkz.HasMoreTokens()) 
		return;  // no text in message

	wxString tk = tkz.GetNextToken();

	if (m_tablePattern.Matches(tk)) {
		long i;
		if (tk.ToLong(&i)) {
			tableID = i;
			if (tableID < 0L || tableID >= NUMBER_OF_TABLES)
				return;
		} else {
			return; // not a valid number
		}

		if (!tkz.HasMoreTokens()) 
			return;  // received nothing but the tableID

		tk = tkz.GetNextToken();
	}

	if (tableID == 0L) {
		if (!m_dbPattern.Matches(tk))
			return; // no valid key

		retval = tk;
	}
}

void IRCDDBApp::doUpdate(wxString& msg)
{
	long tableID = 0L;

	wxStringTokenizer tkz(msg);

	if (!tkz.HasMoreTokens())
		return;  // no text in message

	wxString tk = tkz.GetNextToken();

	if (m_tablePattern.Matches(tk)) {
		long i;
		if (tk.ToLong(&i)) {
			tableID = i;

			if (tableID < 0L || tableID >= NUMBER_OF_TABLES)
				return;
		} else {
			return; // not a valid number
		}

		if (!tkz.HasMoreTokens())
			return;  // received nothing but the tableID

		tk = tkz.GetNextToken();
	}

	if (m_datePattern.Matches(tk)) {
		if (!tkz.HasMoreTokens()) 
			return;  // nothing after date string

		wxString timeToken = tkz.GetNextToken();

		if (!m_timePattern.Matches(timeToken))
			return; // no time string after date string

		wxDateTime dt;
		if (dt.ParseFormat(tk + wxT(" ") + timeToken, wxT("%Y-%m-%d %H:%M:%S")) == NULL)
			return; // date+time parsing failed

		if (tableID == 0L || tableID == 1L) {
			if (!tkz.HasMoreTokens())
				return;  // nothing after time string

			wxString key = tkz.GetNextToken();

			if (!m_dbPattern.Matches(key))
				return; // no valid key

			if (!tkz.HasMoreTokens())
				return;  // nothing after time string

			wxString value = tkz.GetNextToken();

			if (!m_dbPattern.Matches(value))
				return; // no valid key

			if (tableID == 1L) {
				wxMutexLocker lock(m_rptrMapMutex);

				IRCDDBAppRptrObject newRptr(dt, key, value);

				m_rptrMap[key] = newRptr;

				if (m_initReady) {
					wxString arearp_cs = key;
					wxString zonerp_cs = value;

					arearp_cs.Replace(wxT("_"), wxT(" "));
					zonerp_cs.Replace(wxT("_"), wxT(" "));
					zonerp_cs.SetChar(7, wxT('G'));

					IRCMessage* m2 = new IRCMessage(wxT("IDRT_REPEATER"));
					m2->m_params.Add(arearp_cs);
					m2->m_params.Add(zonerp_cs);
					m2->m_params.Add(getIPAddress(value));
					m_replyQ.putMessage(m2);
				}
			} else if (tableID == 0L && m_initReady) {
				wxMutexLocker lock(m_rptrMapMutex);

				wxString userCallsign = key;
				wxString arearp_cs = value;
				wxString zonerp_cs;
				wxString ip_addr;

				userCallsign.Replace(wxT("_"), wxT(" "));
				arearp_cs.Replace(wxT("_"), wxT(" "));

				if (m_rptrMap.count(value) == 1) {
					IRCDDBAppRptrObject o = m_rptrMap[value];
					zonerp_cs = o.m_zonerp_cs;
					zonerp_cs.Replace(wxT("_"), wxT(" "));
					zonerp_cs.SetChar(7U, wxT('G'));

					ip_addr = getIPAddress(o.m_zonerp_cs);
				}

				IRCMessage* m2 = new IRCMessage(wxT("IDRT_USER"));
				m2->m_params.Add(userCallsign);
				m2->m_params.Add(arearp_cs);
				m2->m_params.Add(zonerp_cs);
				m2->m_params.Add(ip_addr); 
				m2->m_params.Add(tk + wxT(" ") + timeToken);
				m_replyQ.putMessage(m2);
			}
		}
	}
}

wxString IRCDDBApp::getTableIDString(long tableID, bool spaceBeforeNumber)
{
	if (tableID == 0L)
		return wxEmptyString;

	if (tableID > 0L && tableID < NUMBER_OF_TABLES) {
		if (spaceBeforeNumber)
			return wxString::Format(wxT(" %ld"), tableID);
		else
			return wxString::Format(wxT("%ld "), tableID);
	} else {
		return wxT(" TABLE_ID_OUT_OF_RANGE ");
	}
}

void IRCDDBApp::msgQuery(IRCMessage* m)
{
	wxASSERT(m != NULL);

	if (m->getPrefixNick().StartsWith(wxT("s-")) && (m->m_params.GetCount() >= 2U)) {  // server msg
		wxString msg = m->m_params.Item(1);
		wxStringTokenizer tkz(msg);

		if (!tkz.HasMoreTokens()) 
			return;  // no text in message

		wxString cmd = tkz.GetNextToken();

		if (cmd.IsSameAs(wxT("UPDATE"), false)) {
			wxString restOfLine = tkz.GetString();
			doUpdate(restOfLine);
		} else if (cmd.IsSameAs(wxT("LIST_END"), false)) {
			if (m_state == 5) // if in sendlist processing state
				m_state = 3;  // get next table
		} else if (cmd.IsSameAs(wxT("LIST_MORE"), false)) {
			if (m_state == 5) // if in sendlist processing state
				m_state = 4;  // send next SENDLIST
		} else if (cmd.IsSameAs(wxT("NOT_FOUND"), false)) {
			wxString callsign;
			wxString restOfLine = tkz.GetString();
			doNotFound(restOfLine, callsign);

			if (callsign.Len() > 0U) {
				callsign.Replace(wxT("_"), wxT(" "));

				IRCMessage* m2 = new IRCMessage(wxT("IDRT_USER"));
				m2->m_params.Add(callsign);
				m2->m_params.Add(wxEmptyString);
				m2->m_params.Add(wxEmptyString);
				m2->m_params.Add(wxEmptyString);
				m2->m_params.Add(wxEmptyString);
				m_replyQ.putMessage(m2);
			}
		}
	}
}

void IRCDDBApp::setSendQ(IRCMessageQueue* s)
{
	m_sendQ = s;
}

IRCMessageQueue* IRCDDBApp::getSendQ()
{
	return m_sendQ;
}

wxString IRCDDBApp::getLastEntryTime(long tableID)
{
	if (tableID == 1L)
		return IRCDDBAppRptrObject::maxTime.Format( wxT("%Y-%m-%d %H:%M:%S") );

	return wxT("DBERROR");
}

bool IRCDDBApp::needsDatabaseUpdate(long tableID)
{
	return tableID == 1L;
}

wxThread::ExitCode IRCDDBApp::Entry()
{
	wxLogMessage(wxT("Starting the IRCDDB Application thread"));

	long sendlistTableID = 0L;

	while (!m_stopped) {
		m_timer.clock();

		switch (m_state) {
			case 0:  // wait for network to start
				if (getSendQ() != NULL)
					m_state = 1;
				break;

			case 1:
				// connect to db
				m_state = 2;

				m_timer.start(200U);

				break;

			case 2:   // choose server
				if (getSendQ() == NULL) {
					m_state = 10;
				} else {	
					if (findServerUser()) {
						sendlistTableID = NUMBER_OF_TABLES;

						m_state = 3; // next: send "SENDLIST"
					} else if (m_timer.isRunning() && m_timer.hasExpired()) {
						m_timer.stop();

						m_state = 10;
						IRCMessage* m = new IRCMessage(wxT("QUIT"));

						m->m_params.Add(wxT("no op user with 's-' found."));

						IRCMessageQueue* q = getSendQ();
						if (q != NULL)
							q->putMessage(m);
					}
				}
				break;

			case 3:
				if (getSendQ() == NULL) {
					m_state = 10; // disconnect DB
				} else {
					sendlistTableID --;
					if (sendlistTableID < 0L) {
						m_state = 6; // end of sendlist
					} else {
						m_state = 4; // send "SENDLIST"

						m_timer.start(900U); // 15 minutes max for update
					}
				}
				break;

			case 4:
				if (getSendQ() == NULL) {
					m_state = 10; // disconnect DB
				} else {
					if (needsDatabaseUpdate(sendlistTableID)) {
						IRCMessage * m = new IRCMessage(m_currentServer, 
							wxT("SENDLIST") + getTableIDString(sendlistTableID, true) 
							+ wxT(" ") + getLastEntryTime(sendlistTableID) );

						IRCMessageQueue* q = getSendQ();
						if (q != NULL)
							q->putMessage(m);

						m_state = 5; // wait for answers
					} else {
						m_state = 3; // don't send SENDLIST for this table, go to next table
					}
				}
				break;

			case 5: // sendlist processing
				if (getSendQ() == NULL) {
					m_state = 10; // disconnect DB
				} else if (m_timer.isRunning() && m_timer.hasExpired()) {
					m_timer.stop();

					m_state = 10; // disconnect DB
					IRCMessage* m = new IRCMessage(wxT("QUIT"));

					m->m_params.Add(wxT("timeout SENDLIST"));

					IRCMessageQueue* q = getSendQ();
					if (q != NULL)
						q->putMessage(m);
				}
				break;

			case 6:
				if (getSendQ() == NULL) {
					m_state = 10; // disconnect DB
				} else {
					m_moduleDataTimer.start(2U);
	
					m_initReady = true;
					m_state = 7;
				}
				break;

			case 7: // standby state after initialization
				if (getSendQ() == NULL)
					m_state = 10; // disconnect DB

				m_moduleDataTimer.clock();
				m_moduleWDTimer.clock();

				if (m_moduleDataTimer.isRunning() && m_moduleDataTimer.hasExpired()) {
					m_moduleDataTimer.stop();

					wxMutexLocker locker(m_moduleDataMutex);

					for (wxArrayString::const_iterator it = m_moduleData.begin(); it != m_moduleData.end(); ++it) {
						IRCMessage* m = new IRCMessage(m_currentServer, *it);

						IRCMessageQueue* q = getSendQ();
						if (q != NULL)
							q->putMessage(m);
					}

					m_moduleData.clear();
				}

				if (m_moduleWDTimer.isRunning() && m_moduleWDTimer.hasExpired()) {
					m_moduleWDTimer.stop();

					wxMutexLocker locker(m_moduleWDMutex);

					for (wxArrayString::const_iterator it = m_moduleWD.begin(); it != m_moduleWD.end(); ++it) {
						IRCMessage* m = new IRCMessage(m_currentServer, *it);

						IRCMessageQueue* q = getSendQ();
						if (q != NULL)
							q->putMessage(m);
					}

					m_moduleWD.clear();
				}

				break;

			case 10:
				// disconnect db
				m_state = 0;
				m_timer.stop();
				m_initReady = false;
				break;
		}

		Sleep(1000UL);
	}

	wxLogMessage(wxT("Stopping the IRCDDB Application thread"));

	return 0;
}
