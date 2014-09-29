/*
 *   Copyright (C) 2010,2011,2012 by Jonathan Naylor G4KLX
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

#ifndef	APRSWriterThread_H
#define	APRSWriterThread_H

#include "TCPReaderWriterClient.h"
#include "RingBuffer.h"

#include <wx/wx.h>

class CAPRSWriterThread : public wxThread {
public:
	CAPRSWriterThread(const wxString& callsign, const wxString& address, const wxString& hostname, unsigned int port);
	virtual ~CAPRSWriterThread();

	virtual bool start();

	virtual bool isConnected() const;

	virtual void write(const char* data);

	virtual void* Entry();

	virtual void stop();

private:
	char*                  m_username;
	CTCPReaderWriterClient m_socket;
	CRingBuffer<char*>     m_queue;
	bool                   m_exit;
	bool                   m_connected;

	bool connect();
	unsigned int getAPRSPassword(const char* username) const;
};

#endif
