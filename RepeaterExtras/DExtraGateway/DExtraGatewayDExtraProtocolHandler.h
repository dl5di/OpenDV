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

#ifndef	DExtraGatewayDExtraProtocolHander_H
#define	DExtraGatewayDExtraProtocolHander_H

#include "DExtraGatewayHeaderData.h"
#include "DExtraGatewayAMBEData.h"
#include "UDPReaderWriter.h"
#include "DStarDefines.h"
#include "Timer.h"

#include <wx/wx.h>

enum DEXTRA_TYPE {
	DT_NONE,
	DT_HEADER,
	DT_DATA
};

class CDExtraGatewayDExtraProtocolHandler {
public:
	CDExtraGatewayDExtraProtocolHandler();
	~CDExtraGatewayDExtraProtocolHandler();

	void setCallsign(const wxString& callsign);

	bool link(const wxString& hostname);
	void unlink();

	bool writeHeader(const CDExtraGatewayHeaderData& header);
	bool writeData(const CDExtraGatewayAMBEData& data);

	DEXTRA_TYPE read();
	CDExtraGatewayHeaderData* readHeader();
	CDExtraGatewayAMBEData*   readData();

	void clock(unsigned int ms);

	bool isConnected();

	void close();

private:
	CUDPReaderWriter* m_socket;
	unsigned char*    m_callsign;
	DEXTRA_TYPE       m_type;
	unsigned char*    m_buffer;
	unsigned int      m_length;
	CTimer            m_pollTimer;
	CTimer            m_connectTimer;

	bool readPackets();
	bool writePoll();
};

#endif
