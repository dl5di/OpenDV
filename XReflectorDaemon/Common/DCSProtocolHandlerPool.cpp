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

#include "DCSProtocolHandlerPool.h"

CDCSProtocolHandlerPool::CDCSProtocolHandlerPool(unsigned int n, unsigned int port, const wxString& addr) :
m_pool(NULL),
m_n(n),
m_index(0U)
{
	wxASSERT(port > 0U);
	wxASSERT(n > 0U);

	m_pool = new CDCSProtocolHandlerEntry[n];

	for (unsigned int i = 0U; i < n; i++) {
		m_pool[i].m_handler = new CDCSProtocolHandler(port + i, addr);
		m_pool[i].m_port    = port + i;
		m_pool[i].m_inUse   = false;
	}

	wxLogMessage(wxT("Allocated UDP ports %u-%u to DCS"), port, port + n - 1U);
}

CDCSProtocolHandlerPool::~CDCSProtocolHandlerPool()
{
	for (unsigned int i = 0U; i < m_n; i++)
		delete m_pool[i].m_handler;

	delete[] m_pool;
}

bool CDCSProtocolHandlerPool::open()
{
	for (unsigned int i = 0U; i < m_n; i++) {
		bool ret = m_pool[i].m_handler->open();
		if (!ret)
			return false;
	}

	return true;
}

CDCSProtocolHandler* CDCSProtocolHandlerPool::getHandler(unsigned int port)
{
	if (port == 0U) {
		for (unsigned int i = 0U; i < m_n; i++) {
			if (!m_pool[i].m_inUse) {
				m_pool[i].m_inUse = true;
				return m_pool[i].m_handler;
			}
		}
	} else {
		for (unsigned int i = 0U; i < m_n; i++) {
			if (m_pool[i].m_port == port) {
				m_pool[i].m_inUse = true;
				return m_pool[i].m_handler;
			}
		}
	}

	wxLogError(wxT("Cannot find a free DCS port in the pool"));

	return NULL;
}

void CDCSProtocolHandlerPool::release(CDCSProtocolHandler* handler)
{
	wxASSERT(handler != NULL);

	for (unsigned int i = 0U; i < m_n; i++) {
		if (m_pool[i].m_handler == handler && m_pool[i].m_inUse) {
			m_pool[i].m_inUse = false;
			return;
		}
	}

	wxLogError(wxT("Trying to release an unused DCS port"));
}

DCS_TYPE CDCSProtocolHandlerPool::read()
{
	while (m_index < m_n) {
		if (m_pool[m_index].m_inUse) {
			DCS_TYPE type = m_pool[m_index].m_handler->read();
			if (type != DC_NONE)
				return type;
		}

		m_index++;
	}

	m_index = 0U;

	return DC_NONE;
}

CAMBEData* CDCSProtocolHandlerPool::readData()
{
	return m_pool[m_index].m_handler->readData();
}

CPollData* CDCSProtocolHandlerPool::readPoll()
{
	return m_pool[m_index].m_handler->readPoll();
}

CConnectData* CDCSProtocolHandlerPool::readConnect()
{
	return m_pool[m_index].m_handler->readConnect();
}

void CDCSProtocolHandlerPool::close()
{
	for (unsigned int i = 0U; i < m_n; i++)
		m_pool[i].m_handler->close();
}
