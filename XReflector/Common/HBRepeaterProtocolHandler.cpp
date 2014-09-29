/*
 *   Copyright (C) 2010,2011,2012,2013 by Jonathan Naylor G4KLX
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

#include "HBRepeaterProtocolHandler.h"

#include "CCITTChecksum.h"
#include "DStarDefines.h"
#include "Utils.h"

const unsigned int BUFFER_LENGTH = 255U;

CHBRepeaterProtocolHandler::CHBRepeaterProtocolHandler(const wxString& address, unsigned int port) :
m_socket(address, port),
m_type(RT_NONE),
m_buffer(NULL),
m_length(0U),
m_address(),
m_port(0U)
{
	wxASSERT(!address.IsEmpty());
	wxASSERT(port > 0U);

	m_buffer = new unsigned char[BUFFER_LENGTH];
}

CHBRepeaterProtocolHandler::~CHBRepeaterProtocolHandler()
{
	delete[] m_buffer;
}

bool CHBRepeaterProtocolHandler::open()
{
	return m_socket.open();
}

bool CHBRepeaterProtocolHandler::writeHeader(CHeaderData& header)
{
	unsigned char buffer[50U];
	unsigned int length = header.getHBRepeaterData(buffer, 50U, true);

#if defined(DUMP_TX)
	CUtils::dump(wxT("Sending Header"), buffer, length);
	return true;
#else
	return m_socket.write(buffer, length, header.getYourAddress(), header.getYourPort());
#endif
}

bool CHBRepeaterProtocolHandler::writeAMBE(CAMBEData& data)
{
	unsigned char buffer[30U];
	unsigned int length = data.getHBRepeaterData(buffer, 30U);

#if defined(DUMP_TX)
	CUtils::dump(wxT("Sending Data"), buffer, length);
	return true;
#else
	return m_socket.write(buffer, length, data.getYourAddress(), data.getYourPort());
#endif
}

bool CHBRepeaterProtocolHandler::writeDD(CDDData& data)
{
	return true;
}

bool CHBRepeaterProtocolHandler::writeText(CTextData& text)
{
	unsigned char buffer[40U];
	unsigned int length = text.getHBRepeaterData(buffer, 40U);

#if defined(DUMP_TX)
	CUtils::dump(wxT("Sending Text"), buffer, length);
	return true;
#else
	return m_socket.write(buffer, length, text.getAddress(), text.getPort());
#endif
}

bool CHBRepeaterProtocolHandler::writeStatus(CStatusData& status)
{
	unsigned char buffer[30U];
	unsigned int length = status.getHBRepeaterData(buffer, 30U);

#if defined(DUMP_TX)
	CUtils::dump(wxT("Sending Status"), buffer, length);
	return true;
#else
	return m_socket.write(buffer, length, status.getAddress(), status.getPort());
#endif
}

REPEATER_TYPE CHBRepeaterProtocolHandler::read()
{
	bool res = true;

	// Loop until we have no more data from the socket or we have data for the higher layers
	while (res)
		res = readPackets();

	return m_type;
}

bool CHBRepeaterProtocolHandler::readPackets()
{
	m_type = RT_NONE;

	// No more data?
	int length = m_socket.read(m_buffer, BUFFER_LENGTH, m_address, m_port);
	if (length <= 0)
		return false;

	m_length = length;

	// Invalid packet type?
	if (m_buffer[0] == 'D' && m_buffer[1] == 'S' && m_buffer[2] == 'R' && m_buffer[3] == 'P') {
		// Poll data
		if (m_buffer[4] == 0x0AU) {
			m_type = RT_POLL;
			return false;
		}

		// Header data
		else if (m_buffer[4] == 0x20U) {
			m_type = RT_HEADER;
			return false;
		}

		// User data
		else if (m_buffer[4] == 0x21U) {
			m_type = RT_AMBE;
			return false;
		}

		// Busy header data
		else if (m_buffer[4] == 0x22U) {
			m_type = RT_BUSY_HEADER;
			return false;
		}

		// Busy user data
		else if (m_buffer[4] == 0x23U) {
			m_type = RT_BUSY_AMBE;
			return false;
		}
	}

	CUtils::dump(wxT("Unknown packet from the Repeater"), m_buffer, m_length);

	return true;
}

CPollData* CHBRepeaterProtocolHandler::readPoll()
{
	if (m_type != RT_POLL)
		return NULL;

	wxString text = wxString((char*)(m_buffer + 5U), wxConvLocal);

	return new CPollData(text, m_address, m_port, m_socket.getPort());
}

CHeaderData* CHBRepeaterProtocolHandler::readHeader()
{
	if (m_type != RT_HEADER)
		return NULL;

	CHeaderData* header = new CHeaderData;

	bool res = header->setHBRepeaterData(m_buffer, m_length, true, m_address, m_port);
	if (!res) {
		wxLogError(wxT("Invalid checksum from the repeater"));
		delete header;
		return NULL;
	}

	return header;
}

CAMBEData* CHBRepeaterProtocolHandler::readAMBE()
{
	if (m_type != RT_AMBE)
		return NULL;

	CAMBEData* data = new CAMBEData;

	bool res = data->setHBRepeaterData(m_buffer, m_length, m_address, m_port);
	if (!res) {
		wxLogError(wxT("Invalid AMBE data from the repeater"));
		delete data;
		return NULL;
	}

	return data;
}

CHeaderData* CHBRepeaterProtocolHandler::readBusyHeader()
{
	if (m_type != RT_BUSY_HEADER)
		return NULL;

	CHeaderData* header = new CHeaderData;

	bool res = header->setHBRepeaterData(m_buffer, m_length, true, m_address, m_port);
	if (!res) {
		wxLogError(wxT("Invalid checksum from the repeater"));
		delete header;
		return NULL;
	}

	return header;
}

CAMBEData* CHBRepeaterProtocolHandler::readBusyAMBE()
{
	if (m_type != RT_BUSY_AMBE)
		return NULL;

	CAMBEData* data = new CAMBEData;

	bool res = data->setHBRepeaterData(m_buffer, m_length, m_address, m_port);
	if (!res) {
		wxLogError(wxT("Invalid AMBE data from the repeater"));
		delete data;
		return NULL;
	}

	return data;
}

CHeardData* CHBRepeaterProtocolHandler::readHeard()
{
	return NULL;
}

CDDData* CHBRepeaterProtocolHandler::readDD()
{
	return NULL;
}

void CHBRepeaterProtocolHandler::close()
{
	m_socket.close();
}
