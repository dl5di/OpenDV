/*

CIRCDDB - ircDDB client library in C++

Copyright (C) 2010	Michael Dirska, DL1BFF (dl1bff@mdx.de)
Copyright (C) 2014	Jonathan Naylor, G4KLX

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

#if !defined(_IRCPROTOCOL_H)
#define _IRCPROTOCOL_H

#include <wx/wx.h>

#include "IRCMessageQueue.h"
#include "IRCApplication.h"
#include "Timer.h"

class IRCProtocol
{
public:
	IRCProtocol(IRCApplication* app, const wxString& callsign, const wxString& password, const wxString& channel, const wxString& versionInfo);
	~IRCProtocol();

	void setNetworkReady(bool state);

	bool processQueues(IRCMessageQueue* recvQ, IRCMessageQueue* sendQ);

private:
	IRCApplication* m_app;
	wxArrayString   m_nicks;
	wxString        m_password;
	wxString        m_channel;
	wxString        m_name;
	wxString        m_currentNick;
	wxString        m_versionInfo;
	int             m_state;
	CTimer          m_timer;
	wxString        m_debugChannel;

	void chooseNewNick();
};

#endif
