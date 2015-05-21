/*
 *   Copyright (C) 2013,2015 by Jonathan Naylor G4KLX
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

#include "AnnouncementUnit.h"
#include "Log.h"

#include <algorithm>

#include <cstdio>
#include <cassert>

const std::string GLOBAL_FILE_NAME = "Announce";

CAnnouncementUnit::CAnnouncementUnit(IAnnouncementCallback* handler, const std::string& callsign) :
m_handler(handler),
m_localFileName(),
m_reader(),
m_writer(),
m_time(),
m_out(0U),
m_sending(false)
{
	assert(handler != NULL);

	m_localFileName  = "Announce ";
	m_localFileName += callsign;

#if !defined(WIN32)
	std::replace(m_localFileName.begin(), m_localFileName.end(), ' ', '_');
#endif

	m_writer.setDirectory(wxFileName::GetHomeDir());
}

CAnnouncementUnit::~CAnnouncementUnit()
{
}

bool CAnnouncementUnit::writeHeader(const CHeaderData& header)
{
	return m_writer.open(m_localFileName, header);
}

bool CAnnouncementUnit::writeData(const unsigned char* data, unsigned int length, bool end)
{
	bool ret = m_writer.write(data, length);
	if (!ret)
		return false;

	if (end)
		m_writer.close();

	return true;
}

void CAnnouncementUnit::deleteAnnouncement()
{
	wxFileName fileName(wxFileName::GetHomeDir(), m_localFileName, "dvtool");

	if (wxFile::Exists(fileName.GetFullPath()))
		::remove(fileName.GetFullPath());
}

void CAnnouncementUnit::startAnnouncement()
{
	wxFileName fileName1(wxFileName::GetHomeDir(), m_localFileName, "dvtool");
	wxFileName fileName2(wxFileName::GetHomeDir(), GLOBAL_FILE_NAME, "dvtool");

	if (wxFile::Exists(fileName1.GetFullPath())) {
		bool ret = m_reader.open(fileName1.GetFullPath());
		if (!ret)
			return;
	} else if (wxFile::Exists(fileName2.GetFullPath())) {
		bool ret = m_reader.open(fileName2.GetFullPath());
		if (!ret)
			return;
	} else {
		return;
	}

	DVTFR_TYPE type = m_reader.read();
	if (type != DVTFR_HEADER) {
		LogError("Invalid header element in the file - %d", int(type));
		m_reader.close();
		return;
	}

	CHeaderData* header = m_reader.readHeader();
	if (header == NULL) {
		LogError("NULL header element in the file");
		m_reader.close();
		return;
	}

	// Remove the repeater bit
	header->setFlag1(header->getFlag1() & ~REPEATER_MASK);

	m_handler->transmitAnnouncementHeader(header);

	m_time.start();

	m_out = 0U;
	m_sending = true;
}

void CAnnouncementUnit::clock()
{
	if (!m_sending)
		return;

	unsigned int needed = m_time.elapsed() / DSTAR_FRAME_TIME_MS;

	while (m_out < needed) {
		DVTFR_TYPE type = m_reader.read();
		if (type != DVTFR_DATA) {
			LogError("Invalid data element in the file - %d", int(type));
			m_handler->transmitAnnouncementData(END_PATTERN_BYTES, DV_FRAME_LENGTH_BYTES, true);
			m_reader.close();
			m_sending = false;
			m_out = 0U;
			return;
		}

		unsigned char data[DV_FRAME_MAX_LENGTH_BYTES];
		bool end;
		m_reader.readData(data, DV_FRAME_MAX_LENGTH_BYTES, end);

		m_handler->transmitAnnouncementData(data, DV_FRAME_LENGTH_BYTES, false);
		m_out++;

		if (end) {
			m_handler->transmitAnnouncementData(END_PATTERN_BYTES, DV_FRAME_LENGTH_BYTES, true);
			m_reader.close();
			m_sending = false;
			m_out = 0U;
			return;
		}
	}
}
