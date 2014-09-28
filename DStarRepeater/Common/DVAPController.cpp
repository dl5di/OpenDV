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
#include "DVAPController.h"
#include "DStarDefines.h"
#include "Timer.h"

const unsigned char DVAP_REQ_NAME[] = {0x04, 0x20, 0x01, 0x00};
const unsigned int  DVAP_REQ_NAME_LEN = 4U;

const unsigned char DVAP_RESP_NAME[] = {0x10, 0x00, 0x01, 0x00, 'D', 'V', 'A', 'P', ' ', 'D', 'o', 'n', 'g', 'l', 'e', 0x00};
const unsigned int  DVAP_RESP_NAME_LEN = 16U;

const unsigned char DVAP_REQ_SERIAL[] = {0x04, 0x20, 0x02, 0x00};
const unsigned int  DVAP_REQ_SERIAL_LEN = 4U;

const unsigned char DVAP_RESP_SERIAL[] = {0x0C, 0x00, 0x02, 0x00};
const unsigned int  DVAP_RESP_SERIAL_LEN = 4U;

const unsigned char DVAP_REQ_FIRMWARE[] = {0x05, 0x20, 0x04, 0x00, 0x01};
const unsigned int  DVAP_REQ_FIRMWARE_LEN = 5U;

const unsigned char DVAP_RESP_FIRMWARE[] = {0x07, 0x00, 0x04, 0x00, 0x01, 0x00, 0x00};
const unsigned int  DVAP_RESP_FIRMWARE_LEN = 7U;

const unsigned char DVAP_REQ_MODULATION[] = {0x05, 0x00, 0x28, 0x00, 0x01};
const unsigned int  DVAP_REQ_MODULATION_LEN = 5U;

const unsigned char DVAP_RESP_MODULATION[] = {0x05, 0x00, 0x28, 0x00, 0x01};
const unsigned int  DVAP_RESP_MODULATION_LEN = 5U;

const unsigned char DVAP_REQ_MODE[] = {0x05, 0x00, 0x2A, 0x00, 0x00};
const unsigned int  DVAP_REQ_MODE_LEN = 5U;

const unsigned char DVAP_RESP_MODE[] = {0x05, 0x00, 0x2A, 0x00, 0x00};
const unsigned int  DVAP_RESP_MODE_LEN = 5U;

const unsigned char DVAP_REQ_SQUELCH[] = {0x05, 0x00, 0x80, 0x00, 0x00};
const unsigned int  DVAP_REQ_SQUELCH_LEN = 5U;

const unsigned char DVAP_RESP_SQUELCH[] = {0x05, 0x00, 0x80, 0x00, 0x00};
const unsigned int  DVAP_RESP_SQUELCH_LEN = 5U;

const unsigned char DVAP_REQ_POWER[] = {0x06, 0x00, 0x38, 0x01, 0x00, 0x00};
const unsigned int  DVAP_REQ_POWER_LEN = 6U;

const unsigned char DVAP_RESP_POWER[] = {0x06, 0x00, 0x38, 0x01, 0x00, 0x00};
const unsigned int  DVAP_RESP_POWER_LEN = 6U;

const unsigned char DVAP_REQ_FREQUENCY[] = {0x08, 0x00, 0x20, 0x02, 0x00, 0x00, 0x00, 0x00};
const unsigned int  DVAP_REQ_FREQUENCY_LEN = 8U;

const unsigned char DVAP_RESP_FREQUENCY[] = {0x08, 0x00, 0x20, 0x02, 0x00, 0x00, 0x00, 0x00};
const unsigned int  DVAP_RESP_FREQUENCY_LEN = 8U;

const unsigned char DVAP_REQ_FREQLIMITS[] = {0x04, 0x20, 0x30, 0x02};
const unsigned int  DVAP_REQ_FREQLIMITS_LEN = 4U;

const unsigned char DVAP_RESP_FREQLIMITS[] = {0x0C, 0x00, 0x30, 0x02};
const unsigned int  DVAP_RESP_FREQLIMITS_LEN = 4U;

const unsigned char DVAP_REQ_START[] = {0x05, 0x00, 0x18, 0x00, 0x01};
const unsigned int  DVAP_REQ_START_LEN = 5U;

const unsigned char DVAP_RESP_START[] = {0x05, 0x00, 0x18, 0x00, 0x01};
const unsigned int  DVAP_RESP_START_LEN = 5U;

const unsigned char DVAP_REQ_STOP[] = {0x05, 0x00, 0x18, 0x00, 0x00};
const unsigned int  DVAP_REQ_STOP_LEN = 5U;

const unsigned char DVAP_RESP_STOP[] = {0x05, 0x00, 0x18, 0x00, 0x00};
const unsigned int  DVAP_RESP_STOP_LEN = 5U;

const unsigned char DVAP_HEADER[] = {0x2F, 0xA0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
									 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
									 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
									 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
									 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const unsigned int  DVAP_HEADER_LEN = 47U;

const unsigned char DVAP_RESP_HEADER[] = {0x2F, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
										  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
										  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
										  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
										  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const unsigned int  DVAP_RESP_HEADER_LEN = 47U;

const unsigned char DVAP_RESP_PTT[] = {0x05, 0x20, 0x18, 0x01, 0x00};
const unsigned int  DVAP_RESP_PTT_LEN = 5U;

const unsigned char DVAP_GMSK_DATA[] = {0x12, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
										0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const unsigned int  DVAP_GMSK_DATA_LEN = 18U;

const unsigned char DVAP_STATUS[] = {0x07, 0x20, 0x90, 0x00, 0x00, 0x00, 0x00};
const unsigned int  DVAP_STATUS_LEN = 7U;

const unsigned char DVAP_ACK[] = {0x03, 0x60, 0x00};
const unsigned int  DVAP_ACK_LEN = 3U;

const unsigned char DVAP_FM_DATA[] = {0x42, 0x81};
const unsigned int  DVAP_FM_DATA_LEN = 2U;

const unsigned int DVAP_HEADER_LENGTH = 2U;

const unsigned int MAX_RESPONSES = 20U;

const unsigned int BUFFER_LENGTH = 200U;

const unsigned int DVAP_DUMP_LENGTH = 30U;

CDVAPController::CDVAPController(const wxString& port, unsigned int frequency, int power, int squelch) :
CModem(),
m_serial(port, SERIAL_230400),
m_frequency(frequency),
m_power(power),
m_squelch(squelch),
m_squelchOpen(false),
m_signal(0),
m_buffer(NULL),
m_streamId(0U),
m_framePos(0U),
m_seq(0U),
m_txData(1000U)
#if defined(DVAP_DUMP)
,
m_dvapData(NULL),
m_dvapLength(NULL),
m_dvapIndex(0U)
#endif
{
	wxASSERT(!port.IsEmpty());
	wxASSERT((frequency >= 144000000U && frequency <= 148000000U) ||
			 (frequency >= 420000000U && frequency <= 450000000U));
	wxASSERT(power >= -12 && power <= 10);
	wxASSERT(squelch >= -128 && squelch <= -45);

	m_buffer = new unsigned char[BUFFER_LENGTH];

#if defined(DVAP_DUMP)
	m_dvapData = new unsigned char*[DVAP_DUMP_LENGTH];
	for (unsigned int i = 0U; i < DVAP_DUMP_LENGTH; i++)
		m_dvapData[i] = new unsigned char[100U];

	m_dvapLength = new unsigned int[DVAP_DUMP_LENGTH];
	for (unsigned int i = 0U; i < DVAP_DUMP_LENGTH; i++)
		m_dvapLength[i] = 0U;
#endif
}

CDVAPController::~CDVAPController()
{
	delete[] m_buffer;

#if defined(DVAP_DUMP)
	for (unsigned int i = 0U; i < DVAP_DUMP_LENGTH; i++)
		delete[] m_dvapData[i];
	delete[] m_dvapData;

	delete[] m_dvapLength;
#endif
}

bool CDVAPController::start()
{
	bool res = m_serial.open();
	if (!res)
		return false;

	res = getName();
	if (!res) {
		m_serial.close();
		return false;
	}

	res = getFirmware();
	if (!res) {
		m_serial.close();
		return false;
	}

	res = getSerial();
	if (!res) {
		m_serial.close();
		return false;
	}

	res = setModulation();
	if (!res) {
		m_serial.close();
		return false;
	}

	res = setMode();
	if (!res) {
		m_serial.close();
		return false;
	}

	res = setSquelch();
	if (!res) {
		m_serial.close();
		return false;
	}

	res = setPower();
	if (!res) {
		m_serial.close();
		return false;
	}

	res = setFrequency();
	if (!res) {
		m_serial.close();
		return false;
	}

	res = startDVAP();
	if (!res) {
		m_serial.close();
		return false;
	}

	Create();
	SetPriority(100U);
	Run();

	return true;
}

void* CDVAPController::Entry()
{
	wxLogMessage(wxT("Starting DVAP Controller thread"));

	// Clock every 5ms-ish
	CTimer pollTimer(200U, 2U);
	pollTimer.start();

	unsigned char  writeLength = 0U;
	unsigned char* writeBuffer = new unsigned char[BUFFER_LENGTH];

	unsigned int space = 0U;

	while (!m_stopped) {
		// Poll the modem every 2s
		if (pollTimer.hasExpired()) {
			writePoll();
			pollTimer.start();
		}

		unsigned int length;
		RESP_TYPE type = getResponse(m_buffer, length);

		switch (type) {
			case RT_TIMEOUT:
				break;
			case RT_ERROR:
				wxLogMessage(wxT("Stopping DVAP Controller thread"));
#if defined(DVAP_DUMP)
				dumpPackets();
#endif
				m_serial.close();
				return NULL;
			case RT_STATE:
				m_signal      = int(m_buffer[4U]) - 256;
				m_squelchOpen = m_buffer[5U] == 0x01U;
				space         = m_buffer[6U];
				break;
			case RT_PTT:
				m_tx = m_buffer[4U] == 0x01U;	
				break;
			case RT_START:
				break;
			case RT_STOP:
				wxLogWarning(wxT("DVAP has stopped, restarting"));
#if defined(DVAP_DUMP)
				dumpPackets();
#endif
				startDVAP();
				break;
			case RT_HEADER: {
					wxMutexLocker locker(m_mutex);

					unsigned char hdr[2U];
					hdr[0U] = DSMTT_HEADER;
					hdr[1U] = RADIO_HEADER_LENGTH_BYTES;
					m_rxData.addData(hdr, 2U);

					m_rxData.addData(m_buffer + 6U, RADIO_HEADER_LENGTH_BYTES);
				}
				break;
			case RT_HEADER_ACK:
				break;
			case RT_GMSK_DATA: {
					wxMutexLocker locker(m_mutex);

					bool end = (m_buffer[4U] & 0x40U) == 0x40U;
					if (end) {
						unsigned char hdr[2U];
						hdr[0U] = DSMTT_EOT;
						hdr[1U] = 0U;
						m_rxData.addData(hdr, 2U);
					} else {
						unsigned char hdr[2U];
						hdr[0U] = DSMTT_DATA;
						hdr[1U] = length - 6U;
						m_rxData.addData(hdr, 2U);

						m_rxData.addData(m_buffer + 6U, length - 6U);
					}
				}
				break;
			case RT_FM_DATA:
				wxLogWarning(wxT("The DVAP has gone into FM mode, restarting the DVAP"));
#if defined(DVAP_DUMP)
				dumpPackets();
#endif
				stopDVAP();
				setModulation();
				startDVAP();
				break;
			default:
				wxLogMessage(wxT("Unknown message"));
				CUtils::dump(wxT("Buffer dump"), m_buffer, length);
#if defined(DVAP_DUMP)
				dumpPackets();
#endif
				break;
		}

		// Use the status packet every 20ms to trigger the sending of data to the DVAP
		if (space > 0U && type == RT_STATE) {
			if (writeLength == 0U && m_txData.hasData()) {
				wxMutexLocker locker(m_mutex);

				m_txData.getData(&writeLength, 1U);
				m_txData.getData(writeBuffer, writeLength);
			}

			// Only send the header when the TX is off
			if (!m_tx && writeLength == DVAP_HEADER_LEN) {
				// CUtils::dump(wxT("Write Header"), writeBuffer, writeLength);

#if defined(DVAP_DUMP)
				storePacket(writeBuffer, writeLength);
#endif
				int ret = m_serial.write(writeBuffer, writeLength);
				if (ret != int(writeLength))
					wxLogWarning(wxT("Error when writing the header to the DVAP"));

				writeLength = 0U;
				space--;
			}
			
			if (writeLength == DVAP_GMSK_DATA_LEN) {
				// CUtils::dump(wxT("Write Data"), writeBuffer, writeLength);

#if defined(DVAP_DUMP)
				storePacket(writeBuffer, writeLength);
#endif
				int ret = m_serial.write(writeBuffer, writeLength);
				if (ret != int(writeLength))
					wxLogWarning(wxT("Error when writing data to the DVAP"));

				writeLength = 0U;
				space--;
			}
		}

		Sleep(5UL);

		pollTimer.clock();
	}

	wxLogMessage(wxT("Stopping DVAP Controller thread"));

	stopDVAP();

	delete[] writeBuffer;

	m_serial.close();

	return NULL;
}

bool CDVAPController::writeHeader(const CHeaderData& header)
{
	bool ret = m_txData.hasSpace(DVAP_HEADER_LEN + 1U);
	if (!ret) {
		wxLogWarning(wxT("No space to write the header"));
		return false;
	}

	m_streamId++;

	unsigned char buffer[50U];

	::memcpy(buffer, DVAP_HEADER, DVAP_HEADER_LEN);

	wxUint16 sid = wxUINT16_SWAP_ON_BE(m_streamId);
	::memcpy(buffer + 2U, &sid, sizeof(wxUint16));

	buffer[4U] = 0x80U;
	buffer[5U] = 0U;

	::memset(buffer + 6U, ' ', RADIO_HEADER_LENGTH_BYTES);

	buffer[6U] = header.getFlag1();
	buffer[7U] = header.getFlag2();
	buffer[8U] = header.getFlag3();

	wxString rpt2 = header.getRptCall2();
	for (unsigned int i = 0U; i < rpt2.Len() && i < LONG_CALLSIGN_LENGTH; i++)
		buffer[i + 9U]  = rpt2.GetChar(i);

	wxString rpt1 = header.getRptCall1();
	for (unsigned int i = 0U; i < rpt1.Len() && i < LONG_CALLSIGN_LENGTH; i++)
		buffer[i + 17U] = rpt1.GetChar(i);

	wxString your = header.getYourCall();
	for (unsigned int i = 0U; i < your.Len() && i < LONG_CALLSIGN_LENGTH; i++)
		buffer[i + 25U] = your.GetChar(i);

	wxString my1 = header.getMyCall1();
	for (unsigned int i = 0U; i < my1.Len() && i < LONG_CALLSIGN_LENGTH; i++)
		buffer[i + 33U] = my1.GetChar(i);

	wxString my2 = header.getMyCall2();
	for (unsigned int i = 0U; i < my2.Len() && i < SHORT_CALLSIGN_LENGTH; i++)
		buffer[i + 41U] = my2.GetChar(i);

	CCCITTChecksumReverse cksum;
	cksum.update(buffer + 6U, RADIO_HEADER_LENGTH_BYTES - 2U);
	cksum.result(buffer + 45U);

	m_framePos = 0U;
	m_seq = 0U;

	wxMutexLocker locker(m_mutex);

	unsigned char len = DVAP_HEADER_LEN;
	m_txData.addData(&len, 1U);

	m_txData.addData(buffer, DVAP_HEADER_LEN);

	return true;
}

bool CDVAPController::writeData(const unsigned char* data, unsigned int length, bool end)
{
	bool ret = m_txData.hasSpace(DVAP_GMSK_DATA_LEN + 1U);
	if (!ret) {
		wxLogWarning(wxT("No space to write data"));
		return false;
	}

	unsigned char buffer[20U];

	::memcpy(buffer + 0U, DVAP_GMSK_DATA, DVAP_GMSK_DATA_LEN);

	if (::memcmp(data + VOICE_FRAME_LENGTH_BYTES, DATA_SYNC_BYTES, DATA_FRAME_LENGTH_BYTES) == 0)
		m_framePos = 0U;

	wxUint16 sid = wxUINT16_SWAP_ON_BE(m_streamId);
	::memcpy(buffer + 2U, &sid, sizeof(wxUint16));

	buffer[4U] = m_framePos;
	buffer[5U] = m_seq;

	if (end)
		buffer[4U] |= 0x40U;

	::memcpy(buffer + 6U, data, DV_FRAME_LENGTH_BYTES);

	wxMutexLocker locker(m_mutex);

	unsigned char len = DVAP_GMSK_DATA_LEN;
	m_txData.addData(&len, 1U);

	m_txData.addData(buffer, DVAP_GMSK_DATA_LEN);

	m_framePos++;
	m_seq++;

	return true;
}

unsigned int CDVAPController::getSpace()
{
	return m_txData.freeSpace() / (DVAP_GMSK_DATA_LEN + 1U);
}

bool CDVAPController::isTXReady()
{
	if (m_tx)
		return false;

	return m_txData.isEmpty();
}

bool CDVAPController::getSquelch() const
{
	return m_squelchOpen;
}

int CDVAPController::getSignal() const
{
	return m_signal;
}

void CDVAPController::writePoll()
{
#if defined(DVAP_DUMP)
	storePacket(DVAP_ACK, DVAP_ACK_LEN);
#endif
	m_serial.write(DVAP_ACK, DVAP_ACK_LEN);
}

bool CDVAPController::getName()
{
	unsigned int count = 0U;
	unsigned int length;
	RESP_TYPE resp;
	do {
		int ret = m_serial.write(DVAP_REQ_NAME, DVAP_REQ_NAME_LEN);
		if (ret != int(DVAP_REQ_NAME_LEN)) {
			m_serial.close();
			return false;
		}

		::wxMilliSleep(50UL);

		resp = getResponse(m_buffer, length);

		if (resp != RT_NAME) {
			count++;
			if (count >= MAX_RESPONSES) {
				wxLogError(wxT("The Dongle is not replying with its name"));
				return false;
			}
		}
	} while (resp != RT_NAME);

	bool cmp = ::memcmp(m_buffer, DVAP_RESP_NAME, length) == 0;
	if (!cmp) {
		wxLogError(wxT("The Dongle is not responding as a DVAP"));
		return false;
	}

	return true;
}

bool CDVAPController::getFirmware()
{
	unsigned int count = 0U;
	unsigned int length;
	RESP_TYPE resp;
	do {
		int ret = m_serial.write(DVAP_REQ_FIRMWARE, DVAP_REQ_FIRMWARE_LEN);
		if (ret != int(DVAP_REQ_FIRMWARE_LEN)) {
			m_serial.close();
			return false;
		}

		::wxMilliSleep(50UL);

		resp = getResponse(m_buffer, length);

		if (resp != RT_FIRMWARE) {
			count++;
			if (count >= MAX_RESPONSES) {
				wxLogError(wxT("The DVAP is not responding with its firmware version"));
				return false;
			}
		}
	} while (resp != RT_FIRMWARE);

	unsigned int version = m_buffer[6U] * 256U + m_buffer[5U];
	wxLogInfo(wxT("DVAP Firmware version: %u.%u"), version / 100U, version % 100U);

	return true;
}

bool CDVAPController::getSerial()
{
	unsigned int count = 0U;
	unsigned int length;
	RESP_TYPE resp;
	do {
		int ret = m_serial.write(DVAP_REQ_SERIAL, DVAP_REQ_SERIAL_LEN);
		if (ret != int(DVAP_REQ_SERIAL_LEN)) {
			m_serial.close();
			return false;
		}

		::wxMilliSleep(50UL);

		resp = getResponse(m_buffer, length);

		if (resp != RT_SERIAL) {
			count++;
			if (count >= MAX_RESPONSES) {
				wxLogError(wxT("The DVAP is not responding with its serial number"));
				return false;
			}
		}
	} while (resp != RT_SERIAL);

	wxString serial((char*)(m_buffer + 4U), wxConvLocal, length - 5U);
	wxLogInfo(wxT("DVAP Serial number: %s"), serial.c_str());

	return true;
}

bool CDVAPController::startDVAP()
{
	unsigned int count = 0U;
	unsigned int length;
	RESP_TYPE resp;
	do {
		int ret = m_serial.write(DVAP_REQ_START, DVAP_REQ_START_LEN);
		if (ret != int(DVAP_REQ_START_LEN)) {
			m_serial.close();
			return false;
		}

		::wxMilliSleep(50UL);

		resp = getResponse(m_buffer, length);

		if (resp != RT_START) {
			count++;
			if (count >= MAX_RESPONSES) {
				wxLogError(wxT("The DVAP is not responding to the start command"));
				return false;
			}
		}
	} while (resp != RT_START);

	return true;
}

bool CDVAPController::stopDVAP()
{
	unsigned int count = 0U;
	unsigned int length;
	RESP_TYPE resp;
	do {
		int ret = m_serial.write(DVAP_REQ_STOP, DVAP_REQ_STOP_LEN);
		if (ret != int(DVAP_REQ_STOP_LEN)) {
			m_serial.close();
			return false;
		}

		::wxMilliSleep(50UL);

		resp = getResponse(m_buffer, length);

		if (resp != RT_STOP) {
			count++;
			if (count >= MAX_RESPONSES) {
				wxLogError(wxT("The DVAP is not responding to the stop command"));
				return false;
			}
		}
	} while (resp != RT_STOP);

	return true;
}

bool CDVAPController::setModulation()
{
	unsigned int count = 0U;
	unsigned int length;
	RESP_TYPE resp;
	do {
		int ret = m_serial.write(DVAP_REQ_MODULATION, DVAP_REQ_MODULATION_LEN);
		if (ret != int(DVAP_REQ_MODULATION_LEN)) {
			m_serial.close();
			return false;
		}

		::wxMilliSleep(50UL);

		resp = getResponse(m_buffer, length);

		if (resp != RT_MODULATION) {
			count++;
			if (count >= MAX_RESPONSES) {
				wxLogError(wxT("The DVAP is not responding to the modulation command"));
				return false;
			}
		}
	} while (resp != RT_MODULATION);

	return true;
}

bool CDVAPController::setMode()
{
	unsigned int count = 0U;
	unsigned int length;
	RESP_TYPE resp;
	do {
		int ret = m_serial.write(DVAP_REQ_MODE, DVAP_REQ_MODE_LEN);
		if (ret != int(DVAP_REQ_MODE_LEN)) {
			m_serial.close();
			return false;
		}

		::wxMilliSleep(50UL);

		resp = getResponse(m_buffer, length);

		if (resp != RT_MODE) {
			count++;
			if (count >= MAX_RESPONSES) {
				wxLogError(wxT("The DVAP is not responding to the mode command"));
				return false;
			}
		}
	} while (resp != RT_MODE);

	return true;
}

bool CDVAPController::setSquelch()
{
	unsigned int count = 0U;
	unsigned int length;
	RESP_TYPE resp;
	do {
		unsigned char buffer[10U];
		::memcpy(buffer, DVAP_REQ_SQUELCH, DVAP_REQ_SQUELCH_LEN);
		::memcpy(buffer + 4U, &m_squelch, sizeof(wxInt8));

		int ret = m_serial.write(buffer, DVAP_REQ_SQUELCH_LEN);
		if (ret != int(DVAP_REQ_SQUELCH_LEN)) {
			m_serial.close();
			return false;
		}

		::wxMilliSleep(50UL);

		resp = getResponse(m_buffer, length);

		if (resp != RT_SQUELCH) {
			count++;
			if (count >= MAX_RESPONSES) {
				wxLogError(wxT("The DVAP is not responding to the squelch command"));
				return false;
			}
		}
	} while (resp != RT_SQUELCH);

	return true;
}

bool CDVAPController::setPower()
{
	unsigned int count = 0U;
	unsigned int length;
	RESP_TYPE resp;
	do {
		unsigned char buffer[10U];
		::memcpy(buffer, DVAP_REQ_POWER, DVAP_REQ_POWER_LEN);

		wxInt16 power = wxINT16_SWAP_ON_BE(m_power);
		::memcpy(buffer + 4U, &power, sizeof(wxInt16));

		int ret = m_serial.write(buffer, DVAP_REQ_POWER_LEN);
		if (ret != int(DVAP_REQ_POWER_LEN)) {
			m_serial.close();
			return false;
		}

		::wxMilliSleep(50UL);

		resp = getResponse(m_buffer, length);

		if (resp != RT_POWER) {
			count++;
			if (count >= MAX_RESPONSES) {
				wxLogError(wxT("The DVAP is not responding to the power command"));
				return false;
			}
		}
	} while (resp != RT_POWER);

	return true;
}

bool CDVAPController::setFrequency()
{
	unsigned int count = 0U;
	unsigned int length;
	RESP_TYPE resp;
	do {
		int ret = m_serial.write(DVAP_REQ_FREQLIMITS, DVAP_REQ_FREQLIMITS_LEN);
		if (ret != int(DVAP_REQ_FREQLIMITS_LEN)) {
			m_serial.close();
			return false;
		}

		::wxMilliSleep(50UL);

		resp = getResponse(m_buffer, length);

		if (resp != RT_FREQLIMITS) {
			count++;
			if (count >= MAX_RESPONSES) {
				wxLogError(wxT("The DVAP is not responding to the frequency limits command"));
				return false;
			}
		}
	} while (resp != RT_FREQLIMITS);

	wxUint32* pFreq1 = (wxUint32*)(m_buffer + 4U);
	wxUint32* pFreq2 = (wxUint32*)(m_buffer + 8U);

	wxUint32 lower = wxUINT32_SWAP_ON_BE(*pFreq1);
	wxUint32 upper = wxUINT32_SWAP_ON_BE(*pFreq2);

	wxLogInfo(wxT("DVAP frequency limits are %u Hz to %u Hz"), lower, upper);

	if (m_frequency < lower || m_frequency > upper) {
		wxLogError(wxT("The required frequency is out of the range of the DVAP hardware"));
		m_serial.close();
		return false;
	}

	count = 0U;
	do {
		unsigned char buffer[10U];
		::memcpy(buffer, DVAP_REQ_FREQUENCY, DVAP_REQ_FREQUENCY_LEN);

		wxUint32 frequency = wxUINT32_SWAP_ON_BE(m_frequency);
		::memcpy(buffer + 4U, &frequency, sizeof(wxUint32));

		int ret = m_serial.write(buffer, DVAP_REQ_FREQUENCY_LEN);
		if (ret != int(DVAP_REQ_FREQUENCY_LEN)) {
			m_serial.close();
			return false;
		}

		::wxMilliSleep(50UL);

		resp = getResponse(m_buffer, length);

		if (resp != RT_FREQUENCY) {
			count++;
			if (count >= MAX_RESPONSES) {
				wxLogError(wxT("The DVAP is not responding to the frequency command"));
				return false;
			}
		}
	} while (resp != RT_FREQUENCY);

	return true;
}

RESP_TYPE CDVAPController::getResponse(unsigned char *buffer, unsigned int& length)
{
	int ret = m_serial.read(buffer, DVAP_HEADER_LENGTH);
	if (ret == 0)
		return RT_TIMEOUT;
	if (ret != int(DVAP_HEADER_LENGTH))
		return RT_ERROR;

	unsigned int offset = DVAP_HEADER_LENGTH;

	// First byte has gone missing from the operation status message, fix it
	if (buffer[0U] == 0x20U && buffer[1U] == 0x90U) {
		buffer[0U] = 0x07U;
		buffer[1U] = 0x20U;
		buffer[2U] = 0x90U;
		offset = DVAP_HEADER_LENGTH + 1U;
	}

	length = buffer[0U] + (buffer[1U] & 0x1FU) * 256U;

	// Check for silliness
	if (length > 50U) {
		CUtils::dump(wxT("Bad DVAP header"), buffer, DVAP_HEADER_LENGTH);
#if defined(DVAP_DUMP)
		dumpPackets();
#endif
		resync();
		return RT_TIMEOUT;
	}

	while (offset < length) {
		int ret = m_serial.read(buffer + offset, length - offset);
		if (ret < 0)
			return RT_ERROR;
		if (ret > 0)
			offset += ret;
		if (ret == 0)
			Sleep(5UL);
	}

	// CUtils::dump(wxT("Received"), buffer, length);

	if (::memcmp(buffer, DVAP_STATUS, 4U) == 0)
		return RT_STATE;
	else if (::memcmp(buffer, DVAP_GMSK_DATA, 2U) == 0)
		return RT_GMSK_DATA;
	else if (::memcmp(buffer, DVAP_HEADER, 2U) == 0)
		return RT_HEADER;
	else if (::memcmp(buffer, DVAP_RESP_HEADER, 2U) == 0)
		return RT_HEADER_ACK;
	else if (::memcmp(buffer, DVAP_RESP_PTT, 4U) == 0)
		return RT_PTT;
	else if (::memcmp(buffer, DVAP_ACK, DVAP_ACK_LEN) == 0)
		return RT_ACK;
	else if (::memcmp(buffer, DVAP_FM_DATA, 2U) == 0)
		return RT_FM_DATA;
	else if (::memcmp(buffer, DVAP_RESP_START, DVAP_RESP_START_LEN) == 0)
		return RT_START;
	else if (::memcmp(buffer, DVAP_RESP_STOP, DVAP_RESP_STOP_LEN) == 0)
		return RT_STOP;
	else if (::memcmp(buffer, DVAP_RESP_NAME, 4U) == 0)
		return RT_NAME;
	else if (::memcmp(buffer + 1U, DVAP_RESP_SERIAL + 1U, 3U) == 0)
		return RT_SERIAL;
	else if (::memcmp(buffer, DVAP_RESP_FIRMWARE, 5U) == 0)
		return RT_FIRMWARE;
	else if (::memcmp(buffer, DVAP_RESP_FREQUENCY, 4U) == 0)
		return RT_FREQUENCY;
	else if (::memcmp(buffer, DVAP_RESP_FREQLIMITS, 4U) == 0)
		return RT_FREQLIMITS;
	else if (::memcmp(buffer, DVAP_RESP_MODULATION, DVAP_RESP_MODULATION_LEN) == 0)
		return RT_MODULATION;
	else if (::memcmp(buffer, DVAP_RESP_MODE, DVAP_RESP_MODE_LEN) == 0)
		return RT_MODE;
	else if (::memcmp(buffer, DVAP_RESP_POWER, 4U) == 0)
		return RT_POWER;
	else if (::memcmp(buffer, DVAP_RESP_SQUELCH, 4U) == 0)
		return RT_SQUELCH;
	else {
		CUtils::dump(wxT("Bad DVAP data"), buffer, length);
#if defined(DVAP_DUMP)
		dumpPackets();
#endif
		resync();
		return RT_TIMEOUT;
	}
}

void CDVAPController::resync()
{
	wxLogWarning(wxT("Resynchronising the DVAP data stream"));

	unsigned char data[DVAP_STATUS_LEN];
	::memset(data, 0x00U, DVAP_STATUS_LEN);

	while (::memcmp(data, DVAP_STATUS, 4U) != 0) {
		unsigned char c;
		int n = m_serial.read(&c, 1U);
		if (n > 0) {
			data[0U] = data[1U];
			data[1U] = data[2U];
			data[2U] = data[3U];
			data[3U] = data[4U];
			data[4U] = data[5U];
			data[5U] = data[6U];
			data[6U] = c;

			// CUtils::dump(wxT("Resync buffer"), data, DVAP_STATUS_LEN);
		}
	}

	wxLogMessage(wxT("End resynchronising"));
}

#if defined(DVAP_DUMP)

void CDVAPController::storePacket(const unsigned char* data, unsigned int length)
{
	::memcpy(m_dvapData[m_dvapIndex], data, length);
	m_dvapLength[m_dvapIndex] = length;

	m_dvapIndex++;
	if (m_dvapIndex >= DVAP_DUMP_LENGTH)
		m_dvapIndex = 0U;
}

void CDVAPController::dumpPackets()
{
	unsigned int n = m_dvapIndex;
	unsigned int i = 0U;
	while (n != m_dvapIndex) {
		if (m_dvapLength[n] > 0U) {
			wxString text;
			text.Printf(wxT("Packet: %u"), i);

			CUtils::dump(text, m_dvapData[n], m_dvapLength[n]);
		}

		i++;
		n++;
		if (n >= DVAP_DUMP_LENGTH)
			n = 0U;
	}

	m_dvapIndex = 0U;
}

#endif
