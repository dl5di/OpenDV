/*

CIRCDDB - ircDDB client library in C++

Copyright (C) 2010-2011   Michael Dirska, DL1BFF (dl1bff@mdx.de)
Copyright (C) 2012        Jonathan Naylor, G4KLX

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


#include "IRCClient.h"
#include "IRCutils.h"

#include <wx/wx.h>



#include <fcntl.h>
#include <errno.h>






IRCClient::IRCClient( IRCApplication * app, const wxString& update_channel,
    const wxString& hostName, unsigned int port, const wxString& callsign, const wxString& password,
    const wxString& versionInfo, const wxString& localAddr )
     : wxThread(wxTHREAD_JOINABLE)
{
#if defined(__WINDOWS__)
	WSAData data;
	int wsaRet = ::WSAStartup(MAKEWORD(2, 2), &data);
	if (wsaRet != 0)
		wxLogError(wxT("IRCClient::IRCClient: Error from WSAStartup"));
#endif

  safeStringCopy(host_name, hostName.mb_str(), sizeof host_name);

  this -> callsign = callsign.Lower();
  this -> port = port;
  this -> password = password;

  this -> app = app;
  
  if (localAddr.IsEmpty())
  {
    safeStringCopy(local_addr, "0.0.0.0", sizeof local_addr);
  }
  else
  {
    safeStringCopy(local_addr, localAddr.mb_str(), sizeof local_addr);
  }

  proto = new IRCProtocol ( app, this->callsign, password, update_channel, versionInfo );

  recvQ = NULL;
  sendQ = NULL;

  recv = NULL;
}

IRCClient::~IRCClient()
{
	delete proto;

#if defined(__WINDOWS__)
	::WSACleanup();
#endif
}

void IRCClient::startWork()
{
	terminateThread = false;

	Create();
	Run();
}

void IRCClient::stopWork()
{
	terminateThread = true;

	Wait();
}

wxThread::ExitCode IRCClient::Entry ()
{
  unsigned int numAddr;

#define MAXIPV4ADDR 10
  struct sockaddr_in addr[MAXIPV4ADDR];

  struct sockaddr_in myaddr;

  int state = 0;
  int timer = 0;
  int sock = 0;
  unsigned int currentAddr = 0;

  int result;


  numAddr = 0;

  result = getAllIPV4Addresses(local_addr, 0, &numAddr, &myaddr, 1);

  if ((result != 0) || (numAddr != 1))
  {
    wxLogVerbose(wxT("IRCClient::Entry: local address not parseable, using 0.0.0.0"));
    memset(&myaddr, 0x00, sizeof(struct sockaddr_in));
  }

  while (true)
  {

    if (timer > 0)
    {
      timer --;
    }

    switch (state)
    {
    case 0:
      if (terminateThread)
      {
	wxLogVerbose(wxT("IRCClient::Entry: thread terminated at state=%d"), state);
	return 0;
      }
      
      if (timer == 0)
      {
	timer = 30;

	if (getAllIPV4Addresses(host_name, port, &numAddr, addr, MAXIPV4ADDR) == 0)
	{
	  wxLogVerbose(wxT("IRCClient::Entry: number of DNS entries %d"), numAddr);
	  if (numAddr > 0)
	  {
	    currentAddr = 0;
	    state = 1;
	    timer = 0;
	  }
	}
      }
      break;

    case 1:
      if (terminateThread)
      {
	wxLogVerbose(wxT("IRCClient::Entry: thread terminated at state=%d"), state);
	return 0;
      }
      
      if (timer == 0)
      {
	sock = socket( PF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (sock < 0)
	{
	  wxLogSysError(wxT("IRCClient::Entry: socket"));
	  timer = 30;
	  state = 0;
	}
	else
	{
#if defined(__WINDOWS__)
	  u_long nonBlock = 1UL;
	  if (ioctlsocket( sock, FIONBIO, &nonBlock ) != 0)
	  {
	    wxLogSysError(wxT("IRCClient::Entry: ioctlsocket"));
	    closesocket(sock);
	    timer = 30;
	    state = 0;
	  }
#else
	  if (fcntl( sock, F_SETFL, O_NONBLOCK ) < 0)
	  {
	    wxLogSysError(wxT("IRCClient::Entry: fcntl"));
	    close(sock);
	    timer = 30;
	    state = 0;
	  }
#endif
	  else
	  {
	    unsigned char * h = (unsigned char *) &(myaddr.sin_addr);
	    int res;

	    if ((h[0] != 0) || (h[1] != 0) || (h[2] != 0) || (h[3] != 0))
	    {
	      wxLogVerbose(wxT("IRCClient::Entry: bind: local address %d.%d.%d.%d"), 
		   h[0], h[1], h[2], h[3]);
	    }

	    res = bind(sock, (struct sockaddr *) &myaddr, sizeof (struct sockaddr_in));

	    if (res != 0)
	    {

		wxLogSysError(wxT("IRCClient::Entry: bind"));
#if defined(__WINDOWS__)
		closesocket(sock);
#else
		close(sock);
#endif
		state = 0;
		timer = 30;
		break;
	    }


	    h = (unsigned char *) &(addr[currentAddr].sin_addr);
	    wxLogVerbose(wxT("IRCClient::Entry: trying to connect to %d.%d.%d.%d"), 
		   h[0], h[1], h[2], h[3]);
		
	    res = connect(sock, (struct sockaddr *) (addr + currentAddr), sizeof (struct sockaddr_in));

	    if (res == 0)
	    {
	      wxLogVerbose(wxT("IRCClient::Entry: connected"));
	      state = 4;
	    }
	    else 
	    { 
#if defined(__WINDOWS__)
	      if (WSAGetLastError() == WSAEWOULDBLOCK)
#else
	      if (errno == EINPROGRESS)
#endif
	      {
		wxLogVerbose(wxT("IRCClient::Entry: connect in progress"));
		state = 3;
		timer = 10;  // 5 second timeout
	      }
	      else
	      {
		wxLogSysError(wxT("IRCClient::Entry: connect"));
#if defined(__WINDOWS__)
		closesocket(sock);
#else
		close(sock);
#endif
		currentAddr ++;
		if (currentAddr >= numAddr)
		{
		  state = 0;
		  timer = 30;
		}
		else
		{
		  state = 1;
		  timer = 4;
		}
	      }
	    }
	  } // connect
	}
      }
      break;
    
    case 3:
      {
	struct timeval tv;
	tv.tv_sec = 0; 
	tv.tv_usec = 0; 

	fd_set myset;
	FD_ZERO(&myset);
#if defined(__WINDOWS__)
	FD_SET((unsigned int)sock, &myset);
#else
	FD_SET(sock, &myset);
#endif

	int res = select(sock+1, NULL, &myset, NULL, &tv); 
	if (res < 0)
	{
	  wxLogSysError(wxT("IRCClient::Entry: select"));
#if defined(__WINDOWS__)
	  closesocket(sock);
#else
	  close(sock);
#endif
	  state = 0;
	  timer = 30;
	}
	else if (res > 0) // connect is finished
	{
#if defined(__WINDOWS__)
	  int val_len;
#else
	  socklen_t val_len;
#endif
	  int value;

	  val_len = sizeof value;

	  if (getsockopt(sock, SOL_SOCKET, SO_ERROR, (char *) &value, &val_len) < 0)
	  {
	     wxLogSysError(wxT("IRCClient::Entry: getsockopt"));
#if defined(__WINDOWS__)
	     closesocket(sock);
#else
	     close(sock);
#endif
	     state = 0;
	     timer = 30;
	  }
	  else
	  {
	    if (value != 0)
	    {
	      wxLogWarning(wxT("IRCClient::Entry: SO_ERROR=%d"), value);
#if defined(__WINDOWS__)
	      closesocket(sock);
#else
	      close(sock);
#endif
	      currentAddr ++;
	      if (currentAddr >= numAddr)
	      {
		state = 0;
		timer = 30;
	      }
	      else
	      {
		state = 1;
		timer = 2;
	      }
	    }
	    else
	    {
	      wxLogVerbose(wxT("IRCClient::Entry: connected2"));
	      state = 4;
	    }
	  }

	}
	else if (timer == 0)
	{  // select timeout and timer timeout
	  wxLogVerbose(wxT("IRCClient::Entry: connect timeout"));
#if defined(__WINDOWS__)
	  closesocket(sock);
#else
	  close(sock);
#endif
	  currentAddr ++;
	  if (currentAddr >= numAddr)
	  {
	    state = 0;
	    timer = 30;
	  }
	  else
	  {
	    state = 1; // open new socket
	    timer = 2;
	  }
	}

      }
      break;

    case 4:
      {
	recvQ = new IRCMessageQueue();
	sendQ = new IRCMessageQueue();

	recv = new IRCReceiver(sock, recvQ);
	recv->startWork();

	proto->setNetworkReady(true);
	state = 5;
	timer = 0;

      }
      break;


    case 5:
      if (terminateThread)
      {
	state = 6;
      }
      else
      {

	if (recvQ -> isEOF())
	{
	  timer = 0;
	  state = 6;
	}
	else if (proto -> processQueues(recvQ, sendQ) == false)
	{
	  timer = 0;
	  state = 6;
	}

	while ((state == 5) && sendQ->messageAvailable())
	{
	  IRCMessage * m = sendQ -> getMessage();

	  wxString out;

	  m -> composeMessage ( out );

	  char buf[200];
	  safeStringCopy(buf, out.mb_str(wxConvUTF8), sizeof buf);
	  int len = strlen(buf);

	  if (buf[len - 1] == 10)  // is there a NL char at the end?
	  {
	    int r = send(sock, buf, len, 0);

	    if (r != len)
	    {
	      wxLogVerbose(wxT("IRCClient::Entry: short write %d < %d"), r, len);

	      timer = 0;
	      state = 6;
	    }
/*	    else
	    {
	      wxLogVerbose(wxT("write %d bytes (") + out + wxT(")"), len );
	    } */
	  }
	  else
	  {
	      wxLogVerbose(wxT("IRCClient::Entry: no NL at end, len=%d"), len);

	      timer = 0;
	      state = 6;
	  }

	  delete m;
	}
      }
      break;

    case 6:
      {
	if (app != NULL)
	{
	  app->setSendQ(NULL);
	  app->userListReset();
	}

	proto->setNetworkReady(false);
	recv->stopWork();

	wxThread::Sleep(2000);

	delete recv;
	delete recvQ;
	delete sendQ;

#if defined(__WINDOWS__)
	closesocket(sock);
#else
	close(sock);
#endif

	if (terminateThread) // request to end the thread
	{
	  wxLogVerbose(wxT("IRCClient::Entry: thread terminated at state=%d"), state);
	  return 0;
	}

	timer = 30;
	state = 0;  // reconnect to IRC server
      }
      break;

    }

    wxThread::Sleep(500);

  }

  return 0;
}





