/*
 *   Copyright (C) 2011,2013 by Jonathan Naylor G4KLX
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

#include "TimerControlItemFile.h"

#include <wx/ffile.h>
#include <wx/file.h>

const unsigned int RECORD_LENGTH = 32U;

CTimerControlItemFile::CTimerControlItemFile(const wxString& fileName) :
m_fileName(fileName),
m_mutex()
{
}

CTimerControlItemFile::~CTimerControlItemFile()
{
}

bool CTimerControlItemFile::writeItems(CTimerControlItem** items, unsigned int count)
{
	wxMutexLocker locker(m_mutex);

	wxFFile file;
	bool ret = file.Open(m_fileName, wxT("wb"));
	if (!ret) {
		wxLogError(wxT("Cannot open %s for writing"), m_fileName.c_str());
		return false;
	}

	file.Write("TC1", 3U);

	for (unsigned int i = 0U; i < count; i++) {
		unsigned char buffer[RECORD_LENGTH];
		::memset(buffer, ' ', RECORD_LENGTH);

		wxString repeater = items[i]->m_repeater;
		for (unsigned int j = 0U; j < repeater.Len(); j++)
			buffer[j + 0U] = repeater.GetChar(j);

		wxUint32 day = wxUint32(items[i]->m_day);
		::memcpy(buffer + 8U, &day, sizeof(wxUint32));

		wxUint32 hour = wxUint32(items[i]->m_hour);
		::memcpy(buffer + 12U, &hour, sizeof(wxUint32));

		wxUint32 minute = wxUint32(items[i]->m_minute);
		::memcpy(buffer + 16U, &minute, sizeof(wxUint32));

		wxString reflector = items[i]->m_reflector;
		for (unsigned int j = 0U; j < reflector.Len(); j++)
			buffer[j + 20U] = reflector.GetChar(j);

		wxUint32 reconnect = wxUint32(items[i]->m_reconnect);
		::memcpy(buffer + 28U, &reconnect, sizeof(wxUint32));

		file.Write(buffer, RECORD_LENGTH);
	}

	file.Close();

	return true;
}

CTimerControlItem** CTimerControlItemFile::readItems(unsigned int& count)
{
	wxMutexLocker locker(m_mutex);

	count = 0U;

	bool exists = wxFile::Exists(m_fileName);
	if (!exists)
		return NULL;

	wxFFile file;
	bool ret = file.Open(m_fileName, wxT("rb"));
	if (!ret)
		return NULL;

	unsigned int length = file.Length();
	if (length < 3U) {
		file.Close();
		return NULL;
	}

	count = (length - 3U) / RECORD_LENGTH;

	unsigned char buffer[RECORD_LENGTH];
	file.Read(buffer, 3U);

	if (::memcmp(buffer, "TC1", 3U) != 0) {
		file.Close();
		return NULL;
	}

	CTimerControlItem** items = new CTimerControlItem*[count];

	for (unsigned int i = 0U; i < count; i++) {
		file.Read(buffer, RECORD_LENGTH);

		wxString repeater   = wxString((char*)(buffer + 0U), wxConvLocal, 8U);
		wxUint32* day       = (wxUint32*)(buffer + 8U);
		wxUint32* hour      = (wxUint32*)(buffer + 12U);
		wxUint32* minute    = (wxUint32*)(buffer + 16U);
		wxString reflector  = wxString((char*)(buffer + 20U), wxConvLocal, 8U);
		wxUint32* reconnect = (wxUint32*)(buffer + 28U);

		CTimerControlItem* item = new CTimerControlItem;
		item->m_repeater  = repeater;
		item->m_day       = *day;
		item->m_hour      = *hour;
		item->m_minute    = *minute;
		item->m_reflector = reflector.Trim();
		item->m_reconnect = RECONNECT(*reconnect);
		items[i] = item;
	}

	file.Close();

	return items;
}
