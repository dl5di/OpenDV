/*

CIRCDDB - ircDDB client library in C++

Copyright (C) 2010	Michael Dirska, DL1BFF (dl1bff@mdx.de)
Copyright (C) 2014	Jonathan Naylor, G4KLX

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

#if !defined(_IRCUTILS_H)
#define _IRCUTILS_H

#include <wx/wx.h>

class IRCUtils {
public:
	static int getAllIPV4Addresses(const char* name, unsigned short port, unsigned int& num, struct sockaddr_in* addr, unsigned int max_addr );

	static void safeStringCopy(char* dest, const char* src, unsigned int buf_size);

	static wxString getCurrentTime();
};

#endif
