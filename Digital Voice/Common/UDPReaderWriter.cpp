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


CUDPReaderWriter::CUDPReaderWriter() :
m_remAddr(),
m_fd(-1)
{
}

CUDPReaderWriter::~CUDPReaderWriter()
{
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

bool CUDPReaderWriter::open(const wxString& remoteAddress, unsigned int remotePort, unsigned int localPort)
{
	wxASSERT(!remoteAddress.IsEmpty());
	wxASSERT(remotePort > 0U);

#if defined(__WINDOWS__)
	WSAData data;
	int wsaRet =  ::WSAStartup(0x101, &data);
	if (wsaRet != 0)
		return false;
#endif

	::memset(&m_remAddr, 0x00, sizeof(sockaddr_in));
	m_remAddr.sin_family = AF_INET;
	m_remAddr.sin_port   = htons(remotePort);
	m_remAddr.sin_addr   = CUDPReaderWriter::lookup(remoteAddress);

	m_fd = ::socket(PF_INET, SOCK_DGRAM, 0);
	if (m_fd < 0) {
		wxLogError(wxT("Cannot create the socket"));
		return false;
	}

	sockaddr_in addr;
	::memset(&addr, 0x00, sizeof(sockaddr_in));
	addr.sin_family      = AF_INET;
	addr.sin_port        = htons(localPort);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

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

unsigned int CUDPReaderWriter::read(unsigned char* buffer, unsigned int length)
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
		return 0U;
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
		return 0U;
	}

	// Check if the data is for us, only check the IP address now (used to include port number too)
	if (m_remAddr.sin_addr.s_addr != addr.sin_addr.s_addr)
		return 0U;

	return len;
}

bool CUDPReaderWriter::write(const unsigned char* buffer, unsigned int length)
{
	ssize_t ret = ::sendto(m_fd, (char *)buffer, length, 0, (sockaddr *)&m_remAddr, sizeof(sockaddr_in));
	if (ret < 0) {
		wxLogError(wxT("Error returned from sendto()"));
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
	::WSACleanup();
#else
	::close(m_fd);
#endif
}
