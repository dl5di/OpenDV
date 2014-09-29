/*
 *   Copyright (C) 2010-2013 by Jonathan Naylor G4KLX
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

#include "DExtraGatewayHeaderData.h"
#include "DExtraGatewayAMBEData.h"
#include "DExtraGatewayHostFile.h"
#include "DExtraGatewayThread.h"
#include "DExtraGatewayApp.h"
#include "DStarDefines.h"
#include "Utils.h"

#include <wx/filename.h>

const unsigned int TIME_FRACTION = 2U;

const unsigned int CYCLE_TIME = DSTAR_FRAME_TIME_MS / TIME_FRACTION;

CDExtraGatewayThread::CDExtraGatewayThread() :
wxThread(wxTHREAD_JOINABLE),
m_killed(false),
m_stopped(true),
m_reflectors(),
m_hostnames(),
m_repeaterHandler(NULL),
m_dextraHandler(),
m_audioUnit(NULL),
m_echoUnit(NULL),
m_myCall(),
m_yourCall(),
m_rptCall1(),
m_rptCall2(),
m_repeaterCall(),
m_state(LINK_NONE),
m_reflector(),
m_startupReflector(),
m_atStartup(false),
m_reconnect(RECONNECT_NEVER),
m_language(TL_ENGLISH_UK),
m_reconnectTimer(1000U, 0U),
m_watchdogTimer(1000U, 2U),
m_id(0U),
m_echo(false),
m_info(false)
{
}

CDExtraGatewayThread::~CDExtraGatewayThread()
{
}

void* CDExtraGatewayThread::Entry()
{
	// Wait here until we have the essentials to run
	while (!m_killed && m_repeaterHandler == NULL)
		Sleep(500UL);		// 1/2 sec

	if (m_killed)
		return NULL;

	m_stopped = false;

	wxLogMessage(wxT("Starting the DExtra Gateway thread"));

	wxFileName fileName(wxFileName::GetHomeDir(), HOSTS_FILE_NAME);

	CDExtraGatewayAudioUnit::initialise();
	CDExtraGatewayAudioUnit::setLanguage(m_language);

	m_audioUnit = new CDExtraGatewayAudioUnit(this, m_repeaterCall);

	m_echoUnit = new CDExtraGatewayEchoUnit(this, m_repeaterCall);

	if (!fileName.IsFileReadable()) {
		wxLogMessage(wxT("File %s not readable"), fileName.GetFullPath().c_str());
#if defined(__WINDOWS__)
		fileName.Assign(::wxGetCwd(), HOSTS_FILE_NAME);
#else
		fileName.Assign(wxT(DATA_DIR), HOSTS_FILE_NAME);
#endif
		if (!fileName.IsFileReadable())
			wxLogMessage(wxT("File %s not readable"), fileName.GetFullPath().c_str());
	}

	CDExtraGatewayHostFile hostFile(fileName.GetFullPath());
	m_reflectors = hostFile.getNames();
	m_hostnames  = hostFile.getHosts();

	m_reconnectTimer.start();

	if (m_atStartup && !m_startupReflector.IsEmpty()) {
		writeLinkingTo(m_startupReflector);

		wxLogMessage(wxT("Linking at startup to %s"), m_startupReflector.c_str());
		link(m_startupReflector);
	}

	wxStopWatch timer;

	try {
		while (!m_killed) {
			timer.Start();

			for (unsigned int i = 0U; i < 10U; i++) {
				CDExtraGatewayHeaderData* header;
				CDExtraGatewayAMBEData* data;

				bool rptData = true;
				bool deData  = true;

				REPEATER_TYPE rptType = m_repeaterHandler->read();
				switch (rptType) {
					case RT_NONE:
						rptData = false;
						break;
					case RT_HEADER:
						header = m_repeaterHandler->readHeader();
						if (header != NULL) {
							wxLogMessage(wxT("Repeater header - My: %s/%s  Your: %s  Rpt1: %s  Rpt2: %s  Flags: %02X %02X %02X"), header->getMyCall1().c_str(), header->getMyCall2().c_str(), header->getYourCall().c_str(), header->getRptCall1().c_str(), header->getRptCall2().c_str(), header->getFlag1(), header->getFlag2(), header->getFlag3());
							showHeader(*header);
							commandHandler(header->getYourCall());
							if (m_state == LINK_LINKED && !m_echo && !m_info) {
								header->setReflector(m_reflector);
								m_dextraHandler.writeHeader(*header);
							} else if (m_echo) {
								m_echoUnit->writeHeader(*header);
							}
							delete header;
						}
						break;
					case RT_DATA:
						data = m_repeaterHandler->readData();
						if (data != NULL) {
							if (m_state == LINK_LINKED && !m_echo && !m_info)
								m_dextraHandler.writeData(*data);
							else if (m_echo)
								m_echoUnit->writeData(*data);

							bool end = data->isEnd();
							if (end) {
								if (m_info)
									m_audioUnit->sendState(m_state, m_reflector);

								clearHeader();
								m_echo = false;
								m_info = false;
							}
							delete data;
						}
						break;
				}

				DEXTRA_TYPE deType = m_dextraHandler.read();
				switch (deType) {
					case DT_NONE:
						deData = false;
						break;
					case DT_HEADER:
						header = m_dextraHandler.readHeader();
						if (header != NULL) {
							unsigned int id = header->getId();
							if (header->getRptCall2().IsSameAs(m_reflector) && m_state == LINK_LINKED && m_id == 0x00U) {
								wxLogMessage(wxT("DExtra header - My: %s/%s  Your: %s  Rpt1: %s  Rpt2: %s"), header->getMyCall1().c_str(), header->getMyCall2().c_str(), header->getYourCall().c_str(), header->getRptCall1().c_str(), header->getRptCall2().c_str());
								m_id = id;
								showHeader(*header);
								writeHeader(*header);
								m_watchdogTimer.start();
							}
							delete header;
						}
						break;
					case DT_DATA:
						data = m_dextraHandler.readData();
						if (data != NULL) {
							unsigned int id = data->getId();
							if (m_state == LINK_LINKED && id == m_id) {
								writeData(*data);
								m_watchdogTimer.reset();

								bool end = data->isEnd();
								if (end) {
									m_watchdogTimer.stop();
									clearHeader();
									m_id = 0x00U;
								}
							}
							delete data;
						}
						break;
				}

				// Any activity resets the reconnect timer
				if (rptData || deData)
					m_reconnectTimer.start();

				// No more work to be done?
				if (!rptData && !deData)
					break;
			}

			if (m_watchdogTimer.isRunning() && m_watchdogTimer.hasExpired()) {
				wxLogMessage(wxT("Network watchdog timer has expired"));
				clearHeader();
				m_watchdogTimer.stop();
				m_id = 0x00U;
			}

			if (m_reconnectTimer.isRunning() && m_reconnectTimer.hasExpired()) {
				if (!m_startupReflector.IsEmpty() && (m_state == LINK_NONE || !m_reflector.IsSameAs(m_startupReflector))) {
					wxLogMessage(wxT("Reconnecting to %s"), m_startupReflector.c_str());
					unlink();
					link(m_startupReflector);

					writeLinkingTo(m_startupReflector);
				}
			}

			if (m_state != LINK_NONE) {
				bool connected = m_dextraHandler.isConnected();

				if (m_state == LINK_LINKING && connected) {
					writeLinkedTo(m_reflector);

					wxLogMessage(wxT("Linked to %s"), m_reflector.c_str());
					m_state = LINK_LINKED;
				} else if (m_state == LINK_LINKED && !connected) {
					writeLinkingTo(m_reflector);

					wxLogMessage(wxT("Link lost with %s, relinking"), m_reflector.c_str());
					m_state = LINK_LINKING;
				}
			}

			unsigned int ms = timer.Time();

			if (ms < CYCLE_TIME) {
				Sleep(CYCLE_TIME - ms);
				ms = timer.Time();
			}

			m_dextraHandler.clock(ms);
			m_reconnectTimer.clock(ms);
			m_watchdogTimer.clock(ms);

			m_audioUnit->clock(ms);
			m_echoUnit->clock(ms);
		}
	}
	catch (std::exception& e) {
		wxString message(e.what(), wxConvLocal);
		wxLogError(wxT("Exception raised - \"%s\""), message.c_str());
	}
	catch (...) {
		wxLogError(wxT("Unknown exception raised"));
	}

	wxLogMessage(wxT("Stopping the DExtra Gateway thread"));

	m_dextraHandler.unlink();

	m_repeaterHandler->close();
	delete m_repeaterHandler;

	delete m_audioUnit;
	delete m_echoUnit;

	CDExtraGatewayAudioUnit::finalise();

	return NULL;
}

void CDExtraGatewayThread::kill()
{
	m_killed = true;
}

void CDExtraGatewayThread::setReflector(const wxString& callsign, const wxString& reflector, bool atStartup, RECONNECT reconnect, TEXT_LANG language)
{
	m_dextraHandler.setCallsign(callsign);

	m_startupReflector = reflector;
	m_atStartup        = atStartup;
	m_reconnect        = reconnect;
	m_language         = language;

	switch (reconnect) {
		case RECONNECT_5MINS:
			m_reconnectTimer.setTimeout(5U * 60U);
			break;
		case RECONNECT_10MINS:
			m_reconnectTimer.setTimeout(10U * 60U);
			break;
		case RECONNECT_15MINS:
			m_reconnectTimer.setTimeout(15U * 60U);
			break;
		case RECONNECT_20MINS:
			m_reconnectTimer.setTimeout(20U * 60U);
			break;
		case RECONNECT_25MINS:
			m_reconnectTimer.setTimeout(25U * 60U);
			break;
		case RECONNECT_30MINS:
			m_reconnectTimer.setTimeout(30U * 60U);
			break;
		default:
			break;
	}
}

void CDExtraGatewayThread::setRepeater(const wxString& repeaterCallsign, CDExtraGatewayRepeaterProtocolHandler* repeaterHandler)
{
	wxASSERT(!repeaterCallsign.IsEmpty());
	wxASSERT(repeaterHandler != NULL);

	if (!m_stopped)
		return;

	m_repeaterHandler = repeaterHandler;
	m_repeaterCall    = repeaterCallsign;
}

CDExtraGatewayStatusData* CDExtraGatewayThread::getStatus()
{
	return new CDExtraGatewayStatusData(m_myCall, m_yourCall, m_rptCall1, m_rptCall2, m_state,
		m_reflector, m_reconnectTimer.getTimer(), m_reconnectTimer.getTimeout());
}

void CDExtraGatewayThread::commandHandler(const wxString& yourCall)
{
	m_echo = false;
	m_info = false;

	if (yourCall.IsSameAs(wxT("       E"))) {
		m_echo = true;
		return;
	}

	if (yourCall.IsSameAs(wxT("       I"))) {
		m_info = true;
		return;
	}

	if (m_reconnect == RECONNECT_FIXED)
		return;

	wxString letter = yourCall.Right(1);

	if (letter.IsSameAs(wxT("U"))) {
		writeNotLinked();

		wxLogMessage(wxT("Unlink command issued by %s"), m_myCall.c_str());
		unlink();
	} else if (letter.IsSameAs(wxT("L"))) {
		// Extract the callsign "1234567L" -> "123456 7"
		wxString reflector = yourCall.Left(LONG_CALLSIGN_LENGTH - 2U);
		reflector.Append(wxT(" "));
		reflector.Append(yourCall.Mid(LONG_CALLSIGN_LENGTH - 2U, 1));

		// Check for a duplicate link command
		if (m_reflector.IsSameAs(reflector))
			return;

		// Check for only a change of channel
		if (m_reflector.Left(LONG_CALLSIGN_LENGTH - 1U).IsSameAs(reflector.Left(LONG_CALLSIGN_LENGTH - 1U))) {
			writeLinkedTo(reflector);

			wxLogMessage(wxT("Link command to %s issued by %s"), reflector.c_str(), m_myCall.c_str());
			m_reflector = reflector;
			return;
		}

		// Validate the reflector callsign
		wxString baseName = reflector.Left(LONG_CALLSIGN_LENGTH - 1U).Trim();
		int index = m_reflectors.Index(baseName);
		if (index == wxNOT_FOUND) {
			wxLogMessage(wxT("Invalid link command to %s issued by %s"), reflector.c_str(), m_myCall.c_str());
			return;
		}

		wxLogMessage(wxT("Link command to %s issued by %s"), reflector.c_str(), m_myCall.c_str());
		unlink();
		link(reflector);

		writeLinkingTo(reflector);
	}
}

void CDExtraGatewayThread::unlink()
{
	// Anything to do?
	if (m_state == LINK_NONE)
		return;

	m_dextraHandler.unlink();

	m_state = LINK_NONE;
}

bool CDExtraGatewayThread::link(const wxString &reflector)
{
	if (m_state != LINK_NONE)
		unlink();

	wxString baseName = reflector.Left(LONG_CALLSIGN_LENGTH - 1U).Trim();

	int index = m_reflectors.Index(baseName);
	if (index == wxNOT_FOUND) {
		wxLogError(wxT("Cannot find %s in the reflector list"), baseName.c_str());
		return false;
	}

	wxString hostname = m_hostnames.Item(index);

	bool res = m_dextraHandler.link(hostname);
	if (res) {
		m_reflector = reflector;
		m_state     = LINK_LINKING;
	}

	return res;
}

bool CDExtraGatewayThread::writeHeader(CDExtraGatewayHeaderData &header)
{
	header.setReflector(m_repeaterCall);

	return m_repeaterHandler->writeHeader(header);
}

bool CDExtraGatewayThread::writeData(CDExtraGatewayAMBEData &data)
{
	return m_repeaterHandler->writeData(data);
}

void CDExtraGatewayThread::showHeader(const CDExtraGatewayHeaderData &header)
{
	m_myCall   = header.getMyCall1();
	if (!header.getMyCall2().IsSameAs(wxT("    "))) {
		m_myCall.Append(wxT("/"));
		m_myCall.Append(header.getMyCall2());
	}

	m_yourCall = header.getYourCall();

	m_rptCall1 = header.getRptCall1();

	m_rptCall2 = header.getRptCall2();
}

void CDExtraGatewayThread::clearHeader()
{
	m_myCall.Clear();
	m_yourCall.Clear();
	m_rptCall1.Clear();
	m_rptCall2.Clear();
}

void CDExtraGatewayThread::writeLinkingTo(const wxString& callsign)
{
	wxString text;

	switch (m_language) {
		case TL_DEUTSCH:
			text.Printf(wxT("Verbinde mit %s"), callsign.c_str());
			break;
		case TL_DANSK:
			text.Printf(wxT("Linker til %s"), callsign.c_str());
			break;
		case TL_FRANCAIS:
			text.Printf(wxT("Connection a %s"), callsign.c_str());
			break;
		case TL_ITALIANO:
			text.Printf(wxT("In conn con %s"), callsign.c_str());
			break;
		case TL_POLSKI:
			text.Printf(wxT("Linkuje do %s"), callsign.c_str());
			break;
		case TL_ESPANOL:
			text.Printf(wxT("Enlazando %s"), callsign.c_str());
			break;
		case TL_SVENSKA:
			text.Printf(wxT("Lankad till %s"), callsign.c_str());
			break;
		case TL_NEDERLANDS:
			text.Printf(wxT("Linken naar %s"), callsign.c_str());
			break;
		case TL_NORSK:
			text.Printf(wxT("Kobler til %s"), callsign.c_str());
			break;
		default:
			text.Printf(wxT("Linking to %s"), callsign.c_str());
			break;
	}

	m_repeaterHandler->writeText(text, LS_LINKING_DEXTRA, callsign);
	m_audioUnit->setText(text);
}

void CDExtraGatewayThread::writeLinkedTo(const wxString& callsign)
{
	wxString text;

	switch (m_language) {
		case TL_DEUTSCH:
			text.Printf(wxT("Verlinkt zu %s"), callsign.c_str());
			break;
		case TL_DANSK:
			text.Printf(wxT("Linket til %s"), callsign.c_str());
			break;
		case TL_FRANCAIS:
			text.Printf(wxT("Connecte a %s"), callsign.c_str());
			break;
		case TL_ITALIANO:
			text.Printf(wxT("Connesso a %s"), callsign.c_str());
			break;
		case TL_POLSKI:
			text.Printf(wxT("Polaczony z %s"), callsign.c_str());
			break;
		case TL_ESPANOL:
			text.Printf(wxT("Enlazado %s"), callsign.c_str());
			break;
		case TL_SVENSKA:
			text.Printf(wxT("Lankar till %s"), callsign.c_str());
			break;
		case TL_NEDERLANDS:
			text.Printf(wxT("Gelinkt met %s"), callsign.c_str());
			break;
		case TL_NORSK:
			text.Printf(wxT("Tilkoblet %s"), callsign.c_str());
			break;
		default:
			text.Printf(wxT("Linked to %s"), callsign.c_str());
			break;
	}

	m_repeaterHandler->writeText(text, LS_LINKED_DEXTRA, callsign);
	m_audioUnit->setText(text);
}

void CDExtraGatewayThread::writeNotLinked()
{
	wxString text;

	switch (m_language) {
		case TL_DEUTSCH:
			text = wxT("Nicht verbunden");
			break;
		case TL_DANSK:
			text = wxT("Ikke forbundet");
			break;
		case TL_FRANCAIS:
			text = wxT("Non connecte");
			break;
		case TL_ITALIANO:
			text = wxT("Non connesso");
			break;
		case TL_POLSKI:
			text = wxT("Nie polaczony");
			break;
		case TL_ESPANOL:
			text = wxT("No enlazado");
			break;
		case TL_SVENSKA:
			text = wxT("Ej lankad");
			break;
		case TL_NEDERLANDS:
			text = wxT("Niet gelinkt");
			break;
		case TL_NORSK:
			text = wxT("Ikke linket");
			break;
		default:
			text = wxT("Not linked");
			break;
	}

	m_repeaterHandler->writeText(text, LS_NONE, wxEmptyString);
	m_audioUnit->setText(text);
}
