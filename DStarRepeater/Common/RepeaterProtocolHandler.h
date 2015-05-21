/*
 *   Copyright (C) 2009-2015 by Jonathan Naylor G4KLX
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

#ifndef	RepeaterProtocolHander_H
#define	RepeaterProtocolHander_H

#include "UDPReaderWriter.h"
#include "DStarDefines.h"
#include "HeaderData.h"
#include "Types.h"

#include <string>

class CRepeaterProtocolHandler {
public:
	CRepeaterProtocolHandler(const std::string& gatewayAddress, unsigned int gatewayPort, const std::string& localAddress, unsigned int localPort, const std::string& name);
	~CRepeaterProtocolHandler();

	bool open();

	bool writeHeader(const CHeaderData& header);
	bool writeBusyHeader(const CHeaderData& header);
	bool writeData(const unsigned char* data, unsigned int length, unsigned int errors, bool end);
	bool writeBusyData(const unsigned char* data, unsigned int length, unsigned int errors, bool end);
	bool writePoll(const std::string& text);
	bool writeRegister();

	NETWORK_TYPE read();
	void         readText(std::string& text, LINK_STATUS& status, std::string& reflector);
	void         readTempText(std::string& text);
	std::string  readStatus1();
	std::string  readStatus2();
	std::string  readStatus3();
	std::string  readStatus4();
	std::string  readStatus5();
	CHeaderData* readHeader();
	unsigned int readData(unsigned char* data, unsigned int length, unsigned char& seqNo);

	void reset();

	void close();

private:
	CUDPReaderWriter m_socket;
	in_addr          m_address;
	unsigned int     m_port;
	std::string      m_name;
	uint16_t         m_outId;
	uint8_t          m_outSeq;
	NETWORK_TYPE     m_type;
	uint16_t         m_inId;
	unsigned char*   m_buffer;
	unsigned int     m_length;

	bool readPackets();
};

#endif
