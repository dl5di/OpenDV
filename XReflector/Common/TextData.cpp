/*
 *   Copyright (C) 2010,2012,2013 by Jonathan Naylor G4KLX
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

#include "TextData.h"

#include "DStarDefines.h"
#include "Utils.h"

CTextData::CTextData(LINK_STATUS status, const wxString& reflector, const wxString& text, const in_addr& address, unsigned int port, bool temporary) :
m_status(status),
m_reflector(NULL),
m_text(NULL),
m_address(address),
m_port(port),
m_temporary(temporary)
{
	wxASSERT(port > 0U);

	m_reflector = new unsigned char[8U];
	m_text      = new unsigned char[20U];

	::memset(m_reflector, ' ', 8U);
	::memset(m_text, ' ', 20U);

	if (status != LS_NONE) {
		for (unsigned int i = 0U; i < reflector.Length() && i < 8U; i++)
			m_reflector[i] = reflector.GetChar(i);
	}

	for (unsigned int i = 0U; i < text.Length() && i < 20U; i++)
		m_text[i] = text.GetChar(i);
}

CTextData::CTextData(const wxString& text, const in_addr& address, unsigned int port, bool temporary) :
m_status(LS_NONE),
m_reflector(NULL),
m_text(NULL),
m_address(address),
m_port(port),
m_temporary(temporary)
{
	wxASSERT(port > 0U);

	m_reflector = new unsigned char[8U];
	m_text      = new unsigned char[20U];

	::memset(m_reflector, ' ', 8U);
	::memset(m_text, ' ', 20U);

	for (unsigned int i = 0U; i < text.Length() && i < 20U; i++)
		m_text[i] = text.GetChar(i);
}

CTextData::~CTextData()
{
	delete[] m_reflector;
	delete[] m_text;
}

unsigned int CTextData::getHBRepeaterData(unsigned char *data, unsigned int length) const
{
	wxASSERT(data != NULL);
	wxASSERT(length >= 34U);

	data[0] = 'D';
	data[1] = 'S';
	data[2] = 'R';
	data[3] = 'P';

	if (m_temporary) {
		data[4] = 0x01U;					// Temporary text data
		::memcpy(data + 5U, m_text, 20U);
		return 25U;
	} else {
		data[4] = 0x00U;					// Permanent text data
		::memcpy(data + 5U, m_text, 20U);
		data[25U] = (unsigned char)m_status;
		::memcpy(data + 26U, m_reflector, 8U);
		return 34U;
	}
}

in_addr CTextData::getAddress() const
{
	return m_address;
}

unsigned int CTextData::getPort() const
{
	return m_port;
}
