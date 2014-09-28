/*

CIRCDDB - ircDDB client library in C++

Copyright (C) 2010-2011		Michael Dirska, DL1BFF (dl1bff@mdx.de)
Copyright (C) 2012,2014		Jonathan Naylor, G4KLX

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
#include "Utils.h"

#include <wx/wx.h>

#include <fcntl.h>
#include <errno.h>

IRCClient::IRCClient(IRCApplication* app, const wxString& update_channel, const wxString& hostName, unsigned int port, const wxString& callsign, const wxString& password, const wxString& versionInfo, const wxString& localAddr) :
wxThread(wxTHREAD_JOINABLE),
m_app(app),
m_hostName(NULL),
m_localAddr(NULL),
m_port(port),
m_callsign(callsign.Lower()),
m_password(password),
m_stopped(false),
m_recv(NULL),
m_recvQ(NULL),
m_sendQ(NULL),
m_proto(NULL)
{
	wxASSERT(app != NULL);

#if defined(__WINDOWS__)
	WSAData data;
	int wsaRet = ::WSAStartup(MAKEWORD(2, 2), &data);
	if (wsaRet != 0)
		wxLogError(wxT("IRCClient::IRCClient: Error from WSAStartup"));
#endif

	m_hostName = new char[100U];
	IRCUtils::safeStringCopy(m_hostName, hostName.mb_str(), 100U);

	m_localAddr = new char[100U];
	if (localAddr.IsEmpty())
		IRCUtils::safeStringCopy(m_localAddr, "0.0.0.0", 100U);
	else
		IRCUtils::safeStringCopy(m_localAddr, localAddr.mb_str(), 100U);

	m_proto = new IRCProtocol(m_app, m_callsign, m_password, update_channel, versionInfo);
}

IRCClient::~IRCClient()
{
	delete   m_proto;
	delete[] m_hostName;
	delete[] m_localAddr;

#if defined(__WINDOWS__)
	::WSACleanup();
#endif
}

void IRCClient::startWork()
{
	m_stopped = false;

	Create();

	Run();
}

void IRCClient::stopWork()
{
	m_stopped = true;

	Wait();
}

wxThread::ExitCode IRCClient::Entry()
{
	wxLogMessage(wxT("Starting the IRC Client thread"));

	unsigned int numAddr = 0U;

	const unsigned int MAXIPV4ADDR = 10U;
	struct sockaddr_in addr[MAXIPV4ADDR];

	struct sockaddr_in myaddr;

	int state = 0;
	int sock = 0;
	unsigned int currentAddr = 0U;

	int result = IRCUtils::getAllIPV4Addresses(m_localAddr, 0U, numAddr, &myaddr, 1U);
	if ((result != 0) || (numAddr != 1))
		memset(&myaddr, 0x00, sizeof(struct sockaddr_in));

	CTimer timer(2U, 1U);
	timer.start();

	while (true) {
		timer.clock();

		switch (state) {
			case 0:
				if (m_stopped) {
					wxLogMessage(wxT("Stopping the IRC Client thread :1"));
					return 0;
				}

				if (timer.isRunning() && timer.hasExpired()) {
					timer.start(15U);

					if (IRCUtils::getAllIPV4Addresses(m_hostName, m_port, numAddr, addr, MAXIPV4ADDR) == 0) {
						if (numAddr > 0U) {
							currentAddr = 0U;
							state = 1;

							timer.start(1U);
						}
					}
				}
				break;

			case 1: {
					if (m_stopped) {
						wxLogMessage(wxT("Stopping the IRC Client thread :2"));
						return 0;
					}

					if (timer.isRunning() && timer.hasExpired()) {
						sock = ::socket( PF_INET, SOCK_STREAM, IPPROTO_TCP);
						if (sock < 0) {
							wxLogError(wxT("IRCClient::Entry: socket"));

							timer.start(15U);

							state = 0;

							break;
						}
#if defined(__WINDOWS__)
						u_long nonBlock = 1UL;
						if (::ioctlsocket(sock, FIONBIO, &nonBlock) != 0) {
							wxLogError(wxT("IRCClient::Entry: ioctlsocket"));

							::closesocket(sock);

							timer.start(15U);

							state = 0;

							break;
						}
#else
						if (::fcntl(sock, F_SETFL, O_NONBLOCK) < 0) {
							wxLogError(wxT("IRCClient::Entry: fcntl"));

							::close(sock);

							timer.start(15U);

							state = 0;

							break;
						}
#endif
						int res = ::bind(sock, (struct sockaddr*)&myaddr, sizeof (struct sockaddr_in));
						if (res != 0) {
							wxLogError(wxT("IRCClient::Entry: bind"));
#if defined(__WINDOWS__)
							::closesocket(sock);
#else
							::close(sock);
#endif
							state = 0;

							timer.start(15U);

							break;
						}


						res = ::connect(sock, (struct sockaddr *)(addr + currentAddr), sizeof (struct sockaddr_in));
						if (res == 0) {
							state = 4;
						} else {
#if defined(__WINDOWS__)
							if (::WSAGetLastError() == WSAEWOULDBLOCK) {
#else
							if (errno == EINPROGRESS) {
#endif
								state = 3;

								timer.start(5U);
							} else {
								wxLogError(wxT("IRCClient::Entry: connect"));
#if defined(__WINDOWS__)
								::closesocket(sock);
#else
								::close(sock);
#endif
								currentAddr++;
								if (currentAddr >= numAddr) {
									state = 0;

									timer.start(15U);
								} else {
									state = 1;

									timer.start(2U);
								}
							}
						} // connect
					}
				}
				break;

			case 3: {
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

					int res = ::select(sock + 1, NULL, &myset, NULL, &tv); 
					if (res < 0) {
						wxLogError(wxT("IRCClient::Entry: select"));
#if defined(__WINDOWS__)
						::closesocket(sock);
#else
						::close(sock);
#endif
						state = 0;

						timer.start(15U);
					} else if (res > 0) { // connect is finished
#if defined(__WINDOWS__)
						int val_len;
#else
						socklen_t val_len;
#endif
						int value;
						val_len = sizeof value;
						if (::getsockopt(sock, SOL_SOCKET, SO_ERROR, (char *)&value, &val_len) < 0) {
							wxLogError(wxT("IRCClient::Entry: getsockopt"));
#if defined(__WINDOWS__)
							::closesocket(sock);
#else
							::close(sock);
#endif
							state = 0;

							timer.start(15U);
						} else {
							if (value != 0) {
								wxLogWarning(wxT("IRCClient::Entry: SO_ERROR=%d"), value);
#if defined(__WINDOWS__)
								::closesocket(sock);
#else
								::close(sock);
#endif
								currentAddr++;
								if (currentAddr >= numAddr) {
									state = 0;
									timer.start(15U);
								} else {
									state = 1;
									timer.start(1U);
								}
							} else {
								state = 4;
							}
						}
					} else if (timer.isRunning() && timer.hasExpired()) {  // select timeout and timer timeout
#if defined(__WINDOWS__)
						::closesocket(sock);
#else
						::close(sock);
#endif
						currentAddr++;
						if (currentAddr >= numAddr) {
							state = 0;

							timer.start(15U);
						} else {
							state = 1; // open new socket

							timer.start(1U);
						}
					}
				}
				break;

			case 4: {
					m_recvQ = new IRCMessageQueue;
					m_sendQ = new IRCMessageQueue;

					m_recv = new IRCReceiver(sock, m_recvQ);
					m_recv->startWork();

					m_proto->setNetworkReady(true);
					state = 5;
					timer.stop();
				}
				break;

			case 5:
				if (m_stopped) {
					state = 6;
				} else {
					if (m_recvQ->isEOF()) {
						timer.stop();
						state = 6;
					} else if (!m_proto->processQueues(m_recvQ, m_sendQ)) {
						timer.stop();
						state = 6;
					}

					while ((state == 5) && m_sendQ->messageAvailable()) {
						IRCMessage* m = m_sendQ->getMessage();

						wxString out = m->composeMessage();

						char buf[200];
						IRCUtils::safeStringCopy(buf, out.mb_str(wxConvUTF8), sizeof buf);
						int len = ::strlen(buf);

						if (buf[len - 1] == 10) {  // is there a NL char at the end?
							int r = ::send(sock, buf, len, 0);
							if (r != len) {
								timer.stop();
								state = 6;
							}
						} else {
							timer.stop();
							state = 6;
						}

						delete m;
					}
				}
				break;

			case 6: {
					if (m_app != NULL) {
						m_app->setSendQ(NULL);
						m_app->userListReset();
					}

					m_proto->setNetworkReady(false);
					m_recv->stopWork();

					Sleep(2000UL);

					delete m_recvQ;
					delete m_sendQ;

#if defined(__WINDOWS__)
					::closesocket(sock);
#else
					::close(sock);
#endif

					if (m_stopped) {	// request to end the thread
						wxLogMessage(wxT("Stopping the IRC Client thread :3"));
						return 0;
					}

					timer.start(15U);

					state = 0;  // reconnect to IRC server
				}
				break;
		}

		Sleep(500UL);
	}

	wxLogMessage(wxT("Stopping the IRC Client thread :4"));

	return 0;
}
