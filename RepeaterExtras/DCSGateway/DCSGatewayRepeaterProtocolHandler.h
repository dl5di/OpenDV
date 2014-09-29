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

#ifndef	DCSGatewayRepeaterProtocolHander_H
#define	DCSGatewayRepeaterProtocolHander_H

#include "DCSGatewayHeaderData.h"
#include "DCSGatewayAMBEData.h"
#include "UDPReaderWriter.h"
#include "DStarDefines.h"

#include <wx/wx.h>

enum REPEATER_TYPE {
	RT_NONE,
	RT_HEADER,
	RT_DATA
};

class CDCSGatewayRepeaterProtocolHandler {
public:
	CDCSGatewayRepeaterProtocolHandler(const wxString& repeaterAddress, unsigned int repeaterPort, const wxString& localAddress, unsigned int localPort);
	~CDCSGatewayRepeaterProtocolHandler();

	bool open();

	bool writeHeader(const CDCSGatewayHeaderData& header);
	bool writeData(const CDCSGatewayAMBEData& data);
	bool writeText(const wxString& text, LINK_STATUS status, const wxString& reflector);

	REPEATER_TYPE read();
	CDCSGatewayHeaderData* readHeader();
	CDCSGatewayAMBEData*   readData();

	void close();

private:
	CUDPReaderWriter m_socket;
	wxUint16         m_outId;
	REPEATER_TYPE    m_type;
	wxUint16         m_inId;
	unsigned char*   m_buffer;
	unsigned int     m_length;

	bool readPackets();
};

#endif
