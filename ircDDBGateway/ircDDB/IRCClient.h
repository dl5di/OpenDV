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


#if !defined(_IRCCLIENT_H)
#define _IRCCLIENT_H

#include "IRCReceiver.h"
#include "IRCMessageQueue.h"
#include "IRCProtocol.h"
#include "IRCApplication.h"

#include <wx/wx.h>

class IRCClient : public wxThread
{
public:
	IRCClient(IRCApplication* app, const wxString& update_channel, const wxString& hostName, unsigned int port, const wxString& callsign, const wxString& password, const wxString& versionInfo, const wxString& localAddr);
	virtual ~IRCClient();

	virtual void startWork();

	virtual wxThread::ExitCode Entry();

	virtual void stopWork();

private:
	IRCApplication*  m_app;

	char*            m_hostName;
	char*            m_localAddr;
	unsigned int     m_port;
	wxString         m_callsign;
	wxString         m_password;

	bool             m_stopped;

	IRCReceiver*     m_recv;
	IRCMessageQueue* m_recvQ;
	IRCMessageQueue* m_sendQ;
	IRCProtocol*     m_proto;
};

#endif 
