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

#include "DExtraGatewayEchoUnit.h"
#include "DStarDefines.h"

const unsigned int MAX_FRAMES = 60U * DSTAR_FRAMES_PER_SEC;

CDExtraGatewayEchoUnit::CDExtraGatewayEchoUnit(IDExtraGatewayCallback* handler, const wxString& callsign) :
m_handler(handler),
m_callsign(callsign),
m_status(ES_IDLE),
m_timer(1000U, 2U),			// 2 seconds
m_header(NULL),
m_data(NULL),
m_in(0U),
m_out(0U),
m_time()
{
	wxASSERT(handler != NULL);

	m_data = new CDExtraGatewayAMBEData*[MAX_FRAMES];

	for (unsigned int i = 0U; i < MAX_FRAMES; i++)
		m_data[i] = NULL;
}

CDExtraGatewayEchoUnit::~CDExtraGatewayEchoUnit()
{
	delete[] m_data;
}

void CDExtraGatewayEchoUnit::writeHeader(const CDExtraGatewayHeaderData& header)
{
	if (m_status != ES_IDLE)
		return;

	m_header = new CDExtraGatewayHeaderData(header);

	m_in     = 0U;		
	m_status = ES_RECEIVE;
}

void CDExtraGatewayEchoUnit::writeData(const CDExtraGatewayAMBEData& data)
{
	if (m_status != ES_RECEIVE)
		return;

	if (m_in < MAX_FRAMES) {
		m_data[m_in] = new CDExtraGatewayAMBEData(data);
		m_in++;
	}

	if (data.isEnd()) {
		wxLogMessage(wxT("Received %.1f secs of audio from %s for echoing"), float(m_in) / float(DSTAR_FRAMES_PER_SEC), m_header->getMyCall1().c_str());

		m_timer.start();
		m_status = ES_WAIT;
	}
}

void CDExtraGatewayEchoUnit::clock(unsigned int ms)
{
	m_timer.clock(ms);

	if (m_status == ES_WAIT && m_timer.hasExpired()) {
		m_timer.stop();

		// RPT1 and RPT2 will be filled in later
		m_header->setMyCall1(m_callsign);
		m_header->setMyCall2(wxT("ECHO"));
		m_header->setYourCall(wxT("CQCQCQ  "));

		m_handler->writeHeader(*m_header);

		delete m_header;

		m_out    = 0U;
		m_status = ES_TRANSMIT;

		m_time.Start();

		return;
	}

	if (m_status == ES_TRANSMIT) {
		unsigned int needed = m_time.Time() / DSTAR_FRAME_TIME_MS;

		while (m_out < needed) {
			CDExtraGatewayAMBEData* data = m_data[m_out];
			m_data[m_out] = NULL;
			m_out++;

			if (m_in == m_out)
				data->setEnd(true);

			m_handler->writeData(*data);

			delete data;

			if (m_in == m_out) {
				m_in     = 0U;
				m_out    = 0U;
				m_status = ES_IDLE;
				return;
			}
		}

		return;
	}
}

void CDExtraGatewayEchoUnit::cancel()
{
	for (unsigned int i = 0U; i < MAX_FRAMES; i++) {
		if (m_data[i] != NULL) {
			delete m_data[i];
			m_data[i] = NULL;
		}
	}

	m_status = ES_IDLE;
	m_out    = 0U;
	m_in     = 0U;

	m_timer.stop();
}
