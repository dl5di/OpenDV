/*
 *   Copyright (C) 2013 by Jonathan Naylor G4KLX
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

#include "DummyRepeaterProtocolHandler.h"

#include "DStarDefines.h"
#include "Utils.h"

CDummyRepeaterProtocolHandler::CDummyRepeaterProtocolHandler()
{
}

CDummyRepeaterProtocolHandler::~CDummyRepeaterProtocolHandler()
{
}

bool CDummyRepeaterProtocolHandler::open()
{
	return true;
}

bool CDummyRepeaterProtocolHandler::writeHeader(CHeaderData& header)
{
	unsigned char buffer[50U];
	unsigned int length = header.getHBRepeaterData(buffer, 50U, true);

	wxLogMessage(wxT("Sending Header to port: %u, id: %04X"), header.getYourPort(), header.getId());

	CUtils::dump(wxT("Data"), buffer + 8U, length - 8U);

	return true;
}

bool CDummyRepeaterProtocolHandler::writeAMBE(CAMBEData& data)
{
	unsigned char buffer[30U];
	unsigned int length = data.getHBRepeaterData(buffer, 30U);

	wxLogMessage(wxT("Sending AMBE to port: %u, seq: %02X, id: %04X"), data.getYourPort(), data.getSeq(), data.getId());

	CUtils::dump(wxT("Data"), buffer + 9U, length - 9U);

	return true;
}

bool CDummyRepeaterProtocolHandler::writeDD(CDDData& data)
{
	unsigned char buffer[2000U];
	unsigned int length = data.getHBRepeaterData(buffer, 2000U);

	CUtils::dump(wxT("DD Data"), buffer, length);

	return true;
}

bool CDummyRepeaterProtocolHandler::writeText(CTextData& text)
{
	unsigned char buffer[40U];
	unsigned int length = text.getHBRepeaterData(buffer, 40U);

	CUtils::dump(wxT("Sending Text"), buffer, length);

	return true;
}

bool CDummyRepeaterProtocolHandler::writeStatus(CStatusData& status)
{
	unsigned char buffer[30U];
	unsigned int length = status.getHBRepeaterData(buffer, 30U);

	CUtils::dump(wxT("Sending Status"), buffer, length);

	return true;
}

REPEATER_TYPE CDummyRepeaterProtocolHandler::read()
{
	return RT_NONE;
}

CPollData* CDummyRepeaterProtocolHandler::readPoll()
{
	return NULL;
}

CHeaderData* CDummyRepeaterProtocolHandler::readHeader()
{
	return NULL;
}

CAMBEData* CDummyRepeaterProtocolHandler::readAMBE()
{
	return NULL;
}

CHeaderData* CDummyRepeaterProtocolHandler::readBusyHeader()
{
	return NULL;
}

CAMBEData* CDummyRepeaterProtocolHandler::readBusyAMBE()
{
	return NULL;
}

CHeardData* CDummyRepeaterProtocolHandler::readHeard()
{
	return NULL;
}

CDDData* CDummyRepeaterProtocolHandler::readDD()
{
	return NULL;
}

void CDummyRepeaterProtocolHandler::close()
{
}
