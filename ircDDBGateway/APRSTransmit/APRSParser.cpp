/*
 *   Copyright (C) 2014 by Jonathan Naylor G4KLX
 *   APRSTransmit Copyright (C) 2015 Geoffrey Merck F4FXL / KC3FRA
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

// Most of the code has been borrowed from APRX Software. I just kept the parts I need for my purpose and C++ed it. Hence the copyright text below.
//https://github.com/PhirePhly/aprx/blob/master/parse_aprs.c

/********************************************************************
 *  APRX -- 2nd generation APRS-i-gate with                         *
 *          minimal requirement of esoteric facilities or           *
 *          libraries of any kind beyond UNIX system libc.          *
 *                                                                  *
 * (c) Matti Aarnio - OH2MQK,  2007-2014                            *
 *                                                                  *
 ********************************************************************/
/*
 *	Some parts of this code are copied from:
 *
 *	aprsc
 *
 *	(c) Heikki Hannikainen, OH7LZB <hessu@hes.iki.fi>
 *
 *     This program is licensed under the BSD license, which can be found
 *     in the file LICENSE.
 *	
 */

#include "APRSParser.h"
#include <wx/log.h> 
#include <iostream>

CAPRSPacket::CAPRSPacket() :
m_type(APT_Unknown),
m_latitude(0.0F),
m_longitude(0.0F),
m_infoText(),
m_raw()
{
}
CAPRSPacket::CAPRSPacket(APRSPacketType type, float latitude, float longitude, const wxString& infoText, const wxString raw) :
m_type(type),
m_latitude(latitude),
m_longitude(longitude),
m_infoText(infoText),
m_raw(raw)
{
}

float CAPRSPacket::Longitude(){
	return m_latitude;
}
float CAPRSPacket::Latitude(){
	return m_longitude;
}
wxString& CAPRSPacket::InfoText(){
	return m_infoText;
}
wxString& CAPRSPacket::Raw(){
	return m_raw;
}

APRSPacketType& CAPRSPacket::Type(){
	return m_type;
}

/*****************************************************************************
* Parsed an APRS string into an APRS frame
* currently it does nothing but determine if the packet is Icom supported
* I already added provision for conversion of unsupported packet formats to supported formats
******************************************************************************/
bool CAPRSParser::Parse(const wxString& aprsFrame, CAPRSPacket& packet)
{
	wxString aprsFrameLocal(aprsFrame);
	wxString body = aprsFrameLocal.AfterFirst(':');
	if(body.IsEmpty()){
		wxLogWarning(wxT("Unable to find body in APRS Frame : ") + aprsFrameLocal);
		return false;
	}

	wxChar packetType = body.GetChar(0);//First char contains the packet type
	body = body.Mid(1);//strip the type char from the body

	packet.Raw() = wxString(aprsFrameLocal);
	packet.Type() = APT_Unknown;
	
	switch(packetType)
	{
		case '\'':
		case '`' :
			//Packet is of MicE type
			//Icom radios do not support MicE so ignore for now;
			//TODO? Add converion to icom supported packet format ?
			packet.Type() = APT_Position;
			return false;
		case '!' :
			if(body.GetChar(0) == '!'){
				//Ultimeter 2000 weather station
				//same procedure as above, ignore for now
				return false;
			}
		case '=' :
		case '/' :
		case '@' :
			{
				if(body.Length() < 10) return false;//enough chars to have a chance to parse it ?
				/* Normal or compressed location packet, with or without
				 * timestamp, with or without messaging capability
			 	*
			 	* ! and / have messaging, / and @ have a prepended timestamp
			 	*/
				packet.Type() = APT_Position;
				if(packetType == '/' || packetType == '@')//With a prepended timestamp, jump over it. 
					body = body.Mid(7);

				wxChar posChar = body.GetChar(0);
				if(valid_sym_table_compressed(posChar)//Compressed format
				   && body.Length() >= 13){//we need at least 13 char
					//icom unsupported, ignore for now
					return false;//parse_aprs_compressed(pb, body, body_end);
				}
				else if(posChar >= '0' && posChar <= '9' //Normal uncompressed format
					&& body.Length() >=19){//we need at least 19 chars for it to be valid
					
					if(ensureIsIcomCompatible(aprsFrameLocal, body, packet))
						return Parse(aprsFrameLocal, packet);
					return true;
				}
				break;
			}
		case '$' : //NMEA packet
			if(body.Length() > 10){
				//parsing for later
				return true;
			}
			break;
		case ':' :
			//We have an APRS message
			packet.Type() = APT_Message;
			//Nice future feature would be to add conversion to icom Android App messaging
			return false;
		case ';' :
			//we have an object
			if(body.Length() > 29){
				//TODO : Parsing ...
				packet.Type() = APT_Object;
				return true;
			}
			break;
		case ')' :
			//this is an item
			if(body.Length() > 19){
				//TODO : Parsing ...
				packet.Type() = APT_Item;
				return true;
			}
			break;
		case '>' ://APRS status...
		case '<' ://stat capa
		case '?' ://Query
		case 'T' ://Telemetry
			return false;//not supported
		case '#': /* Peet Bros U-II Weather Station */
		case '*': /* Peet Bros U-I  Weather Station */
		case '_': /* Weather report without position */
			packet.Type() = APT_WX;
			return true; // AFAIK _ is supported, not sure for the others
		case '{' ://custom
			return false;
	}

	return false;
}

/* Stupidly icom always expects a / after the symbol char, even if no data extension is present -_-
* This function makes sure the frame is icom compatible. In case the frame is invalid, A COMPLETE aprsFrame is built.
* If no modifications were made true is returned, otherwise false */
bool CAPRSParser::ensureIsIcomCompatible(wxString& aprsFrame, const wxString& body, CAPRSPacket& packet)
{
	const int maxAprsFrameLen = 64;

	bool changeMade = false;
	wxChar symbol = body.GetChar(18);
	wxChar charAfterSymbol = body.GetChar(19);
	wxString newBody(body);
	if(charAfterSymbol != '/'
	  && symbol != '_'){//do not do this for weather packets !
		newBody = body.Mid(0, 19) + wxT("/") + body.Mid(19);
		aprsFrame.Replace(body, newBody);
		changeMade = true;
	}

	//Trim the path, only keep from and to. Icom device do not parse too long frames.
	if(aprsFrame.Length() > maxAprsFrameLen){
		wxString dataField = aprsFrame.AfterFirst(':');
		wxString addressField = aprsFrame.BeforeFirst(':');
		addressField = addressField.BeforeFirst(',');
		aprsFrame = addressField + wxT(":") + dataField;
		changeMade = true;
	}

	//Still too long ?
	if(aprsFrame.Length() > maxAprsFrameLen){
		aprsFrame = aprsFrame.Left(maxAprsFrameLen);
		changeMade = true;
	}

	return changeMade;
}

bool CAPRSParser::valid_sym_table_compressed(wxChar c)
{
	return (c == '/' || c == '\\' || (c >= 0x41 && c <= 0x5A)
		    || (c >= 0x61 && c <= 0x6A)); /* [\/\\A-Za-j] */
}

bool CAPRSParser::valid_sym_table_uncompressed(wxChar c)
{
	return (c == '/' || c == '\\' || (c >= 0x41 && c <= 0x5A)
		    || (c >= 0x30 && c <= 0x39)); /* [\/\\A-Z0-9] */
}
