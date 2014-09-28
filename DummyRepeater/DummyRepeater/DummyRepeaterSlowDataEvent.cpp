/*
 *   Copyright (C) 2010 by Jonathan Naylor G4KLX
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

#include "DummyRepeaterSlowDataEvent.h"

CDummyRepeaterSlowDataEvent::CDummyRepeaterSlowDataEvent(const wxString& slowData, wxEventType type, int id) :
wxEvent(id, type),
m_slowData(slowData)
{
}

CDummyRepeaterSlowDataEvent::CDummyRepeaterSlowDataEvent(const CDummyRepeaterSlowDataEvent& event) :
wxEvent(event),
m_slowData(event.m_slowData)
{
}

CDummyRepeaterSlowDataEvent::~CDummyRepeaterSlowDataEvent()
{
}

wxString CDummyRepeaterSlowDataEvent::getSlowData() const
{
	return m_slowData;
}

wxEvent* CDummyRepeaterSlowDataEvent::Clone() const
{
	return new CDummyRepeaterSlowDataEvent(*this);
}
