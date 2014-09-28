/*
 *   Copyright (C) 2014 by Jonathan Naylor G4KLX
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

#include "DStarDefines.h"
#include "Modem.h"

const unsigned int BUFFER_LENGTH = 200U;

CModem::CModem() :
wxThread(wxTHREAD_JOINABLE),
m_rxData(1000U),
m_mutex(),
m_tx(false),
m_stopped(false),
m_readType(DSMTT_NONE),
m_readLength(0U),
m_readBuffer(NULL)
{
	m_readBuffer = new unsigned char[BUFFER_LENGTH];
}

CModem::~CModem()
{
	delete[] m_readBuffer;
}

bool CModem::isTX()
{
	return m_tx;
}

DSMT_TYPE CModem::read()
{
	m_readLength = 0U;

	if (m_rxData.isEmpty())
		return DSMTT_NONE;

	wxMutexLocker locker(m_mutex);

	unsigned char hdr[2U];
	m_rxData.getData(hdr, 2U);

	m_readType   = DSMT_TYPE(hdr[0U]);
	m_readLength = hdr[1U];
	m_rxData.getData(m_readBuffer, m_readLength);

	return m_readType;
}

CHeaderData* CModem::readHeader()
{
	if (m_readType != DSMTT_HEADER)
		return NULL;

	return new CHeaderData(m_readBuffer, RADIO_HEADER_LENGTH_BYTES, false);
}

unsigned int CModem::readData(unsigned char* data, unsigned int length)
{
	if (m_readType != DSMTT_DATA)
		return 0U;

	if (length < m_readLength) {
		::memcpy(data, m_readBuffer, length);
		return length;
	} else {
		::memcpy(data, m_readBuffer, m_readLength);
		return m_readLength;
	}
}

void CModem::stop()
{
	m_stopped = true;

	Wait();
}

