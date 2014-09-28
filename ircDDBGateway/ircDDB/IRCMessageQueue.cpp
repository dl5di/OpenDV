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

#include "IRCMessageQueue.h"

IRCMessageQueue::IRCMessageQueue() :
m_first(NULL),
m_last(NULL),
m_mutex(),
m_eof(false)
{
}

IRCMessageQueue::~IRCMessageQueue()
{
	while (messageAvailable())
		delete getMessage();
}

bool IRCMessageQueue::isEOF()
{
	return m_eof;
}

void IRCMessageQueue::signalEOF()
{
	m_eof = true;
}

bool IRCMessageQueue::messageAvailable()
{
	wxMutexLocker lock(m_mutex);

	return m_first != NULL;
}

IRCMessage* IRCMessageQueue::peekFirst()
{
	wxMutexLocker lock(m_mutex);

	IRCMessageQueueItem* k = m_first;

	if (k == NULL)
		return NULL;

	return k->msg;
}

IRCMessage* IRCMessageQueue::getMessage()
{
	wxMutexLocker lock(m_mutex);

	if (m_first == NULL)
		return NULL;

	IRCMessageQueueItem* k = m_first;

	m_first = k->next;

	if (k->next == NULL)
		m_last = NULL;
	else
		k->next->prev = NULL;

	IRCMessage* msg = k->msg;

	delete k;

	return msg;
}

void IRCMessageQueue::putMessage(IRCMessage* m)
{
	wxASSERT(m != NULL);

	wxMutexLocker lock(m_mutex);

	IRCMessageQueueItem* k = new IRCMessageQueueItem(m);

	k->prev = m_last;
	k->next = NULL;

	if (m_last == NULL)
		m_first = k;
	else
		m_last->next = k;

	m_last = k;
}
