/*
 *   Copyright (C) 2011,2012,2013 by Jonathan Naylor G4KLX
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

#include "XReflectorReflectorHandler.h"
#include "XReflectorDExtraHandler.h"
#include "XReflectorDPlusHandler.h"
#include "XReflectorUserLog.h"
#include "XReflectorThread.h"
#include "XReflectorDefs.h"
#include "ConnectData.h"
#include "HeaderData.h"
#include "PollData.h"
#include "AMBEData.h"
#include "Utils.h"

#include <wx/filename.h>
#include <wx/ffile.h>

const unsigned int DPLUS_DUMMY_PORT = 65013U;

CXReflectorThread::CXReflectorThread(const wxString& name, const wxString& logDir) :
m_name(name),
m_logDir(logDir),
m_killed(false),
m_stopped(true),
m_callsign(),
m_address(),
m_dextraHandler(NULL),
m_dplusHandler(NULL),
m_dplusEnabled(false),
m_dplusPort(DPLUS_PORT),
m_logEnabled(false),
m_statusTimer(1000U, 10U)	// 10 seconds
{
	CHeaderData::initialise();
	CXReflectorReflectorHandler::initialise(MAX_REFLECTORS, name);
	CXReflectorDExtraHandler::initialise(MAX_DEXTRA_LINKS);
	CXReflectorDPlusHandler::initialise(MAX_DPLUS_LINKS);
}

CXReflectorThread::~CXReflectorThread()
{
	CHeaderData::finalise();
	CXReflectorReflectorHandler::finalise();
	CXReflectorDExtraHandler::finalise();
	CXReflectorDPlusHandler::finalise();
}

void CXReflectorThread::run()
{
	// Truncate the old Links.log file
	wxString fullName = LINKS_BASE_NAME;

	if (!m_name.IsEmpty()) {
		fullName.Append(wxT("_"));
		fullName.Append(m_name);
	}

	wxFileName fileName(m_logDir, fullName, wxT("log"));

	wxLogMessage(wxT("Truncating %s"), fileName.GetFullPath().c_str());

	wxFFile file;
	bool ret = file.Open(fileName.GetFullPath(), wxT("wt"));
	if (ret)
		file.Close();

	m_dextraHandler = new CDExtraProtocolHandler(DEXTRA_PORT, m_address);
	ret = m_dextraHandler->open();
	if (!ret) {
		wxLogError(wxT("Could not open the DExtra protocol handler"));
		delete m_dextraHandler;
		m_dextraHandler = NULL;
	}

	m_dplusHandler = new CDPlusProtocolHandler(m_dplusPort, m_address);
	ret = m_dplusHandler->open();
	if (!ret) {
		wxLogError(wxT("Could not open the DPlus protocol handler"));
		delete m_dplusHandler;
		m_dplusHandler = NULL;
	}

	// Wait here until we have the essentials to run
	while (!m_killed && (m_dextraHandler == NULL || m_dplusHandler == NULL || m_callsign.IsEmpty()))
		::wxMilliSleep(500UL);		// 1/2 sec

	if (m_killed)
		return;

	m_stopped = false;

	wxLogMessage(wxT("Starting the Reflector thread"));

	CXReflectorUserLog* userLogger = NULL;
	if (m_logEnabled) {
		m_statusTimer.start();

		userLogger = new CXReflectorUserLog(m_logDir, m_name);
		bool ret = userLogger->open();
		if (!ret) {
			delete userLogger;
			userLogger = NULL;
		}
	}

	CXReflectorDExtraHandler::setCallsign(m_callsign);
	CXReflectorDExtraHandler::setDExtraProtocolHandler(m_dextraHandler);
	CXReflectorDExtraHandler::setUserLogger(userLogger);

	CXReflectorDPlusHandler::setCallsign(m_callsign);
	CXReflectorDPlusHandler::setDPlusProtocolHandler(m_dplusHandler);
	CXReflectorDPlusHandler::setUserLogger(userLogger);

	CXReflectorReflectorHandler::setLogDirectory(m_logDir);

	wxStopWatch stopWatch;
	stopWatch.Start();

	while (!m_killed) {
		processDExtra();
		processDPlus();

		if (m_logEnabled && m_statusTimer.hasExpired()) {
			if (CXReflectorDExtraHandler::stateChange() || CXReflectorDPlusHandler::stateChange())
				writeStatus();
			m_statusTimer.reset();
		}

		::wxMilliSleep(TIME_PER_TIC_MS);

		unsigned long ms = stopWatch.Time();

		stopWatch.Start();

		CXReflectorReflectorHandler::clock(ms);
		CXReflectorDExtraHandler::clock(ms);
		CXReflectorDPlusHandler::clock(ms);

		if (m_logEnabled)
			m_statusTimer.clock(ms);
	}

	wxLogMessage(wxT("Stopping the XReflector thread"));

	// Unlink from all reflectors
	CXReflectorDExtraHandler::unlink();
	CXReflectorDPlusHandler::unlink();

	m_dextraHandler->close();
	delete m_dextraHandler;

	m_dplusHandler->close();
	delete m_dplusHandler;

	if (userLogger != NULL) {
		userLogger->close();
		delete userLogger;
	}
}

void CXReflectorThread::kill()
{
	m_killed = true;
}

void CXReflectorThread::setReflector(const wxString& callsign, const wxString& address)
{
	if (!m_stopped)
		return;

	m_callsign = callsign;
	m_address  = address;
}

void CXReflectorThread::setDPlus(bool enabled)
{
	if (enabled) {
		m_dplusPort    = DPLUS_PORT;
		m_dplusEnabled = true;
	} else {
		m_dplusPort    = DPLUS_DUMMY_PORT;
		m_dplusEnabled = false;
	}
}

void CXReflectorThread::setMiscellaneous(bool logEnabled)
{
	m_logEnabled = logEnabled;
}

void CXReflectorThread::processDExtra()
{
	for (;;) {
		DEXTRA_TYPE type = m_dextraHandler->read();

		switch (type) {
			case DE_NONE:
				return;

			case DE_POLL: {
					CPollData* poll = m_dextraHandler->readPoll();
					if (poll != NULL) {
						CXReflectorDExtraHandler::process(*poll);
						delete poll;
					}
				}
				break;

			case DE_CONNECT: {
					CConnectData* connect = m_dextraHandler->readConnect();
					if (connect != NULL) {
						CXReflectorDExtraHandler::process(*connect);
						delete connect;
					}
				}
				break;

			case DE_HEADER: {
					CHeaderData* header = m_dextraHandler->readHeader();
					if (header != NULL) {
						// wxLogMessage(wxT("DExtra header - My: %s/%s  Your: %s  Rpt1: %s  Rpt2: %s"), header->getMyCall1().c_str(), header->getMyCall2().c_str(), header->getYourCall().c_str(), header->getRptCall1().c_str(), header->getRptCall2().c_str());
						CXReflectorDExtraHandler::process(*header);
						delete header;
					}
				}
				break;

			case DE_AMBE: {
					CAMBEData* data = m_dextraHandler->readAMBE();
					if (data != NULL) {
						CXReflectorDExtraHandler::process(*data);
						delete data;
					}
				}
				break;
		}
	}
}

void CXReflectorThread::processDPlus()
{
	for (;;) {
		DPLUS_TYPE type = m_dplusHandler->read();

		switch (type) {
			case DP_NONE:
				return;

			case DP_POLL: {
					CPollData* poll = m_dplusHandler->readPoll();
					if (poll != NULL) {
						CXReflectorDPlusHandler::process(*poll);
						delete poll;
					}
				}
				break;

			case DP_CONNECT: {
					CConnectData* connect = m_dplusHandler->readConnect();
					if (connect != NULL) {
						CXReflectorDPlusHandler::process(*connect);
						delete connect;
					}
				}
				break;

			case DP_HEADER: {
					CHeaderData* header = m_dplusHandler->readHeader();
					if (header != NULL) {
						// wxLogMessage(wxT("DPlus header - My: %s/%s  Your: %s  Rpt1: %s  Rpt2: %s"), header->getMyCall1().c_str(), header->getMyCall2().c_str(), header->getYourCall().c_str(), header->getRptCall1().c_str(), header->getRptCall2().c_str());
						CXReflectorDPlusHandler::process(*header);
						delete header;
					}
				}
				break;

			case DP_AMBE: {
					CAMBEData* data = m_dplusHandler->readAMBE();
					if (data != NULL) {
						CXReflectorDPlusHandler::process(*data);
						delete data;
					}
				}
				break;
		}
	}
}

void CXReflectorThread::writeStatus()
{
	wxString fullName = LINKS_BASE_NAME;

	if (!m_name.IsEmpty()) {
		fullName.Append(wxT("_"));
		fullName.Append(m_name);
	}

	wxFileName fileName(m_logDir, fullName, wxT("log"));

	wxFFile file;
	bool ret = file.Open(fileName.GetFullPath(), wxT("wt"));
	if (!ret) {
		wxLogError(wxT("Unable to open %s for writing"), fileName.GetFullPath().c_str());
		return;
	}

	CXReflectorDExtraHandler::writeStatus(file);
	CXReflectorDPlusHandler::writeStatus(file);

	file.Close();
}
