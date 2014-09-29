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

#include "UDPReaderWriter.h"
#include "Utils.h"

#if !defined(__WINDOWS__)
#include <cerrno>
#endif


CUDPReaderWriter::CUDPReaderWriter(const wxString& remoteAddress, unsigned int remotePort, const wxString& localAddress, unsigned int localPort) :
m_remoteAddress(remoteAddress),
m_remotePort(remotePort),
m_localAddress(localAddress),
m_localPort(localPort),
m_remAddr(),
m_fd(-1)
{
	wxASSERT(!remoteAddress.IsEmpty());
	wxASSERT(remotePort > 0U);
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

bool CUDPReaderWriter::open()
{
#if defined(__WINDOWS__)
	WSAData data;

	int wsaRet =  ::WSAStartup(0x101, &data);
	if (wsaRet != 0)
		return false;
#endif

	::memset(&m_remAddr, 0x00, sizeof(sockaddr_in));
	m_remAddr.sin_family = AF_INET;
	m_remAddr.sin_port   = htons(m_remotePort);
	m_remAddr.sin_addr   = CUDPReaderWriter::lookup(m_remoteAddress);

	m_fd = ::socket(PF_INET, SOCK_DGRAM, 0);
	if (m_fd < 0) {
#if defined(__WINDOWS__)
		wxLogError(wxT("Cannot create the socket, err=%d"), ::GetLastError());
#else
		wxLogError(wxT("Cannot create the socket, err=%d"), errno);
#endif
		return false;
	}

	sockaddr_in addr;
	::memset(&addr, 0x00, sizeof(sockaddr_in));
	addr.sin_family      = AF_INET;
	addr.sin_port        = htons(m_localPort);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (!m_localAddress.IsEmpty()) {
#if defined(__WINDOWS__)
		addr.sin_addr.s_addr = ::inet_addr(m_localAddress.mb_str());
#else
		addr.sin_addr.s_addr = ::inet_addr(m_localAddress.mb_str());
#endif
		if (addr.sin_addr.s_addr == INADDR_NONE) {
			wxLogError(wxT("The local address is invalid - %s"), m_localAddress.c_str());
			return false;
		}
	}

	int reuse = 1;
	if (::setsockopt(m_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse)) == -1) {
#if defined(__WINDOWS__)
		wxLogError(wxT("Cannot set the socket option, err=%d"), ::GetLastError());
#else
		wxLogError(wxT("Cannot set the socket option, err=%d"), errno);
#endif
		return false;
	}

	if (::bind(m_fd, (sockaddr*)&addr, sizeof(sockaddr_in)) == -1) {
#if defined(__WINDOWS__)
		wxLogError(wxT("Cannot bind the local address, err=%d"), ::GetLastError());
#else
		wxLogError(wxT("Cannot bind the local address, err=%d"), errno);
#endif
		return false;
	}

	return true;
}

int CUDPReaderWriter::read(unsigned char* buffer, unsigned int length)
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
		wxLogError(wxT("Error returned from select, err=%d"), ::GetLastError());
#else
		wxLogError(wxT("Error returned from select, err=%d"), errno);
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
	if (len < 0) {
#if defined(__WINDOWS__)
		wxLogError(wxT("Error returned from recvfrom, err=%d"), ::GetLastError());
#else
		wxLogError(wxT("Error returned from recvfrom, err=%d"), errno);
#endif
		return -1;
	}

	// Check if the data is for us
	if (m_remAddr.sin_addr.s_addr != addr.sin_addr.s_addr || m_remAddr.sin_port != addr.sin_port) {
		wxLogMessage(wxT("Packet received from an invalid source, %08X != %08X and/or %u != %u"), m_remAddr.sin_addr.s_addr, addr.sin_addr.s_addr, m_remAddr.sin_port, addr.sin_port);
		CUtils::dump(wxT("Data"), buffer, len);
		return 0;
	}

	return len;
}

bool CUDPReaderWriter::write(const unsigned char* buffer, unsigned int length)
{
	ssize_t ret = ::sendto(m_fd, (char *)buffer, length, 0, (sockaddr *)&m_remAddr, sizeof(sockaddr_in));
	if (ret < 0) {
#if defined(__WINDOWS__)
		wxLogError(wxT("Error returned from sendto, err=%d"), ::GetLastError());
#else
		wxLogError(wxT("Error returned from sendto, err=%d"), errno);
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
	::WSACleanup();
#else
	::close(m_fd);
#endif
}

