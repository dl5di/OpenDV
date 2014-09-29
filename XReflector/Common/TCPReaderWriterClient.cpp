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

#include "TCPReaderWriterClient.h"
#include "UDPReaderWriter.h"

#if !defined(__WINDOWS__)
#include <cerrno>
#endif


CTCPReaderWriterClient::CTCPReaderWriterClient(const wxString& address, unsigned int port, const wxString& localAddress) :
m_address(address),
m_port(port),
m_localAddress(localAddress),
m_fd(-1)
{
	wxASSERT(!address.IsEmpty());
	wxASSERT(port > 0U);

#if defined(__WINDOWS__)
	WSAData data;
	int wsaRet = ::WSAStartup(MAKEWORD(2, 2), &data);
	if (wsaRet != 0)
		wxLogError(wxT("Error from WSAStartup"));
#endif
}

CTCPReaderWriterClient::CTCPReaderWriterClient(int fd) :
m_address(),
m_port(0U),
m_localAddress(),
m_fd(fd)
{
	wxASSERT(fd >= 0);

#if defined(__WINDOWS__)
	WSAData data;
	int wsaRet = ::WSAStartup(MAKEWORD(2, 2), &data);
	if (wsaRet != 0)
		wxLogError(wxT("Error from WSAStartup"));
#endif
}

CTCPReaderWriterClient::CTCPReaderWriterClient() :
m_address(),
m_port(0U),
m_localAddress(),
m_fd(-1)
{
#if defined(__WINDOWS__)
	WSAData data;
	int wsaRet = ::WSAStartup(MAKEWORD(2, 2), &data);
	if (wsaRet != 0)
		wxLogError(wxT("Error from WSAStartup"));
#endif
}

CTCPReaderWriterClient::~CTCPReaderWriterClient()
{
#if defined(__WINDOWS__)
	::WSACleanup();
#endif
}

bool CTCPReaderWriterClient::open(const wxString& address, unsigned int port, const wxString& localAddress)
{
	m_address      = address;
	m_port         = port;
	m_localAddress = localAddress;

	return open();
}

bool CTCPReaderWriterClient::open()
{
	if (m_fd != -1)
		return true;

	if (m_address.IsEmpty() || m_port == 0U)
		return false;

	m_fd = ::socket(PF_INET, SOCK_STREAM, 0);
	if (m_fd < 0) {
#if defined(__WINDOWS__)
		wxLogError(wxT("Cannot create the TCP client socket, err=%d"), ::GetLastError());
#else
		wxLogError(wxT("Cannot create the TCP client socket, err=%d"), errno);
#endif
		return false;
	}

	if (!m_localAddress.IsEmpty()) {
		sockaddr_in addr;
		::memset(&addr, 0x00, sizeof(struct sockaddr_in));
		addr.sin_family = AF_INET;
		addr.sin_port   = 0U;
#if defined(__WINDOWS__)
		addr.sin_addr.s_addr = ::inet_addr(m_localAddress.mb_str());
#else
		addr.sin_addr.s_addr = ::inet_addr(m_localAddress.mb_str());
#endif
		if (addr.sin_addr.s_addr == INADDR_NONE) {
			wxLogError(wxT("The address is invalid - %s"), m_localAddress.c_str());
			close();
			return false;
		}

		if (::bind(m_fd, (sockaddr*)&addr, sizeof(sockaddr_in)) == -1) {
#if defined(__WINDOWS__)
		wxLogError(wxT("Cannot bind the TCP client address, err=%d"), ::GetLastError());
#else
		wxLogError(wxT("Cannot bind the TCP client address, err=%d"), errno);
#endif
			close();
			return false;
		}
	}

	struct sockaddr_in addr;
	::memset(&addr, 0x00, sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_port   = htons(m_port);
	addr.sin_addr   = CUDPReaderWriter::lookup(m_address);

	if (addr.sin_addr.s_addr == INADDR_NONE) {
		close();
		return false;
	}

	if (::connect(m_fd, (sockaddr*)&addr, sizeof(struct sockaddr_in)) == -1) {
#if defined(__WINDOWS__)
		wxLogError(wxT("Cannot connect the TCP client socket, err=%d"), ::GetLastError());
#else
		wxLogError(wxT("Cannot connect the TCP client socket, err=%d"), errno);
#endif
		close();
		return false;
	}

	int noDelay = 1;
	if (::setsockopt(m_fd, IPPROTO_TCP, TCP_NODELAY, (char *)&noDelay, sizeof(noDelay)) == -1) {
#if defined(__WINDOWS__)
		wxLogError(wxT("Cannot set the TCP client socket option, err=%d"), ::GetLastError());
#else
		wxLogError(wxT("Cannot set the TCP client socket option, err=%d"), errno);
#endif
		close();
		return false;
	}

	return true;
}

int CTCPReaderWriterClient::read(unsigned char* buffer, unsigned int length, unsigned int secs, unsigned int msecs)
{
	wxASSERT(buffer != NULL);
	wxASSERT(length > 0U);
	wxASSERT(m_fd != -1);

	// Check that the recv() won't block
	fd_set readFds;
	FD_ZERO(&readFds);
#if defined(__WINDOWS__)
	FD_SET((unsigned int)m_fd, &readFds);
#else
	FD_SET(m_fd, &readFds);
#endif

	// Return after timeout
	timeval tv;
	tv.tv_sec  = secs;
	tv.tv_usec = msecs * 1000;

	int ret = ::select(m_fd + 1, &readFds, NULL, NULL, &tv);
	if (ret < 0) {
#if defined(__WINDOWS__)
		wxLogError(wxT("Error returned from TCP client select, err=%d"), ::GetLastError());
#else
		wxLogError(wxT("Error returned from TCP client select, err=%d"), errno);
#endif
		return -1;
	}

#if defined(__WINDOWS__)
	if (!FD_ISSET((unsigned int)m_fd, &readFds))
		return 0;
#else
	if (!FD_ISSET(m_fd, &readFds))
		return 0;
#endif

	ssize_t len = ::recv(m_fd, (char*)buffer, length, 0);
	if (len < 0) {
#if defined(__WINDOWS__)
		wxLogError(wxT("Error returned from recv, err=%d"), ::GetLastError());
#else
		wxLogError(wxT("Error returned from recv, err=%d"), errno);
#endif
		return -1;
	}

	return len;
}

bool CTCPReaderWriterClient::write(const unsigned char* buffer, unsigned int length)
{
	wxASSERT(buffer != NULL);
	wxASSERT(length > 0U);
	wxASSERT(m_fd != -1);

	ssize_t ret = ::send(m_fd, (char *)buffer, length, 0);
	if (ret != ssize_t(length)) {
#if defined(__WINDOWS__)
		wxLogError(wxT("Error returned from send, err=%d"), ::GetLastError());
#else
		wxLogError(wxT("Error returned from send, err=%d"), errno);
#endif
		return false;
	}

	return true;
}

void CTCPReaderWriterClient::close()
{
	if (m_fd != -1) {
#if defined(__WINDOWS__)
		::closesocket(m_fd);
#else
		::close(m_fd);
#endif
		m_fd = -1;
	}
}
