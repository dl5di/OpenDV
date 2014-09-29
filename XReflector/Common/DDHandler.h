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

#ifndef	DDHandler_H
#define	DDHandler_H

#include "HeaderLogger.h"
#include "DDData.h"
#include "IRCDDB.h"
#include "Timer.h"

#include <wx/wx.h>

class CEthernet {
public:
	CEthernet(const unsigned char* address, const wxString& callsign);
	~CEthernet();

	unsigned char* getAddress() const;
	wxString       getCallsign() const;

private:
	unsigned char* m_address;
	wxString       m_callsign;
};

class CDDHandler {
public:
	static void initialise(unsigned int maxRoutes, const wxString& name);

	static void setLogging(bool enabled, const wxString& dir);
	static void setHeaderLogger(CHeaderLogger* logger);
	static void setIRC(CIRCDDB* irc);

	static void process(CDDData& data);

	static CDDData* read();

	static void clock(unsigned int ms);

	static void finalise();

private:
	static CIRCDDB*       m_irc;
	static CHeaderLogger* m_headerLogger;
	static int            m_fd;
	static unsigned int   m_maxRoutes;
	static CEthernet**    m_list;
	static unsigned char* m_buffer;
	static bool           m_logEnabled;
	static wxString       m_logDir;
	static wxString       m_name;
	static CTimer         m_timer;

	static void writeStatus(const CEthernet& ethernet);
};

#endif
