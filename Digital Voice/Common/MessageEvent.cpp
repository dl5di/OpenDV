/*
 *   Copyright (C) 2009 by Jonathan Naylor G4KLX
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "MessageEvent.h"

CMessageEvent::CMessageEvent(CMessageData* message, wxEventType type, int id) :
wxEvent(id, type),
m_message(message)
{
}

CMessageEvent::CMessageEvent(const CMessageEvent& event) :
wxEvent(event),
m_message(event.m_message)
{
}

CMessageEvent::~CMessageEvent()
{
}

CMessageData* CMessageEvent::getMessageData() const
{
	return m_message;
}

wxEvent* CMessageEvent::Clone() const
{
	return new CMessageEvent(*this);
}
