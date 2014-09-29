/*
 *   Copyright (C) 2009,2012 by Jonathan Naylor G4KLX
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

#ifndef	ParrotControllerProtocolHander_H
#define	ParrotControllerProtocolHander_H

#include "UDPReaderWriter.h"
#include "DStarDefines.h"
#include "HeaderData.h"

#include <wx/wx.h>

class CParrotControllerProtocolHandler {
public:
	CParrotControllerProtocolHandler(const wxString& repeaterAddress, unsigned int repeaterPort, const wxString& localAddress, unsigned int localPort);
	~CParrotControllerProtocolHandler();

	bool open();

	bool writeHeader(const CHeaderData& header);
	bool writeData(const unsigned char* data, unsigned int length, bool end);

	NETWORK_TYPE read();
	CHeaderData* readHeader();
	unsigned int readData(unsigned char* data, unsigned int length, bool& end);

	void close();

private:
	CUDPReaderWriter m_socket;
	wxUint16         m_outId;
	wxUint8          m_outSeq;
	NETWORK_TYPE     m_type;
	unsigned char*   m_buffer;
	unsigned int     m_length;

	bool readPackets();
};

#endif
