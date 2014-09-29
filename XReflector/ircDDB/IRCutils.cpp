/*

CIRCDDB - ircDDB client library in C++

Copyright (C) 2010-2011   Michael Dirska, DL1BFF (dl1bff@mdx.de)

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


int getAllIPV4Addresses ( const char * name, unsigned short port,
    unsigned int * num, struct sockaddr_in * addr, unsigned int max_addr )
{

  struct addrinfo hints;
  struct addrinfo * res;

  memset(&hints, 0x00, sizeof(struct addrinfo));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;

  int r = getaddrinfo( name, NULL, &hints, &res );

  if (r == 0)
  {
    struct addrinfo * rp;
    unsigned int numAddr = 0;

    for (rp = res; rp != NULL; rp = rp->ai_next)
    {
      if (rp->ai_family == AF_INET)
      {
        numAddr ++;
      }
    }

    if (numAddr > 0)
    {
      if (numAddr > max_addr)
      {
        numAddr = max_addr;
      }

      int * shuffle = new int[numAddr];

      unsigned int i;

      for (i=0; i < numAddr; i++)
      {
        shuffle[i] = i;
      }

      for (i=0; i < (numAddr - 1); i++)
      {
        if (rand() & 1)
        {
          int tmp;
          tmp = shuffle[i];
          shuffle[i] = shuffle[i+1];
          shuffle[i+1] = tmp;
        }
      }

      for (i=(numAddr - 1); i > 0; i--)
      {
        if (rand() & 1)
        {
          int tmp;
          tmp = shuffle[i];
          shuffle[i] = shuffle[i-1];
          shuffle[i-1] = tmp;
        }
      }

      for (rp = res, i=0 ; (rp != NULL) && (i < numAddr); rp = rp->ai_next)
      {
        if (rp->ai_family == AF_INET)
        {
          memcpy( addr+shuffle[i], rp->ai_addr, sizeof (struct sockaddr_in) );

          addr[shuffle[i]].sin_port = htons(port);

          i++;
        }
      }

      delete[] shuffle;
    }

    *num = numAddr;

    freeaddrinfo(res);

    return 0;

  }
  else
  {
    wxString e( gai_strerror(r), wxConvUTF8);

    wxLogWarning(wxT("getaddrinfo: ") + e );

    return 1;
  }


}





void safeStringCopy (char * dest, const char * src, unsigned int buf_size)
{
  unsigned int i = 0;

  while (i < (buf_size - 1)  &&  (src[i] != 0))
  {
    dest[i] = src[i];
    i++;
  }

  dest[i] = 0;
}


wxString getCurrentTime(void)
{
  time_t now = time(NULL);
  struct tm* tm;
  struct tm tm_buf;
  char buffer[25];

#if defined(__WINDOWS__)
  gmtime_s( &tm_buf, &now );
  tm = &tm_buf;
#else
  tm = gmtime_r(&now, &tm_buf);
#endif

  strftime(buffer, sizeof buffer, "%Y-%m-%d %H:%M:%S", tm);

  return wxString(buffer, wxConvLocal);
}

