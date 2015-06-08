/*

CIRCDDB - ircDDB client library in C++

Copyright (C) 2010-2011   Michael Dirska, DL1BFF (dl1bff@mdx.de)
Copyright (C) 2012        Jonathan Naylor, G4KLX

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

#include <wx/datetime.h>
#include <wx/regex.h>
#include <wx/tokenzr.h>

class IRCDDBAppUserObject
{
  public:

  wxString nick;
  wxString name;
  wxString host;
  bool op;
  unsigned int usn;

  IRCDDBAppUserObject ()
  {
    IRCDDBAppUserObject (wxT(""), wxT(""), wxT(""));
  }
  
  IRCDDBAppUserObject ( const wxString& n, const wxString& nm, const wxString& h )
  {
    nick = n;
    name = nm;
    host = h;
    op = false;
    usn = counter;
    counter ++;
  }

  static unsigned int counter;
};

unsigned int IRCDDBAppUserObject::counter = 0;


WX_DECLARE_STRING_HASH_MAP( IRCDDBAppUserObject, IRCDDBAppUserMap );


class IRCDDBAppRptrObject
{
  public:

  wxString arearp_cs;
  wxDateTime lastChanged;
  wxString zonerp_cs;

  IRCDDBAppRptrObject ()
  {
  }

  IRCDDBAppRptrObject (wxDateTime& dt, wxString& repeaterCallsign, wxString& gatewayCallsign)
  {
    arearp_cs = repeaterCallsign;
    lastChanged = dt;
    zonerp_cs = gatewayCallsign;

    if (dt.IsLaterThan(maxTime))
    {
      maxTime = dt;
    }
  }

  static wxDateTime maxTime;
}; 

wxDateTime IRCDDBAppRptrObject::maxTime((time_t) 950000000);  // February 2000


WX_DECLARE_STRING_HASH_MAP( IRCDDBAppRptrObject, IRCDDBAppRptrMap );

WX_DECLARE_STRING_HASH_MAP( wxString, IRCDDBAppModuleQRG );
WX_DECLARE_STRING_HASH_MAP( wxString, IRCDDBAppModuleQTH );
WX_DECLARE_STRING_HASH_MAP( wxString, IRCDDBAppModuleURL );
WX_DECLARE_STRING_HASH_MAP( wxString, IRCDDBAppModuleWD );

class IRCDDBAppPrivate
{
  public:

  IRCDDBAppPrivate()
  : tablePattern(wxT("^[0-9]$")),
    datePattern(wxT("^20[0-9][0-9]-((1[0-2])|(0[1-9]))-((3[01])|([12][0-9])|(0[1-9]))$")),
    timePattern(wxT("^((2[0-3])|([01][0-9])):[0-5][0-9]:[0-5][0-9]$")),
    dbPattern(wxT("^[0-9A-Z_]{8}$"))
  {
  }

  IRCMessageQueue * sendQ;

  IRCDDBAppUserMap user;
  wxMutex userMapMutex;

  wxString currentServer;
  wxString myNick;

  wxRegEx tablePattern;
  wxRegEx datePattern;
  wxRegEx timePattern;
  wxRegEx dbPattern;
 
  int state;
  int timer;
  int infoTimer;

  wxString updateChannel;
  wxString channelTopic;
  wxString bestServer;

  bool initReady;

  bool terminateThread;

  IRCDDBAppRptrMap rptrMap;
  wxMutex rptrMapMutex;

  IRCMessageQueue replyQ;

  IRCDDBAppModuleQRG moduleQRG;
  wxMutex moduleQRGMutex;

  IRCDDBAppModuleQTH moduleQTH;
  IRCDDBAppModuleURL moduleURL;
  wxMutex moduleQTHURLMutex;

  IRCDDBAppModuleWD moduleWD;
  wxMutex moduleWDMutex;

  int wdTimer;
};

	
IRCDDBApp::IRCDDBApp( const wxString& u_chan )
  : wxThread(wxTHREAD_JOINABLE),
    d(new IRCDDBAppPrivate)
{

  d->sendQ = NULL;
  d->initReady = false;

  userListReset();
		
  d->state = 0;
  d->timer = 0;
  d->myNick = wxT("none");
		
  d->updateChannel = u_chan;

  d->terminateThread = false;
		
}

IRCDDBApp::~IRCDDBApp()
{
	delete d->sendQ;
	delete d;
}


void IRCDDBApp::rptrQTH(const wxString& callsign, double latitude, double longitude, const wxString& desc1, const wxString& desc2, const wxString& infoURL)
{
	wxString pos;
	pos.Printf(wxT("%+09.5f %+010.5f"), latitude, longitude);

	wxString cs = callsign;
	wxString d1 = desc1;
	wxString d2 = desc2;

	d1.Append(wxT(' '), 20);
	d2.Append(wxT(' '), 20);

	wxRegEx nonValid(wxT("[^a-zA-Z0-9 +&(),./'-]"));
	nonValid.Replace(&d1, wxEmptyString);
	nonValid.Replace(&d2, wxEmptyString);

	pos.Replace(wxT(","), wxT("."));
	d1.Replace(wxT(" "), wxT("_"));
	d2.Replace(wxT(" "), wxT("_"));
	cs.Replace(wxT(" "), wxT("_"));

	wxMutexLocker lock(d->moduleQTHURLMutex);

	d->moduleQTH[cs] = cs + wxT(" ") + pos + wxT(" ") + d1.Mid(0, 20) + wxT(" ") + d2.Mid(0, 20);

	wxLogVerbose(wxT("QTH: ") + d->moduleQTH[cs]);

	wxString url = infoURL;

	wxRegEx urlNonValid(wxT("[^[:graph:]]"));
	urlNonValid.Replace(&url, wxEmptyString);

	if (!url.IsEmpty()) {
		d->moduleURL[cs] = cs + wxT(" ") + url;

		wxLogVerbose(wxT("URL: ") + d->moduleURL[cs]);
	}

	d->infoTimer = 5; // send info in 5 seconds
}


void IRCDDBApp::rptrQRG(const wxString& callsign, double txFrequency, double duplexShift, double range, double agl)
{
	wxString cs = callsign;
	cs.Replace(wxT(" "), wxT("_"));

	wxString f;
	f.Printf(wxT("%011.5f %+010.5f %06.2f %06.1f"), txFrequency, duplexShift, range / 1609.344, agl);
	f.Replace(wxT(","), wxT("."));

	wxMutexLocker lock(d->moduleQRGMutex);

	d->moduleQRG[cs] = cs + wxT(" ") + f;

	wxLogVerbose(wxT("QRG: ") + d->moduleQRG[cs]);

	d->infoTimer = 5; // send info in 5 seconds
}

void IRCDDBApp::kickWatchdog(const wxString& callsign, const wxString& s)
{
	wxString text = s;

	wxRegEx nonValid(wxT("[^[:graph:]]"));
	nonValid.Replace(&text, wxEmptyString);

	if (!text.IsEmpty()) {
		wxString cs = callsign;
		cs.Replace(wxT(" "), wxT("_"));

		wxMutexLocker lock(d->moduleWDMutex);

		d->moduleWD[cs] = cs + wxT(" ") + text;

		d->wdTimer = 60;
	}
}


int IRCDDBApp::getConnectionState()
{
	return d->state;
}

IRCDDB_RESPONSE_TYPE IRCDDBApp::getReplyMessageType()
{
	IRCMessage* m = d->replyQ.peekFirst();
	if (m == NULL)
		return IDRT_NONE;

	wxString msgType = m->getCommand();

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
	return d->replyQ.getMessage();
}



void IRCDDBApp::startWork()
{
	d->terminateThread = false;

	Create();
	Run();
}

void IRCDDBApp::stopWork()
{
    d->terminateThread = true;

    Wait();
}

	
void IRCDDBApp::userJoin (const wxString& nick, const wxString& name, const wxString& host)
{
  wxMutexLocker lock(d->userMapMutex);

  wxString lnick = nick;
  lnick.MakeLower();

  IRCDDBAppUserObject u( lnick, name, host );
		
  d->user[lnick] = u;

  // wxLogVerbose(wxT("add %d: (") + u.nick + wxT(") (") + u.host + wxT(")"), d->user.size());

  if (d->initReady)
  {
    int hyphenPos = nick.Find(wxT('-'));

    if ((hyphenPos >= 4) && (hyphenPos <= 6))
    {
      wxString gatewayCallsign = nick.Mid(0, hyphenPos).Upper();

      while (gatewayCallsign.Length() < 7)
      {
	gatewayCallsign.Append(wxT(' '));
      }

      gatewayCallsign.Append(wxT('G'));

      IRCMessage * m2 = new IRCMessage(wxT( "IDRT_GATEWAY"));
      m2->addParam(gatewayCallsign);
      m2->addParam(host);
      d->replyQ.putMessage(m2);
    }
  }

  // wxLogVerbose(wxT("user %d"), u.usn );
}




void IRCDDBApp::userLeave (const wxString& nick)
{
  wxMutexLocker lock(d->userMapMutex);

  wxString lnick = nick;
  lnick.MakeLower();

  d->user.erase(lnick);

  // wxLogVerbose(wxT("rm %d: ") + nick, d->user.size());

  if (d->currentServer.Len() > 0)
  {
    if (d->user.count(d->myNick) != 1)
    {
      wxLogVerbose(wxT("IRCDDBApp::userLeave: could not find own nick"));
      return;
    }

    IRCDDBAppUserObject me = d->user[d->myNick];

    if (me.op == false)  
    {
	    // if I am not op, then look for new server

      if (d->currentServer.IsSameAs(lnick))
      {
	      // currentServer = null;
	      d->state = 2;  // choose new server
	      d->timer = 200;
	      d->initReady = false;
      }
    }
  }
}

void IRCDDBApp::userListReset()
{
  wxMutexLocker lock(d->userMapMutex);

  d->user.clear();
}

void IRCDDBApp::setCurrentNick(const wxString& nick)
{
  d->myNick = nick;
  wxLogVerbose(wxT("IRCDDBApp::setCurrentNick ") + nick);
}

void IRCDDBApp::setBestServer(const wxString& ircUser)
{
  d->bestServer = ircUser;
  wxLogVerbose(wxT("IRCDDBApp::setBestServer ") + ircUser);
}

void IRCDDBApp::setTopic(const wxString& topic)
{
  d->channelTopic = topic;
}

bool IRCDDBApp::findServerUser()
{
  wxMutexLocker lock(d->userMapMutex);

  bool found = false;

  IRCDDBAppUserMap::iterator it;

  for( it = d->user.begin(); it != d->user.end(); ++it )
  {
    IRCDDBAppUserObject u = it->second;

    if (u.nick.StartsWith(wxT("s-")) && u.op && !d->myNick.IsSameAs(u.nick)
      && u.nick.IsSameAs(d->bestServer))
    {
      d->currentServer = u.nick;
      found = true;
      break;
    }
  }

  if (found)
    return true;

  if (d->bestServer.Len() == 8)
  {
    for( it = d->user.begin(); it != d->user.end(); ++it )
    {
      IRCDDBAppUserObject u = it->second;

      if (u.nick.StartsWith(d->bestServer.Mid(0,7)) && u.op &&
	!d->myNick.IsSameAs(u.nick) )
      {
	d->currentServer = u.nick;
	found = true;
	break;
      }
    }
  }

  if (found)
    return true;

  for( it = d->user.begin(); it != d->user.end(); ++it )
  {
    IRCDDBAppUserObject u = it->second;

    if (u.nick.StartsWith(wxT("s-")) && u.op && !d->myNick.IsSameAs(u.nick))
    {
      d->currentServer = u.nick;
      found = true;
      break;
    }
  }

  return found;
}
	
void IRCDDBApp::userChanOp (const wxString& nick, bool op)
{
  wxMutexLocker lock(d->userMapMutex);

  wxString lnick = nick;
  lnick.MakeLower();

  if (d->user.count(lnick) == 1)
  {
    d->user[lnick].op = op;
  }
}
	


static const int numberOfTables = 2;





wxString IRCDDBApp::getIPAddress(wxString& zonerp_cs)
{
  wxMutexLocker lock(d->userMapMutex);
  wxString gw = zonerp_cs;

  gw.Replace(wxT("_"), wxT(" "));
  gw.MakeLower();

  unsigned int max_usn = 0;
  wxString ipAddr;

  int j;

  for (j=1; j <= 4; j++)
  {
    // int i = 0;
    wxString ircUser = gw.Strip() + wxString::Format(wxT("-%d"), j);

    if (d->user.count(ircUser) == 1)
    {
      IRCDDBAppUserObject o = d->user[ ircUser ];

      if (o.usn >= max_usn)
      {
	max_usn = o.usn;
	ipAddr = o.host;
      }
      // i = 1;
    }
    // wxLogVerbose(wxT("getIP %d (") + ircUser + wxT(") (") + ipAddr + wxT(")"), i);

  }

  return ipAddr;
}


bool IRCDDBApp::findGateway(const wxString& gwCall)
{
  wxString s = gwCall.Mid(0,6);

  IRCMessage * m2 = new IRCMessage(wxT( "IDRT_GATEWAY"));
  m2->addParam(gwCall);
  m2->addParam(getIPAddress(s));
  d->replyQ.putMessage(m2);

  return true;
}

static void findReflector( const wxString& rptrCall, IRCDDBAppPrivate * d )
{
  wxString zonerp_cs;
  wxString ipAddr;

#define MAXIPV4ADDR 5
  struct sockaddr_in addr[MAXIPV4ADDR];
  unsigned int numAddr = 0;

  char host_name[80];

  wxString host = rptrCall.Mid(0,6) + wxT(".reflector.ircddb.net");

  safeStringCopy(host_name, host.mb_str(wxConvUTF8), sizeof host_name);

  if (getAllIPV4Addresses(host_name, 0, &numAddr, addr, MAXIPV4ADDR) == 0)
  {
    if (numAddr > 0)
    {
      unsigned char * a = (unsigned char *) &addr[0].sin_addr;

      ipAddr = wxString::Format(wxT("%d.%d.%d.%d"), a[0], a[1], a[2], a[3]);
      zonerp_cs = rptrCall;
      zonerp_cs.SetChar(7, wxT('G'));
    }
  }
  

  IRCMessage * m2 = new IRCMessage(wxT("IDRT_REPEATER"));
  m2->addParam(rptrCall);
  m2->addParam(zonerp_cs);
  m2->addParam(ipAddr);
  d->replyQ.putMessage(m2);
}

bool IRCDDBApp::findRepeater(const wxString& rptrCall)
{

  if (rptrCall.StartsWith(wxT("XRF")) || rptrCall.StartsWith(wxT("REF")))
  {
    findReflector(rptrCall, d);
    return true;
  }

  wxString arearp_cs = rptrCall;
  arearp_cs.Replace(wxT(" "), wxT("_"));

  wxString zonerp_cs;

  wxMutexLocker lock(d->rptrMapMutex);
  
  wxString s = wxT("NONE");

  if (d->rptrMap.count(arearp_cs) == 1)
  {
    IRCDDBAppRptrObject o = d->rptrMap[arearp_cs];
    zonerp_cs = o.zonerp_cs;
    zonerp_cs.Replace(wxT("_"), wxT(" "));
    zonerp_cs.SetChar(7, wxT('G'));
    s = o.zonerp_cs;
  }

  IRCMessage * m2 = new IRCMessage(wxT("IDRT_REPEATER"));
  m2->addParam(rptrCall);
  m2->addParam(zonerp_cs);
  m2->addParam(getIPAddress(s));
  d->replyQ.putMessage(m2);

  return true;
}


bool IRCDDBApp::sendHeard(const wxString& myCall, const wxString& myCallExt,
        const wxString& yourCall, const wxString& rpt1,
        const wxString& rpt2, unsigned char flag1,
        unsigned char flag2, unsigned char flag3,
	const wxString& destination, const wxString& tx_msg,
	const wxString& tx_stats )
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

  wxString srv = d->currentServer;
  IRCMessageQueue * q = getSendQ();

  if ((srv.Len() > 0) && (d->state >= 6) && (q != NULL))
  {
    wxString cmd =  wxT("UPDATE ");

    cmd.Append( getCurrentTime() );

    cmd.Append(wxT(" "));

    cmd.Append(my);
    cmd.Append(wxT(" "));
    cmd.Append(r1);
    cmd.Append(wxT(" "));
    if (!statsMsg)
    {
      cmd.Append(wxT("0 "));
    }
    cmd.Append(r2);
    cmd.Append(wxT(" "));
    cmd.Append(ur);
    cmd.Append(wxT(" "));

    wxString flags = wxString::Format(wxT("%02X %02X %02X"), flag1, flag2, flag3);

    cmd.Append(flags);
    cmd.Append(wxT(" "));
    cmd.Append(myext);

    if (statsMsg)
    {
      cmd.Append(wxT(" # "));
      cmd.Append(tx_stats);
    }
    else
    {
      cmd.Append(wxT(" 00 "));
      cmd.Append(dest);

      if (tx_msg.Len() == 20)
      {
        cmd.Append(wxT(" "));
	cmd.Append(tx_msg);
      }
    }


    IRCMessage * m = new IRCMessage(srv, cmd);

    q->putMessage(m);
    return true;
  }
  else
  {
    return false;
  }
}

bool IRCDDBApp::findUser(const wxString& usrCall)
{
  wxString srv = d->currentServer;
  IRCMessageQueue * q = getSendQ();

  if ((srv.Len() > 0) && (d->state >= 6) && (q != NULL))
  {
    wxString usr = usrCall;

    usr.Replace(wxT(" "), wxT("_"));

    IRCMessage * m = new IRCMessage(srv,
		  wxT("FIND ") + usr );

    q->putMessage(m);
  }
  else
  {
    IRCMessage * m2 = new IRCMessage(wxT("IDRT_USER"));
    m2->addParam(usrCall);
    m2->addParam(wxT(""));
    m2->addParam(wxT(""));
    m2->addParam(wxT(""));
    m2->addParam(wxT(""));
    d->replyQ.putMessage(m2);
  }

  return true;
}


void IRCDDBApp::msgChannel (IRCMessage * m)
{
  if (m->getPrefixNick().StartsWith(wxT("s-")) && (m->numParams >= 2))  // server msg
  {
    doUpdate(m->params[1]);
  }
}


void IRCDDBApp::doNotFound ( wxString& msg, wxString& retval )
{
  int tableID = 0;

  wxStringTokenizer tkz(msg);

  if (!tkz.HasMoreTokens()) 
  {
    return;  // no text in message
  }

  wxString tk = tkz.GetNextToken();


  if (d->tablePattern.Matches(tk))
  {
    long i;

    if (tk.ToLong(&i))
    {
      tableID = i;
      if ((tableID < 0) || (tableID >= numberOfTables))
      {
	wxLogVerbose(wxT("invalid table ID %d"), tableID);
	return;
      }
    }
    else
    {
      return; // not a valid number
    }

    if (!tkz.HasMoreTokens()) 
    {
      return;  // received nothing but the tableID
    }

    tk = tkz.GetNextToken();
  }

  if (tableID == 0)
  {
    if (! d->dbPattern.Matches(tk))
    {
      return; // no valid key
    }

    retval = tk;
  }
}

void IRCDDBApp::doUpdate ( wxString& msg )
{
    int tableID = 0;

    wxStringTokenizer tkz(msg);

    if (!tkz.HasMoreTokens()) 
    {
      return;  // no text in message
    }

    wxString tk = tkz.GetNextToken();


    if (d->tablePattern.Matches(tk))
    {
      long i;

      if (tk.ToLong(&i))
      {
	tableID = i;
	if ((tableID < 0) || (tableID >= numberOfTables))
	{
	  wxLogVerbose(wxT("invalid table ID %d"), tableID);
	  return;
	}
      }
      else
      {
	return; // not a valid number
      }

      if (!tkz.HasMoreTokens()) 
      {
	return;  // received nothing but the tableID
      }

      tk = tkz.GetNextToken();
    }

    if (d->datePattern.Matches(tk))
    {
      if (!tkz.HasMoreTokens()) 
      {
	return;  // nothing after date string
      }

      wxString timeToken = tkz.GetNextToken();

      if (! d->timePattern.Matches(timeToken))
      {
	return; // no time string after date string
      }

      wxDateTime dt;

      if (dt.ParseFormat(tk + wxT(" ") + timeToken, wxT("%Y-%m-%d %H:%M:%S")) == NULL)
      {
	return; // date+time parsing failed
      }

      if ((tableID == 0) || (tableID == 1))
      {
	if (!tkz.HasMoreTokens())
	{
	  return;  // nothing after time string
	}

	wxString key = tkz.GetNextToken();

	if (! d->dbPattern.Matches(key))
	{
	  return; // no valid key
	}

	if (!tkz.HasMoreTokens())
	{
	  return;  // nothing after time string
	}

	wxString value = tkz.GetNextToken();

	if (! d->dbPattern.Matches(value))
	{
	  return; // no valid key
	}

	//wxLogVerbose(wxT("TABLE %d ") + key + wxT(" ") + value, tableID );


	if (tableID == 1)
	{
	  wxMutexLocker lock(d->rptrMapMutex);

	  IRCDDBAppRptrObject newRptr(dt, key, value);

	  d->rptrMap[key] = newRptr;

	  if (d->initReady)
	  {
	    wxString arearp_cs = key;
	    wxString zonerp_cs = value;

	    arearp_cs.Replace(wxT("_"), wxT(" "));
	    zonerp_cs.Replace(wxT("_"), wxT(" "));
	    zonerp_cs.SetChar(7, wxT('G'));

	    IRCMessage * m2 = new IRCMessage(wxT("IDRT_REPEATER"));
	    m2->addParam(arearp_cs);
	    m2->addParam(zonerp_cs);
	    m2->addParam(getIPAddress(value));
	    d->replyQ.putMessage(m2);
	  }
	}
	else if ((tableID == 0) && d->initReady)
	{
	  wxMutexLocker lock(d->rptrMapMutex);

	  wxString userCallsign = key;
	  wxString arearp_cs = value;
	  wxString zonerp_cs;
	  wxString ip_addr;

	  userCallsign.Replace(wxT("_"), wxT(" "));
	  arearp_cs.Replace(wxT("_"), wxT(" "));

	  if (d->rptrMap.count(value) == 1)
	  {
	    IRCDDBAppRptrObject o = d->rptrMap[value];
	    zonerp_cs = o.zonerp_cs;
	    zonerp_cs.Replace(wxT("_"), wxT(" "));
	    zonerp_cs.SetChar(7, wxT('G'));

	    ip_addr = getIPAddress(o.zonerp_cs);
	  }

	  IRCMessage * m2 = new IRCMessage(wxT("IDRT_USER"));
	  m2->addParam(userCallsign);
	  m2->addParam(arearp_cs);
	  m2->addParam(zonerp_cs);
	  m2->addParam(ip_addr); 
	  m2->addParam(tk + wxT(" ") + timeToken);
	  d->replyQ.putMessage(m2);

	}


      }
    }

}


static wxString getTableIDString( int tableID, bool spaceBeforeNumber )
{
  if (tableID == 0)
  {
    return wxT("");
  }
  else if ((tableID > 0) && (tableID < numberOfTables))
  {
    if (spaceBeforeNumber)
    {
      return wxString::Format(wxT(" %d"),tableID);
    }
    else
    {	
      return wxString::Format(wxT("%d "),tableID);
    }
  }
  else
  {
    return wxT(" TABLE_ID_OUT_OF_RANGE ");
  }
}


void IRCDDBApp::msgQuery (IRCMessage * m)
{

  if (m->getPrefixNick().StartsWith(wxT("s-")) && (m->numParams >= 2))  // server msg
  {
    wxString msg = m->params[1];
    wxStringTokenizer tkz(msg);

    if (!tkz.HasMoreTokens()) 
    {
      return;  // no text in message
    }

    wxString cmd = tkz.GetNextToken();

    if (cmd.IsSameAs(wxT("UPDATE")))
    {
      wxString restOfLine = tkz.GetString();
      doUpdate(restOfLine);
    }
    else if (cmd.IsSameAs(wxT("LIST_END")))
    {
      if (d->state == 5) // if in sendlist processing state
      {
	d->state = 3;  // get next table
      }
    }
    else if (cmd.IsSameAs(wxT("LIST_MORE")))
    {
      if (d->state == 5) // if in sendlist processing state
      {
	d->state = 4;  // send next SENDLIST
      }
    }
    else if (cmd.IsSameAs(wxT("NOT_FOUND")))
    {
      wxString callsign;
      wxString restOfLine = tkz.GetString();
      doNotFound(restOfLine, callsign);

      if (callsign.Len() > 0)
      {
	callsign.Replace(wxT("_"), wxT(" "));

	IRCMessage * m2 = new IRCMessage(wxT("IDRT_USER"));
	m2->addParam(callsign);
	m2->addParam(wxT(""));
	m2->addParam(wxT(""));
	m2->addParam(wxT(""));
	m2->addParam(wxT(""));
	d->replyQ.putMessage(m2);
      }
    }
  }
}
	
	
void IRCDDBApp::setSendQ( IRCMessageQueue * s )
{
  d->sendQ = s;
}
	
IRCMessageQueue * IRCDDBApp::getSendQ()
{
  return d->sendQ;
}
	
	
static wxString getLastEntryTime(int tableID)
{

  if (tableID == 1)
  {
    wxString max = IRCDDBAppRptrObject::maxTime.Format( wxT("%Y-%m-%d %H:%M:%S") );
    return max;
  }

  return wxT("DBERROR");
}


static bool needsDatabaseUpdate( int tableID )
{
  return (tableID == 1);
}
	
	
wxThread::ExitCode IRCDDBApp::Entry()
{

  int sendlistTableID = 0;
		
  while (!d->terminateThread)
  {

    if (d->timer > 0)
    {
      d->timer --;
    }
			
    switch(d->state)
    {
    case 0:  // wait for network to start
					
      if (getSendQ() != NULL)
      {
	      d->state = 1;
      }
      break;
				
    case 1:
      // connect to db
      d->state = 2;
      d->timer = 200;
      break;
			
    case 2:   // choose server
      wxLogVerbose(wxT("IRCDDBApp: state=2 choose new 's-'-user"));
      if (getSendQ() == NULL)
      {
	d->state = 10;
      }
      else
      {	
	if (findServerUser())
	{
	  sendlistTableID = numberOfTables;

	  d->state = 3; // next: send "SENDLIST"
	}
	else if (d->timer == 0)
	{
	  d->state = 10;
	  IRCMessage * m = new IRCMessage(wxT("QUIT"));

	  m->addParam(wxT("no op user with 's-' found."));
						
	  IRCMessageQueue * q = getSendQ();
	  if (q != NULL)
	  {
	    q->putMessage(m);
	  }
	}
      }
      break;
				
    case 3:
      if (getSendQ() == NULL)
      {
	d->state = 10; // disconnect DB
      }
      else
      {
	sendlistTableID --;
	if (sendlistTableID < 0)
	{
	  d->state = 6; // end of sendlist
	}
	else
	{
	  wxLogVerbose(wxT("IRCDDBApp: state=3 tableID=%d"), sendlistTableID);
	  d->state = 4; // send "SENDLIST"
	  d->timer = 900; // 15 minutes max for update
	}
      }
      break;

    case 4:
      if (getSendQ() == NULL)
      {
	d->state = 10; // disconnect DB
      }
      else
      {
	if (needsDatabaseUpdate(sendlistTableID))
	{
	  IRCMessage * m = new IRCMessage(d->currentServer, 
			wxT("SENDLIST") + getTableIDString(sendlistTableID, true) 
			 + wxT(" ") + getLastEntryTime(sendlistTableID) );

	  IRCMessageQueue * q = getSendQ();
	  if (q != NULL)
	  {
	    q->putMessage(m);
	  }

	  d->state = 5; // wait for answers
	}
	else
	{
	  d->state = 3; // don't send SENDLIST for this table, go to next table
	}
      }
      break;

    case 5: // sendlist processing
      if (getSendQ() == NULL)
      {
	d->state = 10; // disconnect DB
      }
      else if (d->timer == 0)
      {
	d->state = 10; // disconnect DB
	  IRCMessage * m = new IRCMessage(wxT("QUIT"));

	  m->addParam(wxT("timeout SENDLIST"));
						
	  IRCMessageQueue * q = getSendQ();
	  if (q != NULL)
	  {
	    q->putMessage(m);
	  }
					
      }
      break;

    case 6:
      if (getSendQ() == NULL)
      {
	d->state = 10; // disconnect DB
      }
      else
      {
	wxLogVerbose(wxT( "IRCDDBApp: state=6 initialization completed"));

	d->infoTimer = 2;

	d->initReady = true;
	d->state = 7;
      }
      break;
				
			
    case 7: // standby state after initialization
      if (getSendQ() == NULL)
      {
	d->state = 10; // disconnect DB
      }

      if (d->infoTimer > 0)
      {
	d->infoTimer --;

	if (d->infoTimer == 0)
	{
	d->moduleQTHURLMutex.Lock();

	for (IRCDDBAppModuleQTH::iterator it = d->moduleQTH.begin(); it != d->moduleQTH.end(); ++it)
	{
	  wxString value = it->second;
	  IRCMessage* m = new IRCMessage(d->currentServer, wxT("IRCDDB RPTRQTH: ") + value);

	  IRCMessageQueue* q = getSendQ();
	  if (q != NULL)
	  {
	    q->putMessage(m);
	  }
	}

	d->moduleQTH.clear();

	for (IRCDDBAppModuleURL::iterator it = d->moduleURL.begin(); it != d->moduleURL.end(); ++it)
	{
	  wxString value = it->second;
	  IRCMessage* m = new IRCMessage(d->currentServer, wxT("IRCDDB RPTRURL: ") + value);

	  IRCMessageQueue* q = getSendQ();
	  if (q != NULL)
	  {
	    q->putMessage(m);
	  }
	}

	d->moduleURL.clear();

	d->moduleQTHURLMutex.Unlock();

	d->moduleQRGMutex.Lock();

	for (IRCDDBAppModuleQRG::iterator it = d->moduleQRG.begin(); it != d->moduleQRG.end(); ++it)
	{
	  wxString value = it->second;
	  IRCMessage* m = new IRCMessage(d->currentServer, wxT("IRCDDB RPTRQRG: ") + value);

	  IRCMessageQueue* q = getSendQ();
	  if (q != NULL)
	  {
	    q->putMessage(m);
	  }
	}

	d->moduleQRG.clear();

	d->moduleQRGMutex.Unlock();

	}
      }

      if (d->wdTimer > 0)
      {
	d->wdTimer --;

	if (d->wdTimer == 0)
	{
	d->moduleWDMutex.Lock();

	for (IRCDDBAppModuleWD::iterator it = d->moduleWD.begin(); it != d->moduleWD.end(); ++it)
	{
	  wxString value = it->second;
	  IRCMessage* m = new IRCMessage(d->currentServer, wxT("IRCDDB RPTRSW: ") + value);

	  IRCMessageQueue* q = getSendQ();
	  if (q != NULL)
	  {
	    q->putMessage(m);
	  }
	}

	d->moduleWD.clear();

	d->moduleWDMutex.Unlock();
	}
      }
      break;
				
    case 10:
      // disconnect db
      d->state = 0;
      d->timer = 0;
      d->initReady = false;
      break;
			
    }

    wxThread::Sleep(1000);

			
			

  } // while

  return 0;
} // Entry()
	



