/*
 *   Copyright (C) 2010,2011,2012 by Jonathan Naylor G4KLX
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

#ifndef	RepeaterProtocolHandler_H
#define	RepeaterProtocolHandler_H

#include "HeaderData.h"
#include "StatusData.h"
#include "HeardData.h"
#include "AMBEData.h"
#include "TextData.h"
#include "PollData.h"
#include "DDData.h"

#include <wx/wx.h>

enum REPEATER_TYPE {
	RT_NONE,
	RT_POLL,
	RT_HEARD,
	RT_HEADER,
	RT_AMBE,
	RT_BUSY_HEADER,
	RT_BUSY_AMBE,
	RT_DD
};

class IRepeaterProtocolHandler {
public:
	virtual bool open() = 0;

	virtual bool writeHeader(CHeaderData& header) = 0;
	virtual bool writeAMBE(CAMBEData& data) = 0;
	virtual bool writeDD(CDDData& data) = 0;
	virtual bool writeText(CTextData& text) = 0;
	virtual bool writeStatus(CStatusData& status) = 0;

	virtual REPEATER_TYPE read() = 0;
	virtual CPollData*    readPoll() = 0;
	virtual CHeardData*   readHeard() = 0;
	virtual CHeaderData*  readHeader() = 0;
	virtual CAMBEData*    readAMBE() = 0;
	virtual CDDData*      readDD() = 0;
	virtual CHeaderData*  readBusyHeader() = 0;
	virtual CAMBEData*    readBusyAMBE() = 0;

	virtual void close() = 0;

private:
};

#endif
