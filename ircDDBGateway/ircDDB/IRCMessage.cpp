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

#include "IRCMessage.h"



IRCMessage::IRCMessage ()
{
  numParams = 0;
  prefixParsed = false;
}

IRCMessage::IRCMessage ( const wxString& toNick, const wxString& msg )
{
  command = wxT("PRIVMSG");
  numParams = 2;
  params.Add( toNick );
  params.Add( msg );
  prefixParsed = false;
}

IRCMessage::IRCMessage ( const wxString& cmd )
{
  command = cmd;
  numParams = 0;
  prefixParsed = false;
}

IRCMessage::~IRCMessage()
{
}


void IRCMessage::addParam( const wxString& p )
{
  params.Add( p );
  numParams = params.GetCount();
}

int IRCMessage::getParamCount()
{
  return params.GetCount();
}

wxString IRCMessage::getParam( int pos )
{
  return params[pos];
}

wxString IRCMessage::getCommand()
{
  return command;
}

	
void IRCMessage::parsePrefix()
{
  unsigned int i;

  for (i=0; i < 3; i++)
  {
    prefixComponents.Add(wxT(""));
  }

  int state = 0;
  
  for (i=0; i < prefix.Len(); i++)
  {
    wxChar c = prefix.GetChar(i);
			
    switch (c)
    {
    case wxT('!'): 
	    state = 1; // next is name
	    break;
	    
    case wxT('@'):
	    state = 2; // next is host
	    break;
	    
    default:
	    prefixComponents[state].Append(c);
	    break;
    }
  }

  prefixParsed = true;
}

wxString& IRCMessage::getPrefixNick()
{
  if (!prefixParsed)
  {
    parsePrefix();
  }
  
  return prefixComponents[0];
}

wxString& IRCMessage::getPrefixName()
{
  if (!prefixParsed)
  {
    parsePrefix();
  }
  
  return prefixComponents[1];
}

wxString& IRCMessage::getPrefixHost()
{
  if (!prefixParsed)
  {
    parsePrefix();
  }
  
  return prefixComponents[2];
}

void IRCMessage::composeMessage ( wxString& output )
{
#if defined(DEBUG_IRC)
  wxString d = wxT("T [") + prefix + wxT("] [") + command + wxT("]");
  for (int i=0; i < numParams; i++)
  {
    d.Append(wxT(" [") + params[i] + wxT("]") );
  }
  d.Replace(wxT("%"), wxT("%%"), true);
  d.Replace(wxT("\\"), wxT("\\\\"), true);
  wxLogVerbose(d);
#endif

  wxString o;

  if (prefix.Len() > 0)
  {
    o = wxT(":") + prefix + wxT(" ");
  }

  o.Append(command);

  for (int i=0; i < numParams; i++)
  {
    if (i == (numParams - 1))
    {
      o.Append(wxT(" :") + params[i]);
    }
    else
    {
      o.Append(wxT(" ") + params[i]);
    }
  }

  o.Append(wxT("\r\n"));

  output = o;
}

