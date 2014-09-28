/*
 *   Copyright (C) 2011 by Jonathan Naylor G4KLX
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

#include "DummyRepeaterStatusEvent.h"

CDummyRepeaterStatusEvent::CDummyRepeaterStatusEvent(const wxString& text, unsigned int n, wxEventType type, int id) :
wxEvent(id, type),
m_text(text),
m_n(n)
{
}

CDummyRepeaterStatusEvent::CDummyRepeaterStatusEvent(const CDummyRepeaterStatusEvent& event) :
wxEvent(event),
m_text(event.m_text),
m_n(event.m_n)
{
}

CDummyRepeaterStatusEvent::~CDummyRepeaterStatusEvent()
{
}

wxString CDummyRepeaterStatusEvent::getText() const
{
	return m_text;
}

unsigned int CDummyRepeaterStatusEvent::getN() const
{
	return m_n;
}

wxEvent* CDummyRepeaterStatusEvent::Clone() const
{
	return new CDummyRepeaterStatusEvent(*this);
}
