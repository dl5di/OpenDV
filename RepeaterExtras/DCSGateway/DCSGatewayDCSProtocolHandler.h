/*
 *   Copyright (C) 2012,2013 by Jonathan Naylor G4KLX
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

#ifndef	DCSGatewayDCSProtocolHander_H
#define	DCSGatewayDCSProtocolHander_H

#include "DCSGatewayHeaderData.h"
#include "DCSGatewayAMBEData.h"
#include "UDPReaderWriter.h"
#include "Timer.h"

#include <wx/wx.h>

enum DCS_TYPE {
	DT_NONE,
	DT_DATA
};

class CDCSGatewayDCSProtocolHandler {
public:
	CDCSGatewayDCSProtocolHandler();
	~CDCSGatewayDCSProtocolHandler();

	void setCallsign(const wxString& callsign);

	bool link(const wxString& reflector, const wxString& address, unsigned int port);

	bool writeData(const CDCSGatewayHeaderData& header, const CDCSGatewayAMBEData& data);

	DCS_TYPE read();
	CDCSGatewayAMBEData* readData();
	CDCSGatewayHeaderData* getHeader();

	void clock(unsigned int ms);

	bool isConnected();

	void reset();

	void unlink();

	static void initialise();
	static void finalise();

private:
	static char* m_html;

	CUDPReaderWriter* m_socket;
	unsigned char*    m_callsign;
	unsigned char*    m_reflector;
	DCS_TYPE          m_type;
	unsigned char*    m_buffer;
	unsigned int      m_length;
	CTimer            m_connectTimer;
	bool              m_linked;

	bool readPackets();
	bool writeConnect();
	bool writePollReply();
	bool writeDisconnect();
};

#endif
