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

#include "TCPReaderWriterServer.h"

#if !defined(__WINDOWS__)
#include <cerrno>
#endif


CTCPReaderWriterServer::CTCPReaderWriterServer(const wxString& address, unsigned int port) :
wxThread(wxTHREAD_JOINABLE),
m_address(address),
m_port(port),
m_fd(-1),
m_client(NULL),
m_stopped(false)
{
	wxASSERT(port > 0U);

#if defined(__WINDOWS__)
	WSAData data;
	int wsaRet = ::WSAStartup(MAKEWORD(2, 2), &data);
	if (wsaRet != 0)
		wxLogError(wxT("Error from WSAStartup"));
#endif
}

CTCPReaderWriterServer::~CTCPReaderWriterServer()
{
#if defined(__WINDOWS__)
	::WSACleanup();
#endif
}

bool CTCPReaderWriterServer::start()
{
	bool ret = open();
	if (!ret) {
		close();
		return false;
	}

	Create();
	Run();

	return true;
}

int CTCPReaderWriterServer::read(unsigned char* buffer, unsigned int length, unsigned int secs)
{
	wxASSERT(buffer != NULL);
	wxASSERT(length > 0U);

	if (m_client != NULL) {
		int ret = m_client->read(buffer, length, secs);
		if (ret < 0) {
			wxLogMessage(wxT("Lost TCP connection to port %u"), m_port);

			m_client->close();
			delete m_client;
			m_client = NULL;

			open();

			return 0;
		}

		return ret;
	}

	return 0;
}

bool CTCPReaderWriterServer::write(const unsigned char* buffer, unsigned int length)
{
	wxASSERT(buffer != NULL);
	wxASSERT(length > 0U);

	if (m_client != NULL) {
		bool ret = m_client->write(buffer, length);
		if (!ret) {
			wxLogMessage(wxT("Lost TCP connection to port %u"), m_port);

			m_client->close();
			delete m_client;
			m_client = NULL;

			open();

			return false;
		}

		return true;
	}

	return true;
}

void* CTCPReaderWriterServer::Entry()
{
	while (!m_stopped) {
		int ret = accept();
		switch (ret) {
			case -2:
				break;
			case -1:
				break;
			default:
				wxLogMessage(wxT("Incoming TCP connection to port %u"), m_port);
				m_client = new CTCPReaderWriterClient(ret);
				close();
				break;
		}

		Sleep(1000UL);
	}

	if (m_client != NULL) {
		m_client->close();
		delete m_client;
	}

	close();

	return NULL;
}

void CTCPReaderWriterServer::stop()
{
	m_stopped = true;

	Wait();
}

bool CTCPReaderWriterServer::open()
{
	m_fd = ::socket(PF_INET, SOCK_STREAM, 0);
	if (m_fd < 0) {
#if defined(__WINDOWS__)
		wxLogError(wxT("Cannot create the TCP server socket, err=%d"), ::GetLastError());
#else
		wxLogError(wxT("Cannot create the TCP server socket, err=%d"), errno);
#endif
		return false;
	}

	struct sockaddr_in addr;
	::memset(&addr, 0x00, sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_port   = htons(m_port);
	if (m_address.IsEmpty())
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
	else
		addr.sin_addr = lookup(m_address);

	if (addr.sin_addr.s_addr == INADDR_NONE) {
		wxLogError(wxT("The address is invalid - %s"), m_address.c_str());
		close();
		return false;
	}

	int reuse = 1;
	if (::setsockopt(m_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse)) == -1) {
#if defined(__WINDOWS__)
		wxLogError(wxT("Cannot set the TCP server socket option, err=%d"), ::GetLastError());
#else
		wxLogError(wxT("Cannot set the TCP server socket option, err=%d"), errno);
#endif
		close();
		return false;
	}

	if (::bind(m_fd, (sockaddr*)&addr, sizeof(struct sockaddr_in)) == -1) {
#if defined(__WINDOWS__)
		wxLogError(wxT("Cannot bind the TCP server address, err=%d"), ::GetLastError());
#else
		wxLogError(wxT("Cannot bind the TCP server address, err=%d"), errno);
#endif
		close();
		return false;
	}

	::listen(m_fd, 5);

	return true;
}

int CTCPReaderWriterServer::accept()
{
	if (m_fd == -1)
		return -1;

	// Check that the accept() won't block
	fd_set readFds;
	FD_ZERO(&readFds);
#if defined(__WINDOWS__)
	FD_SET((unsigned int)m_fd, &readFds);
#else
	FD_SET(m_fd, &readFds);
#endif

	// Return after timeout
	timeval tv;
	tv.tv_sec  = 0L;
	tv.tv_usec = 0L;

	int ret = ::select(m_fd + 1, &readFds, NULL, NULL, &tv);
	if (ret < 0) {
#if defined(__WINDOWS__)
		wxLogError(wxT("Error returned from TCP server select, err=%d"), ::GetLastError());
#else
		wxLogError(wxT("Error returned from TCP server select, err=%d"), errno);
#endif
		return -2;
	}

#if defined(__WINDOWS__)
	if (!FD_ISSET((unsigned int)m_fd, &readFds))
		return -1;
#else
	if (!FD_ISSET(m_fd, &readFds))
		return -1;
#endif

	struct sockaddr_in addr;
#if defined(__WINDOWS__)
	int len = sizeof(struct sockaddr_in);
#else
	socklen_t len = sizeof(struct sockaddr_in);
#endif

	ret = ::accept(m_fd, (sockaddr*)&addr, &len);
	if (ret < 0) {
#if defined(__WINDOWS__)
		wxLogError(wxT("Error returned from TCP server accept, err=%d"), ::GetLastError());
#else
		wxLogError(wxT("Error returned from TCP server accept, err=%d"), errno);
#endif
	}

	return ret;
}

void CTCPReaderWriterServer::close()
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

in_addr CTCPReaderWriterServer::lookup(const wxString& hostname) const
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
		::memcpy(&addr, hp->h_addr_list[0], hp->h_length);
		return addr;
	}

	wxLogError(wxT("Cannot find %s"), hostname.c_str());

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
		::memcpy(&addr, hp->h_addr_list[0], hp->h_length);
		return addr;
	}

	wxLogError(wxT("Cannot find %s"), hostname.c_str());

	addr.s_addr = INADDR_NONE;
	return addr;
#endif
}
