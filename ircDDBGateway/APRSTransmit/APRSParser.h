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

#ifndef	APRSParserAppD_H
#define	APRSParserAppD_H

#include <wx/string.h>

enum APRSPacketType {
	APT_Unknown,	
	APT_Position,
	APT_WX,
	APT_Object,
	APT_Item,
	APT_Message,
	APT_NMEA
};

class CAPRSPacket{
public :
	CAPRSPacket();
	CAPRSPacket(APRSPacketType type, float latitude, float longitude, const wxString& infoText, const wxString raw);
	
	float&          Longitude();
	float&          Latitude();
	wxString&	SourceCall();
	wxChar&           Symbol();
	wxChar&           SymbolTable();
	wxChar&		TypeChar();
	wxString&	DestinationCall();
	wxString&       Body();
	wxString&       Raw();
	APRSPacketType& Type();

private:
	APRSPacketType m_type;
	float          m_latitude;
	float          m_longitude;
	wxChar	       m_symbol;
	wxChar	       m_symbolTable;
	wxChar           m_typeChar;
	wxString       m_body;
	wxString       m_raw;//raw string in TNC2 format, including '\r'
	wxString       m_destCall;
	wxString       m_srcCall;
};

class CAPRSParser{
public:
	static bool Parse(const wxString& aprsString, CAPRSPacket& packet);

private :
	static bool valid_sym_table_compressed(wxChar c);
	static bool valid_sym_table_uncompressed(wxChar c);
	static bool parse_aprs_mice(CAPRSPacket& packet);

	static bool ensureIsIcomCompatible(CAPRSPacket& packet);
	static bool stripFrame(wxString& aprsFrame);
	static bool preprocessFrame(CAPRSPacket& packet, const wxString& aprsFrame);
	static void convertToIcomCompatible(CAPRSPacket& packet);
};

#endif
