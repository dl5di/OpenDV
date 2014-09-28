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

#include "IRCMessage.h"

IRCMessage::IRCMessage() :
m_prefix(),
m_command(),
m_params(),
m_prefixComponents(),
m_prefixParsed(false)
{
}

IRCMessage::IRCMessage ( const wxString& toNick, const wxString& msg ) :
m_prefix(),
m_command(wxT("PRIVMSG")),
m_params(),
m_prefixComponents(),
m_prefixParsed(false)
{
	m_params.Add(toNick);
	m_params.Add(msg);
}

IRCMessage::IRCMessage(const wxString& cmd) :
m_prefix(),
m_command(cmd),
m_params(),
m_prefixComponents(),
m_prefixParsed(false)
{
}

IRCMessage::~IRCMessage()
{
}

void IRCMessage::parsePrefix()
{
	for (unsigned int i = 0U; i < 3U; i++)
		m_prefixComponents.Add(wxEmptyString);

	int state = 0;

	for (unsigned int i = 0U; i < m_prefix.Len(); i++) {
		wxChar c = m_prefix.GetChar(i);

		switch (c) {
			case wxT('!'): 
				state = 1; // next is name
				break;

			case wxT('@'):
				state = 2; // next is host
				break;

			default:
				m_prefixComponents[state].Append(c);
				break;
		}
	}

	m_prefixParsed = true;
}

wxString& IRCMessage::getPrefixNick()
{
	if (!m_prefixParsed)
		parsePrefix();

	return m_prefixComponents.Item(0);
}

wxString& IRCMessage::getPrefixName()
{
	if (!m_prefixParsed)
		parsePrefix();

	return m_prefixComponents.Item(1);
}

wxString& IRCMessage::getPrefixHost()
{
	if (!m_prefixParsed)
		parsePrefix();

	return m_prefixComponents.Item(2);
}

wxString IRCMessage::composeMessage() const
{
	unsigned int numParams = m_params.GetCount();

	wxString o;

	if (m_prefix.Len() > 0U) {
		o.Append(wxT(":"));
		o.Append(m_prefix);
		o.Append(wxT(" "));
	}

	o.Append(m_command);

	for (unsigned int i = 0; i < numParams; i++) {
		if (i == (numParams - 1U))
			o.Append(wxT(" :"));
		else
			o.Append(wxT(" "));

		o.Append(m_params[i]);
	}

	o.Append(wxT("\r\n"));

	return o;
}
