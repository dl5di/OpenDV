/*
 *   Copyright (C) 2010,2011,2012 by Jonathan Naylor G4KLX
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

#include "SplitRepeaterAMBEData.h"
#include "DStarDefines.h"
#include "Utils.h"

CSplitRepeaterAMBEData::CSplitRepeaterAMBEData() :
m_outSeq(0U),
m_id(0U),
m_data(NULL),
m_address(),
m_port(0U),
m_errors(0U),
m_time(0UL)
{
	m_data = new unsigned char[DV_FRAME_LENGTH_BYTES];
}

CSplitRepeaterAMBEData::CSplitRepeaterAMBEData(const CSplitRepeaterAMBEData& data) :
m_outSeq(data.m_outSeq),
m_id(data.m_id),
m_data(NULL),
m_address(data.m_address),
m_port(data.m_port),
m_errors(data.m_errors),
m_time(data.m_time)
{
	m_data = new unsigned char[DV_FRAME_LENGTH_BYTES];
	::memcpy(m_data, data.m_data, DV_FRAME_LENGTH_BYTES);
}

CSplitRepeaterAMBEData::~CSplitRepeaterAMBEData()
{
	delete[] m_data;
}

bool CSplitRepeaterAMBEData::setRepeaterData(const unsigned char *data, unsigned int length, const in_addr& address, unsigned int port)
{
	wxASSERT(data != NULL);
	wxASSERT(length >= 21U);

	m_id     = data[5U] * 256U + data[6U];
	m_outSeq = data[7U];
	m_errors = data[8U];

	// A repeater end packet is longer than usual, so we substitute a normal length set of data
	if (isEnd()) {
		::memset(m_data, 0x00U, DV_FRAME_LENGTH_BYTES);
		::memcpy(m_data, END_PATTERN_BYTES, END_PATTERN_LENGTH_BYTES);
	} else {
		::memcpy(m_data, data + 9U, DV_FRAME_LENGTH_BYTES);
	}

	m_address = address;
	m_port    = port;

	return true;
}

unsigned int CSplitRepeaterAMBEData::getRepeaterData(unsigned char *data, unsigned int length) const
{
	wxASSERT(data != NULL);
	wxASSERT(length >= 21U);

	data[0] = 'D';
	data[1] = 'S';
	data[2] = 'R';
	data[3] = 'P';

	data[4] = 0x21U;

	data[5] = m_id / 256U;			// Unique session id
	data[6] = m_id % 256U;

	data[7] = m_outSeq;

	data[8] = 0U;

	::memcpy(data + 9U, m_data, DV_FRAME_LENGTH_BYTES);

	return 9U + DV_FRAME_LENGTH_BYTES;
}

void CSplitRepeaterAMBEData::setTime(unsigned long ms)
{
	m_time = ms;
}

unsigned long CSplitRepeaterAMBEData::getTime() const
{
	return m_time;
}

unsigned int CSplitRepeaterAMBEData::getId() const
{
	return m_id;
}

void CSplitRepeaterAMBEData::setId(unsigned int id)
{
	m_id = id;
}

unsigned int CSplitRepeaterAMBEData::getSeq() const
{
	return m_outSeq & 0x1FU;
}

void CSplitRepeaterAMBEData::setSeq(unsigned int seqNo)
{
	m_outSeq = seqNo;
}

bool CSplitRepeaterAMBEData::isEnd() const
{
	return (m_outSeq & 0x40U) == 0x40U;
}

void CSplitRepeaterAMBEData::setEnd(bool end)
{
	if (end)
		m_outSeq |= 0x40U;
	else
		m_outSeq &= ~0x40U;
}

bool CSplitRepeaterAMBEData::isSync() const
{
	return (m_outSeq & 0x1FU) == 0x00U;
}

void CSplitRepeaterAMBEData::setDestination(const in_addr& address, unsigned int port)
{
	m_address = address;
	m_port    = port;
}

in_addr CSplitRepeaterAMBEData::getAddress() const
{
	return m_address;
}

unsigned int CSplitRepeaterAMBEData::getPort() const
{
	return m_port;
}

unsigned int CSplitRepeaterAMBEData::getErrors() const
{
	return m_errors;
}

void CSplitRepeaterAMBEData::setData(const unsigned char *data, unsigned int length)
{
	wxASSERT(data != NULL);
	wxASSERT(length >= DV_FRAME_LENGTH_BYTES);

	::memcpy(m_data, data, DV_FRAME_LENGTH_BYTES);
}

unsigned int CSplitRepeaterAMBEData::getData(unsigned char *data, unsigned int length) const
{
	wxASSERT(data != NULL);
	wxASSERT(length >= DV_FRAME_LENGTH_BYTES);

	::memcpy(data, m_data, DV_FRAME_LENGTH_BYTES);

	return DV_FRAME_LENGTH_BYTES;
}
