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

#include "GMSKController.h"
#if defined(__WINDOWS__)
#include "GMSKModemWinUSB.h"
#endif
#include "GMSKModemLibUsb.h"
#include "Timer.h"

const unsigned char DVRPTR_HEADER_LENGTH = 5U;

const unsigned int BUFFER_LENGTH = 200U;

const unsigned int CYCLE_TIME = 15U;

CGMSKController::CGMSKController(USB_INTERFACE iface, unsigned int address, bool duplex) :
CModem(),
m_modem(NULL),
m_duplex(duplex),
m_buffer(NULL),
m_txData(1000U)
{
	wxASSERT(address > 0U);

	m_buffer = new unsigned char[BUFFER_LENGTH];

#if defined(__WINDOWS__)
	switch (iface) {
		case UI_LIBUSB:
			m_modem = new CGMSKModemLibUsb(address);
			break;
		case UI_WINUSB:
			m_modem = new CGMSKModemWinUSB(address);
			break;
		default:
			wxLogError(wxT("Unknown GMSM modem driver type - %d"), int(iface));
			break;
	}
#else
	m_modem = new CGMSKModemLibUsb(address);
#endif
}

CGMSKController::~CGMSKController()
{
	delete[] m_buffer;
}

bool CGMSKController::start()
{
	if (m_modem == NULL)
		return false;

	bool ret = m_modem->open();
	if (!ret) {
		delete m_modem;
		return false;
	}

	Create();
	SetPriority(100U);
	Run();

	return true;
}

void* CGMSKController::Entry()
{
	wxLogMessage(wxT("Starting GMSK Modem Controller thread"));

	CTimer hdrTimer(1000U, 0U, 100U);
	hdrTimer.start();

	CTimer dataTimer(1000U, 0U, 100U);

	bool rx = false;

	DSMT_TYPE      writeType   = DSMTT_HEADER;
	unsigned char  writeLength = 0U;
	unsigned char* writeBuffer = new unsigned char[BUFFER_LENGTH];

	unsigned char  readLength = 0U;
	unsigned char* readBuffer = new unsigned char[DV_FRAME_LENGTH_BYTES];

	wxStopWatch stopWatch;

	while (!m_stopped) {
		stopWatch.Start();

		// Only receive when not transmitting or when in duplex mode
		if (!m_tx || m_duplex) {
			if (rx) {
				unsigned char buffer[GMSK_MODEM_DATA_LENGTH];
				bool end;
				int ret = m_modem->readData(buffer, GMSK_MODEM_DATA_LENGTH, end);
				if (ret >= 0) {
					// CUtils::dump(wxT("Read Data"), buffer, ret);

					if (end) {
						wxMutexLocker locker(m_mutex);

						unsigned char data[2U];
						data[0U] = DSMTT_EOT;
						data[1U] = 0U;
						m_rxData.addData(data, 2U);

						hdrTimer.start();
						readLength = 0U;
						rx = false;
					} else {
						for (int i = 0; i < ret; i++) {
							readBuffer[readLength] = buffer[i];

							readLength++;
							if (readLength >= DV_FRAME_LENGTH_BYTES) {
								wxMutexLocker locker(m_mutex);

								unsigned char data[2U];
								data[0U] = DSMTT_DATA;
								data[1U] = DV_FRAME_LENGTH_BYTES;
								m_rxData.addData(data, 2U);

								m_rxData.addData(readBuffer, DV_FRAME_LENGTH_BYTES);
								readLength = 0U;
							}
						}
					}
				} else {
					bool ret = reopenModem();
					if (!ret)
						break;
				}
			} else {
				// Check for a header every 100ms or so
				if (hdrTimer.isRunning() && hdrTimer.hasExpired()) {
					unsigned char buffer[90U];
					bool ret = m_modem->readHeader(buffer, 90U);
					if (ret) {
						// CUtils::dump(wxT("Read Header"), buffer, RADIO_HEADER_LENGTH_BYTES);

						wxMutexLocker locker(m_mutex);

						unsigned char data[2U];
						data[0U] = DSMTT_HEADER;
						data[1U] = RADIO_HEADER_LENGTH_BYTES - 2U;
						m_rxData.addData(data, 2U);

						m_rxData.addData(buffer, RADIO_HEADER_LENGTH_BYTES - 2U);

						hdrTimer.stop();
						readLength = 0U;
						rx = true;
					} else {
						hdrTimer.start();
					}
				}
			}
		}

		// Only transmit when not receiving or when in duplex mode
		if (!rx || m_duplex) {
			if (writeLength == 0U && m_txData.hasData()) {
				wxMutexLocker locker(m_mutex);

				unsigned char type = DSMTT_NONE;
				m_txData.getData(&type, 1U);
				writeType = DSMT_TYPE(type);

				m_txData.getData(&writeLength, 1U);
				m_txData.getData(writeBuffer, writeLength);
			}

			if (writeLength > 0U) {
				if (writeType == DSMTT_HEADER) {
					TRISTATE tx = m_modem->getPTT();

					// Check that the modem isn't still transmitting before sending the new header
					if (tx == STATE_FALSE) {
						// CUtils::dump(wxT("Write Header"), writeBuffer, writeLength);
						m_modem->writeHeader(writeBuffer, writeLength);
						m_modem->setPTT(true);
						dataTimer.start();
						writeLength = 0U;
						m_tx = true;
					} else if (tx == STATE_UNKNOWN) {
						bool ret = reopenModem();
						if (!ret)
							break;
					}
				} else {
					// Don't start sending data until the header has been gone for 100ms or so
					if (!dataTimer.isRunning() || dataTimer.hasExpired()) {
						dataTimer.stop();

						TRISTATE ret = m_modem->hasSpace();

						// Check that there is space in the modem buffer
						if (ret == STATE_TRUE) {
							// CUtils::dump(wxT("Write Data"), writeBuffer, writeLength);
							int ret = m_modem->writeData(writeBuffer, writeLength);
							if (ret > 0) {
								writeLength -= ret;

								if (writeType == DSMTT_EOT) {
									m_modem->setPTT(false);
									m_tx = false;
								}
							} else if (ret < 0) {
								bool ret = reopenModem();
								if (!ret)
									break;
							}
						} else if (ret == STATE_UNKNOWN) {
							bool ret = reopenModem();
							if (!ret)
								break;
						}
					}
				}
			}
		}

		unsigned long ms = stopWatch.Time();

		// Don't sleep when reading from the modem
		if (!rx) {
			if (ms < CYCLE_TIME)
				Sleep(CYCLE_TIME - ms);

			ms = stopWatch.Time();
		}

		// Catch up with the clock
		dataTimer.clock(ms);
		hdrTimer.clock(ms);
	}

	wxLogMessage(wxT("Stopping GMSK Modem Controller thread"));

	if (m_modem != NULL) {
		m_modem->close();
		delete m_modem;
	}

	delete[] writeBuffer;
	delete[] readBuffer;

	return NULL;
}

bool CGMSKController::writeHeader(const CHeaderData& header)
{
	bool ret = m_txData.hasSpace(RADIO_HEADER_LENGTH_BYTES);
	if (!ret) {
		wxLogWarning(wxT("No space to write the header"));
		return false;
	}

	unsigned char buffer[50U];

	::memset(buffer, ' ', RADIO_HEADER_LENGTH_BYTES - 2U);

	buffer[0U] = header.getFlag1();
	buffer[1U] = header.getFlag2();
	buffer[2U] = header.getFlag3();

	wxString rpt2 = header.getRptCall2();
	for (unsigned int i = 0U; i < rpt2.Len() && i < LONG_CALLSIGN_LENGTH; i++)
		buffer[i + 3U]  = rpt2.GetChar(i);

	wxString rpt1 = header.getRptCall1();
	for (unsigned int i = 0U; i < rpt1.Len() && i < LONG_CALLSIGN_LENGTH; i++)
		buffer[i + 11U] = rpt1.GetChar(i);

	wxString your = header.getYourCall();
	for (unsigned int i = 0U; i < your.Len() && i < LONG_CALLSIGN_LENGTH; i++)
		buffer[i + 19U] = your.GetChar(i);

	wxString my1 = header.getMyCall1();
	for (unsigned int i = 0U; i < my1.Len() && i < LONG_CALLSIGN_LENGTH; i++)
		buffer[i + 27U] = my1.GetChar(i);

	wxString my2 = header.getMyCall2();
	for (unsigned int i = 0U; i < my2.Len() && i < SHORT_CALLSIGN_LENGTH; i++)
		buffer[i + 35U] = my2.GetChar(i);

	wxMutexLocker locker(m_mutex);

	unsigned char data[2U];
	data[0U] = DSMTT_HEADER;
	data[1U] = RADIO_HEADER_LENGTH_BYTES - 2U;
	m_txData.addData(data, 2U);

	m_txData.addData(buffer, RADIO_HEADER_LENGTH_BYTES - 2U);

	return true;
}

bool CGMSKController::writeData(const unsigned char* data, unsigned int length, bool end)
{
	bool ret = m_txData.hasSpace(DV_FRAME_LENGTH_BYTES + 2U);
	if (!ret) {
		wxLogWarning(wxT("No space to write data"));
		return false;
	}

	wxMutexLocker locker(m_mutex);

	unsigned char buffer[2U];
	buffer[0U] = end ? DSMTT_EOT : DSMTT_DATA;
	buffer[1U] = DV_FRAME_LENGTH_BYTES;
	m_txData.addData(buffer, 2U);

	m_txData.addData(data, DV_FRAME_LENGTH_BYTES);

	return true;
}

unsigned int CGMSKController::getSpace()
{
	return m_txData.freeSpace() / (DV_FRAME_LENGTH_BYTES + 2U);
}

bool CGMSKController::isTXReady()
{
	if (m_tx)
		return false;

	return m_txData.isEmpty();
}

bool CGMSKController::reopenModem()
{
	wxLogMessage(wxT("Connection to the GMSK modem has been lost"));

	m_modem->close();

	while (!m_stopped) {
		bool ret = m_modem->open();
		if (ret)
			return true;

		// Reset the drivers state
		m_mutex.Lock();
		m_txData.clear();
		m_mutex.Unlock();

		m_tx = false;

		Sleep(1000UL);
	}

	delete m_modem;
	m_modem = NULL;

	return false;

}
