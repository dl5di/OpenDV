/*
 *   Copyright (C) 2011 by Jonathan Naylor G4KLX
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

#ifndef TCPReaderWriterServer_H
#define TCPReaderWriterServer_H

#include "TCPReaderWriterClient.h"

#include <wx/wx.h>

#if !defined(__WINDOWS__)
#include <netdb.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <errno.h>
#endif

class CTCPReaderWriterServer : public wxThread {
public:
	CTCPReaderWriterServer(const wxString& address, unsigned int port);
	virtual ~CTCPReaderWriterServer();

	virtual bool start();

	virtual bool write(const unsigned char* buffer, unsigned int length);
	virtual int  read(unsigned char* buffer, unsigned int length, unsigned int secs);

	virtual void stop();

	virtual void* Entry();

private:
	wxString                m_address;
	unsigned short          m_port;
	int                     m_fd;
	CTCPReaderWriterClient* m_client;
	bool                    m_stopped;

	bool    open();
	int     accept();
	void    close();
	in_addr lookup(const wxString& hostname) const;
};

#endif
