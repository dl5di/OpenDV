/*
 *   Copyright (C) 2011,2012 by Jonathan Naylor G4KLX
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

#ifndef	DExtraGatewayEchoUnit_H
#define	DExtraGatewayEchoUnit_H

#include "DExtraGatewayCallback.h"
#include "Timer.h"

#include <wx/wx.h>

enum ECHO_STATUS {
	ES_IDLE,
	ES_RECEIVE,
	ES_WAIT,
	ES_TRANSMIT
};

class CDExtraGatewayEchoUnit {
public:
	CDExtraGatewayEchoUnit(IDExtraGatewayCallback* handler, const wxString& callsign);
	~CDExtraGatewayEchoUnit();

	void writeHeader(const CDExtraGatewayHeaderData& header);

	void writeData(const CDExtraGatewayAMBEData& data);

	void cancel();

	void clock(unsigned int ms);

private:
	IDExtraGatewayCallback*   m_handler;
	wxString                  m_callsign;
	ECHO_STATUS               m_status;
	CTimer                    m_timer;
	CDExtraGatewayHeaderData* m_header;
	CDExtraGatewayAMBEData**  m_data;
	unsigned int              m_in;
	unsigned int              m_out;
	wxStopWatch               m_time;
};

#endif
