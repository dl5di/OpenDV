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

#if !defined(_IRCDDBAPP_H)
#define _IRCDDBAPP_H


#include "IRCDDB.h"
#include "IRCApplication.h"

#include <wx/wx.h>

class IRCDDBAppPrivate;

class IRCDDBApp : public IRCApplication, wxThread
{
  public:
    IRCDDBApp(const wxString& update_channel);

    virtual ~IRCDDBApp();

    virtual void userJoin (const wxString& nick, const wxString& name, const wxString& host);

    virtual void userLeave (const wxString& nick);

    virtual void userChanOp (const wxString& nick, bool op);
    virtual void userListReset();

    virtual void msgChannel (IRCMessage * m);
    virtual void msgQuery (IRCMessage * m);

    virtual void setCurrentNick(const wxString& nick);
    virtual void setTopic(const wxString& topic);

    virtual void setBestServer(const wxString& ircUser);

    virtual void setSendQ( IRCMessageQueue * s );
    virtual IRCMessageQueue * getSendQ ();

    void startWork();
    void stopWork();

    IRCDDB_RESPONSE_TYPE getReplyMessageType();

    IRCMessage * getReplyMessage();

    bool findUser ( const wxString& s );
    bool findRepeater ( const wxString& s );
    bool findGateway ( const wxString& s );

    bool sendHeard(const wxString& myCall, const wxString& myCallExt,
            const wxString& yourCall, const wxString& rpt1,
	    const wxString& rpt2, unsigned char flag1,
	    unsigned char flag2, unsigned char flag3,
	    const wxString& destination, const wxString& tx_msg,
	    const wxString& tx_stats);

    int getConnectionState();

    void rptrQRG( const wxString& callsign, double txFrequency, double duplexShift,
	double range, double agl );

    void rptrQTH( const wxString& callsign, double latitude, double longitude, const wxString& desc1,
                 const wxString& desc2, const wxString& infoURL );

    void kickWatchdog( const wxString& callsign, const wxString& wdInfo );

  protected:
    virtual wxThread::ExitCode Entry();

  private:
    void doUpdate ( wxString& msg );
    void doNotFound ( wxString& msg, wxString& retval );
    wxString getIPAddress( wxString& zonerp_cs );
    bool findServerUser();
    IRCDDBAppPrivate * d;
};


#endif
