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

#ifndef DExtraProtocolHandlerPool_H
#define	DExtraProtocolHandlerPool_H

#include <wx/wx.h>

#include "DExtraProtocolHandler.h"

class CDExtraProtocolHandlerEntry {
public:
	CDExtraProtocolHandler* m_handler;
	unsigned int            m_port;
	bool                    m_inUse;
};

class CDExtraProtocolHandlerPool {
public:
	CDExtraProtocolHandlerPool(unsigned int n, unsigned int port, const wxString& addr = wxEmptyString);
	~CDExtraProtocolHandlerPool();

	bool open();

	CDExtraProtocolHandler* getHandler(unsigned int port = 0U);
	void release(CDExtraProtocolHandler* handler);

	DEXTRA_TYPE   read();
	CHeaderData*  readHeader();
	CAMBEData*    readAMBE();
	CPollData*    readPoll();
	CConnectData* readConnect();

	void close();

private:
	CDExtraProtocolHandlerEntry* m_pool;
	unsigned int                 m_n;
	unsigned int                 m_index;
};

#endif
