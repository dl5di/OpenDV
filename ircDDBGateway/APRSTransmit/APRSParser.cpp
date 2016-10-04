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
#include <string.h>
#include <math.h>

CAPRSPacket::CAPRSPacket() :
m_type(APT_Unknown),
m_latitude(0.0F),
m_longitude(0.0F),
m_symbol('\0'),
m_symbolTable('\0'),
m_typeChar('\0'),
m_body(),
m_raw()
{
}
CAPRSPacket::CAPRSPacket(APRSPacketType type, float latitude, float longitude, const wxString& infoText, const wxString raw) :
m_type(type),
m_latitude(latitude),
m_longitude(longitude),
m_symbol('\0'),
m_symbolTable('\0'),
m_typeChar('\0'),
m_body(infoText),
m_raw(raw),
m_destCall(),
m_srcCall()
{
}

float& CAPRSPacket::Longitude(){
	return m_longitude;
}
float& CAPRSPacket::Latitude(){
	return m_latitude;
}
wxString& CAPRSPacket::Body(){
	return m_body;
}
wxString& CAPRSPacket::Raw(){
	return m_raw;
}

wxChar& CAPRSPacket::Symbol(){
	return m_symbol;
}

wxChar& CAPRSPacket::SymbolTable(){
	return m_symbolTable;
}

wxChar& CAPRSPacket::TypeChar(){
	return m_typeChar;
}

APRSPacketType& CAPRSPacket::Type(){
	return m_type;
}


wxString& CAPRSPacket::SourceCall(){
	return m_srcCall;
}
wxString& CAPRSPacket::DestinationCall(){
	return m_destCall;
}

/*****************************************************************************
* Parsed an APRS string into an APRS frame
* currently it does nothing but determine if the packet is Icom supported
* I already added provision for conversion of unsupported packet formats to supported formats
******************************************************************************/
bool CAPRSParser::Parse(const wxString& aprsFrame, CAPRSPacket& packet)
{
	wxString aprsFrameLocal(aprsFrame);
	stripFrame(aprsFrameLocal);
	if(!preprocessFrame(packet, aprsFrameLocal))
		return false;
	
	switch(packet.TypeChar())
	{
		case '\'':
		case '`' :
			{
				//Packet is of MicE type
				//Icom radios do not support MicE so convert
				if(parse_aprs_mice(packet)){
					wxLogMessage(wxT("Got MicE format, converting to Icom supported"));
					convertToIcomCompatible(packet);
					return Parse(packet.Raw(), packet);
				}
				packet.Type() = APT_Position;
				return false;
			}
		case '!' :
			if(packet.Body().GetChar(0) == '!'){
				//Ultimeter 2000 weather station
				//same procedure as above, ignore for now
				return false;
			}
		case '=' :
		case '/' :
		case '@' :
			{
				if(packet.Body().Length() < 10) return false;//enough chars to have a chance to parse it ?
				/* Normal or compressed location packet, with or without
				 * timestamp, with or without messaging capability
			 	*
			 	* ! and / have messaging, / and @ have a prepended timestamp
			 	*/
				packet.Type() = APT_Position;
				if(packet.TypeChar() == '/' || packet.TypeChar() == '@')//With a prepended timestamp, jump over it. 
					packet.Body() = packet.Body().Mid(7);

				wxChar posChar = packet.Body().GetChar(0);
				if(valid_sym_table_compressed(posChar)//Compressed format
				   && packet.Body().Length() >= 13){//we need at least 13 char
					//icom unsupported, ignore for now
					return false;//parse_aprs_compressed(pb, body, body_end);
				}
				else if(posChar >= '0' && posChar <= '9' //Normal uncompressed format
					&& packet.Body().Length() >=19){//we need at least 19 chars for it to be valid
					
					if(ensureIsIcomCompatible(packet))
						return Parse(packet.Raw(), packet);
					return true;
				}
				break;
			}
		case '$' : //NMEA packet
			if(packet.Body().Length() > 10){
				packet.Type() = APT_NMEA;
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
			if(packet.Body().Length() > 29){
				//TODO : Parsing ...
				packet.Type() = APT_Object;
				return true;
			}
			break;
		case ')' :
			//this is an item
			if(packet.Body().Length() > 19){
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

void CAPRSParser::convertToIcomCompatible(CAPRSPacket& packet)
{
	//first build the coordinate string
	float degrees = ::floor(::fabs(packet.Latitude()));
	float minutes = (::fabs(packet.Latitude()) - degrees) *  60.0f;
	char hemisphere = packet.Latitude() > 0.0f ? 'N' : 'S';
	wxString latString = wxString::Format(wxT("%02.0f%05.2f%c"), degrees, minutes, hemisphere);

	degrees = ::floor(::fabs(packet.Longitude()));
	minutes = (::fabs(packet.Longitude()) - degrees) * 60.0f;
	hemisphere = packet.Longitude() > 0.0f ? 'E' : 'W';
	wxString longString = wxString::Format(wxT("%03.0f%05.2f%c"), degrees, minutes, hemisphere);

	/*packet.Raw() = wxString::Format(wxT("%s>%s:=%s%c%s%c/%s"),
						packet.SourceCall().mb_str().data(),
						packet.DestinationCall().mb_str().data(),
						latString.mb_str().data(),
						(char)packet.SymbolTable(),
						longString.mb_str().data(),
						(char)packet.Symbol(),
						packet.Body().mb_str().data());*/

	//above code behaved in different manner under wx2.8 let's do it the brutal way !
	packet.Raw() = packet.SourceCall()
			+ wxT(">")
			+ packet.DestinationCall()
			+ wxT(":=")
			+ latString
			+ packet.SymbolTable()
			+ longString
			+ packet.Symbol()
			+ wxT("/")
			+ packet.Body();
}

//configures the packet's raw, source call and dest call
bool CAPRSParser::preprocessFrame(CAPRSPacket& packet, const wxString& aprsFrame)
{
	wxString addressField = aprsFrame.BeforeFirst(':');
	wxString body = aprsFrame.AfterFirst(':');
	if(addressField == aprsFrame || body.IsEmpty()){
		wxLogWarning(wxT("Failed to find body of aprs frame"));
		return false;
	}

	wxString srcCall = addressField.BeforeFirst('>');
	wxString destCall = addressField.AfterFirst('>');
	if(srcCall == addressField){
		wxLogWarning(wxT("Failed to extract source and destination call"));
		return false;
	}

	packet.TypeChar() = body.GetChar(0);
	packet.Body() = body.Mid(1);//strip the type char
	packet.SourceCall() = srcCall;
	packet.DestinationCall() = destCall;
	packet.Raw() = wxString(aprsFrame);
	packet.Type() = APT_Unknown;

	return true;
}

/* Stupidly icom always expects a / after the symbol char, even if no data extension is present -_-
* This function makes sure the frame is icom compatible. In case the frame is invalid, A COMPLETE aprsFrame is built.
* If no modifications were made true is returned, otherwise false */
bool CAPRSParser::ensureIsIcomCompatible(CAPRSPacket& packet)
{
	bool changeMade = false;
	wxChar symbol = packet.Body().GetChar(18);
	wxChar charAfterSymbol = packet.Body().GetChar(19);
	wxString newBody(packet.Body());
	wxString aprsFrame(packet.Raw());

	if(charAfterSymbol != '/'
	  && symbol != '_'){//do not do this for weather packets !
		newBody = packet.Body().Mid(0, 19) + wxT("/") + packet.Body().Mid(19);
		aprsFrame.Replace(packet.Body(), newBody);
		changeMade = true;
	}

	if(stripFrame(aprsFrame)) changeMade = true;

	packet.Body() = newBody;
	packet.Raw() = aprsFrame;

	return changeMade;
}

bool CAPRSParser::stripFrame(wxString& aprsFrame)
{
	const unsigned int maxAprsFrameLen = 64U;

	bool changeMade = false;

	//Trim the path, only keep from and to. Icom device do not support too long frames.
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


bool CAPRSParser::parse_aprs_mice(CAPRSPacket& packet)
{
	float lat = 0.0, lng = 0.0;
	unsigned int lat_deg = 0, lat_min = 0, lat_min_frag = 0, lng_deg = 0, lng_min = 0, lng_min_frag = 0;
	//const char *d_start;
	char dstcall[7];
	char *p;
	char sym_table, sym_code;
	int posambiguity = 0;
	int i;
	
	//code below is just to map wxWidgets stuff to original APRX pointer based logic.
	char* body = new char[packet.Body().length()];
	for (unsigned int i = 0U; i < packet.Body().length(); i++)
		body[i] = packet.Body().GetChar(i);
	char* body_end = body + packet.Body().length();

	char* d_start = new char[packet.Body().length()];
	for (unsigned int i = 0U; i < packet.Body().length(); i++)
		d_start[i] = packet.DestinationCall().GetChar(i);
	char* dstcall_end_or_ssid = d_start + packet.DestinationCall().length();


	//Original APRX code follows.. Just a few minor changes

	/* check packet length */
	if (body_end - body < 8) {
		delete[] body;
		delete[] d_start;
		return false;
	}
	
	/* check that the destination call exists and is of the right size for mic-e */
	//d_start = pb->srccall_end+1;
	if (dstcall_end_or_ssid - d_start != 6) {
		//DEBUG_LOG(".. bad destcall length! ");
		delete[] body;
		delete[] d_start;
		return false; /* eh...? */
	}
	
	/* validate destination call:
	 * A-K characters are not used in the last 3 characters
	 * and MNO are never used
	 */
 	//if (debug)printf(" destcall='%6.6s'",d_start);
	for (i = 0; i < 3; i++)
		if (!((d_start[i] >= '0' && d_start[i] <= '9')
			|| (d_start[i] >= 'A' && d_start[i] <= 'L')
			|| (d_start[i] >= 'P' && d_start[i] <= 'Z'))) {
			//DEBUG_LOG(".. bad destcall characters in posits 1..3");
			delete[] body;
			delete[] d_start;
			return false;
		}
	
	for (i = 3; i < 6; i++)
		if (!((d_start[i] >= '0' && d_start[i] <= '9')
			|| (d_start[i] == 'L')
			|| (d_start[i] >= 'P' && d_start[i] <= 'Z'))) {
			//DEBUG_LOG(".. bad destcall characters in posits 4..6");
			delete[] body;
			delete[] d_start;
			return false;
		}
	
	//DEBUG_LOG("\tpassed dstcall format check");
	
	/* validate information field (longitude, course, speed and
	 * symbol table and code are checked). Not bullet proof..
	 *
	 *   0          1          23            4          5          6              7
	 * /^[\x26-\x7f][\x26-\x61][\x1c-\x7f]{2}[\x1c-\x7d][\x1c-\x7f][\x21-\x7b\x7d][\/\\A-Z0-9]/
	 */
	if (body[0] < 0x26 || body[0] > 0x7f) {
		//DEBUG_LOG("..bad infofield column 1");
		delete[] body;
		delete[] d_start;
		return false;
	}
	if (body[1] < 0x26 || body[1] > 0x61) {
		//DEBUG_LOG("..bad infofield column 2");
		delete[] body;
		delete[] d_start;
		return false;
	}
	if (body[2] < 0x1c || body[2] > 0x7f) {
		//DEBUG_LOG("..bad infofield column 3");
		delete[] body;
		delete[] d_start;
		return false;
	}
	if (body[3] < 0x1c || body[3] > 0x7f) {
		//DEBUG_LOG("..bad infofield column 4");
		delete[] body;
		delete[] d_start;
		return false;
	}
	if (body[4] < 0x1c || body[4] > 0x7d) {
		//DEBUG_LOG("..bad infofield column 5");
		//delete[] body;
		//delete[] d_start;
		//return false;
	}
	if (body[5] < 0x1c || body[5] > 0x7f) {
		//DEBUG_LOG("..bad infofield column 6");
		delete[] body;
		delete[] d_start;
		return false;
	}
	if ((body[6] < 0x21 || body[6] > 0x7b)
		&& body[6] != 0x7d) {
		//DEBUG_LOG("..bad infofield column 7");
		delete[] body;
		delete[] d_start;
		return false;
	}
	if (!valid_sym_table_uncompressed(body[7])) {
		//DEBUG_LOG("..bad symbol table entry on column 8");
		delete[] body;
		delete[] d_start;
		return false;
	}
	
	//DEBUG_LOG("\tpassed info format check");
	
	/* make a local copy, we're going to modify it */
	strncpy(dstcall, d_start, 6);
	dstcall[6] = 0;
	
	/* First do the destination callsign
	 * (latitude, message bits, N/S and W/E indicators and long. offset)
	 *
	 * Translate the characters to get the latitude
	 */
	 
	//fprintf(stderr, "\tuntranslated dstcall: %s\n", dstcall);
	for (p = dstcall; *p; p++) {
		if (*p >= 'A' && *p <= 'J')
			*p -= 'A' - '0';
		else if (*p >= 'P' && *p <= 'Y')
			*p -= 'P' - '0';
		else if (*p == 'K' || *p == 'L' || *p == 'Z')
			*p = '_';
	}
	//fprintf(stderr, "\ttranslated dstcall: %s\n", dstcall);
	
	// position ambiquity is going to get ignored now,
	// it's not needed in this application.

	if (dstcall[5] == '_') { dstcall[5] = '5'; posambiguity = 1; }
	if (dstcall[4] == '_') { dstcall[4] = '5'; posambiguity = 2; }
	if (dstcall[3] == '_') { dstcall[3] = '5'; posambiguity = 3; }
	if (dstcall[2] == '_') { dstcall[2] = '3'; posambiguity = 4; }
	if (dstcall[1] == '_' || dstcall[0] == '_') {
		//DEBUG_LOG("..bad pos-ambiguity on destcall");
		delete[] body;
		delete[] d_start;
		return false;
	} // cannot use posamb here
	
	// convert to degrees, minutes and decimal degrees,
	//  and then to a float lat

	if (sscanf(dstcall, "%2u%2u%2u",
	    &lat_deg, &lat_min, &lat_min_frag) != 3) {
		//DEBUG_LOG("\tsscanf failed");
		delete[] body;
		delete[] d_start;
		return false;
	}
	lat = (float)lat_deg + (float)lat_min / 60.0 + (float)lat_min_frag / 6000.0;
	
	// check the north/south direction and correct the latitude if necessary
	if (d_start[3] <= 0x4c)
		lat = 0 - lat;
	
	/* Decode the longitude, the first three bytes of the body
	 * after the data type indicator. First longitude degrees,
	 * remember the longitude offset.
	 */
	lng_deg = body[0] - 28;
	if (d_start[4] >= 0x50)
		lng_deg += 100;
	if (lng_deg >= 180 && lng_deg <= 189)
		lng_deg -= 80;
	else if (lng_deg >= 190 && lng_deg <= 199)
		lng_deg -= 190;

	/* Decode the longitude minutes */
	lng_min = body[1] - 28;
	if (lng_min >= 60)
		lng_min -= 60;
		
	/* ... and minute decimals */
	lng_min_frag = body[2] - 28;
	
	/* apply position ambiguity to longitude */
	switch (posambiguity) {
	case 0:
		/* use everything */
		lng = (float)lng_deg + (float)lng_min / 60.0
			+ (float)lng_min_frag / 6000.0;
		break;
	case 1:
		/* ignore last number of lng_min_frag */
		lng = (float)lng_deg + (float)lng_min / 60.0
			+ (float)(lng_min_frag - lng_min_frag % 10 + 5) / 6000.0;
		break;
	case 2:
		/* ignore lng_min_frag */
		lng = (float)lng_deg + ((float)lng_min + 0.5) / 60.0;
		break;
	case 3:
		/* ignore lng_min_frag and last number of lng_min */
		lng = (float)lng_deg + (float)(lng_min - lng_min % 10 + 5) / 60.0;
		break;
	case 4:
		/* minute is unused -> add 0.5 degrees to longitude */
		lng = (float)lng_deg + 0.5;
		break;
	default:
		//DEBUG_LOG(".. posambiguity code BUG!");
		delete[] body;
		delete[] d_start;
		return false;
	}
	
	/* check the longitude E/W sign */
	if (d_start[5] >= 0x50)
		lng = 0 - lng;
	
	/* save the symbol table and code */
	sym_code = body[6];
	sym_table = body[7];
	
	/* ok, we're done */
	/*
	fprintf(stderr, "\tlat %u %u.%u (%.4f) lng %u %u.%u (%.4f)\n",
	 	lat_deg, lat_min, lat_min_frag, lat,
	 	lng_deg, lng_min, lng_min_frag, lng);
	fprintf(stderr, "\tsym '%c' '%c'\n", sym_table, sym_code);
	*/
	

	//return pbuf_fill_pos(pb, lat, lng, sym_table, sym_code);

	///End of APRX original code
	packet.Latitude() = lat;
	packet.Longitude() = lng;
	packet.Symbol() = sym_code;
	packet.SymbolTable() = sym_table;
	packet.Body() = packet.Body().Mid(9);//if MicE has additional info like heading it'll be longer than 9, ignore for now

	delete[] body;
	delete[] d_start;

	return true;
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
