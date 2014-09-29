/*
 *   Copyright (C) 2006-2013 by Jonathan Naylor G4KLX
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

#include "UDPReaderWriter.h"

#if !defined(__WINDOWS__)
#include <cerrno>
#endif


CUDPReaderWriter::CUDPReaderWriter(const wxString& address, unsigned int port) :
m_address(address),
m_port(port),
m_addr(),
m_fd(-1)
{
#if defined(__WINDOWS__)
	WSAData data;
	int wsaRet = ::WSAStartup(MAKEWORD(2, 2), &data);
	if (wsaRet != 0)
		wxLogError(wxT("Error from WSAStartup"));
#endif
}

CUDPReaderWriter::~CUDPReaderWriter()
{
#if defined(__WINDOWS__)
	::WSACleanup();
#endif
}

in_addr CUDPReaderWriter::lookup(const wxString& hostname)
{
	in_addr addr;
#if defined(WIN32)
	unsigned long address = ::inet_addr(hostname.mb_str());
	if (address != INADDR_NONE && address != INADDR_ANY) {
		addr.s_addr = address;
		return addr;
	}

	struct hostent* hp = ::gethostbyname(hostname.mb_str());
	if (hp != NULL) {
		::memcpy(&addr, hp->h_addr_list[0], sizeof(struct in_addr));
		return addr;
	}

	wxLogError(wxT("Cannot find address for host %s"), hostname.c_str());

	addr.s_addr = INADDR_NONE;
	return addr;
#else
	in_addr_t address = ::inet_addr(hostname.mb_str());
	if (address != in_addr_t(-1)) {
		addr.s_addr = address;
		return addr;
	}

	struct hostent* hp = ::gethostbyname(hostname.mb_str());
	if (hp != NULL) {
		::memcpy(&addr, hp->h_addr_list[0], sizeof(struct in_addr));
		return addr;
	}

	wxLogError(wxT("Cannot find address for host %s"), hostname.c_str());

	addr.s_addr = INADDR_NONE;
	return addr;
#endif
}

bool CUDPReaderWriter::open()
{
	m_fd = ::socket(PF_INET, SOCK_DGRAM, 0);
	if (m_fd < 0) {
#if defined(__WINDOWS__)
		wxLogError(wxT("Cannot create the UDP socket, err: %d"), ::GetLastError());
#else
		wxLogError(wxT("Cannot create the UDP socket, err: %d"), errno);
#endif
		return false;
	}

	if (m_port > 0U) {
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
				wxLogError(wxT("The address is invalid - %s"), m_address.c_str());
				return false;
			}
		}

		int reuse = 1;
		if (::setsockopt(m_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse)) == -1) {
#if defined(__WINDOWS__)
			wxLogError(wxT("Cannot set the UDP socket option (port: %u), err: %d"), m_port, ::GetLastError());
#else
			wxLogError(wxT("Cannot set the UDP socket option (port: %u), err: %d"), m_port, errno);
#endif
			return false;
		}

		if (::bind(m_fd, (sockaddr*)&addr, sizeof(sockaddr_in)) == -1) {
#if defined(__WINDOWS__)
			wxLogError(wxT("Cannot bind the UDP address (port: %u), err: %d"), m_port, ::GetLastError());
#else
			wxLogError(wxT("Cannot bind the UDP address (port: %u), err: %d"), m_port, errno);
#endif
			return false;
		}
	}

	return true;
}

int CUDPReaderWriter::read(unsigned char* buffer, unsigned int length, in_addr& address, unsigned int& port)
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
#if defined(__WINDOWS__)
		wxLogError(wxT("Error returned from UDP select (port: %u), err: %d"), m_port, ::GetLastError());
#else
		wxLogError(wxT("Error returned from UDP select (port: %u), err: %d"), m_port, errno);
#endif
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
	if (len <= 0) {
#if defined(__WINDOWS__)
		wxLogError(wxT("Error returned from recvfrom (port: %u), err: %d"), m_port, ::GetLastError());
#else
		wxLogError(wxT("Error returned from recvfrom (port: %u), err: %d"), m_port, errno);
#endif
		return -1;
	}

	address = addr.sin_addr;
	port    = ntohs(addr.sin_port);

	return len;
}

bool CUDPReaderWriter::write(const unsigned char* buffer, unsigned int length, const in_addr& address, unsigned int port)
{
	sockaddr_in addr;
	::memset(&addr, 0x00, sizeof(sockaddr_in));

	addr.sin_family = AF_INET;
	addr.sin_addr   = address;
	addr.sin_port   = htons(port);

	ssize_t ret = ::sendto(m_fd, (char *)buffer, length, 0, (sockaddr *)&addr, sizeof(sockaddr_in));
	if (ret < 0) {
#if defined(__WINDOWS__)
		wxLogError(wxT("Error returned from sendto (port: %u), err: %d"), m_port, ::GetLastError());
#else
		wxLogError(wxT("Error returned from sendto (port: %u), err: %d"), m_port, errno);
#endif
		return false;
	}

	if (ret != ssize_t(length))
		return false;

	return true;
}

void CUDPReaderWriter::close()
{
#if defined(__WINDOWS__)
	::closesocket(m_fd);
#else
	::close(m_fd);
#endif
}

unsigned int CUDPReaderWriter::getPort() const
{
	return m_port;
}
