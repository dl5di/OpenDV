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

#ifndef	G2ProtocolHandler_H
#define	G2ProtocolHandler_H

#include "UDPReaderWriter.h"
#include "DStarDefines.h"
#include "HeaderData.h"
#include "AMBEData.h"

#if defined(__WINDOWS__)
#include "Inaddr.h"
#else
#include <netinet/in.h>
#endif

#include <wx/wx.h>

enum G2_TYPE {
	GT_NONE,
	GT_HEADER,
	GT_AMBE
};

class CG2ProtocolHandler {
public:
	CG2ProtocolHandler(unsigned int port, const wxString& addr = wxEmptyString);
	~CG2ProtocolHandler();

	bool open();

	bool writeHeader(const CHeaderData& header);
	bool writeAMBE(const CAMBEData& data);

	G2_TYPE read();
	CHeaderData* readHeader();
	CAMBEData*   readAMBE();

	void close();

private:
	CUDPReaderWriter m_socket;
	G2_TYPE          m_type;
	unsigned char*   m_buffer;
	unsigned int     m_length;
	in_addr          m_address;
	unsigned int     m_port;

	bool readPackets();
};

#endif
