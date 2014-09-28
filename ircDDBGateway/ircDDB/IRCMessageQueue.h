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

#if !defined(_IRCMESSAGEQUEUE_H)
#define _IRCMESSAGEQUEUE_H

#include <wx/wx.h>

#include "IRCMessage.h"

class IRCMessageQueueItem
{
public:
	IRCMessageQueueItem( IRCMessage * m )
	{
		msg = m;
	}

	~IRCMessageQueueItem()
	{
	}

	IRCMessage * msg;

	IRCMessageQueueItem * prev;
	IRCMessageQueueItem * next;
};


class IRCMessageQueue
{
public:
	IRCMessageQueue();
	~IRCMessageQueue();

	bool isEOF();

	void signalEOF();

	bool messageAvailable();

	IRCMessage* getMessage();

	IRCMessage* peekFirst();

	void putMessage(IRCMessage* m);

private:
	IRCMessageQueueItem* m_first;
	IRCMessageQueueItem* m_last;
	wxMutex              m_mutex;
	bool                 m_eof;
};

#endif
