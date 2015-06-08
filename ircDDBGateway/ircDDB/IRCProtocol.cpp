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


#include "IRCProtocol.h"

#include <wx/regex.h>

#define CIRCDDB_VERSION	  "1.2.4"

IRCProtocol::IRCProtocol ( IRCApplication * app,
    const wxString& callsign, const wxString& password, const wxString& channel,
    const wxString& versionInfo )
{
  this -> password = password;
  this -> channel = channel;
  this -> app = app;

  this->versionInfo = wxT("CIRCDDB:");
  this->versionInfo.Append(wxT(CIRCDDB_VERSION));

  if (versionInfo.Len() > 0)
  {
    this->versionInfo.Append(wxT(" "));
    this->versionInfo.Append(versionInfo);
  }


  int hyphenPos = callsign.find(wxT('-'));

  if (hyphenPos == wxNOT_FOUND)
  {
    wxString n;

    n = callsign + wxT("-1");
    nicks.Add(n);
    n = callsign + wxT("-2");
    nicks.Add(n);
    n = callsign + wxT("-3");
    nicks.Add(n);
    n = callsign + wxT("-4");
    nicks.Add(n);
  }
  else
  {
    nicks.Add(callsign);
  }

  name = callsign;

  pingTimer = 60; // 30 seconds
  state = 0;
  timer = 0;


  chooseNewNick();
}

IRCProtocol::~IRCProtocol()
{
}

void IRCProtocol::chooseNewNick()
{
  int r = rand() % nicks.GetCount();

  currentNick = nicks[r];
}

void IRCProtocol::setNetworkReady( bool b )
{
  if (b == true)
  {
    if (state != 0)
    {
      wxLogError(wxT("IRCProtocol::setNetworkReady: unexpected state"));
    }

    state = 1;
    chooseNewNick();
  }
  else
  {
    state = 0;
  }
}


bool IRCProtocol::processQueues ( IRCMessageQueue * recvQ, IRCMessageQueue * sendQ )
{
  if (timer > 0)
  {
    timer --;
  }

  while (recvQ->messageAvailable())
  {
    IRCMessage * m = recvQ -> getMessage();

#if defined(DEBUG_IRC)
    wxString d = wxT("R [") + m->prefix + wxT("] [") + m->command + wxT("]");
    for (int i=0; i < m->numParams; i++)
    {
      d.Append(wxT(" [") + m->params[i] + wxT("]") );
    }
    d.Replace(wxT("%"), wxT("%%"), true);
    d.Replace(wxT("\\"), wxT("\\\\"), true);
    wxLogVerbose(d);
#endif

    if (m->command.IsSameAs(wxT("004")))
    {
      if (state == 4)
      {
	if (m->params.GetCount() > 1)
	{
	  wxRegEx serverNamePattern(wxT("^grp[1-9]s[1-9].ircDDB$"));

	  if (serverNamePattern.Matches( m->params[1] ))
	  {
	    app->setBestServer(wxT("s-") + m->params[1].Mid(0,6));
	  }
	}
	state = 5;  // next: JOIN
	app->setCurrentNick(currentNick);
      }
    }
    else if (m->command.IsSameAs(wxT("PING")))
    {
      IRCMessage * m2 = new IRCMessage();
      m2->command = wxT("PONG");
      if (m->params.GetCount() > 0)
      {
	m2->numParams = 1;
	m2->params.Add( m->params[0] );
      }
      sendQ -> putMessage(m2);
    }
    else if (m->command.IsSameAs(wxT("JOIN")))
    {
      if ((m->numParams >= 1) && m->params[0].IsSameAs(channel))
      {
	if (m->getPrefixNick().IsSameAs(currentNick) && (state == 6))
	{
	  if (debugChannel.Len() > 0)
	  {
	    state = 7;  // next: join debug_channel
	  }
	  else
	  {
	    state = 10; // next: WHO *
	  }
	}
	else if (app != NULL)
	{
		app->userJoin( m->getPrefixNick(), m->getPrefixName(), m->getPrefixHost());
	}
      }

      if ((m->numParams >= 1) && m->params[0].IsSameAs(debugChannel))
      {
	if (m->getPrefixNick().IsSameAs(currentNick) && (state == 8))
	{
	  state = 10; // next: WHO *
	}
      }
    }
    else if (m->command.IsSameAs(wxT("PONG")))
    {
      if (state == 12)
      {
	timer = pingTimer;
	state = 11;
      }
    }
    else if (m->command.IsSameAs(wxT("PART")))
    {
      if ((m->numParams >= 1) && m->params[0].IsSameAs(channel))
      {
	if (app != NULL)
	{
	  app->userLeave( m->getPrefixNick() );
	}
      }
    }
    else if (m->command.IsSameAs(wxT("KICK")))
    {
      if ((m->numParams >= 2) && m->params[0].IsSameAs(channel))
      {
	if (m->params[1].IsSameAs(currentNick))
	{
		// i was kicked!!
		delete m;
		return false;
	}
	else if (app != NULL)
	{
	  app->userLeave( m->params[1] );
	}
      }
    }
    else if (m->command.IsSameAs(wxT("QUIT")))
    {
      if (app != NULL)
      {
	app->userLeave( m->getPrefixNick() );
      }
    }
    else if (m->command.IsSameAs(wxT("MODE")))
    {
      if ((m->numParams >= 3) && m->params[0].IsSameAs(channel))
      {
	if (app != NULL)
	{
	  size_t i;
	  wxString mode = m->params[1];

	  for (i = 1; (i < mode.Len()) && ((size_t) m->numParams >= (i+2)); i++)
	  {
	    if ( mode[i] == wxT('o') )
	    {
	      if ( mode[0] == wxT('+') )
	      {
		      app->userChanOp(m->params[i+1], true);
	      }
	      else if ( mode[0] == wxT('-') )
	      {
		      app->userChanOp(m->params[i+1], false);
	      }
	    }
	  } // for
	}
      }
    }
    else if (m->command.IsSameAs(wxT("PRIVMSG")))
    {
      if ((m->numParams == 2) && (app != NULL))
      {
	if (m->params[0].IsSameAs(channel))
	{
	  app->msgChannel(m);
	}
	else if (m->params[0].IsSameAs(currentNick))
	{
	  app->msgQuery(m);
	}
      }
    }
    else if (m->command.IsSameAs(wxT("352")))  // WHO list
    {
      if ((m->numParams >= 7) && m->params[0].IsSameAs(currentNick)
	      && m->params[1].IsSameAs(channel))
      {
	if (app != NULL)
	{
	  app->userJoin( m->params[5], m->params[2], m->params[3]);
	  app->userChanOp ( m->params[5], m->params[6].IsSameAs(wxT("H@")));
	}
      }
    }
    else if (m->command.IsSameAs(wxT("433")))  // nick collision
    {
      if (state == 2)
      {
	state = 3;  // nick collision, choose new nick
	timer = 10; // wait 5 seconds..
      }
    }
    else if (m->command.IsSameAs(wxT("332")) ||
                  m->command.IsSameAs(wxT("TOPIC")))  // topic
    {
      if ((m->numParams == 2) && (app != NULL) &&
	      m->params[0].IsSameAs(channel) )
      {
	      app->setTopic(m->params[1]);
      }
    }

    delete m;
  }

  IRCMessage * m;

  switch (state)
  {
  case 1:
    m = new IRCMessage();
    m->command = wxT("PASS");
    m->numParams = 1;
    m->params.Add(password);
    sendQ->putMessage(m);

    m = new IRCMessage();
    m->command = wxT("NICK");
    m->numParams = 1;
    m->params.Add(currentNick);
    sendQ->putMessage(m);

    timer = 10;  // wait for possible nick collision message
    state = 2;
    break;

  case 2:
    if (timer == 0)
    {
      m = new IRCMessage();
      m->command = wxT("USER");
      m->numParams = 4;
      m->params.Add(name);
      m->params.Add(wxT("0"));
      m->params.Add(wxT("*"));
      m->params.Add(versionInfo);
      sendQ->putMessage(m);

      timer = 30;
      state = 4; // wait for login message
    }
    break;

  case 3:
    if (timer == 0)
    {
      chooseNewNick();
      m = new IRCMessage();
      m->command = wxT("NICK");
      m->numParams = 1;
      m->params.Add(currentNick);
      sendQ->putMessage(m);

      timer = 10;  // wait for possible nick collision message
      state = 2;
    }
    break;

  case 4:
    if (timer == 0)
    {
      // no login message received -> disconnect
      return false;
    }
    break;

  case 5:
    m = new IRCMessage();
    m->command = wxT("JOIN");
    m->numParams = 1;
    m->params.Add(channel);
    sendQ->putMessage(m);

    timer = 30;
    state = 6; // wait for join message
    break;

  case 6:
    if (timer == 0)
    {
      // no join message received -> disconnect
      return false;
    }
    break;

  case 7:
    if (debugChannel.Len() == 0)
    {
      return false; // this state cannot be processed if there is no debug_channel
    }

    m = new IRCMessage();
    m->command = wxT("JOIN");
    m->numParams = 1;
    m->params.Add(debugChannel);
    sendQ->putMessage(m);

    timer = 30;
    state = 8; // wait for join message
    break;

  case 8:
    if (timer == 0)
    {
      // no join message received -> disconnect
      return false;
    }
    break;

  case 10:
    m = new IRCMessage();
    m->command = wxT("WHO");
    m->numParams = 2;
    m->params.Add(channel);
    m->params.Add(wxT("*"));
    sendQ->putMessage(m);

    timer = pingTimer;
    state = 11; // wait for timer and then send ping

    if (app != NULL)
    {
      app->setSendQ(sendQ);  // this switches the application on
    }
    break;

  case 11:
    if (timer == 0)
    {
      m = new IRCMessage();
      m->command = wxT("PING");
      m->numParams = 1;
      m->params.Add(currentNick);
      sendQ->putMessage(m);

      timer = pingTimer;
      state = 12; // wait for pong
    }
    break;

  case 12:
    if (timer == 0)
    {
      // no pong message received -> disconnect
      return false;
    }
    break;
  }

  return true;
}


