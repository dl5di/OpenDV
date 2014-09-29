/*
 *   Copyright (C) 2009-2013 by Jonathan Naylor G4KLX
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

#include "ParrotControllerThread.h"
#include "ParrotControllerApp.h"
#include "DStarDefines.h"
#include "HeaderData.h"
#include "Utils.h"


CParrotControllerThread::CParrotControllerThread() :
m_mode(PM_PARROT),
m_state(PS_WAITING),
m_beaconTimer(1000U, 0U),
m_beaconFileName(),
m_keepFile(false),
m_killed(false),
m_stopped(true),
m_protocolHandler(NULL),
m_header(NULL),
m_dataTimer(0U),
m_dataTimeout(0U),
m_turnaroundTimer(1000U, 2U),
m_watchdogTimer(1000U, 1U),
m_fileName(),
m_reader(),
m_writer()
{
}

CParrotControllerThread::~CParrotControllerThread()
{
}

void CParrotControllerThread::run()
{
	// Wait here until we have the essentials to run
	while (!m_killed && m_protocolHandler == NULL)
		::wxMilliSleep(500UL);		// 1/2 sec

	if (m_killed)
		return;

	m_writer.setDirectory(::wxGetHomeDir());

	m_stopped = false;

	m_beaconTimer.start();

	wxLogMessage(wxT("Starting the Parrot Controller thread"));

	wxStopWatch timer;

	try {
		while (!m_killed) {
			timer.Start();

			NETWORK_TYPE type;
			bool res;

			switch (m_mode) {
				case PM_PARROT:
					switch (m_state) {
						case PS_WAITING:
							type = m_protocolHandler->read();
							switch (type) {
								case NETWORK_NONE:
									if (m_turnaroundTimer.hasExpired()) {
										m_turnaroundTimer.stop();
										transmit(m_fileName);
									}
									break;
								case NETWORK_HEADER:
									receive(type);
									break;
								default:
									break;
							}
							break;
						case PS_RECEIVING:
							type = m_protocolHandler->read();
							receive(type);
							break;
						case PS_TRANSMITTING:
							res = transmit();
							if (!res) {
								if (!m_keepFile) {
									wxLogMessage(wxT("Deleting file %s"), m_fileName.c_str());
									::wxRemoveFile(m_fileName);
									m_fileName.Clear();
								}
							}
							break;
					}
					break;

				case PM_BEACON:
					switch (m_state) {
						case PS_WAITING:
							if (m_beaconTimer.hasExpired()) {
								res = transmit(m_beaconFileName);
								if (res)
									m_beaconTimer.stop();
								else
									m_beaconTimer.start();
							}
							break;
						case PS_RECEIVING:
							break;
						case PS_TRANSMITTING:
							res = transmit();
							if (!res)
								m_beaconTimer.start();
							break;
					}
					break;
			}

			unsigned int ms = timer.Time();

			if (ms < DSTAR_FRAME_TIME_MS) {
				::wxMilliSleep(DSTAR_FRAME_TIME_MS - ms);
				ms = timer.Time();
			}

			m_turnaroundTimer.clock(ms);
			m_watchdogTimer.clock(ms);
			m_beaconTimer.clock(ms);
		}
	}
	catch (std::exception& e) {
		wxString message(e.what(), wxConvLocal);
		wxLogError(wxT("Exception raised - \"%s\""), message.c_str());
	}
	catch (...) {
		wxLogError(wxT("Unknown exception raised"));
	}

	wxLogMessage(wxT("Stopping the Parrot Controller thread"));

	m_protocolHandler->close();
	delete m_protocolHandler;
}

void CParrotControllerThread::kill()
{
	m_killed = true;
}

void CParrotControllerThread::setMode(PARROT_MODE mode, unsigned int beaconTime, const wxString& beaconFileName, unsigned int turnaroundTime, bool keepFile)
{
	if (!m_stopped)
		return;

	m_mode = mode;

	switch (m_mode) {
		case PM_PARROT:
			m_beaconTimer.setTimeout(0U);
			m_turnaroundTimer.setTimeout(turnaroundTime);
			m_beaconFileName = wxEmptyString;
			m_keepFile = keepFile;
			break;
		case PM_BEACON:
			m_beaconTimer.setTimeout(beaconTime);
			m_turnaroundTimer.setTimeout(0U);
			m_beaconFileName = beaconFileName;
			m_keepFile = true;
			break;
	}
}

void CParrotControllerThread::setNetwork(CParrotControllerProtocolHandler* handler)
{
	wxASSERT(handler != NULL);

	if (!m_stopped)
		return;

	m_protocolHandler = handler;
}

CParrotControllerStatusData* CParrotControllerThread::getStatus()
{
	if (m_header == NULL)
		return new CParrotControllerStatusData(m_state, wxEmptyString, wxEmptyString, wxEmptyString, wxEmptyString,
				wxEmptyString, 0x00, 0x00, 0x00, m_dataTimer, m_dataTimeout, m_turnaroundTimer.getTimer(),
				m_turnaroundTimer.getTimeout(), m_beaconTimer.getTimer(), m_beaconTimer.getTimeout());
	else
		return new CParrotControllerStatusData(m_state, m_header->getMyCall1(), m_header->getMyCall2(),
				m_header->getYourCall(), m_header->getRptCall1(), m_header->getRptCall2(), m_header->getFlag1(),
				m_header->getFlag2(), m_header->getFlag3(), m_dataTimer, m_dataTimeout, m_turnaroundTimer.getTimer(),
				m_turnaroundTimer.getTimeout(), m_beaconTimer.getTimer(), m_beaconTimer.getTimeout());
}

bool CParrotControllerThread::receive(NETWORK_TYPE type)
{
	unsigned char buffer[255U];
	unsigned int length;
	bool end;

	switch (type) {
		case NETWORK_HEADER:
			m_header = m_protocolHandler->readHeader();
			if (m_header == NULL)
				return false;
			wxLogMessage(wxT("Radio header received - My: %s/%s  Your: %s  Rpt1: %s  Rpt2: %s  Flags: %02X %02X %02X"), m_header->getMyCall1().c_str(), m_header->getMyCall2().c_str(), m_header->getYourCall().c_str(), m_header->getRptCall1().c_str(), m_header->getRptCall2().c_str(), m_header->getFlag1(), m_header->getFlag2(), m_header->getFlag3());

			m_writer.open(*m_header);
			m_fileName = m_writer.getFileName();
			wxLogMessage(wxT("Writing to %s"), m_fileName.c_str());
			m_watchdogTimer.start();
			m_dataTimeout = 0U;
			m_dataTimer   = 0U;
			m_state = PS_RECEIVING;
			break;

		case NETWORK_DATA:
			length = m_protocolHandler->readData(buffer, 255U, end);
			if (length == 0U)
				return false;

			m_writer.write(buffer, length);
			m_dataTimeout++;
			m_watchdogTimer.reset();

			if (end) {
				m_writer.close();
				m_watchdogTimer.stop();
				m_turnaroundTimer.start();

				delete m_header;
				m_header = NULL;

				m_dataTimeout = 0U;
				m_state = PS_WAITING;

				return false;
			}
			break;

		default:
			if (m_watchdogTimer.hasExpired()) {
				wxLogMessage(wxT("Network watchdog has expired"));

				m_writer.close();
				m_watchdogTimer.stop();
				m_turnaroundTimer.start();

				delete m_header;
				m_header = NULL;

				m_dataTimeout = 0U;
				m_state = PS_WAITING;

				return false;
			}
			break;
	}

	return true;
}

bool CParrotControllerThread::transmit(const wxString& fileName)
{
	wxLogMessage(wxT("Transmitting file %s"), fileName.c_str());

	bool res = m_reader.open(fileName);
	if (!res) {
		wxLogError(wxT("Couldn't open the file"));
		return false;
	}

	DVTFR_TYPE type = m_reader.read();
	if (type != DVTFR_HEADER) {
		wxLogError(wxT("First record is not the header"));
		m_reader.close();
		return false;
	}

	m_header = m_reader.readHeader();
	if (m_header == NULL) {
		m_reader.close();
		return false;
	}

	wxLogMessage(wxT("File header - My: %s/%s  Your: %s  Rpt1: %s  Rpt2: %s  Flags: %02X %02X %02X"), m_header->getMyCall1().c_str(), m_header->getMyCall2().c_str(), m_header->getYourCall().c_str(), m_header->getRptCall1().c_str(), m_header->getRptCall2().c_str(), m_header->getFlag1(), m_header->getFlag2(), m_header->getFlag3());

	// Swap RPT1 and RPT2
	wxString rpt1 = m_header->getRptCall1();
	wxString rpt2 = m_header->getRptCall2();
	m_header->setRptCall1(rpt2);
	m_header->setRptCall2(rpt1);

	res = m_protocolHandler->writeHeader(*m_header);
	if (!res) {
		m_reader.close();
		return false;
	}

	m_state = PS_TRANSMITTING;

	m_dataTimeout = m_reader.getRecords();
	m_dataTimer   = 0U;

	return true;
}

bool CParrotControllerThread::transmit()
{
	DVTFR_TYPE type = m_reader.read();
	if (type != DVTFR_DATA) {
		m_reader.close();
		return false;
	}

	unsigned char buffer[255U];
	bool end;
	unsigned int length = m_reader.readData(buffer, 255U, end);
	if (length == 0U) {
		m_reader.close();
		return false;
	}

	bool res = m_protocolHandler->writeData(buffer, length, end);
	if (!res) {
		m_reader.close();
		return false;
	}

	if (end) {
		m_dataTimer = 0U;
		m_dataTimeout = 0U;
		m_reader.close();

		m_state = PS_WAITING;
		delete m_header;
		m_header = NULL;

		return false;
	} else {
		m_dataTimer++;
		return true;
	}
}

