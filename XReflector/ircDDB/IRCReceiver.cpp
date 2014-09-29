/*

CIRCDDB - ircDDB client library in C++

Copyright (C) 2010   Michael Dirska, DL1BFF (dl1bff@mdx.de)

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

#endif



#include "IRCReceiver.h"

#include <wx/wx.h>

IRCReceiver::IRCReceiver(int sock, IRCMessageQueue * q )
  : wxThread(wxTHREAD_JOINABLE)
{
  this->sock = sock;

  recvQ = q;
}

IRCReceiver::~IRCReceiver()
{
}

void IRCReceiver::startWork()
{
	terminateThread = false;

	Create();
	Run();
}

void IRCReceiver::stopWork()
{
	terminateThread = true;

	Wait();
}

static int doRead( int sock, char * buf, int buf_size )
{
  struct timeval tv;
  tv.tv_sec = 1;
  tv.tv_usec = 0;
  fd_set rdset;
  fd_set errset;

  FD_ZERO(&rdset);
  FD_ZERO(&errset);
  FD_SET(sock, &rdset);
  FD_SET(sock, &errset);

  int res;

  res = select(sock+1, &rdset, NULL, &errset, &tv);

  if ( res < 0 )
  {
    wxLogSysError(wxT("IRCReceiver::doRead: select"));
    return -1;
  }
  else if ( res > 0 )
  {
    if (FD_ISSET(sock, &errset))
    {
      wxLogVerbose(wxT("IRCReceiver::doRead: select (FD_ISSET(sock, exceptfds))"));
      return -1;
    }
    
    if (FD_ISSET(sock, &rdset))
    {
      res = recv(sock, buf, buf_size, 0);

      if (res < 0)
      {
	wxLogSysError(wxT("IRCReceiver::doRead: read"));
	return -1;
      }
      else if (res == 0)
      {
	wxLogVerbose(wxT("IRCReceiver::doRead: EOF read==0"));
	return -1;
      }
      else
      {
	return res;
      }
    }

  }

  return 0;
}

wxThread::ExitCode IRCReceiver::Entry ()
{
  IRCMessage * m = new IRCMessage();

  int i;
  int state = 0;

  while (!terminateThread)
  {

    // wxLogVerbose(wxT("IRCReceiver: tick"));

    char buf[200];
    int r = doRead( sock, buf, sizeof buf );

    if (r < 0)
    {
      recvQ -> signalEOF();
      delete m;  // delete unfinished IRCMessage
      break;
    }

    for (i=0; i < r; i++)
    {
      char b = buf[i];

      if (b > 0)
      {
	if (b == 10)
	{
	  recvQ -> putMessage(m);
	  m = new IRCMessage();
	  state = 0;
	}
	else if (b == 13)
	{
		// do nothing
	}
	else switch (state)
	{
	case 0:
	  if (b == ':')
	  {
	    state = 1; // prefix
	  }
	  else if (b == 32)
	  {
	      // do nothing
	  }
	  else
	  {
	    m -> command.Append(wxChar(b));
	    state = 2; // command
	  }
	  break;

	case 1:
	  if (b == 32)
	  {
	    state = 2; // command is next
	  }
	  else
	  {
	    m -> prefix.Append(wxChar(b));
	  }
	  break;

	case 2:
	  if (b == 32)
	  {
	    state = 3; // params
	    m -> numParams = 1;
	    m -> params.Add(wxT(""));
	  }
	  else
	  {
	    m -> command.Append(wxChar(b));
	  }
	  break;

        case 3:
	  if (b == 32)
	  {
	    m -> numParams ++;
	    if (m -> numParams >= 15)
	    {
	      state = 5; // ignore the rest
	    }

	    m -> params.Add(wxT(""));
	  }
	  else if ((b == ':') && (m -> params[ m -> numParams-1 ].Len() == 0))
	  {
		  state = 4; // rest of line is this param
	  }
	  else
	  {
		  m -> params[ m -> numParams-1 ].Append(wxChar(b));
	  }
	  break;

	case 4:
	  m -> params[ m -> numParams-1 ].Append(wxChar(b));
	  break;


        } // switch
      } // if
     } // for
  } // while

  return 0;
}

