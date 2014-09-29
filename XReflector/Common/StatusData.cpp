/*
 *   Copyright (C) 2011,2012 by Jonathan Naylor G4KLX
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

#include "StatusData.h"

#include "DStarDefines.h"
#include "Utils.h"

CStatusData::CStatusData(const wxString& text, unsigned int n) :
m_data(NULL),
m_n(n),
m_address(),
m_port(0U)
{
	m_data = new unsigned char[20U];
	::memset(m_data, ' ', 20U);

	for (unsigned int i = 0U; i < text.Length() && i < 20U; i++)
		m_data[i] = text.GetChar(i);
}

CStatusData::~CStatusData()
{
	delete[] m_data;
}

unsigned int CStatusData::getHBRepeaterData(unsigned char *data, unsigned int length) const
{
	wxASSERT(data != NULL);
	wxASSERT(length >= 26U);

	data[0] = 'D';
	data[1] = 'S';
	data[2] = 'R';
	data[3] = 'P';

	data[4] = 0x04;					// Status text data

	data[5] = m_n;

	::memcpy(data + 6U, m_data, 20U);

	return 26U;
}

void CStatusData::setDestination(const in_addr& address, unsigned int port)
{
	m_address = address;
	m_port    = port;
}

in_addr CStatusData::getAddress() const
{
	return m_address;
}

unsigned int CStatusData::getPort() const
{
	return m_port;
}
