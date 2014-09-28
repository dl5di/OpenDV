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

#include "CCITTChecksumReverse.h"
#include "DVMegaController.h"
#include "CCITTChecksum.h"
#include "DStarDefines.h"
#include "Timer.h"

#if defined(__WINDOWS__)
#include <setupapi.h>
#else
#include <wx/dir.h>
#endif

const unsigned char DVRPTR_HEADER_LENGTH = 5U;

const unsigned char DVRPTR_FRAME_START = 0xD0U;

const unsigned char DVRPTR_GET_STATUS  = 0x10U;
const unsigned char DVRPTR_GET_VERSION = 0x11U;
const unsigned char DVRPTR_GET_SERIAL  = 0x12U;
const unsigned char DVRPTR_GET_CONFIG  = 0x13U;
const unsigned char DVRPTR_SET_CONFIG  = 0x14U;
const unsigned char DVRPTR_RXPREAMBLE  = 0x15U;
const unsigned char DVRPTR_START       = 0x16U;
const unsigned char DVRPTR_HEADER      = 0x17U;
const unsigned char DVRPTR_RXSYNC      = 0x18U;
const unsigned char DVRPTR_DATA        = 0x19U;
const unsigned char DVRPTR_EOT         = 0x1AU;
const unsigned char DVRPTR_RXLOST      = 0x1BU;
const unsigned char DVRPTR_MSG_RSVD1   = 0x1CU;
const unsigned char DVRPTR_MSG_RSVD2   = 0x1DU;
const unsigned char DVRPTR_MSG_RSVD3   = 0x1EU;
const unsigned char DVRPTR_SET_TESTMDE = 0x1FU;

const unsigned char DVRPTR_ACK = 0x06U;
const unsigned char DVRPTR_NAK = 0x15U;

const unsigned int MAX_RESPONSES = 30U;

const unsigned int BUFFER_LENGTH = 200U;

CDVMegaController::CDVMegaController(const wxString& port, const wxString& path, bool rxInvert, bool txInvert, unsigned int txDelay) :
CModem(),
m_port(port),
m_path(path),
m_rxInvert(rxInvert),
m_txInvert(txInvert),
m_txDelay(txDelay),
m_frequency(0U),
m_power(0U),
m_serial(port, SERIAL_115200),
m_buffer(NULL),
m_txData(1000U),
m_txCounter(0U),
m_pktCounter(0U),
m_rx(false),
m_txSpace(0U),
m_txEnabled(false),
m_checksum(false)
{
	wxASSERT(!port.IsEmpty());

	m_buffer = new unsigned char[BUFFER_LENGTH];
}

CDVMegaController::CDVMegaController(const wxString& port, const wxString& path, unsigned int txDelay, unsigned int frequency, unsigned int power) :
CModem(),
m_port(port),
m_path(path),
m_rxInvert(false),
m_txInvert(false),
m_txDelay(txDelay),
m_frequency(frequency),
m_power(power),
m_serial(port, SERIAL_115200),
m_buffer(NULL),
m_txData(1000U),
m_txCounter(0U),
m_pktCounter(0U),
m_rx(false),
m_txSpace(0U),
m_txEnabled(false),
m_checksum(false)
{
	wxASSERT(!port.IsEmpty());
	wxASSERT((frequency >= 144000000U && frequency <= 148000000U) ||
			 (frequency >= 420000000U && frequency <= 450000000U));

	m_buffer = new unsigned char[BUFFER_LENGTH];
}

CDVMegaController::~CDVMegaController()
{
	delete[] m_buffer;
}

bool CDVMegaController::start()
{
	findPort();

	bool ret = openModem();
	if (!ret)
		return false;

	findPath();

	Create();
	SetPriority(100U);
	Run();

	return true;
}

void* CDVMegaController::Entry()
{
	wxLogMessage(wxT("Starting DVMEGA Controller thread"));

	// Clock every 5ms-ish
	CTimer pollTimer(200U, 0U, 100U);
	pollTimer.start();

	unsigned char  writeType   = DSMTT_NONE;
	unsigned char  writeLength = 0U;
	unsigned char* writeBuffer = new unsigned char[BUFFER_LENGTH];

	unsigned int space = 0U;

	while (!m_stopped) {
		// Poll the modem status every 100ms
		if (pollTimer.hasExpired()) {
			bool ret = readStatus();
			if (!ret) {
				ret = findModem();
				if (!ret) {
					wxLogMessage(wxT("Stopping DVMEGA Controller thread"));
					return NULL;
				}
			}

			pollTimer.start();
		}

		unsigned int length;
		RESP_TYPE_MEGA type = getResponse(m_buffer, length);

		switch (type) {
			case RTM_TIMEOUT:
				break;

			case RTM_ERROR: {
					bool ret = findModem();
					if (!ret) {
						wxLogMessage(wxT("Stopping DVMEGA Controller thread"));
						return NULL;
					}
				}
				break;

			case RTM_RXPREAMBLE:
				// wxLogMessage(wxT("RT_PREAMBLE"));
				break;

			case RTM_START:
				// wxLogMessage(wxT("RT_START"));
				break;

			case RTM_HEADER:
				// CUtils::dump(wxT("RT_HEADER"), m_buffer, length);
				if (length == 7U) {
					if (m_buffer[4U] == DVRPTR_NAK)
						wxLogWarning(wxT("Received a header NAK from the DVMEGA"));
				} else {
					bool correct = (m_buffer[5U] & 0x80U) == 0x00U;
					if (correct) {
						wxMutexLocker locker(m_mutex);

						unsigned char data[2U];
						data[0U] = DSMTT_HEADER;
						data[1U] = RADIO_HEADER_LENGTH_BYTES;
						m_rxData.addData(data, 2U);

						m_rxData.addData(m_buffer + 8U, RADIO_HEADER_LENGTH_BYTES);

						m_rx = true;
					}
				}
				break;

			case RTM_RXSYNC:
				// wxLogMessage(wxT("RT_RXSYNC"));
				break;

			case RTM_DATA:
				// CUtils::dump(wxT("RT_DATA"), m_buffer, length);
				if (length == 7U) {
					if (m_buffer[4U] == DVRPTR_NAK)
						wxLogWarning(wxT("Received a data NAK from the DVMEGA"));
				} else {
					wxMutexLocker locker(m_mutex);

					unsigned char data[2U];
					data[0U] = DSMTT_DATA;
					data[1U] = DV_FRAME_LENGTH_BYTES;
					m_rxData.addData(data, 2U);

					m_rxData.addData(m_buffer + 8U, DV_FRAME_LENGTH_BYTES);

					m_rx = true;
				}
				break;

			case RTM_EOT: {
					// wxLogMessage(wxT("RT_EOT"));
					wxMutexLocker locker(m_mutex);

					unsigned char data[2U];
					data[0U] = DSMTT_EOT;
					data[1U] = 0U;
					m_rxData.addData(data, 2U);

					m_rx = false;
				}
				break;

			case RTM_RXLOST: {
					// wxLogMessage(wxT("RT_LOST"));
					wxMutexLocker locker(m_mutex);

					unsigned char data[2U];
					data[0U] = DSMTT_LOST;
					data[1U] = 0U;
					m_rxData.addData(data, 2U);

					m_rx = false;
				}
				break;

			case RTM_GET_STATUS: {
					m_txEnabled = (m_buffer[4U] & 0x02U) == 0x02U;
					m_checksum  = (m_buffer[4U] & 0x08U) == 0x08U;
					m_tx        = (m_buffer[5U] & 0x02U) == 0x02U;
					m_txSpace   = m_buffer[8U];
					space       = m_txSpace - m_buffer[9U];
					// CUtils::dump(wxT("GET_STATUS"), m_buffer, length);
					// wxLogMessage(wxT("PTT=%d tx=%u space=%u cksum=%d, tx enabled=%d"), int(m_tx), m_txSpace, space, int(m_checksum), int(m_txEnabled));
				}
				break;

			// These should not be received in this loop, but don't complain if we do
			case RTM_GET_VERSION:
			case RTM_GET_SERIAL:
			case RTM_GET_CONFIG:
				break;

			default:
				wxLogMessage(wxT("Unknown message, type: %02X"), m_buffer[3U]);
				CUtils::dump(wxT("Buffer dump"), m_buffer, length);
				break;
		}

		if (space > 0U) {
			if (writeType == DSMTT_NONE && m_txData.hasData()) {
				wxMutexLocker locker(m_mutex);

				m_txData.getData(&writeType, 1U);
				m_txData.getData(&writeLength, 1U);
				m_txData.getData(writeBuffer, writeLength);
			}

			// Only send the start when the TX is off
			if (!m_tx && writeType == DSMTT_START) {
				// CUtils::dump(wxT("Write Header"), writeBuffer, writeLength);

				int ret = m_serial.write(writeBuffer, writeLength);
				if (ret != int(writeLength))
					wxLogWarning(wxT("Error when writing the header to the DVMEGA"));

				writeType = DSMTT_NONE;
				space--;
			}
			
			if (writeType == DSMTT_HEADER || writeType == DSMTT_DATA || writeType == DSMTT_EOT) {
				// CUtils::dump(wxT("Write Data"), writeBuffer, writeLength);

				int ret = m_serial.write(writeBuffer, writeLength);
				if (ret != int(writeLength))
					wxLogWarning(wxT("Error when writing data to the DVMEGA"));

				writeType = DSMTT_NONE;
				space--;
			}
		}

		Sleep(5UL);

		pollTimer.clock();
	}

	wxLogMessage(wxT("Stopping DVMEGA Controller thread"));

	setEnabled(false);

	delete[] writeBuffer;

	m_serial.close();

	return NULL;
}

bool CDVMegaController::writeHeader(const CHeaderData& header)
{
	if (!m_txEnabled)
		return false;

	bool ret = m_txData.hasSpace(64U);
	if (!ret) {
		wxLogWarning(wxT("No space to write the header"));
		return false;
	}

	m_txCounter++;
	if (m_txCounter == 0U)
		m_txCounter = 1U;

	unsigned char buffer1[10U];

	buffer1[0U] = DVRPTR_FRAME_START;

	buffer1[1U] = 0x03U;
	buffer1[2U] = 0x00U;

	buffer1[3U] = DVRPTR_START;

	buffer1[4U] = m_txCounter;
	buffer1[5U] = 0x00U;

	if (m_checksum) {
		CCCITTChecksum cksum;
		cksum.update(buffer1 + 0U, 6U);
		cksum.result(buffer1 + 6U);
	} else {
		buffer1[6U] = 0x00U;
		buffer1[7U] = 0x0BU;
	}

	unsigned char buffer2[60U];

	buffer2[0U] = DVRPTR_FRAME_START;

	buffer2[1U] = 0x2FU;
	buffer2[2U] = 0x00U;

	buffer2[3U] = DVRPTR_HEADER;

	buffer2[4U] = m_txCounter;
	buffer2[5U] = 0x00U;

	buffer2[6U] = 0x00U;
	buffer2[7U] = 0x00U;

	::memset(buffer2 + 8U, ' ', RADIO_HEADER_LENGTH_BYTES);

	buffer2[8U]  = header.getFlag1();
	buffer2[9U]  = header.getFlag2();
	buffer2[10U] = header.getFlag3();

	wxString rpt2 = header.getRptCall2();
	for (unsigned int i = 0U; i < rpt2.Len() && i < LONG_CALLSIGN_LENGTH; i++)
		buffer2[i + 11U]  = rpt2.GetChar(i);

	wxString rpt1 = header.getRptCall1();
	for (unsigned int i = 0U; i < rpt1.Len() && i < LONG_CALLSIGN_LENGTH; i++)
		buffer2[i + 19U] = rpt1.GetChar(i);

	wxString your = header.getYourCall();
	for (unsigned int i = 0U; i < your.Len() && i < LONG_CALLSIGN_LENGTH; i++)
		buffer2[i + 27U] = your.GetChar(i);

	wxString my1 = header.getMyCall1();
	for (unsigned int i = 0U; i < my1.Len() && i < LONG_CALLSIGN_LENGTH; i++)
		buffer2[i + 35U] = my1.GetChar(i);

	wxString my2 = header.getMyCall2();
	for (unsigned int i = 0U; i < my2.Len() && i < SHORT_CALLSIGN_LENGTH; i++)
		buffer2[i + 43U] = my2.GetChar(i);

	CCCITTChecksumReverse cksum1;
	cksum1.update(buffer2 + 8U, RADIO_HEADER_LENGTH_BYTES - 2U);
	cksum1.result(buffer2 + 47U);

	buffer2[49U] = 0x00U;

	if (m_checksum) {
		CCCITTChecksum cksum;
		cksum.update(buffer2 + 0U, 50U);
		cksum.result(buffer2 + 50U);
	} else {
		buffer2[50U] = 0x00U;
		buffer2[51U] = 0x0BU;
	}

	m_pktCounter = 0U;

	wxMutexLocker locker(m_mutex);

	unsigned char type1 = DSMTT_START;
	m_txData.addData(&type1, 1U);

	unsigned char len1 = 8U;
	m_txData.addData(&len1, 1U);

	m_txData.addData(buffer1, 8U);

	unsigned char type2 = DSMTT_HEADER;
	m_txData.addData(&type2, 1U);

	unsigned char len2 = 52U;
	m_txData.addData(&len2, 1U);

	m_txData.addData(buffer2, 52U);

	return true;
}

bool CDVMegaController::writeData(const unsigned char* data, unsigned int length, bool end)
{
	if (!m_txEnabled)
		return false;

	bool ret = m_txData.hasSpace(26U);
	if (!ret) {
		wxLogWarning(wxT("No space to write data"));
		return false;
	}

	unsigned char buffer[30U];

	if (end) {
		buffer[0U] = DVRPTR_FRAME_START;

		buffer[1U] = 0x03U;
		buffer[2U] = 0x00U;

		buffer[3U] = DVRPTR_EOT;

		buffer[4U] = m_txCounter;
		buffer[5U] = 0xFFU;

		if (m_checksum) {
			CCCITTChecksum cksum;
			cksum.update(buffer + 0U, 6U);
			cksum.result(buffer + 6U);
		} else {
			buffer[6U] = 0x00U;
			buffer[7U] = 0x0BU;
		}

		wxMutexLocker locker(m_mutex);

		unsigned char type = DSMTT_EOT;
		m_txData.addData(&type, 1U);

		unsigned char len = 8U;
		m_txData.addData(&len, 1U);

		m_txData.addData(buffer, 8U);

		return true;
	}

	buffer[0U] = DVRPTR_FRAME_START;

	buffer[1U] = 0x13U;
	buffer[2U] = 0x00U;

	buffer[3U] = DVRPTR_DATA;

	buffer[4U] = m_txCounter;
	buffer[5U] = m_pktCounter;

	m_pktCounter++;
	if (m_pktCounter >= m_txSpace)
		m_pktCounter = 0U;

	buffer[6U] = 0x00U;
	buffer[7U] = 0x00U;

	::memcpy(buffer + 8U, data, DV_FRAME_LENGTH_BYTES);

	buffer[20U] = 0x00U;
	buffer[21U] = 0x00U;

	if (m_checksum) {
		CCCITTChecksum cksum;
		cksum.update(buffer + 0U, 22U);
		cksum.result(buffer + 22U);
	} else {
		buffer[22U] = 0x00U;
		buffer[23U] = 0x0BU;
	}

	wxMutexLocker locker(m_mutex);

	unsigned char type = DSMTT_DATA;
	m_txData.addData(&type, 1U);

	unsigned char len = 24U;
	m_txData.addData(&len, 1U);

	m_txData.addData(buffer, 24U);

	return true;
}

unsigned int CDVMegaController::getSpace()
{
	return m_txData.freeSpace() / 26U;
}

bool CDVMegaController::isTXReady()
{
	if (m_tx)
		return false;

	return m_txData.isEmpty();
}

bool CDVMegaController::readVersion()
{
	unsigned char buffer[10U];

	buffer[0U] = DVRPTR_FRAME_START;

	buffer[1U] = 0x01U;
	buffer[2U] = 0x00U;

	buffer[3U] = DVRPTR_GET_VERSION;

	if (m_checksum) {
		CCCITTChecksum cksum;
		cksum.update(buffer + 0U, 4U);
		cksum.result(buffer + 4U);
	} else {
		buffer[4U] = 0x00U;
		buffer[5U] = 0x0BU;
	}

	// CUtils::dump(wxT("Written"), buffer, 6U);

	int ret = m_serial.write(buffer, 6U);
	if (ret != 6)
		return false;

	unsigned int count = 0U;
	unsigned int length;
	RESP_TYPE_MEGA resp;
	do {
		::wxMilliSleep(10UL);

		resp = getResponse(m_buffer, length);

		if (resp != RTM_GET_VERSION) {
			count++;
			if (count >= MAX_RESPONSES) {
				wxLogError(wxT("The DVMEGA is not responding to the version command"));
				return false;
			}
		}
	} while (resp != RTM_GET_VERSION);

	wxString firmware;
	if ((m_buffer[4U] & 0x0FU) > 0x00U)
		firmware.Printf(wxT("%u.%u%u%c"), (m_buffer[5U] & 0xF0U) >> 4, m_buffer[5U] & 0x0FU, (m_buffer[4U] & 0xF0U) >> 4, (m_buffer[4U] & 0x0FU) + wxT('a') - 1U);
	else
		firmware.Printf(wxT("%u.%u%u"), (m_buffer[5U] & 0xF0U) >> 4, m_buffer[5U] & 0x0FU, (m_buffer[4U] & 0xF0U) >> 4);

	wxString hardware((char*)(m_buffer + 6U), wxConvLocal, length - DVRPTR_HEADER_LENGTH - 3U);

	wxLogInfo(wxT("DVMEGA Firmware version: %s, hardware: %s"), firmware.c_str(), hardware.c_str());

	return true;
}

bool CDVMegaController::readStatus()
{
	unsigned char buffer[10U];

	buffer[0U] = DVRPTR_FRAME_START;

	buffer[1U] = 0x01U;
	buffer[2U] = 0x00U;

	buffer[3U] = DVRPTR_GET_STATUS;

	if (m_checksum) {
		CCCITTChecksum cksum;
		cksum.update(buffer + 0U, 4U);
		cksum.result(buffer + 4U);
	} else {
		buffer[4U] = 0x00U;
		buffer[5U] = 0x0BU;
	}

	return m_serial.write(buffer, 6U) == 6;
}

bool CDVMegaController::setConfig()
{
	unsigned char buffer[20U];

	buffer[0U] = DVRPTR_FRAME_START;

	buffer[1U] = 0x07U;
	buffer[2U] = 0x00U;

	buffer[3U] = DVRPTR_SET_CONFIG;

	buffer[4U] = 0xC0U;		// Physical layer

	buffer[5U] = 0x04U;		// Block length

	buffer[6U] = 0x00U;
	if (m_rxInvert)
		buffer[6U] |= 0x01U;
	if (m_txInvert)
		buffer[6U] |= 0x02U;

	wxUint16* txDelay = (wxUint16*)(buffer + 8U);
	*txDelay = wxUINT16_SWAP_ON_BE(m_txDelay);

	if (m_checksum) {
		CCCITTChecksum cksum;
		cksum.update(buffer + 0U, 10U);
		cksum.result(buffer + 10U);
	} else {
		buffer[10U] = 0x00U;
		buffer[11U] = 0x0BU;
	}

	// CUtils::dump(wxT("Written"), buffer, 12U);

	int ret = m_serial.write(buffer, 12U);
	if (ret != 12)
		return false;

	unsigned int count = 0U;
	unsigned int length;
	RESP_TYPE_MEGA resp;
	do {

		::wxMilliSleep(10UL);

		resp = getResponse(m_buffer, length);

		if (resp != RTM_SET_CONFIG) {
			count++;
			if (count >= MAX_RESPONSES) {
				wxLogError(wxT("The DVMEGA is not responding to the SET_CONFIG command"));
				return false;
			}
		}
	} while (resp != RTM_SET_CONFIG);

	// CUtils::dump(wxT("Response"), m_buffer, length);

	unsigned char type = m_buffer[4U];
	if (type != DVRPTR_ACK) {
		wxLogError(wxT("Received a NAK to the SET_CONFIG command from the modem"));
		return false;
	}

	return true;
}

bool CDVMegaController::setFrequencyAndPower()
{
	unsigned char buffer[25U];

	::memset(buffer, 0x00U, 21U);

	buffer[0U] = DVRPTR_FRAME_START;

	buffer[1U] = 0x10U;
	buffer[2U] = 0x00U;

	buffer[3U] = DVRPTR_SET_CONFIG;

	buffer[4U] = 0xC1U;		// RF layer

	buffer[5U] = 0x0CU;		// Block length

	wxUint32 freq = wxUINT32_SWAP_ON_BE(wxUint32(m_frequency));

	::memcpy(buffer + 7U, &freq, sizeof(wxUint32));

	::memcpy(buffer + 11U, &freq, sizeof(wxUint32));

	buffer[16U] = (m_power * 64U) / 100U;

	if (m_checksum) {
		CCCITTChecksum cksum;
		cksum.update(buffer + 0U, 19U);
		cksum.result(buffer + 19U);
	} else {
		buffer[19U] = 0x00U;
		buffer[20U] = 0x0BU;
	}

	// CUtils::dump(wxT("Written"), buffer, 21U);

	int ret = m_serial.write(buffer, 21U);
	if (ret != 21)
		return false;

	unsigned int count = 0U;
	unsigned int length;
	RESP_TYPE_MEGA resp;
	do {

		::wxMilliSleep(10UL);

		resp = getResponse(m_buffer, length);

		if (resp != RTM_SET_CONFIG) {
			count++;
			if (count >= MAX_RESPONSES) {
				wxLogError(wxT("The DVMEGA is not responding to the SET_CONFIG command"));
				return false;
			}
		}
	} while (resp != RTM_SET_CONFIG);

	// CUtils::dump(wxT("Response"), m_buffer, length);

	unsigned char type = m_buffer[4U];
	if (type != DVRPTR_ACK) {
		wxLogError(wxT("Received a NAK to the SET_CONFIG command from the modem"));
		return false;
	}

	return true;
}

bool CDVMegaController::setEnabled(bool enable)
{
	unsigned char buffer[10U];

	buffer[0U] = DVRPTR_FRAME_START;

	buffer[1U] = 0x02U;
	buffer[2U] = 0x00U;

	buffer[3U] = DVRPTR_GET_STATUS;

	// Enable RX, TX, and Watchdog
	if (enable)
		buffer[4U] = 0x01U | 0x02U | 0x04U;
	else
		buffer[4U] = 0x00U;

	if (m_checksum) {
		CCCITTChecksum cksum;
		cksum.update(buffer + 0U, 5U);
		cksum.result(buffer + 5U);
	} else {
		buffer[5U] = 0x00U;
		buffer[6U] = 0x0BU;
	}

	// CUtils::dump(wxT("Written"), buffer, 7U);

	int ret = m_serial.write(buffer, 7U);
	if (ret != 7)
		return false;

	unsigned int count = 0U;
	unsigned int length;
	RESP_TYPE_MEGA resp;
	do {
		::wxMilliSleep(10UL);

		resp = getResponse(m_buffer, length);

		if (resp != RTM_GET_STATUS) {
			count++;
			if (count >= MAX_RESPONSES) {
				wxLogError(wxT("The DVMEGA is not responding to the SET_STATUS command"));
				return false;
			}
		}
	} while (resp != RTM_GET_STATUS);

	// CUtils::dump(wxT("Response"), m_buffer, length);

	unsigned char type = m_buffer[4U];
	if (type != DVRPTR_ACK) {
		wxLogError(wxT("Received a NAK to the SET_STATUS command from the modem"));
		return false;
	}

	return true;
}

RESP_TYPE_MEGA CDVMegaController::getResponse(unsigned char *buffer, unsigned int& length)
{
	// Get the start of the frame or nothing at all
	int ret = m_serial.read(buffer, 1U);
	if (ret < 0) {
		wxLogError(wxT("Error when reading from the DVMEGA"));
		return RTM_ERROR;
	}

	if (ret == 0)
		return RTM_TIMEOUT;

	unsigned int offset = 1U;

	while (offset < DVRPTR_HEADER_LENGTH) {
		int ret = m_serial.read(buffer + offset, DVRPTR_HEADER_LENGTH - offset);
		if (ret < 0) {
			wxLogError(wxT("Error when reading from the DVMEGA"));
			return RTM_ERROR;
		}

		if (ret > 0)
			offset += ret;

		if (ret == 0)
			Sleep(5UL);
	}

	length = buffer[1U] + buffer[2U] * 256U;

	// Remove the response bit
	unsigned int type = buffer[3U] & 0x7FU;

	offset = 0U;

	while (offset < length) {
		int ret = m_serial.read(buffer + offset + DVRPTR_HEADER_LENGTH, length - offset);
		if (ret < 0) {
			wxLogError(wxT("Error when reading from the DVMEGA"));
			return RTM_ERROR;
		}

		if (ret > 0)
			offset += ret;

		if (ret == 0)
			Sleep(5UL);
	}

	length += DVRPTR_HEADER_LENGTH;

	// CUtils::dump(wxT("Received"), buffer, length);

	switch (type) {
		case DVRPTR_GET_STATUS:
			return RTM_GET_STATUS;
		case DVRPTR_GET_VERSION:
			return RTM_GET_VERSION;
		case DVRPTR_GET_SERIAL:
			return RTM_GET_SERIAL;
		case DVRPTR_GET_CONFIG:
			return RTM_GET_CONFIG;
		case DVRPTR_SET_CONFIG:
			return RTM_SET_CONFIG;
		case DVRPTR_RXPREAMBLE:
			return RTM_RXPREAMBLE;
		case DVRPTR_START:
			return RTM_START;
		case DVRPTR_HEADER:
			return RTM_HEADER;
		case DVRPTR_RXSYNC:
			return RTM_RXSYNC;
		case DVRPTR_DATA:
			return RTM_DATA;
		case DVRPTR_EOT:
			return RTM_EOT;
		case DVRPTR_RXLOST:
			return RTM_RXLOST;
		case DVRPTR_SET_TESTMDE:
			return RTM_SET_TESTMDE;
		default:
			return RTM_UNKNOWN;
	}
}

wxString CDVMegaController::getPath() const
{
	return m_path;
}

bool CDVMegaController::findPort()
{
	if (m_path.IsEmpty())
		return false;

#if defined(__WINDOWS__)
#else
	wxDir dir;
	bool ret1 = dir.Open(wxT("/sys/class/tty"));
	if (!ret1) {
		wxLogError(wxT("Cannot open directory /sys/class/tty"));
		return false;
	}

	wxString fileName;
	ret1 = dir.GetFirst(&fileName, wxT("ttyACM*"));
	while (ret1) {
		wxString path;
		path.Printf(wxT("/sys/class/tty/%s"), fileName.c_str());

		char cpath[255U];
		::memset(cpath, 0x00U, 255U);

		for (unsigned int i = 0U; i < path.Len(); i++)
			cpath[i] = path.GetChar(i);

		char symlink[255U];
		int ret2 = ::readlink(cpath, symlink, 255U);
		if (ret2 < 0) {
			::strcat(cpath, "/device");
			ret2 = ::readlink(cpath, symlink, 255U);
			if (ret2 < 0) {
				wxLogError(wxT("Error from readlink()"));
				return false;
			}

			path = wxString(symlink, wxConvLocal, ret2);
		} else {
			// Get all but the last section
			wxString fullPath = wxString(symlink, wxConvLocal, ret2);
			path = fullPath.BeforeLast(wxT('/'));
		}

		if (path.IsSameAs(m_path)) {
			m_port.Printf(wxT("/dev/%s"), fileName.c_str());

			wxLogMessage(wxT("Found modem port of %s based on the path"), m_port.c_str());

			return true;
		}

		ret1 = dir.GetNext(&fileName);
	}
#endif

	return false;
}

bool CDVMegaController::findPath()
{
#if defined(__WINDOWS__)
#ifdef notdef
	GUID guids[5U];

	DWORD count;
	BOOL res = ::SetupDiClassGuidsFromName(L"Multifunction", guids, 5U, &count);
	if (!res) {
		wxLogError(wxT("Error from SetupDiClassGuidsFromName: err=%u"), ::GetLastError());
		return false;
	}

	for (DWORD i = 0U; i < count; i++) {
		HDEVINFO devInfo = ::SetupDiGetClassDevs(&guids[i], NULL, NULL, DIGCF_DEVICEINTERFACE | DIGCF_PRESENT);
		if (devInfo == INVALID_HANDLE_VALUE) {
			wxLogError(wxT("Error from SetupDiGetClassDevs: err=%u"), ::GetLastError());
			return false;
		}

		SP_DEVICE_INTERFACE_DATA devInfoData;
		devInfoData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

		for (DWORD index = 0U; ::SetupDiEnumDeviceInterfaces(devInfo, NULL, &guids[i], index, &devInfoData); index++) {
			// Find the required length of the device structure
			DWORD length;
			::SetupDiGetDeviceInterfaceDetail(devInfo, &devInfoData, NULL, 0U, &length, NULL);

			PSP_DEVICE_INTERFACE_DETAIL_DATA detailData = PSP_DEVICE_INTERFACE_DETAIL_DATA(::malloc(length));
			detailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

			// Get the detailed data into the newly allocated device structure
			DWORD required;
			res = ::SetupDiGetDeviceInterfaceDetail(devInfo, &devInfoData, detailData, length, &required, NULL);
			if (!res) {
				wxLogError(wxT("Error from SetupDiGetDeviceInterfaceDetail: err=%u"), ::GetLastError());
				::SetupDiDestroyDeviceInfoList(devInfo);
				::free(detailData);
				return false;
			}

			::free(detailData);
		}

		::SetupDiDestroyDeviceInfoList(devInfo);
	}

	return false;
#endif
#else
	wxString path;
	path.Printf(wxT("/sys/class/tty/%s"), m_port.Mid(5U).c_str());	

	char cpath[255U];
	::memset(cpath, 0x00U, 255U);

	for (unsigned int i = 0U; i < path.Len(); i++)
		cpath[i] = path.GetChar(i);

	char symlink[255U];
	int ret = ::readlink(cpath, symlink, 255U);
	if (ret < 0) {
		::strcat(cpath, "/device");
		ret = ::readlink(cpath, symlink, 255U);
		if (ret < 0) {
			wxLogError(wxT("Error from readlink()"));
			return false;
		}

		path = wxString(symlink, wxConvLocal, ret);
	} else {
		wxString fullPath = wxString(symlink, wxConvLocal, ret);
		path = fullPath.BeforeLast(wxT('/'));
	}

	if (m_path.IsEmpty())
		wxLogMessage(wxT("Found modem path of %s"), path.c_str());

	m_path = path;
#endif

	return true;
}

bool CDVMegaController::findModem()
{
	m_serial.close();

	// Tell the repeater that the signal has gone away
	if (m_rx) {
		wxMutexLocker locker(m_mutex);

		unsigned char data[2U];
		data[0U] = DSMTT_EOT;
		data[1U] = 0U;
		m_rxData.addData(data, 2U);

		m_rx = false;
	}

	unsigned int count = 0U;

	// Purge the transmit buffer every 500ms to avoid overflow, but only try and reopen the modem every 2s
	while (!m_stopped) {
		count++;
		if (count >= 4U) {
			wxLogMessage(wxT("Trying to reopen the modem"));

			bool ret = findPort();
			if (ret) {
				ret = openModem();
				if (ret)
					return true;
			}

			count = 0U;
		}

		Sleep(500UL);
	}

	return false;
}

bool CDVMegaController::openModem()
{
	bool ret = m_serial.open();
	if (!ret)
		return false;

	ret = readVersion();
	if (!ret) {
		m_serial.close();
		return false;
	}

	ret = setConfig();
	if (!ret) {
		m_serial.close();
		return false;
	}

	if (m_frequency != 0U) {
		ret = setFrequencyAndPower();
		if (!ret) {
			m_serial.close();
			return false;
		}
	}

	ret = setEnabled(true);
	if (!ret) {
		m_serial.close();
		return false;
	}

	return true;
}

