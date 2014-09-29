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

#include "DExtraGatewayAMBEData.h"

#include "DStarDefines.h"
#include "Utils.h"

CDExtraGatewayAMBEData::CDExtraGatewayAMBEData() :
m_outSeq(0U),
m_id(0U),
m_data(NULL)
{
	m_data = new unsigned char[DV_FRAME_LENGTH_BYTES];
}

CDExtraGatewayAMBEData::CDExtraGatewayAMBEData(const CDExtraGatewayAMBEData& data) :
m_outSeq(data.m_outSeq),
m_id(data.m_id),
m_data(NULL)
{
	m_data = new unsigned char[DV_FRAME_LENGTH_BYTES];

	::memcpy(m_data, data.m_data, DV_FRAME_LENGTH_BYTES);
}

CDExtraGatewayAMBEData::~CDExtraGatewayAMBEData()
{
	delete[] m_data;
}

bool CDExtraGatewayAMBEData::setRepeaterData(const unsigned char *data, unsigned int length)
{
	wxASSERT(data != NULL);

	if (length < 21U) {
		wxLogMessage(wxT("AMBE data from the repeater is too short, %u < 22"), length);
		return false;
	}

	if (data[0] != 'D' || data[1] != 'S' || data[2] != 'R' || data[3] != 'P') {
		CUtils::dump(wxT("Invalid signature from the repeater"), data, length);
		return false;
	}

	m_id     = data[5] * 256U + data[6];
	m_outSeq = data[7];

	if (data[4] != 0x21U) {
		CUtils::dump(wxT("Invalid data type from the repeater"), data, length);
		return false;
	}

	::memcpy(m_data, data + 9U, DV_FRAME_LENGTH_BYTES);

	return true;
}

bool CDExtraGatewayAMBEData::setDExtraData(const unsigned char *data, unsigned int length)
{
	wxASSERT(data != NULL);

	if (length < 27U) {
		wxLogMessage(wxT("AMBE data from DExtra is too short, %u < 27"), length);
		return false;
	}

	if (data[0] != 'D' || data[1] != 'S' || data[2] != 'V' || data[3] != 'T') {
		CUtils::dump(wxT("Invalid signature from DExtra"), data, length);
		return false;
	}

	m_id     = data[12] * 256U + data[13];
	m_outSeq = data[14];

	if (m_outSeq == 0x80) {
		CUtils::dump(wxT("Invalid data type from DExtra"), data, length);
		return false;
	}

	::memcpy(m_data, data + 15U, DV_FRAME_LENGTH_BYTES);

	return true;
}

unsigned int CDExtraGatewayAMBEData::getRepeaterData(unsigned char *data, unsigned int length) const
{
	wxASSERT(data != NULL);
	wxASSERT(length >= 21U);

	data[0] = 'D';
	data[1] = 'S';
	data[2] = 'R';
	data[3] = 'P';

	data[4] = 0x21U;

	data[5] = m_id / 256U;		// Unique session id
	data[6] = m_id % 256U;

	data[7] = m_outSeq;

	data[8] = 0U;

	::memcpy(data + 9U, m_data, DV_FRAME_LENGTH_BYTES);

	return 21U;
}

unsigned int CDExtraGatewayAMBEData::getDExtraData(unsigned char* data, unsigned int length) const
{
	wxASSERT(data != NULL);
	wxASSERT(length >= 27U);

	data[0]  = 'D';
	data[1]  = 'S';
	data[2]  = 'V';
	data[3]  = 'T';

	data[4]  = 0x20;
	data[5]  = 0x00;
	data[6]  = 0x00;
	data[7]  = 0x00;
	data[8]  = 0x20;
	data[9]  = 0x00;
	data[10] = 0x01;
	data[11] = 0x02;

	data[12] = m_id % 256U;			// Unique session id
	data[13] = m_id / 256U;

	data[14] = m_outSeq;

	::memcpy(data + 15U, m_data, DV_FRAME_LENGTH_BYTES);

	return 27U;
}

void CDExtraGatewayAMBEData::setData(const unsigned char *data, unsigned int length)
{
	wxASSERT(data != NULL);
	wxASSERT(length >= DV_FRAME_LENGTH_BYTES);

	::memcpy(m_data, data, DV_FRAME_LENGTH_BYTES);
}

unsigned int CDExtraGatewayAMBEData::getId() const
{
	return m_id;
}

void CDExtraGatewayAMBEData::setId(unsigned int id)
{
	m_id = id;
}

unsigned int CDExtraGatewayAMBEData::getSeq() const
{
	return m_outSeq & 0x0FU;
}

void CDExtraGatewayAMBEData::setSeq(unsigned int seqNo)
{
	m_outSeq = seqNo;
}

bool CDExtraGatewayAMBEData::isEnd() const
{
	return (m_outSeq & 0x40U) == 0x40U;
}

void CDExtraGatewayAMBEData::setEnd(bool end)
{
	if (end)
		m_outSeq |= 0x40U;
	else
		m_outSeq &= ~0x40U;
}
