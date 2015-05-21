/*
 *   Copyright (C) 2011-2015 by Jonathan Naylor G4KLX
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
#include "DVRPTRV1Controller.h"
#include "CCITTChecksum.h"
#include "DStarDefines.h"
#include "MutexLocker.h"
#include "Types.h"
#include "Timer.h"
#include "Log.h"

#if defined(WIN32)
#include <setupapi.h>
#else
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
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

CDVRPTRV1Controller::CDVRPTRV1Controller(const std::string& port, const std::string& path, bool rxInvert, bool txInvert, bool channel, unsigned int modLevel, unsigned int txDelay) :
CModem(),
m_port(port),
m_path(path),
m_rxInvert(rxInvert),
m_txInvert(txInvert),
m_channel(channel),
m_modLevel(modLevel),
m_txDelay(txDelay),
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
	assert(!port.empty());

	m_buffer = new unsigned char[BUFFER_LENGTH];
}

CDVRPTRV1Controller::~CDVRPTRV1Controller()
{
	delete[] m_buffer;
}

bool CDVRPTRV1Controller::start()
{
	findPort();

	bool ret = openModem();
	if (!ret)
		return false;

	findPath();

	run();

	return true;
}

void CDVRPTRV1Controller::entry()
{
	LogMessage("Starting DV-RPTR1 Modem Controller thread");

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
					LogMessage("Stopping DV-RPTR1 Modem Controller thread");
					return;
				}
			}

			pollTimer.start();
		}

		unsigned int length;
		RESP_TYPE_V1 type = getResponse(m_buffer, length);

		switch (type) {
			case RT1_TIMEOUT:
				break;

			case RT1_ERROR: {
					bool ret = findModem();
					if (!ret) {
						LogMessage("Stopping DV-RPTR1 Modem Controller thread");
						return;
					}
				}
				break;

			case RT1_RXPREAMBLE:
				// LogMessage("RT_PREAMBLE");
				break;

			case RT1_START:
				// LogMessage("RT_START");
				break;

			case RT1_HEADER:
				// CUtils::dump("RT_HEADER", m_buffer, length);
				if (length == 7U) {
					if (m_buffer[4U] == DVRPTR_NAK)
						LogWarning("Received a header NAK from the modem");
				} else {
					bool correct = (m_buffer[5U] & 0x80U) == 0x00U;
					if (correct) {
						CMutexLocker locker(m_mutex);

						unsigned char data[2U];
						data[0U] = DSMTT_HEADER;
						data[1U] = RADIO_HEADER_LENGTH_BYTES;
						m_rxData.addData(data, 2U);

						m_rxData.addData(m_buffer + 8U, RADIO_HEADER_LENGTH_BYTES);

						m_rx = true;
					}
				}
				break;

			case RT1_RXSYNC:
				// LogMessage("RT_RXSYNC");
				break;

			case RT1_DATA:
				// CUtils::dump("RT_DATA", m_buffer, length);
				if (length == 7U) {
					if (m_buffer[4U] == DVRPTR_NAK)
						LogWarning("Received a data NAK from the modem");
				} else {
					CMutexLocker locker(m_mutex);

					unsigned char data[2U];
					data[0U] = DSMTT_DATA;
					data[1U] = DV_FRAME_LENGTH_BYTES;
					m_rxData.addData(data, 2U);

					m_rxData.addData(m_buffer + 8U, DV_FRAME_LENGTH_BYTES);

					m_rx = true;
				}
				break;

			case RT1_EOT: {
					// LogMessage("RT_EOT");
					CMutexLocker locker(m_mutex);

					unsigned char data[2U];
					data[0U] = DSMTT_EOT;
					data[1U] = 0U;
					m_rxData.addData(data, 2U);

					m_rx = false;
				}
				break;

			case RT1_RXLOST: {
					// LogMessage("RT_LOST");
					CMutexLocker locker(m_mutex);

					unsigned char data[2U];
					data[0U] = DSMTT_LOST;
					data[1U] = 0U;
					m_rxData.addData(data, 2U);

					m_rx = false;
				}
				break;

			case RT1_GET_STATUS: {
					m_txEnabled = (m_buffer[4U] & 0x02U) == 0x02U;
					m_checksum  = (m_buffer[4U] & 0x08U) == 0x08U;
					m_tx        = (m_buffer[5U] & 0x02U) == 0x02U;
					m_txSpace   = m_buffer[8U];
					space       = m_txSpace - m_buffer[9U];
					// CUtils::dump("GET_STATUS", m_buffer, length);
					// LogMessage("PTT=%d tx=%u space=%u cksum=%d, tx enabled=%d", int(m_tx), m_txSpace, space, int(m_checksum), int(m_txEnabled));
				}
				break;

			// These should not be received in this loop, but don't complain if we do
			case RT1_GET_VERSION:
			case RT1_GET_SERIAL:
			case RT1_GET_CONFIG:
				break;

			default:
				LogMessage("Unknown message, type: %02X", m_buffer[3U]);
				CUtils::dump("Buffer dump", m_buffer, length);
				break;
		}

		if (space > 0U) {
			if (writeType == DSMTT_NONE && m_txData.hasData()) {
				CMutexLocker locker(m_mutex);

				m_txData.getData(&writeType, 1U);
				m_txData.getData(&writeLength, 1U);
				m_txData.getData(writeBuffer, writeLength);
			}

			// Only send the start when the TX is off
			if (!m_tx && writeType == DSMTT_START) {
				// CUtils::dump("Write Header", writeBuffer, writeLength);

				int ret = m_serial.write(writeBuffer, writeLength);
				if (ret != int(writeLength))
					LogWarning("Error when writing the header to the DV-RPTR modem");

				writeType = DSMTT_NONE;
				space--;
			}
			
			if (writeType == DSMTT_HEADER || writeType == DSMTT_DATA || writeType == DSMTT_EOT) {
				// CUtils::dump("Write Data", writeBuffer, writeLength);

				int ret = m_serial.write(writeBuffer, writeLength);
				if (ret != int(writeLength))
					LogWarning("Error when writing data to the DV-RPTR modem");

				writeType = DSMTT_NONE;
				space--;
			}
		}

		Sleep(5UL);

		pollTimer.clock();
	}

	LogMessage("Stopping DV-RPTR1 Modem Controller thread");

	setEnabled(false);

	delete[] writeBuffer;

	m_serial.close();
}

bool CDVRPTRV1Controller::writeHeader(const CHeaderData& header)
{
	if (!m_txEnabled)
		return false;

	bool ret = m_txData.hasSpace(64U);
	if (!ret) {
		LogWarning("No space to write the header");
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

	std::string rpt2 = header.getRptCall2();
	for (unsigned int i = 0U; i < rpt2.length() && i < LONG_CALLSIGN_LENGTH; i++)
		buffer2[i + 11U]  = rpt2.at(i);

	std::string rpt1 = header.getRptCall1();
	for (unsigned int i = 0U; i < rpt1.length() && i < LONG_CALLSIGN_LENGTH; i++)
		buffer2[i + 19U] = rpt1.at(i);

	std::string your = header.getYourCall();
	for (unsigned int i = 0U; i < your.length() && i < LONG_CALLSIGN_LENGTH; i++)
		buffer2[i + 27U] = your.at(i);

	std::string my1 = header.getMyCall1();
	for (unsigned int i = 0U; i < my1.length() && i < LONG_CALLSIGN_LENGTH; i++)
		buffer2[i + 35U] = my1.at(i);

	std::string my2 = header.getMyCall2();
	for (unsigned int i = 0U; i < my2.length() && i < SHORT_CALLSIGN_LENGTH; i++)
		buffer2[i + 43U] = my2.at(i);

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

	CMutexLocker locker(m_mutex);

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

bool CDVRPTRV1Controller::writeData(const unsigned char* data, unsigned int length, bool end)
{
	if (!m_txEnabled)
		return false;

	bool ret = m_txData.hasSpace(26U);
	if (!ret) {
		LogWarning("No space to write data");
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

		CMutexLocker locker(m_mutex);

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

	CMutexLocker locker(m_mutex);

	unsigned char type = DSMTT_DATA;
	m_txData.addData(&type, 1U);

	unsigned char len = 24U;
	m_txData.addData(&len, 1U);

	m_txData.addData(buffer, 24U);

	return true;
}

unsigned int CDVRPTRV1Controller::getSpace()
{
	return m_txData.freeSpace() / 26U;
}

bool CDVRPTRV1Controller::isTXReady()
{
	if (m_tx)
		return false;

	return m_txData.isEmpty();
}

bool CDVRPTRV1Controller::readVersion()
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

	// CUtils::dump("Written", buffer, 6U);

	int ret = m_serial.write(buffer, 6U);
	if (ret != 6)
		return false;

	unsigned int count = 0U;
	unsigned int length;
	RESP_TYPE_V1 resp;
	do {
		milliSleep(10U);

		resp = getResponse(m_buffer, length);

		if (resp != RT1_GET_VERSION) {
			count++;
			if (count >= MAX_RESPONSES) {
				LogError("The DV-RPTR modem is not responding to the version command");
				return false;
			}
		}
	} while (resp != RT1_GET_VERSION);

	char firmware[50U];
	if ((m_buffer[4U] & 0x0FU) > 0x00U)
		::sprintf(firmware, "%u.%u%u%c", (m_buffer[5U] & 0xF0U) >> 4, m_buffer[5U] & 0x0FU, (m_buffer[4U] & 0xF0U) >> 4, (m_buffer[4U] & 0x0FU) + 'a' - 1U);
	else
		::sprintf(firmware, "%u.%u%u", (m_buffer[5U] & 0xF0U) >> 4, m_buffer[5U] & 0x0FU, (m_buffer[4U] & 0xF0U) >> 4);

	std::string hardware((char*)(m_buffer + 6U), length - DVRPTR_HEADER_LENGTH - 3U);

	LogInfo("DV-RPTR Modem Firmware version: %s, hardware: %s", firmware, hardware.c_str());

	return true;
}

bool CDVRPTRV1Controller::readStatus()
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

bool CDVRPTRV1Controller::setConfig()
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
	if (m_channel)
		buffer[6U] |= 0x04U;

	buffer[7U] = (m_modLevel * 256U) / 100U;

	buffer[8U] = m_txDelay >> 0;
	buffer[9U] = m_txDelay >> 8;

	if (m_checksum) {
		CCCITTChecksum cksum;
		cksum.update(buffer + 0U, 10U);
		cksum.result(buffer + 10U);
	} else {
		buffer[10U] = 0x00U;
		buffer[11U] = 0x0BU;
	}

	// CUtils::dump("Written", buffer, 12U);

	int ret = m_serial.write(buffer, 12U);
	if (ret != 12)
		return false;

	unsigned int count = 0U;
	unsigned int length;
	RESP_TYPE_V1 resp;
	do {

		milliSleep(10U);

		resp = getResponse(m_buffer, length);

		if (resp != RT1_SET_CONFIG) {
			count++;
			if (count >= MAX_RESPONSES) {
				LogError("The DV-RPTR modem is not responding to the SET_CONFIG command");
				return false;
			}
		}
	} while (resp != RT1_SET_CONFIG);

	// CUtils::dump("Response", m_buffer, length);

	unsigned char type = m_buffer[4U];
	if (type != DVRPTR_ACK) {
		LogError("Received a NAK to the SET_CONFIG command from the modem");
		return false;
	}

	return true;
}

bool CDVRPTRV1Controller::setEnabled(bool enable)
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

	// CUtils::dump("Written", buffer, 7U);

	int ret = m_serial.write(buffer, 7U);
	if (ret != 7)
		return false;

	unsigned int count = 0U;
	unsigned int length;
	RESP_TYPE_V1 resp;
	do {
		milliSleep(10U);

		resp = getResponse(m_buffer, length);

		if (resp != RT1_GET_STATUS) {
			count++;
			if (count >= MAX_RESPONSES) {
				LogError("The DV-RPTR modem is not responding to the SET_STATUS command");
				return false;
			}
		}
	} while (resp != RT1_GET_STATUS);

	// CUtils::dump("Response", m_buffer, length);

	unsigned char type = m_buffer[4U];
	if (type != DVRPTR_ACK) {
		LogError("Received a NAK to the SET_STATUS command from the modem");
		return false;
	}

	return true;
}

RESP_TYPE_V1 CDVRPTRV1Controller::getResponse(unsigned char *buffer, unsigned int& length)
{
	// Get the start of the frame or nothing at all
	int ret = m_serial.read(buffer, 1U);
	if (ret < 0) {
		LogError("Error when reading from the DV-RPTR");
		return RT1_ERROR;
	}

	if (ret == 0)
		return RT1_TIMEOUT;

	unsigned int offset = 1U;

	while (offset < DVRPTR_HEADER_LENGTH) {
		int ret = m_serial.read(buffer + offset, DVRPTR_HEADER_LENGTH - offset);
		if (ret < 0) {
			LogError("Error when reading from the DV-RPTR");
			return RT1_ERROR;
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
			LogError("Error when reading from the DV-RPTR");
			return RT1_ERROR;
		}

		if (ret > 0)
			offset += ret;

		if (ret == 0)
			Sleep(5UL);
	}

	length += DVRPTR_HEADER_LENGTH;

	// CUtils::dump("Received", buffer, length);

	switch (type) {
		case DVRPTR_GET_STATUS:
			return RT1_GET_STATUS;
		case DVRPTR_GET_VERSION:
			return RT1_GET_VERSION;
		case DVRPTR_GET_SERIAL:
			return RT1_GET_SERIAL;
		case DVRPTR_GET_CONFIG:
			return RT1_GET_CONFIG;
		case DVRPTR_SET_CONFIG:
			return RT1_SET_CONFIG;
		case DVRPTR_RXPREAMBLE:
			return RT1_RXPREAMBLE;
		case DVRPTR_START:
			return RT1_START;
		case DVRPTR_HEADER:
			return RT1_HEADER;
		case DVRPTR_RXSYNC:
			return RT1_RXSYNC;
		case DVRPTR_DATA:
			return RT1_DATA;
		case DVRPTR_EOT:
			return RT1_EOT;
		case DVRPTR_RXLOST:
			return RT1_RXLOST;
		case DVRPTR_SET_TESTMDE:
			return RT1_SET_TESTMDE;
		default:
			return RT1_UNKNOWN;
	}
}

std::string CDVRPTRV1Controller::getPath() const
{
	return m_path;
}

bool CDVRPTRV1Controller::findPort()
{
	if (m_path.empty())
		return false;

#if !defined(WIN32)
	DIR* dir = ::opendir("/sys/class/tty");
	if (dir == NULL) {
		LogError("Cannot open directory /sys/class/tty");
		return false;
	}

	dirent* entry = ::readdir(dir);
	while (entry != NULL) {
		if (::strncmp(entry->d_name, "ttyACM", 6U) != 0) {
			entry = ::readdir(dir);
			continue;
		}

		char cpath[255U];
		::sprintf(cpath, "/sys/class/tty/%s", entry->d_name);

		char symlink[255U];
		int ret = ::readlink(cpath, symlink, 255U);
		if (ret < 0) {
			::strcat(cpath, "/device");
			ret = ::readlink(cpath, symlink, 255U);
			if (ret < 0) {
				LogError("Error from readlink()");
				return false;
			}

			path = std::string(symlink, ret);
		} else {
			// Get all but the last section
			int n = ::strrchr(symlink, '/');
			if (n != -1)
				path = std::string(symlink, n);
			else
				path = std::string(symlink);
		}

		if (path == m_usbPath)) {
			char device[50U];
			::sprintf(device, "/dev/%s", entry->d_name);
			m_usbPort = std::string(device);
			LogMessage("Found modem port of %s based on the path", device);
			return true;
		}

		entry = ::readdir(dir);
	}
#endif

	return false;
}

bool CDVRPTRV1Controller::findPath()
{
#if !defined(WIN32)
	char cpath[255U];
	::sprintf(cpath, "/sys/class/tty/%s", m_port.substr(5U).c_str());

	char symlink[255U];
	int ret = ::readlink(cpath, symlink, 255U);
	if (ret < 0) {
		::strcat(cpath, "/device");
		ret = ::readlink(cpath, symlink, 255U);
		if (ret < 0) {
			LogError("Error from readlink()");
			return false;
		}

		path = std::string(symlink, ret);
	} else {
		// Get all but the last section
		int n = ::strrchr(symlink, '/');
		if (n != -1)
			path = std::string(symlink, n);
		else
			path = std::string(symlink);
	}

	if (m_path.empty())
		LogMessage("Found modem path of %s", path.c_str());

	m_path = path;
#endif

	return true;
}

bool CDVRPTRV1Controller::findModem()
{
	m_serial.close();

	// Tell the repeater that the signal has gone away
	if (m_rx) {
		CMutexLocker locker(m_mutex);

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
			LogMessage("Trying to reopen the modem");

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

bool CDVRPTRV1Controller::openModem()
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

	ret = setEnabled(true);
	if (!ret) {
		m_serial.close();
		return false;
	}

	return true;
}

