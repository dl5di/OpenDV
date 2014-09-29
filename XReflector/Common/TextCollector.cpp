/*
 *   Copyright (C) 2010,2011 by Jonathan Naylor G4KLX
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

#include "TextCollector.h"
#include "DStarDefines.h"
#include "Utils.h"

const unsigned int TEXT_DATA_LENGTH       = 20U;
const unsigned int SLOW_DATA_BLOCK_LENGTH = 6U;

CTextCollector::CTextCollector() :
m_data(NULL),
m_buffer(NULL),
m_slowData(SS_FIRST),
m_has0(false),
m_has1(false),
m_has2(false),
m_has3(false)
{
	m_data   = new char[TEXT_DATA_LENGTH];
	m_buffer = new unsigned char[SLOW_DATA_BLOCK_LENGTH];
}

CTextCollector::~CTextCollector()
{
	delete[] m_data;
	delete[] m_buffer;
}

void CTextCollector::writeData(const CAMBEData& data)
{
	if (data.isSync()) {
		sync();
		return;
	}

	unsigned char buffer[DV_FRAME_MAX_LENGTH_BYTES];
	data.getData(buffer, DV_FRAME_MAX_LENGTH_BYTES);

	switch (m_slowData) {
		case SS_FIRST:
			m_buffer[0U] = buffer[VOICE_FRAME_LENGTH_BYTES + 0U] ^ SCRAMBLER_BYTE1;
			m_buffer[1U] = buffer[VOICE_FRAME_LENGTH_BYTES + 1U] ^ SCRAMBLER_BYTE2;
			m_buffer[2U] = buffer[VOICE_FRAME_LENGTH_BYTES + 2U] ^ SCRAMBLER_BYTE3;
			m_slowData = SS_SECOND;
			return;

		case SS_SECOND:
			m_buffer[3U] = buffer[VOICE_FRAME_LENGTH_BYTES + 0U] ^ SCRAMBLER_BYTE1;
			m_buffer[4U] = buffer[VOICE_FRAME_LENGTH_BYTES + 1U] ^ SCRAMBLER_BYTE2;
			m_buffer[5U] = buffer[VOICE_FRAME_LENGTH_BYTES + 2U] ^ SCRAMBLER_BYTE3;
			m_slowData = SS_FIRST;
			break;
	}

	switch (m_buffer[0U]) {
		case SLOW_DATA_TYPE_TEXT | 0U:
			m_data[0U] = m_buffer[1U] & 0x7FU;
			m_data[1U] = m_buffer[2U] & 0x7FU;
			m_data[2U] = m_buffer[3U] & 0x7FU;
			m_data[3U] = m_buffer[4U] & 0x7FU;
			m_data[4U] = m_buffer[5U] & 0x7FU;
			m_has0 = true;
			break;
		case SLOW_DATA_TYPE_TEXT | 1U:
			m_data[5U] = m_buffer[1U] & 0x7FU;
			m_data[6U] = m_buffer[2U] & 0x7FU;
			m_data[7U] = m_buffer[3U] & 0x7FU;
			m_data[8U] = m_buffer[4U] & 0x7FU;
			m_data[9U] = m_buffer[5U] & 0x7FU;
			m_has1 = true;
			break;
		case SLOW_DATA_TYPE_TEXT | 2U:
			m_data[10U] = m_buffer[1U] & 0x7FU;
			m_data[11U] = m_buffer[2U] & 0x7FU;
			m_data[12U] = m_buffer[3U] & 0x7FU;
			m_data[13U] = m_buffer[4U] & 0x7FU;
			m_data[14U] = m_buffer[5U] & 0x7FU;
			m_has2 = true;
			break;
		case SLOW_DATA_TYPE_TEXT | 3U:
			m_data[15U] = m_buffer[1U] & 0x7FU;
			m_data[16U] = m_buffer[2U] & 0x7FU;
			m_data[17U] = m_buffer[3U] & 0x7FU;
			m_data[18U] = m_buffer[4U] & 0x7FU;
			m_data[19U] = m_buffer[5U] & 0x7FU;
			m_has3 = true;
			break;
		default:
			break;
	}
}

void CTextCollector::reset()
{
	m_slowData = SS_FIRST;
	m_has0 = false;
	m_has1 = false;
	m_has2 = false;
	m_has3 = false;
}

void CTextCollector::sync()
{
	m_slowData = SS_FIRST;
}

bool CTextCollector::hasData() const
{
	return m_has0 && m_has1 && m_has2 && m_has3;
}

wxString CTextCollector::getData()
{
	wxString text;

	if (!m_has0 || !m_has1 || !m_has2 || !m_has3)
		return text;

	m_has0 = false;
	m_has1 = false;
	m_has2 = false;
	m_has3 = false;

	return wxString(m_data, wxConvLocal, TEXT_DATA_LENGTH);
}
