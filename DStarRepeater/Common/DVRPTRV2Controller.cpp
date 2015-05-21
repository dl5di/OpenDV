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

#include "DVRPTRV2Controller.h"

#include "DStarDefines.h"
#include "MutexLocker.h"
#include "Timer.h"
#include "Log.h"

#include <cassert>

#if defined(WIN32)
#include <setupapi.h>
#else
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#endif

const unsigned int MAX_RESPONSES = 30U;

const unsigned int BUFFER_LENGTH = 200U;

CDVRPTRV2Controller::CDVRPTRV2Controller(const std::string& port, const std::string& path, bool txInvert, unsigned int modLevel, bool duplex, const std::string& callsign, unsigned int txDelay) :
CModem(),
m_connection(CT_USB),
m_usbPort(port),
m_usbPath(path),
m_address(),
m_port(0U),
m_txInvert(txInvert),
m_modLevel(modLevel),
m_duplex(duplex),
m_callsign(callsign),
m_txDelay(txDelay),
m_usb(NULL),
m_network(NULL),
m_buffer(NULL),
m_txData(1000U),
m_rx(false)
{
	assert(!port.empty());

	m_usb = new CSerialDataController(port, SERIAL_115200);

	m_buffer = new unsigned char[BUFFER_LENGTH];
}

CDVRPTRV2Controller::CDVRPTRV2Controller(const std::string& address, unsigned int port, bool txInvert, unsigned int modLevel, bool duplex, const std::string& callsign, unsigned int txDelay) :
CModem(),
m_connection(CT_NETWORK),
m_usbPort(),
m_usbPath(),
m_address(address),
m_port(port),
m_txInvert(txInvert),
m_modLevel(modLevel),
m_duplex(duplex),
m_callsign(callsign),
m_txDelay(txDelay),
m_usb(NULL),
m_network(NULL),
m_buffer(NULL),
m_txData(1000U),
m_rx(false)
{
	assert(!address.empty());
	assert(port > 0U);

	m_network = new CTCPReaderWriter(address, port);

	m_buffer = new unsigned char[BUFFER_LENGTH];
}

CDVRPTRV2Controller::~CDVRPTRV2Controller()
{
	delete m_usb;
	delete m_network;

	delete[] m_buffer;
}

bool CDVRPTRV2Controller::start()
{
	findPort();

	bool ret = openModem();
	if (!ret)
		return false;

	findPath();

	run();

	return true;
}

void CDVRPTRV2Controller::entry()
{
	LogMessage("Starting DV-RPTR2 Modem Controller thread");

	// Clock every 5ms-ish
	CTimer pollTimer(200U, 0U, 250U);
	pollTimer.start();

	unsigned int space = 0U;

	while (!m_stopped) {
		// Poll the modem status every 250ms
		if (pollTimer.hasExpired()) {
			bool ret = readSpace();
			if (!ret) {
				ret = findModem();
				if (!ret) {
					LogMessage("Stopping DV-RPTR2 Modem Controller thread");
					return;
				}
			}

			pollTimer.start();
		}

		unsigned int length;
		RESP_TYPE_V2 type = getResponse(m_buffer, length);

		switch (type) {
			case RT2_TIMEOUT:
				break;

			case RT2_ERROR: {
					bool ret = findModem();
					if (!ret) {
						LogMessage("Stopping DV-RPTR2 Modem Controller thread");
						return;
					}
				}
				break;

			case RT2_HEADER: {
					// CUtils::dump("RT2_HEADER", m_buffer, length);
					CMutexLocker locker(m_mutex);

					unsigned char data[2U];
					data[0U] = DSMTT_HEADER;
					data[1U] = RADIO_HEADER_LENGTH_BYTES;
					m_rxData.addData(data, 2U);

					m_rxData.addData(m_buffer + 9U, RADIO_HEADER_LENGTH_BYTES - 2U);

					// Dummy checksum
					data[0U] = 0xFFU;
					data[1U] = 0xFFU;
					m_rxData.addData(data, 2U);

					data[0U] = DSMTT_DATA;
					data[1U] = DV_FRAME_LENGTH_BYTES;
					m_rxData.addData(data, 2U);

					m_rxData.addData(m_buffer + 51U, DV_FRAME_LENGTH_BYTES);

					m_rx = true;
				}
				break;

			case RT2_DATA: {
					// CUtils::dump("RT2_DATA", m_buffer, length);
					CMutexLocker locker(m_mutex);

					unsigned char data[2U];
					data[0U] = DSMTT_DATA;
					data[1U] = DV_FRAME_LENGTH_BYTES;
					m_rxData.addData(data, 2U);

					m_rxData.addData(m_buffer + 5U, DV_FRAME_LENGTH_BYTES);

					m_rx = true;

					// End of transmission?
					bool end = (m_buffer[19U] & 0x40U) == 0x40U;
					if (end) {
						data[0U] = DSMTT_EOT;
						data[1U] = 0U;
						m_rxData.addData(data, 2U);

						m_rx = false;
					}
				}
				break;

			case RT2_SPACE:
				space = m_buffer[9U];
				// CUtils::dump("RT2_SPACE", m_buffer, length);
				break;

			// These should not be received in this loop, but don't complain if we do
			case RT2_QUERY:
			case RT2_CONFIG:
				break;

			default:
				LogMessage("Unknown DV-RPTR2 message, type");
				CUtils::dump("Buffer dump", m_buffer, length);
				break;
		}

		if (space > 0U) {
			if (m_txData.hasData()) {
				unsigned char len = 0U;
				unsigned char data[200U];

				{
					CMutexLocker locker(m_mutex);

					m_txData.getData(&len, 1U);
					m_txData.getData(data, len);
				}

				// CUtils::dump("Write", data, len);

				bool ret = writeModem(data, len);
				if (!ret) {
					bool ret = findModem();
					if (!ret) {
						LogMessage("Stopping DV-RPTR2 Modem Controller thread");
						return;
					}
				} else {
					space--;
				}
			}
		}

		Sleep(5UL);

		pollTimer.clock();
	}

	LogMessage("Stopping DV-RPTR2 Modem Controller thread");

	closeModem();
}

bool CDVRPTRV2Controller::writeHeader(const CHeaderData& header)
{
	bool ret = m_txData.hasSpace(106U);
	if (!ret) {
		LogWarning("No space to write the header");
		return false;
	}

	unsigned char buffer[105U];

	::memset(buffer, 0x00U, 105U);

	buffer[0U] = 'H';
	buffer[1U] = 'E';
	buffer[2U] = 'A';
	buffer[3U] = 'D';
	buffer[4U] = 'X';
	buffer[5U] = '0';
	buffer[6U] = '0';
	buffer[7U] = '0';
	buffer[8U] = '2';

	::memset(buffer + 9U, ' ', RADIO_HEADER_LENGTH_BYTES);

	buffer[9U]  = header.getFlag1();
	buffer[10U] = header.getFlag2();
	buffer[11U] = header.getFlag3();

	std::string rpt2 = header.getRptCall2();
	for (unsigned int i = 0U; i < rpt2.length() && i < LONG_CALLSIGN_LENGTH; i++)
		buffer[i + 12U]  = rpt2.at(i);

	std::string rpt1 = header.getRptCall1();
	for (unsigned int i = 0U; i < rpt1.length() && i < LONG_CALLSIGN_LENGTH; i++)
		buffer[i + 20U] = rpt1.at(i);

	std::string your = header.getYourCall();
	for (unsigned int i = 0U; i < your.length() && i < LONG_CALLSIGN_LENGTH; i++)
		buffer[i + 28U] = your.at(i);

	std::string my1 = header.getMyCall1();
	for (unsigned int i = 0U; i < my1.length() && i < LONG_CALLSIGN_LENGTH; i++)
		buffer[i + 36U] = my1.at(i);

	std::string my2 = header.getMyCall2();
	for (unsigned int i = 0U; i < my2.length() && i < SHORT_CALLSIGN_LENGTH; i++)
		buffer[i + 44U] = my2.at(i);

	CMutexLocker locker(m_mutex);

	unsigned char len = 105U;
	m_txData.addData(&len, 1U);

	m_txData.addData(buffer, 105U);

	m_tx = true;

	return true;
}

bool CDVRPTRV2Controller::writeData(const unsigned char* data, unsigned int length, bool end)
{
	bool ret = m_txData.hasSpace(18U);
	if (!ret) {
		LogWarning("No space to write data");
		return false;
	}

	unsigned char buffer[17U];

	::memset(buffer, 0x00U, 17U);

	buffer[0U] = 'H';
	buffer[1U] = 'E';
	buffer[2U] = 'A';
	buffer[3U] = 'D';
	buffer[4U] = 'Z';

	::memcpy(buffer + 5U, data, DV_FRAME_LENGTH_BYTES);

	if (end) {
		buffer[14U] = 0x55U;
		buffer[15U] = 0x55U;
		buffer[16U] = 0x55U;

		m_tx = false;
	}

	CMutexLocker locker(m_mutex);

	unsigned char len = 17U;
	m_txData.addData(&len, 1U);

	m_txData.addData(buffer, 17U);

	return true;
}

unsigned int CDVRPTRV2Controller::getSpace()
{
	return m_txData.freeSpace() / 18U;
}

bool CDVRPTRV2Controller::isTXReady()
{
	return m_txData.isEmpty();
}

bool CDVRPTRV2Controller::readSerial()
{
	unsigned char buffer[105U];

	::memset(buffer, 0x00U, 105U);

	buffer[0U] = 'H';
	buffer[1U] = 'E';
	buffer[2U] = 'A';
	buffer[3U] = 'D';
	buffer[4U] = 'X';
	buffer[5U] = '9';
	buffer[6U] = '0';
	buffer[7U] = '0';
	buffer[8U] = '0';

	// CUtils::dump("Written", buffer, 105U);

	bool ret = writeModem(buffer, 105U);
	if (!ret)
		return false;

	unsigned int count = 0U;
	unsigned int length;
	RESP_TYPE_V2 resp;
	do {
		milliSleep(10U);

		resp = getResponse(m_buffer, length);

		if (resp != RT2_QUERY) {
			count++;
			if (count >= MAX_RESPONSES) {
				LogError("The DV-RPTR modem is not responding to the query command");
				return false;
			}
		}
	} while (resp != RT2_QUERY);

	LogInfo("DV-RPTR Modem Hardware serial: 0x%02X%02X%02x%02X", m_buffer[9U], m_buffer[10U], m_buffer[11U], m_buffer[12U]);

	return true;
}

bool CDVRPTRV2Controller::readSpace()
{
	unsigned char buffer[10U];

	buffer[0U] = 'H';
	buffer[1U] = 'E';
	buffer[2U] = 'A';
	buffer[3U] = 'D';
	buffer[4U] = 'Y';
	buffer[5U] = '9';
	buffer[6U] = '0';
	buffer[7U] = '1';
	buffer[8U] = '1';
	buffer[9U] = 0x00U;

	// CUtils::dump("Written", buffer, 10U);

	return writeModem(buffer, 10U);
}

bool CDVRPTRV2Controller::setConfig()
{
	unsigned char buffer[105U];

	::memset(buffer, 0x00U, 105U);

	buffer[0U] = 'H';
	buffer[1U] = 'E';
	buffer[2U] = 'A';
	buffer[3U] = 'D';
	buffer[4U] = 'X';
	buffer[5U] = '9';
	buffer[6U] = '0';
	buffer[7U] = '0';
	buffer[8U] = '1';

	::memset(buffer + 9U, ' ', LONG_CALLSIGN_LENGTH);
	for (unsigned int i = 0U; i < LONG_CALLSIGN_LENGTH && i < m_callsign.length(); i++)
		buffer[9U + i] = m_callsign.at(i);

	buffer[65U] = m_duplex ? 0x03U : 0x00U;

	buffer[66U] = m_txInvert ? 0x01U : 0x00U;

	buffer[73U] = (m_modLevel * 256U) / 100U;

	// Internal and external speaker on
	buffer[86U] = 0x03U;

	buffer[87U] = 0x01U;

	if (m_txDelay < 100U)
		m_txDelay = 100U;
	if (m_txDelay > 850U)
		m_txDelay = 850U;

	buffer[89U] = m_txDelay / 10U;

	// CUtils::dump("Written", buffer, 105U);

	bool ret = writeModem(buffer, 105U);
	if (!ret)
		return false;

	unsigned int count = 0U;
	unsigned int length;
	RESP_TYPE_V2 resp;
	do {
		milliSleep(10U);

		resp = getResponse(m_buffer, length);

		if (resp != RT2_CONFIG) {
			count++;
			if (count >= MAX_RESPONSES) {
				LogError("The DV-RPTR modem is not responding to the configure command");
				return false;
			}
		}
	} while (resp != RT2_CONFIG);

	// CUtils::dump("Response", m_buffer, length);

	std::string firmware((char*)(m_buffer + 9U));

	LogInfo("DV-RPTR Modem Firmware version: %s", firmware.c_str());

	return true;
}

RESP_TYPE_V2 CDVRPTRV2Controller::getResponse(unsigned char *buffer, unsigned int& length)
{
	// Get the start of the frame or nothing at all
	int ret = readModem(buffer + 0U, 5U);
	if (ret < 0) {
		LogError("Error when reading from the DV-RPTR");
		return RT2_ERROR;
	}

	if (ret == 0)
		return RT2_TIMEOUT;

	while (::memcmp(buffer + 0U, "HEAD", 4U) != 0) {
		buffer[0U] = buffer[1U];
		buffer[1U] = buffer[2U];
		buffer[2U] = buffer[3U];
		buffer[3U] = buffer[4U];

		ret = readModem(buffer + 4U, 1U);
		if (ret < 0) {
			LogError("Error when reading from the DV-RPTR");
			return RT2_ERROR;
		}

		if (ret == 0)
			return RT2_TIMEOUT;
	}

	switch (buffer[4U]) {
		case 'X':
			length = 105U;
			break;
		case 'Y':
			length = 10U;
			break;
		case 'Z':
			length = 20U;
			break;
		default:
			LogError("DV-RPTR frame type is incorrect - 0x%02X", buffer[4U]);
			return RT2_UNKNOWN;
	}

	unsigned int offset = 5U;

	while (offset < length) {
		int ret = readModem(buffer + offset, length - offset);
		if (ret < 0) {
			LogError("Error when reading from the DV-RPTR");
			return RT2_ERROR;
		}

		if (ret > 0)
			offset += ret;

		if (ret == 0)
			Sleep(5UL);
	}

	// CUtils::dump("Received", buffer, length);

	if (::memcmp(buffer + 0U, "HEADZ", 5U) == 0) {
		return RT2_DATA;
	} else if (::memcmp(buffer + 5U, "0001", 4U) == 0) {
		if (buffer[104U] == 0x01U)
			return RT2_HEADER;
	} else if (::memcmp(buffer + 5U, "9900", 4U) == 0) {
		return RT2_QUERY;
	} else if (::memcmp(buffer + 5U, "9001", 4U) == 0) {
		return RT2_CONFIG;
	} else if (::memcmp(buffer + 5U, "9011", 4U) == 0) {
		return RT2_SPACE;
	} else if (::memcmp(buffer + 5U, "9021", 4U) == 0) {
		return RT2_TIMEOUT;
	}

	return RT2_UNKNOWN;
}

std::string CDVRPTRV2Controller::getPath() const
{
	return m_usbPath;
}

bool CDVRPTRV2Controller::findPort()
{
	if (m_connection != CT_USB)
		return true;

	if (m_usbPath.empty())
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

bool CDVRPTRV2Controller::findPath()
{
	if (m_connection != CT_USB)
		return true;

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

bool CDVRPTRV2Controller::findModem()
{
	closeModem();

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

bool CDVRPTRV2Controller::openModem()
{
	bool ret = false;

	switch (m_connection) {
		case CT_USB:
			ret = m_usb->open();
			break;
		case CT_NETWORK:
			ret = m_network->open();
			break;
		default:
			LogError("Invalid connection type: %d", int(m_connection));
			break;
	}

	if (!ret)
		return false;

	ret = readSerial();
	if (!ret) {
		closeModem();
		return false;
	}

	ret = setConfig();
	if (!ret) {
		closeModem();
		return false;
	}

	return true;
}

int CDVRPTRV2Controller::readModem(unsigned char* buffer, unsigned int length)
{
	switch (m_connection) {
		case CT_USB:
			return m_usb->read(buffer, length);
		case CT_NETWORK:
			return m_network->read(buffer, length, 0U);
		default:
			return -1;
	}
}

bool CDVRPTRV2Controller::writeModem(const unsigned char* buffer, unsigned int length)
{
	switch (m_connection) {
		case CT_USB:
			return m_usb->write(buffer, length) == int(length);
		case CT_NETWORK:
			return m_network->write(buffer, length);
		default:
			return false;
	}
}

void CDVRPTRV2Controller::closeModem()
{
	switch (m_connection) {
		case CT_USB:
			return m_usb->close();
		case CT_NETWORK:
			return m_network->close();
		default:
			return;
	}
}
