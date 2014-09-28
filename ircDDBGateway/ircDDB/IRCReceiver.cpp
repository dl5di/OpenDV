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

#if defined(WIN32)
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#endif

#include "IRCReceiver.h"
#include "Utils.h"

#include <wx/wx.h>

IRCReceiver::IRCReceiver(int sock, IRCMessageQueue* q) :
wxThread(wxTHREAD_JOINABLE),
m_stopped(false),
m_sock(sock),
m_recvQ(q)
{
	wxASSERT(sock >= 0);
	wxASSERT(q != NULL);
}

IRCReceiver::~IRCReceiver()
{
}

void IRCReceiver::startWork()
{
	m_stopped = false;

	Create();

	Run();
}

void IRCReceiver::stopWork()
{
	m_stopped = true;

	Wait();
}

int IRCReceiver::doRead(char* buf, int buf_size)
{
	struct timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;

	fd_set rdset;
	FD_ZERO(&rdset);
#if defined(_WIN32)
	FD_SET((unsigned int)m_sock, &rdset);
#else
	FD_SET(m_sock, &rdset);
#endif

	int res = ::select(m_sock + 1, &rdset, NULL, NULL, &tv);
	if (res < 0) {
		wxLogError(wxT("IRCReceiver::doRead: select"));
		return -1;
	}

	if (res > 0)
	{
		if (FD_ISSET(m_sock, &rdset)) {
			res = ::recv(m_sock, buf, buf_size, 0);
			if (res < 0) {
				wxLogError(wxT("IRCReceiver::doRead: read"));
				return -1;
			} else if (res == 0) {
				return -1;
			} else {
				return res;
			}
		}
	}

	return 0;
}

wxThread::ExitCode IRCReceiver::Entry()
{
	wxLogMessage(wxT("Starting the IRC Receiver thread"));

	IRCMessage* m = new IRCMessage;

	int state = 0;

	while (!m_stopped) {
		char buf[200];
		int r = doRead(buf, sizeof(buf));
		if (r < 0) {
			m_recvQ->signalEOF();
			delete m;  // delete unfinished IRCMessage
			break;
		}

		for (int i = 0; i < r; i++) {
			char b = buf[i];

			if (b > 0) {
				if (b == 10) {
					m_recvQ->putMessage(m);
					m = new IRCMessage;
					state = 0;
				} else if (b == 13) {
					// do nothing
				} else switch (state) {
					case 0:
						if (b == ':') {
							state = 1; // prefix
						} else if (b == 32) {
							// do nothing
						} else {
							m->m_command.Append(wxChar(b));
							state = 2; // command
						}
						break;

					case 1:
						if (b == 32) {
							state = 2; // command is next
						} else {
							m->m_prefix.Append(wxChar(b));
						}
						break;

					case 2:
						if (b == 32) {
							state = 3; // params
							m->m_params.Add(wxEmptyString);
						} else {
							m->m_command.Append(wxChar(b));
						}
						break;

					case 3:
						if (b == 32) {
							if (m->m_params.GetCount() >= 15U)
								state = 5; // ignore the rest

							m->m_params.Add(wxEmptyString);
						} else if ((b == ':') && (m->m_params.Last().Len() == 0)) {
							state = 4; // rest of line is this param
						} else {
							m->m_params.Last().Append(wxChar(b));
						}
						break;

					case 4:
						m->m_params.Last().Append(wxChar(b));
						break;

					default:
						break;
				}
			}
		}
	}

	wxLogMessage(wxT("Stopping the IRC Receiver thread"));

	return 0;
}
