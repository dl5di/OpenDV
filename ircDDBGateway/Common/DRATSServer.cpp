/*
 *   Copyright (C) 2011-2014 by Jonathan Naylor G4KLX
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

#include "DStarDefines.h"
#include "DRATSServer.h"
#include "Utils.h"

// #define	LOOPBACK

const unsigned int BUFFER_LENGTH = 30000U;

CDRATSServer::CDRATSServer(const wxString& address, unsigned int port, const wxString& callsign, IRepeaterCallback* handler) :
wxThread(wxTHREAD_JOINABLE),
m_address(address),
m_port(port),
m_callsign(callsign),
m_handler(handler),
m_socket(NULL),
m_stopped(false),
m_readState(SS_FIRST),
m_readBuffer(NULL),
m_readLength(0U),
m_readPos(0U),
m_readEnd(false),
m_writeText(NULL),
m_writeState(SS_FIRST),
m_writeBuffer(NULL),
m_writeLength(0U)
{
	wxASSERT(handler != NULL);
	wxASSERT(port > 0U);

	m_readBuffer  = new unsigned char[BUFFER_LENGTH];
	m_writeBuffer = new unsigned char[BUFFER_LENGTH];
	m_writeText   = new unsigned char[6U];
}

CDRATSServer::~CDRATSServer()
{
	delete[] m_readBuffer;
	delete[] m_writeBuffer;
	delete[] m_writeText;
}

bool CDRATSServer::open()
{
	m_socket = new CTCPReaderWriterServer(m_address, m_port);
	bool ret = m_socket->start();
	if (!ret) {
		delete m_socket;
		m_socket = NULL;
		return false;
	}

	Create();
	Run();

	return true;
}

void CDRATSServer::writeHeader(const CHeaderData&)
{
	m_writeState = SS_FIRST;

	if (m_writeLength > 0U && m_socket != NULL) {
		// CUtils::dump(wxT("From RF"), m_writeBuffer, m_writeLength);
		m_socket->write(m_writeBuffer, m_writeLength);
	}

	m_writeLength = 0U;
}

void CDRATSServer::writeData(const CAMBEData& data)
{
	// Sync data isn't sent on
	if (data.isSync()) {
		m_writeState = SS_FIRST;
		return;
	}

	if (data.isEnd()) {
		if (m_writeLength > 0U && m_socket != NULL) {
			// CUtils::dump(wxT("From RF"), m_writeBuffer, m_writeLength);
			m_socket->write(m_writeBuffer, m_writeLength);
		}

		m_writeLength = 0U;
		return;
	}

	unsigned char buffer[DV_FRAME_MAX_LENGTH_BYTES];
	unsigned int length = data.getData(buffer, DV_FRAME_MAX_LENGTH_BYTES);

	if (length != DV_FRAME_LENGTH_BYTES)
		return;

	unsigned char byte1 = buffer[VOICE_FRAME_LENGTH_BYTES + 0U] ^ SCRAMBLER_BYTE1;
	unsigned char byte2 = buffer[VOICE_FRAME_LENGTH_BYTES + 1U] ^ SCRAMBLER_BYTE2;
	unsigned char byte3 = buffer[VOICE_FRAME_LENGTH_BYTES + 2U] ^ SCRAMBLER_BYTE3;

	switch (m_writeState) {
		case SS_FIRST:
			m_writeText[0U] = byte1;
			m_writeText[1U] = byte2;
			m_writeText[2U] = byte3;
			m_writeState = SS_SECOND;
			return;

		case SS_SECOND:
			m_writeText[3U] = byte1;
			m_writeText[4U] = byte2;
			m_writeText[5U] = byte3;
			m_writeState = SS_FIRST;
			break;
	}

	if ((m_writeText[0U] & SLOW_DATA_TYPE_MASK) != SLOW_DATA_TYPE_GPS)
		return;

	length = m_writeText[0U] & 0x07;	// Maximum value of 5
	if (length > 5U)
		length = 5U;

	for (unsigned int i = 0U; i < length; i++) {
		m_writeBuffer[m_writeLength++] = m_writeText[i + 1U];

		// Check for [EOB] in the buffer to signal the end of the D-RATS data.
		// To allow strstr() to run correctly
		m_writeBuffer[m_writeLength] = 0x00U;

		if (::strstr((char*)m_writeBuffer, "[EOB]") != NULL) {
			if (m_socket != NULL) {
				// CUtils::dump(wxT("From RF"), m_writeBuffer, m_writeLength);
				m_socket->write(m_writeBuffer, m_writeLength);
			}

			m_writeLength = 0U;
		}
	}
}

void CDRATSServer::writeEnd()
{
	if (m_writeLength > 0U && m_socket != NULL) {
		// CUtils::dump(wxT("From RF"), m_writeBuffer, m_writeLength);
		m_socket->write(m_writeBuffer, m_writeLength);
	}

	m_writeLength = 0U;
}

void* CDRATSServer::Entry()
{
	wxLogMessage(wxT("Starting the D-RATS server thread for %s"), m_callsign.c_str());

	bool sending = false;
	unsigned int id = 0U;

	unsigned char seqNo = 0U;
	unsigned int  sent = 0U;

	wxStopWatch time;

	while (!m_stopped) {
		serviceSocket();

		if (m_readEnd && !sending) {
			id = CHeaderData::createId();

			// Write header
			CHeaderData header;
			header.setMyCall1(m_callsign);
			header.setMyCall2(wxT("DATA"));
			header.setYourCall(wxT("CQCQCQ  "));
			header.setId(id);

#if defined(LOOPBACK)
			writeHeader(header);
#else
			m_handler->process(header, DIR_INCOMING, AS_DRATS);
#endif

			m_readState = SS_FIRST;
			m_readPos   = 0U;
			sending     = true;
			seqNo       = 0U;
			sent        = 0U;

			time.Start();
		}

		if (m_readEnd && sending) {
			unsigned int needed = time.Time() / DSTAR_FRAME_TIME_MS;

			while (sent < needed && sending) {
				// Write AMBE data
				CAMBEData data;
				data.setId(id);

				unsigned char buffer[DV_FRAME_LENGTH_BYTES];
				::memcpy(buffer + 0U, NULL_AMBE_DATA_BYTES, VOICE_FRAME_LENGTH_BYTES);

				// Insert sync bytes when the sequence number is zero, slow data otherwise
				if (seqNo == 0U) {
					::memcpy(buffer + VOICE_FRAME_LENGTH_BYTES, DATA_SYNC_BYTES, DATA_FRAME_LENGTH_BYTES);
					m_readState = SS_FIRST;
				} else {
					if (m_readState == SS_FIRST) {
						unsigned char readText[3U];
						::memset(readText, 'f', 3U);

						unsigned int length = m_readLength - m_readPos;
						unsigned char bytes = 5U;
						if (length < 5U)
							bytes = length;

						readText[0U] = SLOW_DATA_TYPE_GPS | bytes;

						for (unsigned int i = 0U; i < 2U && m_readPos < m_readLength; i++)
							readText[i + 1U] = m_readBuffer[m_readPos++];

						readText[0U] ^= SCRAMBLER_BYTE1;
						readText[1U] ^= SCRAMBLER_BYTE2;
						readText[2U] ^= SCRAMBLER_BYTE3;

						::memcpy(buffer + VOICE_FRAME_LENGTH_BYTES, readText, DATA_FRAME_LENGTH_BYTES);

						m_readState = SS_SECOND;
					} else {
						unsigned char readText[3U];
						::memset(readText, 'f', 3U);

						for (unsigned int i = 0U; i < 3U && m_readPos < m_readLength; i++)
							readText[i] = m_readBuffer[m_readPos++];

						readText[0U] ^= SCRAMBLER_BYTE1;
						readText[1U] ^= SCRAMBLER_BYTE2;
						readText[2U] ^= SCRAMBLER_BYTE3;

						::memcpy(buffer + VOICE_FRAME_LENGTH_BYTES, readText, DATA_FRAME_LENGTH_BYTES);				

						m_readState = SS_FIRST;
					}
				}

				data.setSeq(seqNo);
				data.setData(buffer, DV_FRAME_LENGTH_BYTES);
				sent++;

#if defined(LOOPBACK)
				writeData(data);
#else
				m_handler->process(data, DIR_INCOMING, AS_DRATS);
#endif

				if (m_readPos == m_readLength) {
					if (m_readState == SS_SECOND) {
						seqNo++;
						if (seqNo == 21U)
							seqNo = 0U;

						unsigned char readText[3U];
						readText[0U] = 'f' ^ SCRAMBLER_BYTE1;
						readText[1U] = 'f' ^ SCRAMBLER_BYTE2;
						readText[2U] = 'f' ^ SCRAMBLER_BYTE3;

						::memcpy(buffer + VOICE_FRAME_LENGTH_BYTES, readText, DATA_FRAME_LENGTH_BYTES);

						data.setSeq(seqNo);
						data.setData(buffer, DV_FRAME_LENGTH_BYTES);
						sent++;

#if defined(LOOPBACK)
						writeData(data);
#else
						m_handler->process(data, DIR_INCOMING, AS_DRATS);
#endif
					}

					seqNo++;
					if (seqNo == 21U)
						seqNo = 0U;

					if (seqNo == 0U)
						::memcpy(buffer + VOICE_FRAME_LENGTH_BYTES, DATA_SYNC_BYTES, DATA_FRAME_LENGTH_BYTES);
					else
						::memcpy(buffer + VOICE_FRAME_LENGTH_BYTES, NULL_SLOW_DATA_BYTES, DATA_FRAME_LENGTH_BYTES);

					data.setData(buffer, DV_FRAME_LENGTH_BYTES);
					data.setSeq(seqNo);
					data.setEnd(true);
					sent++;

#if defined(LOOPBACK)
					writeData(data);
#else
					m_handler->process(data, DIR_INCOMING, AS_DRATS);
#endif

					m_readLength = 0U;
					m_readPos    = 0U;
					m_readEnd    = false;
					sending      = false;
					sent         = 0U;
				}

				seqNo++;
				if (seqNo == 21U)
					seqNo = 0U;
			}
		}

		// 50ms
		Sleep(50UL);
	}

	if (m_socket != NULL)
		m_socket->stop();

	wxLogMessage(wxT("Stopping the D-RATS server thread for %s"), m_callsign.c_str());

	return NULL;
}

void CDRATSServer::close()
{
	m_stopped = true;

	Wait();
}

void CDRATSServer::serviceSocket()
{
	if (m_socket == NULL) {
		m_readLength  = 0U;
		m_readPos     = 0U;
		m_readEnd     = false;
		return;
	}

	int len = m_socket->read(m_readBuffer + m_readLength, BUFFER_LENGTH - m_readLength, 0U);
	if (len > 0) {
		m_readLength += len;

		if (!m_readEnd) {
			// To allow strstr() to run correctly
			m_readBuffer[m_readLength] = 0x00U;

			if (::strstr((char*)m_readBuffer, "[EOB]") != NULL) {
				// CUtils::dump(wxT("To RF"), m_readBuffer, m_readLength);
				m_readEnd = true;
			}
		}
	}
}
