/*

CIRCDDB - ircDDB client library in C++

Copyright (C) 2010-2011	Michael Dirska, DL1BFF (dl1bff@mdx.de)
Copyright (C) 2014		Jonathan Naylor, G4KLX

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/


#if defined(WIN32)
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#endif

#include <wx/wx.h>

#include "IRCutils.h"


int IRCUtils::getAllIPV4Addresses(const char* name, unsigned short port, unsigned int& num, struct sockaddr_in* addr, unsigned int max_addr )
{
	struct addrinfo hints;
	struct addrinfo* res;

	memset(&hints, 0x00, sizeof(struct addrinfo));
	hints.ai_family   = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	num = 0U;

	int r = ::getaddrinfo(name, NULL, &hints, &res);
	if (r == 0) {
		for (struct addrinfo* rp = res; rp != NULL; rp = rp->ai_next) {
			if (rp->ai_family == AF_INET)
				num++;
		}

		if (num > 0U) {
			if (num > max_addr)
				num = max_addr;

			int* shuffle = new int[num];

			for (unsigned int i = 0U; i < num; i++)
				shuffle[i] = i;

			for (unsigned int i = 0U; i < (num - 1U); i++) {
				if (::rand() & 1) {
					int tmp = shuffle[i];
					shuffle[i] = shuffle[i+1];
					shuffle[i+1] = tmp;
				}
			}

			for (unsigned int i = (num - 1U); i > 0U; i--) {
				if (::rand() & 1) {
					int tmp = shuffle[i];
					shuffle[i] = shuffle[i-1];
					shuffle[i-1] = tmp;
				}
			}

			unsigned int i = 0U;
			for (struct addrinfo* rp = res; (rp != NULL) && (i < num); rp = rp->ai_next) {
				if (rp->ai_family == AF_INET) {
					::memcpy( addr+shuffle[i], rp->ai_addr, sizeof(struct sockaddr_in));
					addr[shuffle[i]].sin_port = htons(port);
					i++;
				}
			}

			delete[] shuffle;
		}

		::freeaddrinfo(res);

		return 0;
	} else {
		wxString e(::gai_strerror(r), wxConvUTF8);
		wxLogWarning(wxT("getaddrinfo: %s"), e.c_str());
		return 1;
	}
}

void IRCUtils::safeStringCopy(char* dest, const char* src, unsigned int buf_size)
{
	unsigned int i = 0U;

	while (i < (buf_size - 1U) && (src[i] != 0)) {
		dest[i] = src[i];
		i++;
	}

	dest[i] = 0;
}

wxString IRCUtils::getCurrentTime()
{
	time_t now = ::time(NULL);

	struct tm* tm;
	struct tm tm_buf;

#if defined(__WINDOWS__)
	::gmtime_s(&tm_buf, &now);
	tm = &tm_buf;
#else
	tm = ::gmtime_r(&now, &tm_buf);
#endif

	char buffer[25];
	::strftime(buffer, sizeof buffer, "%Y-%m-%d %H:%M:%S", tm);

	return wxString(buffer, wxConvLocal);
}
