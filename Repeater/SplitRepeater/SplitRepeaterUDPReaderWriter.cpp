/*
 *   Copyright (C) 2006-2012 by Jonathan Naylor G4KLX
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

#include "SplitRepeaterUDPReaderWriter.h"
#include "Utils.h"

CSplitRepeaterUDPReaderWriter::CSplitRepeaterUDPReaderWriter(const wxString& address, unsigned int port) :
m_address(address),
m_port(port),
m_remAddr(),
m_fd(-1)
{
	wxASSERT(port > 0U);
}

CSplitRepeaterUDPReaderWriter::~CSplitRepeaterUDPReaderWriter()
{
}

bool CSplitRepeaterUDPReaderWriter::open()
{
#if defined(__WINDOWS__)
	WSAData data;

	int wsaRet =  ::WSAStartup(0x101, &data);
	if (wsaRet != 0)
		return false;
#endif

	m_fd = ::socket(PF_INET, SOCK_DGRAM, 0);
	if (m_fd < 0) {
		wxLogError(wxT("Cannot create the socket"));
		return false;
	}

	sockaddr_in addr;
	::memset(&addr, 0x00, sizeof(sockaddr_in));
	addr.sin_family      = AF_INET;
	addr.sin_port        = htons(m_port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (!m_address.IsEmpty()) {
#if defined(__WINDOWS__)
		addr.sin_addr.s_addr = ::inet_addr(m_address.mb_str());
#else
		addr.sin_addr.s_addr = ::inet_addr(m_address.mb_str());
#endif
		if (addr.sin_addr.s_addr == INADDR_NONE) {
			wxLogError(wxT("The local address is invalid - %s"), m_address.c_str());
			return false;
		}
	}

	int reuse = 1;
	if (::setsockopt(m_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse)) == -1) {
		wxLogError(wxT("Cannot set the socket option"));
		return false;
	}

	if (::bind(m_fd, (sockaddr*)&addr, sizeof(sockaddr_in)) == -1) {
		wxLogError(wxT("Cannot bind the local address"));
		return false;
	}

	return true;
}

int CSplitRepeaterUDPReaderWriter::read(unsigned char* buffer, unsigned int length, in_addr& address, unsigned int& port)
{
	// Check that the readfrom() won't block
	fd_set readFds;
	FD_ZERO(&readFds);
#if defined(__WINDOWS__)
	FD_SET((unsigned int)m_fd, &readFds);
#else
	FD_SET(m_fd, &readFds);
#endif

	// Return immediately
	timeval tv;
	tv.tv_sec  = 0L;
	tv.tv_usec = 0L;

	int ret = ::select(m_fd + 1, &readFds, NULL, NULL, &tv);
	if (ret < 0) {
		wxLogError(wxT("Error returned from select()"));
		return -1;
	}

	if (ret == 0)
		return 0;

	sockaddr_in addr;
#if defined(__WINDOWS__)
	int size = sizeof(sockaddr_in);
#else
	socklen_t size = sizeof(sockaddr_in);
#endif

	ssize_t len = ::recvfrom(m_fd, (char*)buffer, length, 0, (sockaddr *)&addr, &size);
	if (len < 0) {
		wxLogError(wxT("Error returned from recvfrom()"));
		return -1;
	}

	address = addr.sin_addr;
	port    = ntohs(addr.sin_port);

	return len;
}

bool CSplitRepeaterUDPReaderWriter::write(const unsigned char* buffer, unsigned int length, const in_addr& address, unsigned int port)
{
	sockaddr_in addr;
	::memset(&addr, 0x00, sizeof(sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_port   = htons(port);
	addr.sin_addr   = address;

	ssize_t ret = ::sendto(m_fd, (char *)buffer, length, 0, (sockaddr *)&addr, sizeof(sockaddr_in));
	if (ret < 0) {
		wxLogError(wxT("Error returned from sendto()"));
		return false;
	}

	if (ret != ssize_t(length))
		return false;

	return true;
}

void CSplitRepeaterUDPReaderWriter::close()
{
#if defined(__WINDOWS__)
	::closesocket(m_fd);
	::WSACleanup();
#else
	::close(m_fd);
#endif
}
