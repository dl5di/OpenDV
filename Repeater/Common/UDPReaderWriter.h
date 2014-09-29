/*
 *   Copyright (C) 2009,2011,2012 by Jonathan Naylor G4KLX
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

#ifndef UDPReaderWriter_H
#define UDPReaderWriter_H

#include <wx/wx.h>

#if !defined(__WINDOWS__)
#include <netdb.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#endif

class CUDPReaderWriter {
public:
	CUDPReaderWriter(const wxString& remoteAddress, unsigned int remotePort, const wxString& localAddress = wxEmptyString, unsigned int localPort = 0U);
	~CUDPReaderWriter();

	static in_addr lookup(const wxString& hostName);

	bool open();

	int  read(unsigned char* buffer, unsigned int length);
	bool write(const unsigned char* buffer, unsigned int length);

	void close();

private:
	wxString       m_remoteAddress;
	unsigned short m_remotePort;
	wxString       m_localAddress;
	unsigned short m_localPort;
	sockaddr_in    m_remAddr;
	int            m_fd;
};

#endif
