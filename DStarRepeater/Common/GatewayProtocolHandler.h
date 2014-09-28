/*
 *   Copyright (C) 2009-2014 by Jonathan Naylor G4KLX
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

#ifndef	GatewayProtocolHander_H
#define	GatewayProtocolHander_H

#include "UDPReaderWriter.h"
#include "DStarDefines.h"

#include <wx/wx.h>

class CGatewayProtocolHandler {
public:
	CGatewayProtocolHandler(const wxString& localAddress, unsigned int localPort);
	~CGatewayProtocolHandler();

	bool open();

	bool writeHeader(const unsigned char* header, wxUint16 id, const in_addr& address, unsigned int port);
	bool writeData(const unsigned char* data, unsigned int length, wxUint16 id, wxUint8 seqNo, const in_addr& address, unsigned int port);

	NETWORK_TYPE read(wxUint16& id, in_addr& address, unsigned int& port);
	unsigned int readHeader(unsigned char* data, unsigned int length);
	unsigned int readData(unsigned char* data, unsigned int length, wxUint8& seqNo, unsigned int& errors);
	unsigned int readRegister(wxString& name);

	void close();

private:
	CUDPReaderWriter m_socket;
	NETWORK_TYPE     m_type;
	unsigned char*   m_buffer;
	unsigned int     m_length;

	bool readPackets(wxUint16& id, in_addr& address, unsigned int& port);
};

#endif
