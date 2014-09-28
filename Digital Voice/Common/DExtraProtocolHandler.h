/*
 *   Copyright (C) 2009,2010,2011,2013 by Jonathan Naylor G4KLX
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

#ifndef	DExtraProtocolHander_H
#define	DExtraProtocolHander_H

#include "UDPReaderWriter.h"
#include "HeaderData.h"
#include "Timer.h"

#include <wx/wx.h>

enum NETWORK_TYPE {
	NETWORK_NONE,
	NETWORK_HEADER,
	NETWORK_DATA
};

class CDExtraProtocolHandler {
public:
	CDExtraProtocolHandler();
	~CDExtraProtocolHandler();

	void setCallsign(const wxString& callsign);

	bool open(const wxString& address, unsigned int port);

	bool writeHeader(const CHeaderData& header);
	bool writeData(const unsigned char* data, unsigned int length, bool end);

	NETWORK_TYPE read();
	CHeaderData* readHeader();
	unsigned int readData(unsigned char* data, unsigned int length, bool& sync, bool& end);

	void clock();

	void setModule(wxChar module);
	bool isConnected();

	void reset();

	void close();

private:
	CUDPReaderWriter* m_socket;
	unsigned char*    m_callsign;
	unsigned char     m_module;
	wxUint16          m_outId;
	wxUint8           m_outSeq;
	NETWORK_TYPE      m_type;
	wxUint16          m_inId;
	wxUint8           m_inSeq;
	unsigned char*    m_buffer;
	unsigned int      m_length;
	CTimer            m_pollTimer;
	CTimer            m_connectTimer;

	bool readPackets();
	bool writePoll();
};

#endif
