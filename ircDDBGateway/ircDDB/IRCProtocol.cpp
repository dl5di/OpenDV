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


#include "IRCProtocol.h"

#include <wx/regex.h>

const wxString CIRCDDB_VERSION = wxT("1.2.5");

const unsigned int PING_TIMEOUT = 30U;

IRCProtocol::IRCProtocol(IRCApplication* app, const wxString& callsign, const wxString& password, const wxString& channel, const wxString& versionInfo) :
m_app(app),
m_nicks(),
m_password(password),
m_channel(channel),
m_name(callsign),
m_currentNick(),
m_versionInfo(),
m_state(0),
m_timer(2U),
m_debugChannel()
{
	wxASSERT(app != NULL);

	m_versionInfo = wxT("CIRCDDB:");
	m_versionInfo.Append(CIRCDDB_VERSION);
	m_versionInfo.Append(wxT(" "));
	m_versionInfo.Append(versionInfo);

	int hyphenPos = callsign.find(wxT('-'));
	if (hyphenPos == wxNOT_FOUND) {
		wxString n;

		n = callsign + wxT("-1");
		m_nicks.Add(n);
		n = callsign + wxT("-2");
		m_nicks.Add(n);
		n = callsign + wxT("-3");
		m_nicks.Add(n);
		n = callsign + wxT("-4");
		m_nicks.Add(n);
	} else {
		m_nicks.Add(callsign);
	}

	chooseNewNick();
}

IRCProtocol::~IRCProtocol()
{
}

void IRCProtocol::chooseNewNick()
{
	int r = ::rand() % m_nicks.GetCount();

	m_currentNick = m_nicks[r];
}

void IRCProtocol::setNetworkReady(bool b)
{
	if (b) {
		if (m_state != 0)
			wxLogError(wxT("IRCProtocol::setNetworkReady: unexpected state"));

		m_state = 1;
		chooseNewNick();
	} else {
		m_state = 0;
	}
}

bool IRCProtocol::processQueues(IRCMessageQueue* recvQ, IRCMessageQueue* sendQ)
{
	wxASSERT(recvQ != NULL);
	wxASSERT(sendQ != NULL);

	m_timer.clock();

	while (recvQ->messageAvailable()) {
		IRCMessage* m = recvQ->getMessage();

		if (m->m_command.IsSameAs(wxT("004"))) {
			if (m_state == 4) {
				if (m->m_params.GetCount() > 1) {
					wxRegEx serverNamePattern(wxT("^grp[1-9]s[1-9].ircDDB$"));

					if (serverNamePattern.Matches(m->m_params.Item(1U)))
						m_app->setBestServer(wxT("s-") + m->m_params.Item(1U).Mid(0U, 6U));
				}

				m_state = 5;  // next: JOIN

				m_app->setCurrentNick(m_currentNick);
			}
		} else if (m->m_command.IsSameAs(wxT("PING"))) {
			IRCMessage* m2 = new IRCMessage;
			m2->m_command = wxT("PONG");

			if (m->m_params.GetCount() > 0U)
				m2->m_params.Add(m->m_params.Item(0U));

			sendQ->putMessage(m2);
		} else if (m->m_command.IsSameAs(wxT("JOIN"))) {
			if ((m->m_params.GetCount() >= 1U) && m->m_params.Item(0U).IsSameAs(m_channel)) {
				if (m->getPrefixNick().IsSameAs(m_currentNick) && (m_state == 6)) {
					if (m_debugChannel.Len() > 0U)
						m_state = 7;  // next: join debug_channel
					else
						m_state = 10; // next: WHO *
				} else if (m_app != NULL) {
					m_app->userJoin(m->getPrefixNick(), m->getPrefixName(), m->getPrefixHost());
				}
			}

			if ((m->m_params.GetCount() >= 1U) && m->m_params.Item(0U).IsSameAs(m_debugChannel)) {
				if (m->getPrefixNick().IsSameAs(m_currentNick) && (m_state == 8))
					m_state = 10; // next: WHO *
			}
		} else if (m->m_command.IsSameAs(wxT("PONG"))) {
			if (m_state == 12) {
				m_timer.start(PING_TIMEOUT);

				m_state = 11;
			}
		} else if (m->m_command.IsSameAs(wxT("PART"))) {
			if ((m->m_params.GetCount() >= 1U) && m->m_params.Item(0U).IsSameAs(m_channel)) {
				if (m_app != NULL)
					m_app->userLeave(m->getPrefixNick());
			}
		} else if (m->m_command.IsSameAs(wxT("KICK"))) {
			if ((m->m_params.GetCount() >= 2U) && m->m_params.Item(0U).IsSameAs(m_channel)) {
				if (m->m_params.Item(1U).IsSameAs(m_currentNick)) {
					// i was kicked!!
					delete m;
					return false;
				} else if (m_app != NULL) {
					m_app->userLeave(m->m_params.Item(1U));
				}
			}
		} else if (m->m_command.IsSameAs(wxT("QUIT"))) {
			if (m_app != NULL)
				m_app->userLeave(m->getPrefixNick());
		} else if (m->m_command.IsSameAs(wxT("MODE"))) {
			if ((m->m_params.GetCount() >= 3U) && m->m_params.Item(0U).IsSameAs(m_channel)) {
				if (m_app != NULL) {
					wxString mode = m->m_params.Item(1U);

					for (unsigned int i = 1; (i < mode.Len()) && (m->m_params.GetCount() >= (i + 2U)); i++) {
						if (mode[i] == wxT('o')) {
							if (mode[0] == wxT('+'))
								m_app->userChanOp(m->m_params.Item(i + 1U), true);
							else if (mode[0] == wxT('-'))
								m_app->userChanOp(m->m_params.Item(i + 1U), false);
						}
					}
				}
			}
		} else if (m->m_command.IsSameAs(wxT("PRIVMSG"))) {
			if ((m->m_params.GetCount() == 2U) && (m_app != NULL)) {
				if (m->m_params.Item(0U).IsSameAs(m_channel))
					m_app->msgChannel(m);
				else if (m->m_params.Item(0U).IsSameAs(m_currentNick))
					m_app->msgQuery(m);
			}
		} else if (m->m_command.IsSameAs(wxT("352"))) {  // WHO list
			if ((m->m_params.GetCount() >= 7U) && m->m_params.Item(0U).IsSameAs(m_currentNick) && m->m_params.Item(1U).IsSameAs(m_channel)) {
				if (m_app != NULL) {
					m_app->userJoin(m->m_params.Item(5U), m->m_params.Item(2U), m->m_params.Item(3U));
					m_app->userChanOp(m->m_params.Item(5U), m->m_params.Item(6U).IsSameAs(wxT("H@")));
				}
			}
		} else if (m->m_command.IsSameAs(wxT("433"))) {  // nick collision
			if (m_state == 2) {
				m_state = 3;  // nick collision, choose new nick

				m_timer.start(5U);		// wait 5 seconds..
			}
		} else if (m->m_command.IsSameAs(wxT("332")) || m->m_command.IsSameAs(wxT("TOPIC"))) {  // topic
			if ((m->m_params.GetCount() == 2U) && (m_app != NULL) && m->m_params.Item(0U).IsSameAs(m_channel))
				m_app->setTopic(m->m_params.Item(1U));
		}

		delete m;
	}

	IRCMessage* m;

	switch (m_state)
	{
		case 1:
			m = new IRCMessage;
			m->m_command = wxT("PASS");
			m->m_params.Add(m_password);
			sendQ->putMessage(m);

			m = new IRCMessage;
			m->m_command = wxT("NICK");
			m->m_params.Add(m_currentNick);
			sendQ->putMessage(m);

			m_timer.start(5U);		// wait for possible nick collision message

			m_state = 2;
			break;

		case 2:
			if (m_timer.isRunning() && m_timer.hasExpired()) {
				m = new IRCMessage;
				m->m_command = wxT("USER");
				m->m_params.Add(m_name);
				m->m_params.Add(wxT("0"));
				m->m_params.Add(wxT("*"));
				m->m_params.Add(m_versionInfo);
				sendQ->putMessage(m);

				m_timer.start(15U);

				m_state = 4; // wait for login message
			}
			break;

		case 3:
			if (m_timer.isRunning() && m_timer.hasExpired()) {
				chooseNewNick();
				m = new IRCMessage;
				m->m_command = wxT("NICK");
				m->m_params.Add(m_currentNick);
				sendQ->putMessage(m);

				m_timer.start(5U);		// wait for possible nick collision message

				m_state = 2;
			}
			break;

		case 4:
			if (m_timer.isRunning() && m_timer.hasExpired())
				// no login message received -> disconnect
				return false;

			break;

		case 5:
			m = new IRCMessage;
			m->m_command = wxT("JOIN");
			m->m_params.Add(m_channel);
			sendQ->putMessage(m);

			m_timer.start(15U);

			m_state = 6; // wait for join message
			break;

		case 6:
			if (m_timer.isRunning() && m_timer.hasExpired())
				// no join message received -> disconnect
				return false;

			break;

		case 7:
			if (m_debugChannel.Len() == 0U)
				return false; // this state cannot be processed if there is no debug_channel

			m = new IRCMessage;
			m->m_command = wxT("JOIN");
			m->m_params.Add(m_debugChannel);
			sendQ->putMessage(m);

			m_timer.start(15U);

			m_state = 8; // wait for join message
			break;

		case 8:
			if (m_timer.isRunning() && m_timer.hasExpired())
				// no join message received -> disconnect
				return false;
			break;

		case 10:
			m = new IRCMessage;
			m->m_command = wxT("WHO");
			m->m_params.Add(m_channel);
			m->m_params.Add(wxT("*"));
			sendQ->putMessage(m);

			m_timer.start(PING_TIMEOUT);

			m_state = 11; // wait for timer and then send ping

			if (m_app != NULL)
				m_app->setSendQ(sendQ);  // this switches the application on

			break;

		case 11:
			if (m_timer.isRunning() && m_timer.hasExpired()) {
				m = new IRCMessage;
				m->m_command = wxT("PING");
				m->m_params.Add(m_currentNick);
				sendQ->putMessage(m);

				m_timer.start(PING_TIMEOUT);

				m_state = 12; // wait for pong
			}
			break;

		case 12:
			if (m_timer.isRunning() && m_timer.hasExpired())
				// no pong message received -> disconnect
				return false;

			break;
	}

	return true;
}
