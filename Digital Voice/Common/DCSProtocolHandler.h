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

#ifndef	DCSProtocolHander_H
#define	DCSProtocolHander_H

#include "UDPReaderWriter.h"
#include "HeaderData.h"
#include "Timer.h"

#include <wx/wx.h>

enum NETWORK_TYPE {
	NETWORK_NONE,
	NETWORK_DATA
};

class CDCSProtocolHandler {
public:
	CDCSProtocolHandler();
	~CDCSProtocolHandler();

	void setCallsign(const wxString& callsign);
	void setText(const wxString& text);

	bool open(const wxString& reflector, const wxString& address, unsigned int port);

	void startTX();

	bool writeData(const unsigned char* data, unsigned int length, bool end);

	NETWORK_TYPE read();
	unsigned int readData(unsigned char* data, unsigned int length, bool& start, bool& sync, bool& end);
	CHeaderData* getHeader();

	void clock();

	bool isConnected();

	void reset();

	void close();

private:
	CUDPReaderWriter* m_socket;
	unsigned char*    m_callsign;
	unsigned char*    m_text;
	unsigned char*    m_reflector;
	char*             m_html;
	wxUint16          m_outId;
	wxUint8           m_outSeq;
	wxUint32          m_outRptSeq;
	NETWORK_TYPE      m_type;
	wxUint16          m_inId;
	wxUint32          m_inRptSeq;
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
