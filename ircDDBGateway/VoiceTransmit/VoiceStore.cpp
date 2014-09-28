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
#include "VoiceStore.h"

CVoiceStore::CVoiceStore(const wxArrayString& filenames) :
m_filenames(filenames),
m_header(NULL),
m_fileNumber(0U),
m_file()
{
}

CVoiceStore::~CVoiceStore()
{
}

bool CVoiceStore::open()
{
	wxString fileName = m_filenames.Item(0U);

	bool ret = m_file.open(fileName);
	if (!ret)
		return false;

	DVTFR_TYPE type = m_file.read();
	if (type != DVTFR_HEADER) {
		m_file.close();
		return false;
	}

	m_header = m_file.readHeader();
	m_fileNumber = 0U;

	return true;
}

CHeaderData* CVoiceStore::getHeader()
{
	CHeaderData* header = m_header;

	m_header = NULL;

	return header;
}

CAMBEData* CVoiceStore::getAMBE()
{
	DVTFR_TYPE type = m_file.read();
	if (type == DVTFR_DATA) {
		CAMBEData* ambe = m_file.readData();
		if (ambe != NULL)
			return ambe;
	}

	for (;;) {
		m_file.close();
		m_fileNumber++;

		// The end of the last file?
		if (m_fileNumber == m_filenames.GetCount())
			return NULL;

		wxString filename = m_filenames.Item(m_fileNumber);

		bool ret = m_file.open(filename);
		if (!ret)
			return NULL;

		// This should get the header
		type = m_file.read();

		if (type == DVTFR_HEADER) {
			// Throw the header away
			CHeaderData* header = m_file.readHeader();
			delete header;
		} else if (type == DVTFR_DATA) {
			// Shouldn't usually happen
			CAMBEData* ambe = m_file.readData();
			if (ambe != NULL)
				return ambe;
			else
				continue;
		} else {
			// !!!!
			continue;
		}

		// This should get the first data record
		type = m_file.read();

		if (type == DVTFR_DATA) {
			CAMBEData* ambe = m_file.readData();
			if (ambe != NULL)
				return ambe;
		}
	}
}

void CVoiceStore::close()
{
	m_file.close();
}
