/*
 *   Copyright (C) 2010-2013 by Jonathan Naylor G4KLX
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

#include "DCSGatewayAMBEData.h"

#include "DStarDefines.h"
#include "Utils.h"

unsigned char* CDCSGatewayAMBEData::m_text = NULL;

void CDCSGatewayAMBEData::initialise()
{
	m_text = new unsigned char[SLOW_DATA_TEXT_LENGTH];

	::memset(m_text, ' ', SLOW_DATA_TEXT_LENGTH);
}

void CDCSGatewayAMBEData::finalise()
{
	delete[] m_text;
}

CDCSGatewayAMBEData::CDCSGatewayAMBEData() :
m_outSeq(0U),
m_rptSeq(0U),
m_id(0U),
m_data(NULL)
{
	m_data = new unsigned char[DV_FRAME_LENGTH_BYTES];
}

CDCSGatewayAMBEData::CDCSGatewayAMBEData(const CDCSGatewayAMBEData& data) :
m_outSeq(data.m_outSeq),
m_rptSeq(data.m_rptSeq),
m_id(data.m_id),
m_data(NULL)
{
	m_data = new unsigned char[DV_FRAME_LENGTH_BYTES];

	::memcpy(m_data, data.m_data, DV_FRAME_LENGTH_BYTES);
}

CDCSGatewayAMBEData::~CDCSGatewayAMBEData()
{
	delete[] m_data;
}

bool CDCSGatewayAMBEData::setRepeaterData(const unsigned char *data, unsigned int length)
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

bool CDCSGatewayAMBEData::setDCSData(const unsigned char *data, unsigned int length)
{
	wxASSERT(data != NULL);

	if (length < 100U) {
		wxLogMessage(wxT("AMBE data from DCS is too short, %u < 100"), length);
		return false;
	}

	if (data[0] != '0' || data[1] != '0' || data[2] != '0' || data[3] != '1') {
		CUtils::dump(wxT("Invalid signature from DCS"), data, length);
		return false;
	}

	m_rptSeq = data[60U] * 65536U + data[59U] * 256U + data[58U];
	m_id     = data[43U] * 256U + data[44U];
	m_outSeq = data[45U];

	::memcpy(m_data, data + 46U, DV_FRAME_LENGTH_BYTES);

	return true;
}

unsigned int CDCSGatewayAMBEData::getRepeaterData(unsigned char *data, unsigned int length) const
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

unsigned int CDCSGatewayAMBEData::getDCSData(unsigned char* data, unsigned int length) const
{
	wxASSERT(data != NULL);
	wxASSERT(length >= 100U);

	::memset(data, 0x00U, 100U);

	data[0]  = '0';
	data[1]  = '0';
	data[2]  = '0';
	data[3]  = '1';

	data[43U] = m_id % 256U;			// Unique session id
	data[44U] = m_id / 256U;

	data[45U] = m_outSeq;

	::memcpy(data + 46U, m_data, DV_FRAME_LENGTH_BYTES);

	if (isEnd()) {
		data[55U] = 0x55U;
		data[56U] = 0x55U;
		data[57U] = 0x55U;
	}

	data[58U] = (m_rptSeq >> 0)  & 0xFFU;
	data[59U] = (m_rptSeq >> 8)  & 0xFFU;
	data[60U] = (m_rptSeq >> 16) & 0xFFU;

	data[61U] = 0x01U;
	data[62U] = 0x00U;

	data[63U] = 0x21U;

	data[64U] = m_text[0U];
	data[65U] = m_text[1U];
	data[66U] = m_text[2U];
	data[67U] = m_text[3U];
	data[68U] = m_text[4U];
	data[69U] = m_text[5U];
	data[70U] = m_text[6U];
	data[71U] = m_text[7U];
	data[72U] = m_text[8U];
	data[73U] = m_text[9U];
	data[74U] = m_text[10U];
	data[75U] = m_text[11U];
	data[76U] = m_text[12U];
	data[77U] = m_text[13U];
	data[78U] = m_text[14U];
	data[79U] = m_text[15U];
	data[80U] = m_text[16U];
	data[81U] = m_text[17U];
	data[82U] = m_text[18U];
	data[83U] = m_text[19U];

	return  100U;
}

void CDCSGatewayAMBEData::setData(const unsigned char *data, unsigned int length)
{
	wxASSERT(data != NULL);
	wxASSERT(length >= DV_FRAME_LENGTH_BYTES);

	::memcpy(m_data, data, DV_FRAME_LENGTH_BYTES);
}

unsigned int CDCSGatewayAMBEData::getData(unsigned char *data, unsigned int length) const
{
	wxASSERT(data != NULL);
	wxASSERT(length >= DV_FRAME_LENGTH_BYTES);

	::memcpy(data, m_data, DV_FRAME_LENGTH_BYTES);

	return DV_FRAME_LENGTH_BYTES;
}

unsigned int CDCSGatewayAMBEData::getId() const
{
	return m_id;
}

void CDCSGatewayAMBEData::setId(unsigned int id)
{
	m_id = id;
}

unsigned int CDCSGatewayAMBEData::getSeq() const
{
	return m_outSeq & 0x0FU;
}

void CDCSGatewayAMBEData::setSeq(unsigned int seqNo)
{
	m_outSeq = seqNo;
}

unsigned int CDCSGatewayAMBEData::getRptSeq() const
{
	return m_rptSeq;
}

void CDCSGatewayAMBEData::setRptSeq(unsigned int seqNo)
{
	m_rptSeq = seqNo;
}

bool CDCSGatewayAMBEData::isEnd() const
{
	return (m_outSeq & 0x40U) == 0x40U;
}

void CDCSGatewayAMBEData::setEnd(bool end)
{
	if (end)
		m_outSeq |= 0x40U;
	else
		m_outSeq &= ~0x40U;
}

bool CDCSGatewayAMBEData::isSync() const
{
	return (m_outSeq & 0x1FU) == 0x00U;
}

void CDCSGatewayAMBEData::setText(const wxString& text)
{
	::memset(m_text, ' ', SLOW_DATA_TEXT_LENGTH);

	if (text.IsEmpty())
		return;

	for (unsigned int i = 0U; i < text.Len(); i++)
		m_text[i] = text.GetChar(i);
}
