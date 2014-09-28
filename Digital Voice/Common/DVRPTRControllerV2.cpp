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

// TODO Handle flags 102 + 103

#include "DVRPTRControllerV2.h"
#include "DStarDefines.h"
#include "Timer.h"

#if defined(__WINDOWS__)
#include <setupapi.h>
#else
#include <wx/dir.h>
#endif

const unsigned int MAX_RESPONSES = 30U;

wxArrayString CDVRPTRControllerV2::getDevices()
{
#if defined(__WINDOWS__)
	return CSerialDataController::getDevices();
#else
	wxArrayString devices;
	devices.Alloc(5U);

	devices.Add(wxT("/dev/ttyACM0"));
	devices.Add(wxT("/dev/ttyACM1"));
	devices.Add(wxT("/dev/ttyACM2"));
	devices.Add(wxT("/dev/ttyACM3"));
	devices.Add(wxT("/dev/ttyACM4"));
	devices.Add(wxT("/dev/ttyACM5"));
	devices.Add(wxT("/dev/ttyACM6"));
	devices.Add(wxT("/dev/ttyACM7"));
	devices.Add(wxT("/dev/ttyACM8"));
	devices.Add(wxT("/dev/ttyACM9"));

	return devices;
#endif
}

const unsigned int BUFFER_LENGTH = 200U;

CDVRPTRControllerV2::CDVRPTRControllerV2(const wxString& port, bool txInvert, unsigned int modLevel) :
wxThread(wxTHREAD_JOINABLE),
m_port(port),
m_txInvert(txInvert),
m_modLevel(modLevel),
m_serial(port, SERIAL_115200),
m_buffer(NULL),
m_rxData(1000U),
m_txData(1000U),
m_counter(0U),
m_ptt(false),
m_rx(false),
m_space(0U),
m_stopped(false),
m_mutex()
{
	wxASSERT(!port.IsEmpty());

	m_buffer = new unsigned char[BUFFER_LENGTH];
}

CDVRPTRControllerV2::~CDVRPTRControllerV2()
{
	delete[] m_buffer;
}

bool CDVRPTRControllerV2::open()
{
	bool ret = m_serial.open();
	if (!ret)
		return false;

	ret = getSerial();
	if (!ret) {
		m_serial.close();
		return false;
	}

	ret = setConfig();
	if (!ret) {
		m_serial.close();
		return false;
	}

	Create();
	SetPriority(100U);
	Run();

	return true;
}

void* CDVRPTRControllerV2::Entry()
{
	wxLogMessage(wxT("Starting DV-RPTR2 Modem Controller thread"));

	// Clock every 5ms-ish
	CTimer pollTimer(200U, 0U, 250U);

	pollTimer.start();

	while (!m_stopped) {
		// Poll the modem status every 250ms
		if (pollTimer.hasExpired()) {
			bool ret = getSpace();
			if (!ret) {
				wxLogMessage(wxT("Stopping DV-RPTR2 Modem Controller thread"));
				return NULL;
			}

			pollTimer.reset();
		}

		unsigned int length;
		RESP_TYPE_V2 type = getResponse(m_buffer, length);

		switch (type) {
			case RT2_TIMEOUT:
				break;

			case RT2_ERROR:
				wxLogMessage(wxT("Stopping DV-RPTR2 Modem Controller thread"));
				return NULL;

			case RT2_HEADER: {
					// CUtils::dump(wxT("RT2_HEADER"), m_buffer, length);
					m_mutex.Lock();

					unsigned int space = m_rxData.freeSpace();
					if (space < 57U) {
						wxLogMessage(wxT("Out of space in the DV-RPTR RX queue"));
					} else {
						unsigned char data[2U];
						data[0U] = DQT_HEADER;
						data[1U] = RADIO_HEADER_LENGTH_BYTES;
						m_rxData.addData(data, 2U);
						m_rxData.addData(m_buffer + 9U, RADIO_HEADER_LENGTH_BYTES - 2U);

						// Dummy checksum
						data[0U] = 0xFFU;
						data[1U] = 0xFFU;
						m_rxData.addData(data, 2U);

						data[0U] = DQT_DATA;
						data[1U] = DV_FRAME_LENGTH_BYTES;
						m_rxData.addData(data, 2U);
						m_rxData.addData(m_buffer + 51U, DV_FRAME_LENGTH_BYTES);

						m_rx = true;
					}

					m_mutex.Unlock();
				}
				break;

			case RT2_DATA: {
					// CUtils::dump(wxT("RT2_DATA"), m_buffer, length);
					m_mutex.Lock();

					unsigned int space = m_rxData.freeSpace();
					if (space < 16U) {
						wxLogMessage(wxT("Out of space in the DV-RPTR RX queue"));
					} else {
						unsigned char data[2U];
						data[0U] = DQT_DATA;
						data[1U] = DV_FRAME_LENGTH_BYTES;
						m_rxData.addData(data, 2U);
						m_rxData.addData(m_buffer + 51U, DV_FRAME_LENGTH_BYTES);

						m_rx = true;

						// End of transmission?
						if ((m_buffer[50U] & 0x40U) == 0x40) {
							data[0U] = DQT_EOT;
							data[1U] = 0U;
							m_rxData.addData(data, 2U);

							m_rx = false;
						}
					}

					m_mutex.Unlock();
				}
				break;

			case RT2_SPACE:
				m_space = m_buffer[9U];
				// CUtils::dump(wxT("RT2_SPACE"), m_buffer, length);
				break;

			// These should not be received in this loop, but don't complain if we do
			case RT2_QUERY:
			case RT2_CONFIG:
				break;

			default:
				wxLogMessage(wxT("Unknown DV-RPTR2 message, type"));
				CUtils::dump(wxT("Buffer dump"), m_buffer, length);
				break;
		}

		if (m_space > 0U) {
			if (!m_txData.isEmpty()) {
				m_mutex.Lock();

				unsigned char len = 0U;
				m_txData.getData(&len, 1U);

				unsigned char data[200U];
				m_txData.getData(data, len);

				m_mutex.Unlock();

				// CUtils::dump(wxT("Write"), data, len);

				int ret = m_serial.write(data, len);
				if (ret == -1) {
					wxLogMessage(wxT("Stopping DV-RPTR2 Modem Controller thread"));
					return NULL;
				} else {
					m_space--;
				}
			}
		}

		Sleep(5UL);

		pollTimer.clock();
	}

	wxLogMessage(wxT("Stopping DV-RPTR2 Modem Controller thread"));

	m_serial.close();

	return NULL;
}

DATA_QUEUE_TYPE CDVRPTRControllerV2::readQueue(unsigned char* data, unsigned int& length)
{
	wxMutexLocker locker(m_mutex);

	if (m_rxData.isEmpty())
		return DQT_NONE;

	unsigned char header[2U];
	m_rxData.getData(header, 2U);

	DATA_QUEUE_TYPE type = DATA_QUEUE_TYPE(header[0U]);

	length = header[1U];

	if (length > 0U)
		m_rxData.getData(data, length);

	return type;
}

bool CDVRPTRControllerV2::writeHeader(const CHeaderData& header)
{
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

	wxString rpt2 = header.getRptCall2();
	for (unsigned int i = 0U; i < rpt2.Len() && i < LONG_CALLSIGN_LENGTH; i++)
		buffer[i + 12U]  = rpt2.GetChar(i);

	wxString rpt1 = header.getRptCall1();
	for (unsigned int i = 0U; i < rpt1.Len() && i < LONG_CALLSIGN_LENGTH; i++)
		buffer[i + 20U] = rpt1.GetChar(i);

	wxString your = header.getYourCall();
	for (unsigned int i = 0U; i < your.Len() && i < LONG_CALLSIGN_LENGTH; i++)
		buffer[i + 28U] = your.GetChar(i);

	wxString my1 = header.getMyCall1();
	for (unsigned int i = 0U; i < my1.Len() && i < LONG_CALLSIGN_LENGTH; i++)
		buffer[i + 36U] = my1.GetChar(i);

	wxString my2 = header.getMyCall2();
	for (unsigned int i = 0U; i < my2.Len() && i < SHORT_CALLSIGN_LENGTH; i++)
		buffer[i + 44U] = my2.GetChar(i);

	m_counter = 0U;

	wxMutexLocker locker(m_mutex);

	unsigned int space = m_txData.freeSpace();
	if (space < 106U)
		return false;

	unsigned char len = 105U;
	m_txData.addData(&len, 1U);

	m_txData.addData(buffer, 105U);

	m_ptt = true;

	return true;
}

bool CDVRPTRControllerV2::writeData(const unsigned char* data, unsigned int length, bool end)
{
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
	buffer[8U] = '1';

	buffer[50U] = m_counter;
	if (end)
		buffer[50U] |= 0x40U;

	m_counter++;
	if (m_counter == 21U)
		m_counter = 0U;

	::memcpy(buffer + 51U, data, DV_FRAME_LENGTH_BYTES);

	if (end) {
		buffer[60U] = 0x55U;
		buffer[61U] = 0x55U;
		buffer[62U] = 0x55U;

		m_ptt = false;
	}

	wxMutexLocker locker(m_mutex);

	unsigned int space = m_txData.freeSpace();
	if (space < 106U)
		return false;

	unsigned char len = 105U;
	m_txData.addData(&len, 1U);

	m_txData.addData(buffer, 105U);

	return true;
}

bool CDVRPTRControllerV2::getPTT() const
{
	return m_ptt;
}

bool CDVRPTRControllerV2::hasSpace()
{
	wxMutexLocker locker(m_mutex);

	return m_txData.freeSpace() > 200U;
}

void CDVRPTRControllerV2::close()
{
	m_stopped = true;

	Wait();
}

bool CDVRPTRControllerV2::getSerial()
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

	// CUtils::dump(wxT("Written"), buffer, 105U);

	int ret = m_serial.write(buffer, 105U);
	if (ret != 105)
		return false;

	unsigned int count = 0U;
	unsigned int length;
	RESP_TYPE_V2 resp;
	do {
		::wxMilliSleep(10UL);

		resp = getResponse(m_buffer, length);

		if (resp != RT2_QUERY) {
			count++;
			if (count >= MAX_RESPONSES) {
				wxLogError(wxT("The DV-RPTR modem is not responding to the query command"));
				return false;
			}
		}
	} while (resp != RT2_QUERY);

	wxLogInfo(wxT("DV-RPTR Modem Hardware serial: 0x%02X%02X%02x%02X"), m_buffer[9U], m_buffer[10U], m_buffer[11U], m_buffer[12U]);

	return true;
}

bool CDVRPTRControllerV2::getSpace()
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

	// CUtils::dump(wxT("Written"), buffer, 10U);

	return m_serial.write(buffer, 10U) == 10;
}

bool CDVRPTRControllerV2::setConfig()
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

	buffer[66U] = m_txInvert ? 0x01U : 0x00U;

	buffer[73U] = (m_modLevel * 256U) / 100U;

	// CUtils::dump(wxT("Written"), buffer, 105U);

	int ret = m_serial.write(buffer, 105U);
	if (ret != 105)
		return false;

	unsigned int count = 0U;
	unsigned int length;
	RESP_TYPE_V2 resp;
	do {

		::wxMilliSleep(10UL);

		resp = getResponse(m_buffer, length);

		if (resp != RT2_CONFIG) {
			count++;
			if (count >= MAX_RESPONSES) {
				wxLogError(wxT("The DV-RPTR modem is not responding to the configure command"));
				return false;
			}
		}
	} while (resp != RT2_CONFIG);

	// CUtils::dump(wxT("Response"), m_buffer, length);

	wxString firmware((char*)(m_buffer + 9U), wxConvLocal);

	wxLogInfo(wxT("DV-RPTR Modem Firmware version: %s"), firmware.c_str());

	return true;
}

RESP_TYPE_V2 CDVRPTRControllerV2::getResponse(unsigned char *buffer, unsigned int& length)
{
	// Get the start of the frame or nothing at all
	int ret = m_serial.read(buffer, 5U);
	if (ret < 0) {
		wxLogError(wxT("Error when reading from the DV-RPTR"));
		return RT2_ERROR;
	}

	if (ret == 0)
		return RT2_TIMEOUT;

	if (::memcmp(buffer + 0U, "HEAD", 4U) != 0) {
		wxLogError(wxT("DV-RPTR frame start is incorrect - 0x%02X 0x%02X 0x%02X 0x%02X"), buffer[0U], buffer[1U], buffer[2U], buffer[3U]);
		return RT2_UNKNOWN;
	}

	if (buffer[4U] == 'X') {
		length = 105U;
	} else if (buffer[4U] == 'Y') {
		length = 10U;
	} else {
		wxLogError(wxT("DV-RPTR frame type is incorrect - 0x%02X"), buffer[4U]);
		return RT2_UNKNOWN;
	}

	unsigned int offset = 5U;

	while (offset < length) {
		int ret = m_serial.read(buffer + offset, length - offset);
		if (ret < 0) {
			wxLogError(wxT("Error when reading from the DV-RPTR"));
			return RT2_ERROR;
		}

		if (ret > 0)
			offset += ret;

		if (ret == 0)
			Sleep(5UL);
	}

	// CUtils::dump(wxT("Received"), buffer, length);

	if (::memcmp(buffer + 5U, "0001", 4U) == 0) {
		if (buffer[104U] == 0x01U)
			return RT2_HEADER;
		else
			return RT2_DATA;
	} else if (::memcmp(buffer + 5U, "9900", 4U) == 0) {
		return RT2_QUERY;
	} else if (::memcmp(buffer + 5U, "9001", 4U) == 0) {
		return RT2_CONFIG;
	} else if (::memcmp(buffer + 5U, "9011", 4U) == 0) {
		return RT2_SPACE;
	} else {
		wxLogError(wxT("DV-RPTR frame type number is incorrect - 0x%02X 0x%02X 0x%02X 0x%02X"), buffer[5U], buffer[6U], buffer[7U], buffer[8U]);
		return RT2_UNKNOWN;
	}
}
