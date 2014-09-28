/*
 *   Copyright (C) 2013 by Jonathan Naylor G4KLX
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

#ifndef	AnnouncementUnit_H
#define	AnnouncementUnit_H

#include "AnnouncementCallback.h"
#include "DVTOOLFileWriter.h"
#include "DVTOOLFileReader.h"
#include "DStarDefines.h"
#include "HeaderData.h"

#include <wx/wx.h>

class CAnnouncementUnit {
public:
	CAnnouncementUnit(IAnnouncementCallback* handler, const wxString& callsign);
	~CAnnouncementUnit();

	bool writeHeader(const CHeaderData& header);
	bool writeData(const unsigned char* data, unsigned int length, bool end);

	void deleteAnnouncement();

	void startAnnouncement();

	void clock();

private:
	IAnnouncementCallback* m_handler;
	wxString               m_localFileName;
	CDVTOOLFileReader      m_reader;
	CDVTOOLFileWriter      m_writer;
	wxStopWatch            m_time;
	unsigned int           m_out;
	bool                   m_sending;
};

#endif
