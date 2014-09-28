/*
 *   Copyright (C) 2009,2011,2014 by Jonathan Naylor G4KLX
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

#include "MessageData.h"

CMessageData::CMessageData(const CMessageData& message) :
m_text(message.m_text)
{
}

CMessageData::CMessageData(const unsigned char* text, unsigned int length) :
m_text()
{
	wxASSERT(text != NULL);
	wxASSERT(length > 0U);

	m_text = wxString((const char*)text, wxConvLocal, length);
}

CMessageData::CMessageData(const wxString& text) :
m_text(text)
{
}

CMessageData::~CMessageData()
{
}

wxString CMessageData::getText() const
{
	return m_text;
}
