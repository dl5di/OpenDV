/*
 *   Copyright (C) 2009,2011,2012,2013 by Jonathan Naylor G4KLX
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

#ifndef	SplitRepeaterProtocolHander_H
#define	SplitRepeaterProtocolHander_H

#include "SplitRepeaterUDPReaderWriter.h"
#include "SplitRepeaterHeaderData.h"
#include "SplitRepeaterAMBEData.h"
#include "DStarDefines.h"

#include <wx/wx.h>

class CSplitRepeaterProtocolHandler {
public:
	CSplitRepeaterProtocolHandler(const wxString& address, unsigned int port);
	~CSplitRepeaterProtocolHandler();

	static void initialise();

	static unsigned int createId();

	bool open();

	bool writeHeader(const CSplitRepeaterHeaderData& header);
	bool writeBusyHeader(const CSplitRepeaterHeaderData& header);
	bool writeData(const CSplitRepeaterAMBEData& data);
	bool writeBusyData(const CSplitRepeaterAMBEData& data);
	bool writePoll(const wxString& text, const in_addr& address, unsigned int port);

	NETWORK_TYPE read();
	void         readText(wxString& text, LINK_STATUS& status, wxString& reflector);
	void         readTempText(wxString& text);
	wxString     readStatus1();
	wxString     readStatus2();
	wxString     readStatus3();
	wxString     readStatus4();
	wxString     readStatus5();
	CSplitRepeaterHeaderData* readHeader();
	CSplitRepeaterAMBEData*   readData();

	void close();

private:
	CSplitRepeaterUDPReaderWriter m_socket;
	NETWORK_TYPE                  m_type;
	unsigned char*                m_buffer;
	unsigned int                  m_length;
	in_addr                       m_address;
	unsigned int                  m_port;

	bool readPackets();
};

#endif
