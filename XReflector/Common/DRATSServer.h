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

#ifndef DRATSServer_H
#define DRATSServer_H

#include "TCPReaderWriterServer.h"
#include "RepeaterCallback.h"
#include "HeaderData.h"
#include "AMBEData.h"
#include "Defs.h"

#include <wx/wx.h>

class CDRATSServer : public wxThread {
public:
	CDRATSServer(const wxString& address, unsigned int port, const wxString& callsign, IRepeaterCallback* handler);
	virtual ~CDRATSServer();

	virtual bool open();

	virtual void writeHeader(const CHeaderData& header);
	virtual void writeData(const CAMBEData& data);
	virtual void writeEnd();

	virtual void close();

	virtual void* Entry();

private:
	wxString                   m_address;
	unsigned int               m_port;
	wxString                   m_callsign;
	IRepeaterCallback*         m_handler;
	CTCPReaderWriterServer*    m_socket;
	bool                       m_stopped;
	SLOWDATA_STATE             m_readState;
	unsigned char*             m_readBuffer;
	unsigned int               m_readLength;
	unsigned int               m_readPos;
	bool                       m_readEnd;
	unsigned char*             m_writeText;
	SLOWDATA_STATE             m_writeState;
	unsigned char*             m_writeBuffer;
	unsigned int               m_writeLength;

	void serviceSocket();
};

#endif
