/*
 *   Copyright (C) 2010-2014 by Jonathan Naylor G4KLX
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

#include "RepeaterHandler.h"
#include "DExtraHandler.h"
#include "DPlusHandler.h"
#include "DStarDefines.h"
#include "DCSHandler.h"
#include "CCSHandler.h"
#include "HeaderData.h"
#include "DDHandler.h"
#include "AMBEData.h"
#include "Utils.h"

const unsigned int  ETHERNET_ADDRESS_LENGTH = 6U;

const unsigned char ETHERNET_BROADCAST_ADDRESS[] = {0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU};
// Multicast address '01:00:5E:00:00:01' - IP: '224.0.0.1' (to all)
const unsigned char TOALL_MULTICAST_ADDRESS[] = {0x01U, 0x00U, 0x5EU, 0x00U, 0x00U, 0x01U};
// Multicast address '01:00:5E:00:00:23' - IP: '224.0.0.35' (DX-Cluster)
const unsigned char DX_MULTICAST_ADDRESS[] = {0x01U, 0x00U, 0x5EU, 0x00U, 0x00U, 0x23U};

unsigned int              CRepeaterHandler::m_maxRepeaters = 0U;
CRepeaterHandler**        CRepeaterHandler::m_repeaters = NULL;

wxString                  CRepeaterHandler::m_localAddress;
CG2ProtocolHandler*       CRepeaterHandler::m_g2Handler = NULL;
CIRCDDB*                  CRepeaterHandler::m_irc = NULL;
CCacheManager*            CRepeaterHandler::m_cache = NULL;
wxString                  CRepeaterHandler::m_gateway;
TEXT_LANG                 CRepeaterHandler::m_language = TL_ENGLISH_UK;
bool                      CRepeaterHandler::m_dextraEnabled = true;
bool                      CRepeaterHandler::m_dplusEnabled = false;
bool                      CRepeaterHandler::m_dcsEnabled = true;
bool                      CRepeaterHandler::m_infoEnabled = true;
bool                      CRepeaterHandler::m_echoEnabled = true;
bool                      CRepeaterHandler::m_dtmfEnabled = true;

CHeaderLogger*            CRepeaterHandler::m_headerLogger = NULL;

CAPRSWriter*              CRepeaterHandler::m_aprsWriter  = NULL;

CCallsignList*            CRepeaterHandler::m_restrictList = NULL;

CRepeaterHandler::CRepeaterHandler(const wxString& callsign, const wxString& band, const wxString& address, unsigned int port, HW_TYPE hwType, const wxString& reflector, bool atStartup, RECONNECT reconnect, bool dratsEnabled, double frequency, double offset, double range, double latitude, double longitude, double agl, const wxString& description1, const wxString& description2, const wxString& url, IRepeaterProtocolHandler* handler, unsigned char band1, unsigned char band2, unsigned char band3) :
m_index(0x00U),
m_rptCallsign(),
m_gwyCallsign(),
m_band(' '),
m_address(),
m_port(port),
m_hwType(hwType),
m_repeaterHandler(handler),
m_frequency(frequency),
m_offset(offset),
m_range(range),
m_latitude(latitude),
m_longitude(longitude),
m_agl(agl),
m_description1(description1),
m_description2(description2),
m_url(url),
m_band1(band1),
m_band2(band2),
m_band3(band3),
m_repeaterId(0x00U),
m_busyId(0x00U),
m_watchdogTimer(1000U, REPEATER_TIMEOUT),
m_ddMode(false),
m_ddCallsign(),
m_queryTimer(1000U, 5U),		// 5 seconds
m_myCall1(),
m_myCall2(),
m_yourCall(),
m_rptCall1(),
m_rptCall2(),
m_flag1(0x00U),
m_flag2(0x00U),
m_flag3(0x00U),
m_restricted(false),
m_frames(0U),
m_silence(0U),
m_errors(0U),
m_textCollector(),
m_text(),
m_xBandRptr(NULL),
m_starNet(NULL),
m_g2Status(G2_NONE),
m_g2User(),
m_g2Repeater(),
m_g2Gateway(),
m_g2Header(NULL),
m_g2Address(),
m_linkStatus(LS_NONE),
m_linkRepeater(),
m_linkGateway(),
m_linkReconnect(reconnect),
m_linkAtStartup(atStartup),
m_linkStartup(reflector),
m_linkReconnectTimer(1000U),
m_linkRelink(false),
m_echo(NULL),
m_audio(NULL),
m_infoNeeded(false),
m_version(NULL),
m_drats(NULL),
m_dtmf(),
m_pollTimer(1000U, 900U),			// 15 minutes
m_ccsHandler(NULL),
m_lastReflector(),
m_heardUser(),
m_heardRepeater(),
m_heardTimer(1000U, 0U, 100U)		// 100ms
{
	wxASSERT(!callsign.IsEmpty());
	wxASSERT(port > 0U);
	wxASSERT(handler != NULL);

	m_ddMode = band.Len() > 1U;

	m_band = band.GetChar(0U);

	m_rptCallsign = callsign;
	m_rptCallsign.Append(wxT("        "));
	m_rptCallsign.Truncate(LONG_CALLSIGN_LENGTH - 1U);
	m_rptCallsign.Append(band);
	m_rptCallsign.Truncate(LONG_CALLSIGN_LENGTH);

	m_gwyCallsign = callsign;
	m_gwyCallsign.Append(wxT("        "));
	m_gwyCallsign.Truncate(LONG_CALLSIGN_LENGTH - 1U);
	m_gwyCallsign.Append(wxT("G"));

	m_address.s_addr = ::inet_addr(address.mb_str());

	m_pollTimer.start();

	switch (m_linkReconnect) {
		case RECONNECT_5MINS:
			m_linkReconnectTimer.start(5U * 60U);
			break;
		case RECONNECT_10MINS:
			m_linkReconnectTimer.start(10U * 60U);
			break;
		case RECONNECT_15MINS:
			m_linkReconnectTimer.start(15U * 60U);
			break;
		case RECONNECT_20MINS:
			m_linkReconnectTimer.start(20U * 60U);
			break;
		case RECONNECT_25MINS:
			m_linkReconnectTimer.start(25U * 60U);
			break;
		case RECONNECT_30MINS:
			m_linkReconnectTimer.start(30U * 60U);
			break;
		case RECONNECT_60MINS:
			m_linkReconnectTimer.start(60U * 60U);
			break;
		case RECONNECT_90MINS:
			m_linkReconnectTimer.start(90U * 60U);
			break;
		case RECONNECT_120MINS:
			m_linkReconnectTimer.start(120U * 60U);
			break;
		case RECONNECT_180MINS:
			m_linkReconnectTimer.start(180U * 60U);
			break;
		default:
			break;
	}

	m_echo    = new CEchoUnit(this, callsign);
	m_audio   = new CAudioUnit(this, callsign);
	m_version = new CVersionUnit(this, callsign);

	if (dratsEnabled) {
		m_drats = new CDRATSServer(m_localAddress, port, callsign, this);
		bool ret = m_drats->open();
		if (!ret) {
			delete m_drats;
			m_drats = NULL;
		}
	}
}

CRepeaterHandler::~CRepeaterHandler()
{
	delete m_echo;
	delete m_audio;
	delete m_version;

	if (m_drats != NULL)
		m_drats->close();
}

void CRepeaterHandler::initialise(unsigned int maxRepeaters)
{
	wxASSERT(maxRepeaters > 0U);

	m_maxRepeaters = maxRepeaters;

	m_repeaters = new CRepeaterHandler*[m_maxRepeaters];
	for (unsigned int i = 0U; i < m_maxRepeaters; i++)
		m_repeaters[i] = NULL;
}

void CRepeaterHandler::setIndex(unsigned int index)
{
	m_index = index;
}

void CRepeaterHandler::add(const wxString& callsign, const wxString& band, const wxString& address, unsigned int port, HW_TYPE hwType, const wxString& reflector, bool atStartup, RECONNECT reconnect, bool dratsEnabled, double frequency, double offset, double range, double latitude, double longitude, double agl, const wxString& description1, const wxString& description2, const wxString& url, IRepeaterProtocolHandler* handler, unsigned char band1, unsigned char band2, unsigned char band3)
{
	wxASSERT(!callsign.IsEmpty());
	wxASSERT(port > 0U);
	wxASSERT(handler != NULL);

	CRepeaterHandler* repeater = new CRepeaterHandler(callsign, band, address, port, hwType, reflector, atStartup, reconnect, dratsEnabled, frequency, offset, range, latitude, longitude, agl, description1, description2, url, handler, band1, band2, band3);

	for (unsigned int i = 0U; i < m_maxRepeaters; i++) {
		if (m_repeaters[i] == NULL) {
			repeater->setIndex(i);
			m_repeaters[i] = repeater;
			return;
		}
	}

	wxLogError(wxT("Cannot add repeater with callsign %s, no space"), callsign.c_str());

	delete repeater;
}

void CRepeaterHandler::setG2Handler(CG2ProtocolHandler* handler)
{
	wxASSERT(handler != NULL);

	m_g2Handler = handler;
}

void CRepeaterHandler::setCache(CCacheManager* cache)
{
	wxASSERT(cache != NULL);

	m_cache = cache;
}

void CRepeaterHandler::setIRC(CIRCDDB* irc)
{
	wxASSERT(irc != NULL);

	m_irc = irc;
}

void CRepeaterHandler::setGateway(const wxString& gateway)
{
	m_gateway = gateway;
}

void CRepeaterHandler::setLanguage(TEXT_LANG language)
{
	m_language = language;
}

void CRepeaterHandler::setDExtraEnabled(bool enabled)
{
	m_dextraEnabled = enabled;
}

void CRepeaterHandler::setDPlusEnabled(bool enabled)
{
	m_dplusEnabled = enabled;
}

void CRepeaterHandler::setDCSEnabled(bool enabled)
{
	m_dcsEnabled = enabled;
}

void CRepeaterHandler::setInfoEnabled(bool enabled)
{
	m_infoEnabled = enabled;
}

void CRepeaterHandler::setEchoEnabled(bool enabled)
{
	m_echoEnabled = enabled;
}

void CRepeaterHandler::setDTMFEnabled(bool enabled)
{
	m_dtmfEnabled = enabled;
}

void CRepeaterHandler::setHeaderLogger(CHeaderLogger* logger)
{
	m_headerLogger = logger;
}

void CRepeaterHandler::setAPRSWriter(CAPRSWriter* writer)
{
	m_aprsWriter = writer;
}

void CRepeaterHandler::setLocalAddress(const wxString& address)
{
	m_localAddress = address;
}

void CRepeaterHandler::setRestrictList(CCallsignList* list)
{
	wxASSERT(list != NULL);

	m_restrictList = list;
}

bool CRepeaterHandler::getRepeater(unsigned int n, wxString& callsign, LINK_STATUS& linkStatus, wxString& linkCallsign)
{
	if (n >= m_maxRepeaters)
		return false;

	if (m_repeaters[n] == NULL)
		return false;

	callsign     = m_repeaters[n]->m_rptCallsign;
	linkStatus   = m_repeaters[n]->m_linkStatus;
	linkCallsign = m_repeaters[n]->m_linkRepeater;

	return true;
}

void CRepeaterHandler::resolveUser(const wxString &user, const wxString& repeater, const wxString& gateway, const wxString &address)
{
	for (unsigned int i = 0U; i < m_maxRepeaters; i++) {
		if (m_repeaters[i] != NULL)
			m_repeaters[i]->resolveUserInt(user, repeater, gateway, address);
	}
}

void CRepeaterHandler::resolveRepeater(const wxString& repeater, const wxString& gateway, const wxString &address, DSTAR_PROTOCOL protocol)
{
	for (unsigned int i = 0U; i < m_maxRepeaters; i++) {
		if (m_repeaters[i] != NULL)
			m_repeaters[i]->resolveRepeaterInt(repeater, gateway, address, protocol);
	}
}

void CRepeaterHandler::startup()
{
	for (unsigned int i = 0U; i < m_maxRepeaters; i++) {
		if (m_repeaters[i] != NULL)
			m_repeaters[i]->startupInt();
	}
}

void CRepeaterHandler::clock(unsigned int ms)
{
	for (unsigned int i = 0U; i < m_maxRepeaters; i++) {
		if (m_repeaters[i] != NULL)
			m_repeaters[i]->clockInt(ms);
	}
}

void CRepeaterHandler::finalise()
{
	for (unsigned int i = 0U; i < m_maxRepeaters; i++) {
		delete m_repeaters[i];
		m_repeaters[i] = NULL;
	}

	if (m_aprsWriter != NULL) {
		m_aprsWriter->close();
		delete m_aprsWriter;
	}

	delete[] m_repeaters;
}

CRepeaterHandler* CRepeaterHandler::findDVRepeater(const CHeaderData& header)
{
	wxString rpt1 = header.getRptCall1();
	in_addr address = header.getYourAddress();

	for (unsigned int i = 0U; i < m_maxRepeaters; i++) {
		CRepeaterHandler* repeater = m_repeaters[i];
		if (repeater != NULL) {
			if (!repeater->m_ddMode && repeater->m_address.s_addr == address.s_addr && repeater->m_rptCallsign.IsSameAs(rpt1))
				return repeater;
		}
	}

	return NULL;
}

CRepeaterHandler* CRepeaterHandler::findDVRepeater(const CAMBEData& data, bool busy)
{
	unsigned int id = data.getId();

	for (unsigned int i = 0U; i < m_maxRepeaters; i++) {
		CRepeaterHandler* repeater = m_repeaters[i];
		if (repeater != NULL) {
			if (!busy && !repeater->m_ddMode && repeater->m_repeaterId == id)
				return repeater;
			if (busy && !repeater->m_ddMode && repeater->m_busyId == id)
				return repeater;
		}
	}

	return NULL;
}

CRepeaterHandler* CRepeaterHandler::findDVRepeater(const wxString& callsign)
{
	for (unsigned int i = 0U; i < m_maxRepeaters; i++) {
		CRepeaterHandler* repeater = m_repeaters[i];
		if (repeater != NULL) {
			if (!repeater->m_ddMode && repeater->m_rptCallsign.IsSameAs(callsign))
				return repeater;
		}
	}

	return NULL;
}

CRepeaterHandler* CRepeaterHandler::findRepeater(const CPollData& data)
{
	in_addr   address = data.getYourAddress();
	unsigned int port = data.getYourPort();

	for (unsigned int i = 0U; i < m_maxRepeaters; i++) {
		CRepeaterHandler* repeater = m_repeaters[i];
		if (repeater != NULL) {
			if (repeater->m_address.s_addr == address.s_addr && repeater->m_port == port)
				return repeater;
		}
	}

	return NULL;
}

CRepeaterHandler* CRepeaterHandler::findDDRepeater(const CDDData& data)
{
	wxString rpt1 = data.getRptCall1();

	for (unsigned int i = 0U; i < m_maxRepeaters; i++) {
		CRepeaterHandler* repeater = m_repeaters[i];
		if (repeater != NULL) {
			if (repeater->m_ddMode && repeater->m_rptCallsign.IsSameAs(rpt1))
				return repeater;
		}
	}

	return NULL;
}

CRepeaterHandler* CRepeaterHandler::findDDRepeater()
{
	for (unsigned int i = 0U; i < m_maxRepeaters; i++) {
		CRepeaterHandler* repeater = m_repeaters[i];
		if (repeater != NULL) {
			if (repeater->m_ddMode)
				return repeater;
		}
	}

	return NULL;
}

wxArrayString CRepeaterHandler::listDVRepeaters()
{
	wxArrayString repeaters;

	for (unsigned int i = 0U; i < m_maxRepeaters; i++) {
		CRepeaterHandler* repeater = m_repeaters[i];
		if (repeater != NULL && !repeater->m_ddMode)
			repeaters.Add(repeater->m_rptCallsign);
	}

	return repeaters;
}

void CRepeaterHandler::pollAllIcom(CPollData& data)
{
	for (unsigned int i = 0U; i < m_maxRepeaters; i++) {
		CRepeaterHandler* repeater = m_repeaters[i];
		if (repeater != NULL && repeater->m_hwType == HW_ICOM)
			repeater->processRepeater(data);
	}
}

CRemoteRepeaterData* CRepeaterHandler::getInfo() const
{
	return new CRemoteRepeaterData(m_rptCallsign, m_linkReconnect, m_linkStartup);
}

void CRepeaterHandler::processRepeater(CHeaderData& header)
{
	unsigned int id = header.getId();

	// Stop duplicate headers
	if (id == m_repeaterId)
		return;

	// Save the header fields
	m_myCall1  = header.getMyCall1();
	m_myCall2  = header.getMyCall2();
	m_yourCall = header.getYourCall();
	m_rptCall1 = header.getRptCall1();
	m_rptCall2 = header.getRptCall2();
	m_flag1    = header.getFlag1();
	m_flag2    = header.getFlag2();
	m_flag3    = header.getFlag3();

	if (m_hwType == HW_ICOM) {
		unsigned char band1 = header.getBand1();
		unsigned char band2 = header.getBand2();
		unsigned char band3 = header.getBand3();

		if (m_band1 != band1 || m_band2 != band2 || m_band3 != band3) {
			m_band1 = band1;
			m_band2 = band2;
			m_band3 = band3;
			wxLogMessage(wxT("Repeater %s registered with bands %u %u %u"), m_rptCall1.c_str(), m_band1, m_band2, m_band3);
		}
	}

	if (m_flag1 == 0x01) {
		wxLogMessage(wxT("Received a busy message from repeater %s"), m_rptCall1.c_str());
		return;
	}

	if (!m_heardUser.IsEmpty() && !m_myCall1.IsSameAs(m_heardUser) && m_irc != NULL)
		m_irc->sendHeard(m_heardUser, wxT("    "), wxT("        "), m_heardRepeater, wxT("        "), 0x00U, 0x00U, 0x00U);

	// Inform CCS
	m_ccsHandler->writeHeard(header);
	m_ccsHandler->writeHeader(header);

	// The Icom heard timer
	m_heardTimer.stop();

	if (m_drats != NULL)
		m_drats->writeHeader(header);

	// Reset the statistics
	m_frames  = 0U;
	m_silence = 0U;
	m_errors  = 0U;

	// An RF header resets the reconnect timer
	m_linkReconnectTimer.start();

	// Incoming links get everything
	sendToIncoming(header);

	// Reset the slow data text collector
	m_textCollector.reset();
	m_text.Clear();

	// Reset the APRS Writer if it's enabled
	if (m_aprsWriter != NULL)
		m_aprsWriter->reset(m_rptCallsign);

	// Write to Header.log if it's enabled
	if (m_headerLogger != NULL)
		m_headerLogger->write(wxT("Repeater"), header);

	// Reset the DTMF decoder
	m_dtmf.reset();

	// Reset the info, echo and version commands if they're running
	m_audio->cancel();
	m_echo->cancel();
	m_version->cancel();

	// A new header resets fields and G2 routing status
	m_repeaterId = id;
	m_busyId     = 0x00U;
	m_watchdogTimer.start();

	m_xBandRptr = NULL;
	m_starNet   = NULL;

	// If we're querying for a user or repeater, kill the query timer
	if (m_g2Status == G2_USER || m_g2Status == G2_REPEATER)
		m_queryTimer.stop();

	delete m_g2Header;
	m_g2Header = NULL;
	m_g2Status = G2_NONE;
	m_g2User.Clear();
	m_g2Repeater.Clear();
	m_g2Gateway.Clear();

	// Check if this user is restricted
	m_restricted = false;
	if (m_restrictList != NULL) {
		bool res = m_restrictList->isInList(m_myCall1);
		if (res)
			m_restricted = true;
	}

	// Reject silly RPT2 values
	if (m_rptCall2.IsSameAs(m_rptCallsign) || m_rptCall2.IsSameAs(wxT("        ")))
		return;

	// Do cross-band routing if RPT2 is not one of the gateway callsigns
	if (!m_rptCall2.IsSameAs(m_gwyCallsign) && !m_rptCall2.IsSameAs(m_gateway)) {
		CRepeaterHandler* repeater = findDVRepeater(m_rptCall2);
		if (repeater != NULL) {
			wxLogMessage(wxT("Cross-band routing by %s from %s to %s"), m_myCall1.c_str(), m_rptCallsign.c_str(), m_rptCall2.c_str());
			m_xBandRptr = repeater;
			m_xBandRptr->process(header, DIR_INCOMING, AS_XBAND);
			m_g2Status = G2_XBAND;
		} else {
			// Keep the transmission local
			wxLogMessage(wxT("Invalid cross-band route by %s from %s to %s"), m_myCall1.c_str(), m_rptCallsign.c_str(), m_rptCall2.c_str());
			m_g2Status = G2_LOCAL;
		}
		return;
	}

	m_starNet = CStarNetHandler::findStarNet(header);
	if (m_starNet != NULL && !m_restricted) {
		wxLogMessage(wxT("StarNet routing by %s to %s"), m_myCall1.c_str(), m_yourCall.c_str());
		m_starNet->process(header);
		m_g2Status = G2_STARNET;
		return;
	}

	// Reject simple cases
	if (m_yourCall.Left(4).IsSameAs(wxT("CQCQ"))) {
		sendToOutgoing(header);
		return;
	}

	// Handle the Echo command
	if (m_echoEnabled && m_yourCall.IsSameAs(wxT("       E"))) {
		m_g2Status = G2_ECHO;
		m_echo->writeHeader(header);
		return;
	}

	// Handle the Info command
	if (m_infoEnabled && m_yourCall.IsSameAs(wxT("       I"))) {
		m_g2Status = G2_LOCAL;
		m_infoNeeded = true;
		return;
	}

	// Handle the Version command
	if (m_infoEnabled && m_yourCall.IsSameAs(wxT("       V"))) {
		m_g2Status = G2_VERSION;
		sendToOutgoing(header);
		return;
	}

	if (m_restricted) {
		sendToOutgoing(header);
		return;
	}

	if (m_yourCall.Left(3U).IsSameAs(wxT("CCS"))) {
		ccsCommandHandler(m_yourCall, m_myCall1, wxT("UR Call"));
		sendToOutgoing(header);
	} else {
		g2CommandHandler(m_yourCall, m_myCall1, header);

		if (m_g2Status == G2_NONE) {
			reflectorCommandHandler(m_yourCall, m_myCall1, wxT("UR Call"));
			sendToOutgoing(header);
		}
	}
}

void CRepeaterHandler::processRepeater(CAMBEData& data)
{
	// AMBE data via RF resets the reconnect timer
	m_linkReconnectTimer.start();
	m_watchdogTimer.start();

	m_frames++;
	m_errors += data.getErrors();

	unsigned char buffer[DV_FRAME_MAX_LENGTH_BYTES];
	data.getData(buffer, DV_FRAME_MAX_LENGTH_BYTES);

	if (::memcmp(buffer, NULL_AMBE_DATA_BYTES, VOICE_FRAME_LENGTH_BYTES) == 0)
		m_silence++;

	// Don't do DTMF decoding or blanking if off and not on crossband either
	if (m_dtmfEnabled && m_g2Status != G2_XBAND) {
		bool pressed = m_dtmf.decode(buffer, data.isEnd());
		if (pressed) {
			// Replace the DTMF with silence
			::memcpy(buffer, NULL_AMBE_DATA_BYTES, VOICE_FRAME_LENGTH_BYTES);
			data.setData(buffer, DV_FRAME_LENGTH_BYTES);
		}

		bool dtmfDone = m_dtmf.hasCommand();
		if (dtmfDone) {
			wxString command = m_dtmf.translate();

			// Only process the DTMF command if the your call is CQCQCQ and not a restricted user
			if (!m_restricted && m_yourCall.Left(4U).IsSameAs(wxT("CQCQ"))) {
				if (command.IsEmpty()) {
					// Do nothing
				} else if (command.Left(3U).IsSameAs(wxT("CCS"))) {
					ccsCommandHandler(command, m_myCall1, wxT("DTMF"));
				} else if (command.IsSameAs(wxT("       I"))) {
					m_infoNeeded = true;
				} else {
					reflectorCommandHandler(command, m_myCall1, wxT("DTMF"));
				}
			}
		}
	}

	// Incoming links get everything
	sendToIncoming(data);

	// CCS gets everything
	m_ccsHandler->writeAMBE(data);

	if (m_drats != NULL)
		m_drats->writeData(data);

	if (m_aprsWriter != NULL)
		m_aprsWriter->writeData(m_rptCallsign, data);

	if (m_text.IsEmpty() && !data.isEnd()) {
		m_textCollector.writeData(data);

		bool hasText = m_textCollector.hasData();
		if (hasText) {
			m_text = m_textCollector.getData();
			sendHeard(m_text);
		}
	}

	data.setText(m_text);

	// If no slow data text has been received, send a heard with no text when the end of the
	// transmission arrives
	if (data.isEnd() && m_text.IsEmpty())
		sendHeard();

	// Send the statistics after the end of the data, any stats from the repeater should have
	// been received by now
	if (data.isEnd()) {
		m_watchdogTimer.stop();
		sendStats();
	}

	switch (m_g2Status) {
		case G2_LOCAL:
			if (data.isEnd()) {
				m_repeaterId = 0x00U;
				m_g2Status   = G2_NONE;
			}
			break;

		case G2_OK:
			data.setDestination(m_g2Address, G2_DV_PORT);
			m_g2Handler->writeAMBE(data);

			if (data.isEnd()) {
				m_repeaterId = 0x00U;
				m_g2Status   = G2_NONE;
			}
			break;

		case G2_USER:
		case G2_REPEATER:
			// Data ended before the callsign could be resolved
			if (data.isEnd()) {
				m_queryTimer.stop();
				delete m_g2Header;
				m_repeaterId = 0x0U;
				m_g2Status   = G2_NONE;
				m_g2Header   = NULL;
			}
			break;

		case G2_NONE:
			if (data.isEnd())
				m_repeaterId = 0x00U;

			sendToOutgoing(data);
			break;

		case G2_XBAND:
			m_xBandRptr->process(data, DIR_INCOMING, AS_XBAND);

			if (data.isEnd()) {
				m_repeaterId = 0x00U;
				m_g2Status   = G2_NONE;
				m_xBandRptr  = NULL;
			}
			break;

		case G2_STARNET:
			m_starNet->process(data);

			if (data.isEnd()) {
				m_repeaterId = 0x00U;
				m_g2Status   = G2_NONE;
				m_starNet    = NULL;
			}
			break;

		case G2_ECHO:
			m_echo->writeData(data);

			if (data.isEnd()) {
				m_repeaterId = 0x00U;
				m_g2Status   = G2_NONE;
			}
			break;

		case G2_VERSION:
			sendToOutgoing(data);

			if (data.isEnd()) {
				m_version->sendVersion();

				m_repeaterId = 0x00U;
				m_g2Status   = G2_NONE;
			}
			break;
	}

	if (data.isEnd() && m_infoNeeded) {
		m_audio->sendStatus();
		m_infoNeeded = false;
	}
}

// Incoming headers when relaying network traffic, as detected by the repeater, will be used as a command
// to the reflector command handler, probably to do an unlink.
void CRepeaterHandler::processBusy(CHeaderData& header)
{
	unsigned int id = header.getId();

	// Ignore duplicate headers
	if (id == m_busyId)
		return;

	wxString rptCall1 = header.getRptCall1();
	wxString rptCall2 = header.getRptCall2();

	if (m_hwType == HW_ICOM) {
		unsigned char band1 = header.getBand1();
		unsigned char band2 = header.getBand2();
		unsigned char band3 = header.getBand3();

		if (m_band1 != band1 || m_band2 != band2 || m_band3 != band3) {
			m_band1 = band1;
			m_band2 = band2;
			m_band3 = band3;
			wxLogMessage(wxT("Repeater %s registered with bands %u %u %u"), rptCall1.c_str(), m_band1, m_band2, m_band3);
		}
	}

	if (header.getFlag1() == 0x01) {
		wxLogMessage(wxT("Received a busy message from repeater %s"), rptCall1.c_str());
		return;
	}

	// Reject the header if the RPT2 value is not one of the gateway callsigns
	if (!rptCall2.IsSameAs(m_gwyCallsign) && !rptCall2.IsSameAs(m_gateway))
		return;

	m_myCall1  = header.getMyCall1();
	m_yourCall = header.getYourCall();
	m_rptCall1 = rptCall1;
	m_rptCall2 = rptCall2;

	m_dtmf.reset();

	m_busyId     = id;
	m_repeaterId = 0x00U;
	m_watchdogTimer.start();

	// If restricted then don't send to the command handler
	m_restricted = false;
	if (m_restrictList != NULL) {
		bool res = m_restrictList->isInList(m_myCall1);
		if (res) {
			m_restricted = true;
			return;
		}
	}

	// Reject simple cases
	if (m_yourCall.Left(4).IsSameAs(wxT("CQCQ")) || m_yourCall.IsSameAs(wxT("       E"))     || m_yourCall.IsSameAs(wxT("       I")))
		return;

	if (m_yourCall.Left(3U).IsSameAs(wxT("CCS")))
		ccsCommandHandler(m_yourCall, m_myCall1, wxT("background UR Call"));
	else
		reflectorCommandHandler(m_yourCall, m_myCall1, wxT("background UR Call"));
}

void CRepeaterHandler::processBusy(CAMBEData& data)
{
	m_watchdogTimer.start();

	unsigned char buffer[DV_FRAME_MAX_LENGTH_BYTES];
	data.getData(buffer, DV_FRAME_MAX_LENGTH_BYTES);

	// Don't do DTMF decoding if off
	if (m_dtmfEnabled) {
		m_dtmf.decode(buffer, data.isEnd());

		bool dtmfDone = m_dtmf.hasCommand();
		if (dtmfDone) {
			wxString command = m_dtmf.translate();

			// Only process the DTMF command if the your call is CQCQCQ and the user isn't restricted
			if (!m_restricted && m_yourCall.Left(4U).IsSameAs(wxT("CQCQ"))) {
				if (command.IsEmpty()) {
					// Do nothing
				} else if (command.Left(3U).IsSameAs(wxT("CCS"))) {
					ccsCommandHandler(command, m_myCall1, wxT("background DTMF"));
				} else if (command.IsSameAs(wxT("       I"))) {
					// Do nothing
				} else {
					reflectorCommandHandler(command, m_myCall1, wxT("background DTMF"));
				}
			}
		}
	}

	if (data.isEnd()) {
		if (m_infoNeeded) {
			m_audio->sendStatus();
			m_infoNeeded = false;
		}

		if (m_g2Status == G2_VERSION)
			m_version->sendVersion();

		m_g2Status = G2_NONE;
		m_busyId = 0x00U;
		m_watchdogTimer.stop();
	}
}

void CRepeaterHandler::processRepeater(CHeardData& heard)
{
	if (m_irc == NULL)
		return;

	// A second heard has come in before the original has been sent or cancelled
	if (m_heardTimer.isRunning() && !m_heardTimer.hasExpired())
		m_irc->sendHeard(m_heardUser, wxT("    "), wxT("        "), m_heardRepeater, wxT("        "), 0x00U, 0x00U, 0x00U);

	m_heardUser     = heard.getUser();
	m_heardRepeater = heard.getRepeater();

	m_heardTimer.start();
}

void CRepeaterHandler::processRepeater(CPollData& data)
{
	if (!m_pollTimer.hasExpired())
		return;

	if (m_irc == NULL)
		return;

	wxString callsign = m_rptCallsign;
	if (m_ddMode)
		callsign.Append(wxT("D"));

	wxString text = data.getData1();

	m_irc->kickWatchdog(callsign, text);

	m_pollTimer.start();
}

void CRepeaterHandler::processRepeater(CDDData& data)
{
	if (!m_ddMode)
		return;

	if (m_ddCallsign.IsEmpty()) {
		m_ddCallsign = data.getYourCall();
		wxLogMessage(wxT("Added DD callsign %s"), m_ddCallsign.c_str());
	}

	CDDHandler::process(data);
}

bool CRepeaterHandler::process(CDDData& data)
{
	unsigned char* address = data.getDestinationAddress();
	if (::memcmp(address, ETHERNET_BROADCAST_ADDRESS, ETHERNET_ADDRESS_LENGTH) == 0)
		data.setRepeaters(m_gwyCallsign, wxT("        "));
	else if (::memcmp(address, TOALL_MULTICAST_ADDRESS, ETHERNET_ADDRESS_LENGTH) == 0){
		data.setRepeaters(m_gwyCallsign, wxT("CQCQCQ  "));
		data.setRptCall2(m_rptCallsign);
	} else if (::memcmp(address, DX_MULTICAST_ADDRESS, ETHERNET_ADDRESS_LENGTH) == 0){
		data.setRepeaters(m_gwyCallsign, wxT("CQCQCQ  "));
		data.setRptCall2(m_rptCallsign);
	} else
		data.setRepeaters(m_gwyCallsign, m_rptCallsign);

	data.setDestination(m_address, m_port);
	data.setFlags(0xC0U, 0x00U, 0x00U);
	data.setMyCall1(m_ddCallsign);
	data.setMyCall2(wxT("    "));

	m_repeaterHandler->writeDD(data);

	return true;
}

bool CRepeaterHandler::process(CHeaderData& header, DIRECTION, AUDIO_SOURCE source)
{
	// If data is coming from the repeater then don't send
	if (m_repeaterId != 0x00U)
		return false;

	// Rewrite the ID if we're using Icom hardware
	if (m_hwType == HW_ICOM) {
		unsigned int id1 = header.getId();
		unsigned int id2 = id1 + m_index;
		header.setId(id2);
	}

	// Send all original headers to all repeater types, and only send duplicate headers to homebrew repeaters
	if (source != AS_DUP || (source == AS_DUP && m_hwType == HW_HOMEBREW)) {
		header.setBand1(m_band1);
		header.setBand2(m_band2);
		header.setBand3(m_band3);
		header.setDestination(m_address, m_port);
		header.setRepeaters(m_gwyCallsign, m_rptCallsign);

		m_repeaterHandler->writeHeader(header);
	}

	// Don't send duplicate headers to anyone else
	if (source == AS_DUP)
		return true;

	sendToIncoming(header);

	if (source == AS_DPLUS || source == AS_DEXTRA || source == AS_DCS)
		m_ccsHandler->writeHeader(header);

	if (source == AS_G2 || source == AS_INFO || source == AS_VERSION || source == AS_XBAND || source == AS_ECHO)
		return true;

	// Reset the slow data text collector, used for DCS text passing
	m_textCollector.reset();
	m_text.Clear();

	sendToOutgoing(header);

	return true;
}

bool CRepeaterHandler::process(CAMBEData& data, DIRECTION, AUDIO_SOURCE source)
{
	// If data is coming from the repeater then don't send
	if (m_repeaterId != 0x00U)
		return false;

	// Rewrite the ID if we're using Icom hardware
	if (m_hwType == HW_ICOM) {
		unsigned int id = data.getId();
		id += m_index;
		data.setId(id);
	}

	data.setBand1(m_band1);
	data.setBand2(m_band2);
	data.setBand3(m_band3);
	data.setDestination(m_address, m_port);

	m_repeaterHandler->writeAMBE(data);

	sendToIncoming(data);

	if (source == AS_DPLUS || source == AS_DEXTRA || source == AS_DCS)
		m_ccsHandler->writeAMBE(data);

	if (source == AS_G2 || source == AS_INFO || source == AS_VERSION || source == AS_XBAND || source == AS_ECHO)
		return true;

	// Collect the text from the slow data for DCS
	if (m_text.IsEmpty() && !data.isEnd()) {
		m_textCollector.writeData(data);

		bool hasText = m_textCollector.hasData();
		if (hasText)
			m_text = m_textCollector.getData();
	}

	data.setText(m_text);

	sendToOutgoing(data);

	return true;
}

void CRepeaterHandler::resolveUserInt(const wxString& user, const wxString& repeater, const wxString& gateway, const wxString &address)
{
	if (m_g2Status == G2_USER && m_g2User.IsSameAs(user)) {
		m_queryTimer.stop();

		if (!address.IsEmpty()) {
			// No point routing to self
			if (repeater.IsSameAs(m_rptCallsign)) {
				m_g2Status = G2_LOCAL;
				delete m_g2Header;
				m_g2Header = NULL;
				return;
			}

			// User found, update the settings and send the header to the correct place
			m_g2Address.s_addr = ::inet_addr(address.mb_str());

			m_g2Repeater = repeater;
			m_g2Gateway  = gateway;

			m_g2Header->setDestination(m_g2Address, G2_DV_PORT);
			m_g2Header->setRepeaters(m_g2Gateway, m_g2Repeater);
			m_g2Handler->writeHeader(*m_g2Header);

			delete m_g2Header;
			m_g2Status = G2_OK;
			m_g2Header = NULL;
		} else {
			// User not found, remove G2 settings
			m_g2Status = G2_LOCAL;
			m_g2User.Clear();
			m_g2Repeater.Clear();
			m_g2Gateway.Clear();

			delete m_g2Header;
			m_g2Header = NULL;
		}
	}
}

void CRepeaterHandler::resolveRepeaterInt(const wxString& repeater, const wxString& gateway, const wxString &address, DSTAR_PROTOCOL protocol)
{
	if (m_g2Status == G2_REPEATER && m_g2Repeater.IsSameAs(repeater)) {
		m_queryTimer.stop();

		if (!address.IsEmpty()) {
			// Repeater found, update the settings and send the header to the correct place
			m_g2Address.s_addr = ::inet_addr(address.mb_str());

			m_g2Repeater = repeater;
			m_g2Gateway  = gateway;

			m_g2Header->setDestination(m_g2Address, G2_DV_PORT);
			m_g2Header->setRepeaters(m_g2Gateway, m_g2Repeater);
			m_g2Handler->writeHeader(*m_g2Header);

			delete m_g2Header;
			m_g2Status = G2_OK;
			m_g2Header = NULL;
		} else {
			// Repeater not found, remove G2 settings
			m_g2Status = G2_LOCAL;
			m_g2User.Clear();
			m_g2Repeater.Clear();
			m_g2Gateway.Clear();

			delete m_g2Header;
			m_g2Header = NULL;
		}
	}

	if (m_linkStatus == LS_PENDING_IRCDDB && m_linkRepeater.IsSameAs(repeater)) {
		m_queryTimer.stop();

		if (!address.IsEmpty()) {
			// Repeater found
			in_addr addr;
			switch (protocol) {
				case DP_DPLUS:
					if (m_dplusEnabled) {
						m_linkGateway = gateway;
						addr.s_addr = ::inet_addr(address.mb_str());
						CDPlusHandler::link(this, m_rptCallsign, m_linkRepeater, addr);
						m_linkStatus = LS_LINKING_DPLUS;
					} else {
						wxLogMessage(wxT("Require D-Plus for linking to %s, but D-Plus is disabled"), repeater.c_str());
						m_linkStatus = LS_NONE;
						m_linkRepeater.Clear();
						m_linkGateway.Clear();
						writeNotLinked();
						triggerInfo();
					}
					break;

				case DP_DCS:
					if (m_dcsEnabled) {
						m_linkGateway = gateway;
						addr.s_addr = ::inet_addr(address.mb_str());
						CDCSHandler::link(this, m_rptCallsign, m_linkRepeater, addr);
						m_linkStatus = LS_LINKING_DCS;
					} else {
						wxLogMessage(wxT("Require DCS for linking to %s, but DCS is disabled"), repeater.c_str());
						m_linkStatus = LS_NONE;
						m_linkRepeater.Clear();
						m_linkGateway.Clear();
						writeNotLinked();
						triggerInfo();
					}
					break;

				case DP_LOOPBACK:
					m_linkGateway = gateway;
					addr.s_addr = ::inet_addr(address.mb_str());
					CDCSHandler::link(this, m_rptCallsign, m_linkRepeater, addr);
					m_linkStatus = LS_LINKING_LOOPBACK;
					break;

				default:
					if (m_dextraEnabled) {
						m_linkGateway = gateway;
						addr.s_addr = ::inet_addr(address.mb_str());
						CDExtraHandler::link(this, m_rptCallsign, m_linkRepeater, addr);
						m_linkStatus = LS_LINKING_DEXTRA;
					} else {
						wxLogMessage(wxT("Require DExtra for linking to %s, but DExtra is disabled"), repeater.c_str());
						m_linkStatus = LS_NONE;
						m_linkRepeater.Clear();
						m_linkGateway.Clear();
						writeNotLinked();
						triggerInfo();
					}
					break;

			}
		} else {
			// Repeater not found
			m_linkStatus = LS_NONE;
			m_linkRepeater.Clear();
			m_linkGateway.Clear();

			writeNotLinked();
			triggerInfo();
		}
	}
}

void CRepeaterHandler::clockInt(unsigned int ms)
{
	m_audio->clock(ms);
	m_echo->clock(ms);
	m_version->clock(ms);

	m_linkReconnectTimer.clock(ms);
	m_watchdogTimer.clock(ms);
	m_queryTimer.clock(ms);
	m_heardTimer.clock(ms);
	m_pollTimer.clock(ms);

	// If the reconnect timer has expired
	if (m_linkReconnectTimer.isRunning() && m_linkReconnectTimer.hasExpired()) {
		if (m_linkStatus != LS_NONE && (m_linkStartup.IsEmpty() || m_linkStartup.IsSameAs(wxT("        ")))) {
			// Unlink if linked to something
			wxLogMessage(wxT("Reconnect timer has expired, unlinking %s from %s"), m_rptCallsign.c_str(), m_linkRepeater.c_str());

			CDExtraHandler::unlink(this);
			CDPlusHandler::unlink(this);
			CDCSHandler::unlink(this);

			m_linkStatus = LS_NONE;
			m_linkRepeater.Clear();

			// Tell the users
			writeNotLinked();
			triggerInfo();
		} else if ((m_linkStatus == LS_NONE && !m_linkStartup.IsEmpty() && !m_linkStartup.IsSameAs(wxT("        "))) ||
				   (m_linkStatus != LS_NONE && !m_linkRepeater.IsSameAs(m_linkStartup))) {
			// Relink if not linked or linked to the wrong reflector
			wxLogMessage(wxT("Reconnect timer has expired, relinking %s to %s"), m_rptCallsign.c_str(), m_linkStartup.c_str());

			// Check for just a change of letter
			if (m_linkStatus != LS_NONE) {
				wxString oldCall = m_linkRepeater.Left(LONG_CALLSIGN_LENGTH - 1U);
				wxString newCall = m_linkStartup.Left(LONG_CALLSIGN_LENGTH - 1U);

				// Just a change of port?
				if (oldCall.IsSameAs(newCall)) {
					switch (m_linkStatus) {
						case LS_LINKING_DEXTRA:
						case LS_LINKED_DEXTRA:
							m_linkRelink = true;
							m_linkRepeater = m_linkStartup;
							CDExtraHandler::unlink(this, m_linkRepeater);

							m_linkStatus = LS_LINKING_DEXTRA;
							writeLinkingTo(m_linkRepeater);
							triggerInfo();
							break;

						case LS_LINKING_DCS:
						case LS_LINKED_DCS:
							m_linkRelink = true;
							m_linkRepeater = m_linkStartup;
							CDCSHandler::unlink(this, m_linkRepeater);

							m_linkStatus = LS_LINKING_DCS;
							writeLinkingTo(m_linkRepeater);
							triggerInfo();
							break;

						case LS_LINKING_LOOPBACK:
						case LS_LINKED_LOOPBACK:
							m_linkRelink = true;
							m_linkRepeater = m_linkStartup;
							CDCSHandler::unlink(this, m_linkRepeater);

							m_linkStatus = LS_LINKING_LOOPBACK;
							writeLinkingTo(m_linkRepeater);
							triggerInfo();
							break;

						case LS_LINKING_DPLUS:
							m_linkRepeater = m_linkStartup;
							CDPlusHandler::relink(this, m_linkRepeater);
							writeLinkingTo(m_linkRepeater);
							triggerInfo();
							break;

						case LS_LINKED_DPLUS:
							m_linkRepeater = m_linkStartup;
							CDPlusHandler::relink(this, m_linkRepeater);
							writeLinkedTo(m_linkRepeater);
							triggerInfo();
							break;

						default:
							break;
					}

					return;
				}
			}

			CDExtraHandler::unlink(this);
			CDPlusHandler::unlink(this);
			CDCSHandler::unlink(this);

			linkInt(m_linkStartup);
		}

		m_linkReconnectTimer.start();
	}

	// If the ircDDB query timer has expired
	if (m_queryTimer.isRunning() && m_queryTimer.hasExpired()) {
		m_queryTimer.stop();

		if (m_g2Status == G2_USER || m_g2Status == G2_REPEATER) {
			// User or repeater not found in time, remove G2 settings
			wxLogMessage(wxT("ircDDB did not reply within five seconds"));

			m_g2Status = G2_LOCAL;
			m_g2User.Clear();
			m_g2Repeater.Clear();
			m_g2Gateway.Clear();

			delete m_g2Header;
			m_g2Header = NULL;
		} else if (m_linkStatus == LS_PENDING_IRCDDB) {
			// Repeater not found in time
			wxLogMessage(wxT("ircDDB did not reply within five seconds"));

			m_linkStatus = LS_NONE;
			m_linkRepeater.Clear();
			m_linkGateway.Clear();

			writeNotLinked();
			triggerInfo();
		} else if (m_linkStatus == LS_LINKING_CCS) {
			// CCS didn't reply in time
			wxLogMessage(wxT("CCS did not reply within five seconds"));

			m_ccsHandler->stopLink();

			m_linkStatus = LS_NONE;
			m_linkRepeater.Clear();

			restoreLinks();
		}
	}

	// Icom heard timer has expired
	if (m_heardTimer.isRunning() && m_heardTimer.hasExpired() && m_irc != NULL) {
		m_irc->sendHeard(m_heardUser, wxT("    "), wxT("        "), m_heardRepeater, wxT("        "), 0x00U, 0x00U, 0x00U);
		m_heardTimer.stop();
	}

	// If the watchdog timer has expired, clean up
	if (m_watchdogTimer.isRunning() && m_watchdogTimer.hasExpired()) {
		wxLogMessage(wxT("Radio watchdog timer for %s has expired"), m_rptCallsign.c_str());
		m_watchdogTimer.stop();

		if (m_repeaterId != 0x00U) {
			if (m_text.IsEmpty())
				sendHeard();

			if (m_drats != NULL)
				m_drats->writeEnd();

			sendStats();

			switch (m_g2Status) {
				case G2_USER:
				case G2_REPEATER:
					m_queryTimer.stop();
					delete m_g2Header;
					m_g2Header = NULL;
					break;

				case G2_XBAND:
					m_xBandRptr = NULL;
					break;

				case G2_STARNET:
					m_starNet = NULL;
					break;

				case G2_ECHO:
					m_echo->end();
					break;

				case G2_VERSION:
					m_version->sendVersion();
					break;

				default:
					break;
			}

			if (m_infoNeeded) {
				m_audio->sendStatus();
				m_infoNeeded = false;
			}

			m_repeaterId = 0x00U;
			m_g2Status   = G2_NONE;
		}

		if (m_busyId != 0x00U) {
			if (m_infoNeeded) {
				m_audio->sendStatus();
				m_infoNeeded = false;
			}

			if (m_g2Status == G2_VERSION)
				m_version->sendVersion();

			m_g2Status = G2_NONE;
			m_busyId = 0x00U;
		}
	}
}

void CRepeaterHandler::linkUp(DSTAR_PROTOCOL protocol, const wxString& callsign)
{
	if (protocol == DP_DEXTRA && m_linkStatus == LS_LINKING_DEXTRA) {
		wxLogMessage(wxT("DExtra link to %s established"), callsign.c_str());
		m_linkStatus = LS_LINKED_DEXTRA;
		writeLinkedTo(callsign);
		triggerInfo();
	}

	if (protocol == DP_DPLUS && m_linkStatus == LS_LINKING_DPLUS) {
		wxLogMessage(wxT("D-Plus link to %s established"), callsign.c_str());
		m_linkStatus = LS_LINKED_DPLUS;
		writeLinkedTo(callsign);
		triggerInfo();
	}

	if (protocol == DP_DCS && m_linkStatus == LS_LINKING_DCS) {
		wxLogMessage(wxT("DCS link to %s established"), callsign.c_str());
		m_linkStatus = LS_LINKED_DCS;
		writeLinkedTo(callsign);
		triggerInfo();
	}

	if (protocol == DP_DCS && m_linkStatus == LS_LINKING_LOOPBACK) {
		wxLogMessage(wxT("Loopback link to %s established"), callsign.c_str());
		m_linkStatus = LS_LINKED_LOOPBACK;
		writeLinkedTo(callsign);
		triggerInfo();
	}
}

bool CRepeaterHandler::linkFailed(DSTAR_PROTOCOL protocol, const wxString& callsign, bool isRecoverable)
{
	// Is relink to another module required?
	if (!isRecoverable && m_linkRelink) {
		m_linkRelink = false;
		wxLogMessage(wxT("Relinking %s from %s to %s"), m_rptCallsign.c_str(), callsign.c_str(), m_linkRepeater.c_str());
		linkInt(m_linkRepeater);
		return false;
	}

	if (!isRecoverable) {
		if (protocol == DP_DEXTRA && callsign.IsSameAs(m_linkRepeater)) {
			wxLogMessage(wxT("DExtra link to %s has ended"), m_linkRepeater.c_str());
			m_linkRepeater.Clear();
			m_linkStatus = LS_NONE;
			writeNotLinked();
			triggerInfo();
		}

		if (protocol == DP_DPLUS && callsign.IsSameAs(m_linkRepeater)) {
			wxLogMessage(wxT("D-Plus link to %s has ended"), m_linkRepeater.c_str());
			m_linkRepeater.Clear();
			m_linkStatus = LS_NONE;
			writeNotLinked();
			triggerInfo();
		}

		if (protocol == DP_DCS && callsign.IsSameAs(m_linkRepeater)) {
			if (m_linkStatus == LS_LINKED_DCS || m_linkStatus == LS_LINKING_DCS)
				wxLogMessage(wxT("DCS link to %s has ended"), m_linkRepeater.c_str());
			else
				wxLogMessage(wxT("Loopback link to %s has ended"), m_linkRepeater.c_str());
			m_linkRepeater.Clear();
			m_linkStatus = LS_NONE;
			writeNotLinked();
			triggerInfo();
		}

		return false;
	}

	if (protocol == DP_DEXTRA) {
		switch (m_linkStatus) {
			case LS_LINKED_DEXTRA:
				wxLogMessage(wxT("DExtra link to %s has failed, relinking"), m_linkRepeater.c_str());
				m_linkStatus = LS_LINKING_DEXTRA;
				writeLinkingTo(m_linkRepeater);
				triggerInfo();
				return true;

			case LS_LINKING_DEXTRA:
				return true;

			default:
				return false;
		}
	}

	if (protocol == DP_DPLUS) {
		switch (m_linkStatus) {
			case LS_LINKED_DPLUS:
				wxLogMessage(wxT("D-Plus link to %s has failed, relinking"), m_linkRepeater.c_str());
				m_linkStatus = LS_LINKING_DPLUS;
				writeLinkingTo(m_linkRepeater);
				triggerInfo();
				return true;

			case LS_LINKING_DPLUS:
				return true;

			default:
				return false;
		}
	}

	if (protocol == DP_DCS) {
		switch (m_linkStatus) {
			case LS_LINKED_DCS:
				wxLogMessage(wxT("DCS link to %s has failed, relinking"), m_linkRepeater.c_str());
				m_linkStatus = LS_LINKING_DCS;
				writeLinkingTo(m_linkRepeater);
				triggerInfo();
				return true;

			case LS_LINKED_LOOPBACK:
				wxLogMessage(wxT("Loopback link to %s has failed, relinking"), m_linkRepeater.c_str());
				m_linkStatus = LS_LINKING_LOOPBACK;
				writeLinkingTo(m_linkRepeater);
				triggerInfo();
				return true;

			case LS_LINKING_DCS:
			case LS_LINKING_LOOPBACK:
				return true;

			default:
				return false;
		}
	}

	return false;
}

void CRepeaterHandler::linkRefused(DSTAR_PROTOCOL protocol, const wxString& callsign)
{
	if (protocol == DP_DEXTRA && callsign.IsSameAs(m_linkRepeater)) {
		wxLogMessage(wxT("DExtra link to %s was refused"), m_linkRepeater.c_str());
		m_linkRepeater.Clear();
		m_linkStatus = LS_NONE;
		writeIsBusy(callsign);
		triggerInfo();
	}

	if (protocol == DP_DPLUS && callsign.IsSameAs(m_linkRepeater)) {
		wxLogMessage(wxT("D-Plus link to %s was refused"), m_linkRepeater.c_str());
		m_linkRepeater.Clear();
		m_linkStatus = LS_NONE;
		writeIsBusy(callsign);
		triggerInfo();
	}

	if (protocol == DP_DCS && callsign.IsSameAs(m_linkRepeater)) {
		if (m_linkStatus == LS_LINKED_DCS || m_linkStatus == LS_LINKING_DCS)
			wxLogMessage(wxT("DCS link to %s was refused"), m_linkRepeater.c_str());
		else
			wxLogMessage(wxT("Loopback link to %s was refused"), m_linkRepeater.c_str());
		m_linkRepeater.Clear();
		m_linkStatus = LS_NONE;
		writeIsBusy(callsign);
		triggerInfo();
	}
}

void CRepeaterHandler::link(RECONNECT reconnect, const wxString& reflector)
{
	// CCS removal
	if (m_linkStatus == LS_LINKING_CCS || m_linkStatus == LS_LINKED_CCS) {
		wxLogMessage(wxT("Dropping CCS link to %s"), m_linkRepeater.c_str());

		m_ccsHandler->stopLink();

		m_linkStatus = LS_NONE;
		m_linkRepeater.Clear();
		m_queryTimer.stop();
	}

	m_linkStartup   = reflector;
	m_linkReconnect = reconnect;

	m_linkReconnectTimer.stop();

	switch (m_linkReconnect) {
		case RECONNECT_5MINS:
			m_linkReconnectTimer.start(5U * 60U);
			break;
		case RECONNECT_10MINS:
			m_linkReconnectTimer.start(10U * 60U);
			break;
		case RECONNECT_15MINS:
			m_linkReconnectTimer.start(15U * 60U);
			break;
		case RECONNECT_20MINS:
			m_linkReconnectTimer.start(20U * 60U);
			break;
		case RECONNECT_25MINS:
			m_linkReconnectTimer.start(25U * 60U);
			break;
		case RECONNECT_30MINS:
			m_linkReconnectTimer.start(30U * 60U);
			break;
		case RECONNECT_60MINS:
			m_linkReconnectTimer.start(60U * 60U);
			break;
		case RECONNECT_90MINS:
			m_linkReconnectTimer.start(90U * 60U);
			break;
		case RECONNECT_120MINS:
			m_linkReconnectTimer.start(120U * 60U);
			break;
		case RECONNECT_180MINS:
			m_linkReconnectTimer.start(180U * 60U);
			break;
		default:
			break;
	}

	// Nothing to do
	if ((m_linkStatus != LS_NONE && m_linkRepeater.IsSameAs(reflector)) ||
		(m_linkStatus == LS_NONE && (reflector.IsEmpty() || reflector.IsSameAs(wxT("        ")))))
		return;

	// Handle unlinking
	if (m_linkStatus != LS_NONE && (reflector.IsEmpty() || reflector.IsSameAs(wxT("        ")))) {
		wxLogMessage(wxT("Unlinking %s from %s"), m_rptCallsign.c_str(), m_linkRepeater.c_str());

		CDExtraHandler::unlink(this);
		CDPlusHandler::unlink(this);
		CDCSHandler::unlink(this);

		m_linkStatus = LS_NONE;
		m_linkRepeater.Clear();

		writeNotLinked();
		triggerInfo();

		return;
	}

	wxLogMessage(wxT("Linking %s to %s"), m_rptCallsign.c_str(), reflector.c_str());

	// Check for just a change of letter
	if (m_linkStatus != LS_NONE) {
		wxString oldCall = m_linkRepeater.Left(LONG_CALLSIGN_LENGTH - 1U);
		wxString newCall = reflector.Left(LONG_CALLSIGN_LENGTH - 1U);

		// Just a change of port?
		if (oldCall.IsSameAs(newCall)) {
			switch (m_linkStatus) {
				case LS_LINKING_DEXTRA:
				case LS_LINKED_DEXTRA:
					m_linkRelink = true;
					m_linkRepeater = reflector;
					CDExtraHandler::unlink(this, m_linkRepeater);

					m_linkStatus = LS_LINKING_DEXTRA;
					writeLinkingTo(m_linkRepeater);
					triggerInfo();
					break;

				case LS_LINKING_DCS:
				case LS_LINKED_DCS:
					m_linkRelink = true;
					m_linkRepeater = reflector;
					CDCSHandler::unlink(this, m_linkRepeater);

					m_linkStatus = LS_LINKING_DCS;
					writeLinkingTo(m_linkRepeater);
					triggerInfo();
					break;

				case LS_LINKING_LOOPBACK:
				case LS_LINKED_LOOPBACK:
					m_linkRelink = true;
					m_linkRepeater = reflector;
					CDCSHandler::unlink(this, m_linkRepeater);

					m_linkStatus = LS_LINKING_LOOPBACK;
					writeLinkingTo(m_linkRepeater);
					triggerInfo();
					break;

				case LS_LINKING_DPLUS:
					m_linkRepeater = reflector;
					CDPlusHandler::relink(this, m_linkRepeater);
					writeLinkingTo(m_linkRepeater);
					triggerInfo();
					break;

				case LS_LINKED_DPLUS:
					m_linkRepeater = reflector;
					CDPlusHandler::relink(this, m_linkRepeater);
					writeLinkedTo(m_linkRepeater);
					triggerInfo();
					break;

				default:
					break;
			}

			return;
		}
	}

	CDExtraHandler::unlink(this);
	CDPlusHandler::unlink(this);
	CDCSHandler::unlink(this);

	linkInt(m_linkStartup);
}

void CRepeaterHandler::unlink(PROTOCOL protocol, const wxString& reflector)
{
	if (protocol == PROTO_CCS) {
		m_ccsHandler->unlink(reflector);
		return;
	}

	if (m_linkReconnect == RECONNECT_FIXED && m_linkRepeater.IsSameAs(reflector)) {
		wxLogMessage(wxT("Cannot unlink %s because it is fixed"), reflector.c_str());
		return;
	}

	switch (protocol) {
		case PROTO_DPLUS:
			CDPlusHandler::unlink(this, reflector, false);
			break;

		case PROTO_DEXTRA:
			CDExtraHandler::unlink(this, reflector, false);
			break;

		case PROTO_DCS:
			CDCSHandler::unlink(this, reflector, false);
			break;

		default:
			break;
	}
}

void CRepeaterHandler::g2CommandHandler(const wxString& callsign, const wxString& user, CHeaderData& header)
{
	if (m_linkStatus == LS_LINKING_CCS || m_linkStatus == LS_LINKED_CCS)
		return;

	if (callsign.Left(1).IsSameAs(wxT("/"))) {
		if (m_irc == NULL) {
			wxLogMessage(wxT("%s is trying to G2 route with ircDDB disabled"), user.c_str());
			m_g2Status = G2_LOCAL;
			return;
		}

		// This a repeater route
		// Convert "/1234567" to "123456 7"
		wxString repeater = callsign.Mid(1, LONG_CALLSIGN_LENGTH - 2U);
		repeater.Append(wxT(" "));
		repeater.Append(callsign.Right(1));

		if (repeater.IsSameAs(m_rptCallsign)) {
			wxLogMessage(wxT("%s is trying to G2 route to self, ignoring"), user.c_str());
			m_g2Status = G2_LOCAL;
			return;
		}

		if (repeater.Left(3U).IsSameAs(wxT("REF")) || repeater.Left(3U).IsSameAs(wxT("XRF")) || repeater.Left(3U).IsSameAs(wxT("DCS"))) {
			wxLogMessage(wxT("%s is trying to G2 route to reflector %s, ignoring"), user.c_str(), repeater.c_str());
			m_g2Status = G2_LOCAL;
			return;
		}

		wxLogMessage(wxT("%s is trying to G2 route to repeater %s"), user.c_str(), repeater.c_str());

		m_g2Repeater = repeater;
		m_g2User = wxT("CQCQCQ  ");

		CRepeaterData* data = m_cache->findRepeater(m_g2Repeater);

		if (data == NULL) {
			m_g2Status = G2_REPEATER;
			m_irc->findRepeater(m_g2Repeater);
			m_g2Header = new CHeaderData(header);
			m_queryTimer.start();
		} else {
			m_g2Status = G2_OK;
			m_g2Address = data->getAddress();
			m_g2Gateway = data->getGateway();
			header.setDestination(m_g2Address, G2_DV_PORT);
			header.setRepeaters(m_g2Gateway, m_g2Repeater);
			m_g2Handler->writeHeader(header);
			delete data;
		}
	} else if (!callsign.Right(1).IsSameAs(wxT("L")) && !callsign.Right(1).IsSameAs(wxT("U"))) {
		if (m_irc == NULL) {
			wxLogMessage(wxT("%s is trying to G2 route with ircDDB disabled"), user.c_str());
			m_g2Status = G2_LOCAL;
			return;
		}

		// This a callsign route
		if (callsign.Left(3U).IsSameAs(wxT("REF")) || callsign.Left(3U).IsSameAs(wxT("XRF")) || callsign.Left(3U).IsSameAs(wxT("DCS"))) {
			wxLogMessage(wxT("%s is trying to G2 route to reflector %s, ignoring"), user.c_str(), callsign.c_str());
			m_g2Status = G2_LOCAL;
			return;
		}

		wxLogMessage(wxT("%s is trying to G2 route to callsign %s"), user.c_str(), callsign.c_str());

		CUserData* data = m_cache->findUser(callsign);

		if (data == NULL) {
			m_g2User   = callsign;
			m_g2Status = G2_USER;
			m_irc->findUser(m_g2User);
			m_g2Header = new CHeaderData(header);
			m_queryTimer.start();
		} else {
			// No point G2 routing to yourself
			if (data->getRepeater().IsSameAs(m_rptCallsign)) {
				m_g2Status = G2_LOCAL;
				delete data;
				return;
			}

			m_g2Status   = G2_OK;
			m_g2User     = callsign;
			m_g2Address  = data->getAddress();
			m_g2Repeater = data->getRepeater();
			m_g2Gateway  = data->getGateway();
			header.setDestination(m_g2Address, G2_DV_PORT);
			header.setRepeaters(m_g2Gateway, m_g2Repeater);
			m_g2Handler->writeHeader(header);

			delete data;
		}
	}
}

void CRepeaterHandler::ccsCommandHandler(const wxString& callsign, const wxString& user, const wxString& type)
{
	if (callsign.IsSameAs(wxT("CCSA    "))) {
		m_ccsHandler->stopLink(user, type);
	} else {
		CCS_STATUS status = m_ccsHandler->getStatus();
		if (status == CS_CONNECTED) {
			suspendLinks();
			m_queryTimer.start();
			m_linkStatus   = LS_LINKING_CCS;
			m_linkRepeater = callsign.Mid(3U);
			m_ccsHandler->startLink(m_linkRepeater, user, type);
		}
	}
}

void CRepeaterHandler::reflectorCommandHandler(const wxString& callsign, const wxString& user, const wxString& type)
{
	if (m_linkStatus == LS_LINKING_CCS || m_linkStatus == LS_LINKED_CCS)
		return;

	if (m_linkReconnect == RECONNECT_FIXED)
		return;

	m_queryTimer.stop();

	wxString letter = callsign.Right(1);

	if (letter.IsSameAs(wxT("U"))) {
		// Ignore duplicate unlink requests
		if (m_linkStatus == LS_NONE)
			return;

		wxLogMessage(wxT("Unlink command issued via %s by %s"), type.c_str(), user.c_str());

		CDExtraHandler::unlink(this);
		CDPlusHandler::unlink(this);
		CDCSHandler::unlink(this);

		m_linkStatus = LS_NONE;
		m_linkRepeater.Clear();

		writeNotLinked();
		triggerInfo();
	} else if (letter.IsSameAs(wxT("L"))) {
		wxString reflector;

		// Handle the special case of "       L"
		if (callsign.IsSameAs(wxT("       L"))) {
			if (m_linkStartup.IsEmpty())
				return;

			reflector = m_linkStartup;
		} else {
			// Extract the callsign "1234567L" -> "123456 7"
			reflector = callsign.Left(LONG_CALLSIGN_LENGTH - 2U);
			reflector.Append(wxT(" "));
			reflector.Append(callsign.Mid(LONG_CALLSIGN_LENGTH - 2U, 1));
		}

		// Ensure duplicate link requests aren't acted on
		if (m_linkStatus != LS_NONE && reflector.IsSameAs(m_linkRepeater)) {
			triggerInfo();
			return;
		}

		// We can't link to ourself
		if (reflector.IsSameAs(m_rptCallsign)) {
			wxLogMessage(wxT("%s is trying to link with self via %s, ignoring"), user.c_str(), type.c_str());
			triggerInfo();
			return;
		}

		wxLogMessage(wxT("Link command from %s to %s issued via %s by %s"), m_rptCallsign.c_str(), reflector.c_str(), type.c_str(), user.c_str());

		// Check for just a change of letter
		if (m_linkStatus != LS_NONE) {
			wxString oldCall = m_linkRepeater.Left(LONG_CALLSIGN_LENGTH - 1U);
			wxString newCall = reflector.Left(LONG_CALLSIGN_LENGTH - 1U);

			// Just a change of port?
			if (oldCall.IsSameAs(newCall)) {
				switch (m_linkStatus) {
					case LS_LINKING_DEXTRA:
					case LS_LINKED_DEXTRA:
						m_linkRelink = true;
						m_linkRepeater = reflector;
						CDExtraHandler::unlink(this, m_linkRepeater);

						m_linkStatus = LS_LINKING_DEXTRA;
						writeLinkingTo(m_linkRepeater);
						triggerInfo();
						break;

					case LS_LINKING_DCS:
					case LS_LINKED_DCS:
						m_linkRelink = true;
						m_linkRepeater = reflector;
						CDCSHandler::unlink(this, m_linkRepeater);

						m_linkStatus = LS_LINKING_DCS;
						writeLinkingTo(m_linkRepeater);
						triggerInfo();
						break;

					case LS_LINKING_LOOPBACK:
					case LS_LINKED_LOOPBACK:
						m_linkRelink = true;
						m_linkRepeater = reflector;
						CDCSHandler::unlink(this, m_linkRepeater);

						m_linkStatus = LS_LINKING_LOOPBACK;
						writeLinkingTo(m_linkRepeater);
						triggerInfo();
						break;

					case LS_LINKING_DPLUS:
						m_linkRepeater = reflector;
						CDPlusHandler::relink(this, m_linkRepeater);
						writeLinkingTo(m_linkRepeater);
						triggerInfo();
						break;

					case LS_LINKED_DPLUS:
						m_linkRepeater = reflector;
						CDPlusHandler::relink(this, m_linkRepeater);
						writeLinkedTo(m_linkRepeater);
						triggerInfo();
						break;

					default:
						break;
				}

				return;
			}
		}

		CDExtraHandler::unlink(this);
		CDPlusHandler::unlink(this);
		CDCSHandler::unlink(this);

		linkInt(reflector);
	}
}

void CRepeaterHandler::linkInt(const wxString& callsign)
{
	// Find the repeater to link to
	CRepeaterData* data = m_cache->findRepeater(callsign);

	// Are we trying to link to an unknown DExtra, D-Plus, or DCS reflector?
	if (data == NULL && (callsign.Left(3U).IsSameAs(wxT("REF")) || callsign.Left(3U).IsSameAs(wxT("XRF")) || callsign.Left(3U).IsSameAs(wxT("DCS")))) {
		wxLogMessage(wxT("%s is unknown, ignoring link request"), callsign.c_str());
		triggerInfo();
		return;
	}

	m_linkRepeater = callsign;

	if (data != NULL) {
		m_linkGateway = data->getGateway();

		switch (data->getProtocol()) {
			case DP_DPLUS:
				if (m_dplusEnabled) {
					m_linkStatus = LS_LINKING_DPLUS;
					CDPlusHandler::link(this, m_rptCallsign, m_linkRepeater, data->getAddress());
					writeLinkingTo(m_linkRepeater);
					triggerInfo();
				} else {
					wxLogMessage(wxT("Require D-Plus for linking to %s, but D-Plus is disabled"), callsign.c_str());
					m_linkStatus = LS_NONE;
					writeNotLinked();
					triggerInfo();
				}
				break;

			case DP_DCS:
				if (m_dcsEnabled) {
					m_linkStatus = LS_LINKING_DCS;
					CDCSHandler::link(this, m_rptCallsign, m_linkRepeater, data->getAddress());
					writeLinkingTo(m_linkRepeater);
					triggerInfo();
				} else {
					wxLogMessage(wxT("Require DCS for linking to %s, but DCS is disabled"), callsign.c_str());
					m_linkStatus = LS_NONE;
					writeNotLinked();
					triggerInfo();
				}
				break;

			case DP_LOOPBACK:
				m_linkStatus = LS_LINKING_LOOPBACK;
				CDCSHandler::link(this, m_rptCallsign, m_linkRepeater, data->getAddress());
				writeLinkingTo(m_linkRepeater);
				triggerInfo();
				break;

			default:
				if (m_dextraEnabled) {
					m_linkStatus = LS_LINKING_DEXTRA;
					CDExtraHandler::link(this, m_rptCallsign, m_linkRepeater, data->getAddress());
					writeLinkingTo(m_linkRepeater);
					triggerInfo();
				} else {
					wxLogMessage(wxT("Require DExtra for linking to %s, but DExtra is disabled"), callsign.c_str());
					m_linkStatus = LS_NONE;
					writeNotLinked();
					triggerInfo();
				}
				break;
		}

		delete data;
	} else {
		if (m_irc != NULL) {
			m_linkStatus = LS_PENDING_IRCDDB;
			m_irc->findRepeater(callsign);
			m_queryTimer.start();
			writeLinkingTo(callsign);
			triggerInfo();
		} else {
			m_linkStatus = LS_NONE;
			writeNotLinked();
			triggerInfo();
		}
	}
}

void CRepeaterHandler::sendToOutgoing(const CHeaderData& header)
{
	CHeaderData temp(header);

	temp.setCQCQCQ();
	temp.setFlags(0x00U, 0x00U, 0x00U);

	// Outgoing DPlus links change the RPT1 and RPT2 values in the DPlus handler
	CDPlusHandler::writeHeader(this, temp, DIR_OUTGOING);

	// Outgoing DExtra links have the currently linked repeater/gateway
	// as the RPT1 and RPT2 values
	temp.setRepeaters(m_linkGateway, m_linkRepeater);
	CDExtraHandler::writeHeader(this, temp, DIR_OUTGOING);

	// Outgoing DCS links have the currently linked repeater and repeater callsign
	// as the RPT1 and RPT2 values
	temp.setRepeaters(m_rptCallsign, m_linkRepeater);
	CDCSHandler::writeHeader(this, temp, DIR_OUTGOING);
}

void CRepeaterHandler::sendToOutgoing(const CAMBEData& data)
{
	CAMBEData temp(data);

	CDExtraHandler::writeAMBE(this, temp, DIR_OUTGOING);

	CDPlusHandler::writeAMBE(this, temp, DIR_OUTGOING);

	CDCSHandler::writeAMBE(this, temp, DIR_OUTGOING);
}

void CRepeaterHandler::sendToIncoming(const CHeaderData& header)
{
	CHeaderData temp(header);

	temp.setCQCQCQ();
	temp.setFlags(0x00U, 0x00U, 0x00U);

	// Incoming DPlus links
	temp.setRepeaters(m_rptCallsign, m_gateway);
	CDPlusHandler::writeHeader(this, temp, DIR_INCOMING);

	// Incoming DExtra links have RPT1 and RPT2 swapped
	temp.setRepeaters(m_gwyCallsign, m_rptCallsign);
	CDExtraHandler::writeHeader(this, temp, DIR_INCOMING);

	// Incoming DCS links have RPT1 and RPT2 swapped
	temp.setRepeaters(m_gwyCallsign, m_rptCallsign);
	CDCSHandler::writeHeader(this, temp, DIR_INCOMING);
}

void CRepeaterHandler::sendToIncoming(const CAMBEData& data)
{
	CAMBEData temp(data);

	CDExtraHandler::writeAMBE(this, temp, DIR_INCOMING);

	CDPlusHandler::writeAMBE(this, temp, DIR_INCOMING);

	CDCSHandler::writeAMBE(this, temp, DIR_INCOMING);
}

void CRepeaterHandler::startupInt()
{
	// Report our existence to ircDDB
	if (m_irc != NULL) {
		wxString callsign = m_rptCallsign;
		if (m_ddMode)
			callsign.Append(wxT("D"));

		if (m_frequency > 0.0)
			m_irc->rptrQRG(callsign, m_frequency, m_offset, m_range * 1000.0, m_agl);

		if (m_latitude != 0.0 && m_longitude != 0.0)
			m_irc->rptrQTH(callsign, m_latitude, m_longitude, m_description1, m_description2, m_url);
	}


	m_ccsHandler = new CCCSHandler(this, m_rptCallsign, m_index + 1U, m_latitude, m_longitude, m_frequency, m_offset, m_description1, m_description2, m_url, CCS_PORT + m_index);

	// Start up our CCS link if we are DV mode
	if (!m_ddMode)
		m_ccsHandler->connect();

	// Link to a startup reflector/repeater
	if (m_linkAtStartup && !m_linkStartup.IsEmpty()) {
		wxLogMessage(wxT("Linking %s at startup to %s"), m_rptCallsign.c_str(), m_linkStartup.c_str());

		// Find the repeater to link to
		CRepeaterData* data = m_cache->findRepeater(m_linkStartup);

		m_linkRepeater = m_linkStartup;

		if (data != NULL) {
			m_linkGateway = data->getGateway();

			DSTAR_PROTOCOL protocol = data->getProtocol();
			switch (protocol) {
				case DP_DPLUS:
					if (m_dplusEnabled) {
						m_linkStatus = LS_LINKING_DPLUS;
						CDPlusHandler::link(this, m_rptCallsign, m_linkRepeater, data->getAddress());
						writeLinkingTo(m_linkRepeater);
						triggerInfo();
					} else {
						wxLogMessage(wxT("Require D-Plus for linking to %s, but D-Plus is disabled"), m_linkRepeater.c_str());
						m_linkStatus = LS_NONE;
						writeNotLinked();
						triggerInfo();
					}
					break;

				case DP_DCS:
					if (m_dcsEnabled) {
						m_linkStatus = LS_LINKING_DCS;
						CDCSHandler::link(this, m_rptCallsign, m_linkRepeater, data->getAddress());
						writeLinkingTo(m_linkRepeater);
						triggerInfo();
					} else {
						wxLogMessage(wxT("Require DCS for linking to %s, but DCS is disabled"), m_linkRepeater.c_str());
						m_linkStatus = LS_NONE;
						writeNotLinked();
						triggerInfo();
					}
					break;

				case DP_LOOPBACK:
					m_linkStatus = LS_LINKING_LOOPBACK;
					CDCSHandler::link(this, m_rptCallsign, m_linkRepeater, data->getAddress());
					writeLinkingTo(m_linkRepeater);
					triggerInfo();
					break;

				default:
					if (m_dextraEnabled) {
						m_linkStatus = LS_LINKING_DEXTRA;
						CDExtraHandler::link(this, m_rptCallsign, m_linkRepeater, data->getAddress());
						writeLinkingTo(m_linkRepeater);
						triggerInfo();
					} else {
						wxLogMessage(wxT("Require DExtra for linking to %s, but DExtra is disabled"), m_linkRepeater.c_str());
						m_linkStatus = LS_NONE;
						writeNotLinked();
						triggerInfo();
					}
					break;
			}

			delete data;
		} else {
			if (m_irc != NULL) {
				m_linkStatus = LS_PENDING_IRCDDB;
				m_irc->findRepeater(m_linkStartup);
				m_queryTimer.start();
				writeLinkingTo(m_linkStartup);
				triggerInfo();
			} else {
				m_linkStatus = LS_NONE;
				writeNotLinked();
				triggerInfo();
			}
		}
	} else {
		writeNotLinked();
		triggerInfo();
	}
}

void CRepeaterHandler::writeLinkingTo(const wxString &callsign)
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
			text.Printf(wxT("Connexion a %s"), callsign.c_str());
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
			text.Printf(wxT("Lankar till %s"), callsign.c_str());
			break;
		case TL_NEDERLANDS_NL:
		case TL_NEDERLANDS_BE:
			text.Printf(wxT("Linken naar %s"), callsign.c_str());
			break;
		case TL_NORSK:
			text.Printf(wxT("Kobler til %s"), callsign.c_str());
			break;
		case TL_PORTUGUES:
			text.Printf(wxT("Conectando, %s"), callsign.c_str());
			break;
		default:
			text.Printf(wxT("Linking to %s"), callsign.c_str());
			break;
	}

	CTextData textData(m_linkStatus, callsign, text, m_address, m_port);
	m_repeaterHandler->writeText(textData);

	m_audio->setStatus(m_linkStatus, m_linkRepeater, text);
	triggerInfo();

	m_ccsHandler->setReflector();
}

void CRepeaterHandler::writeLinkedTo(const wxString &callsign)
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
			text.Printf(wxT("Lankad till %s"), callsign.c_str());
			break;
		case TL_NEDERLANDS_NL:
		case TL_NEDERLANDS_BE:
			text.Printf(wxT("Gelinkt met %s"), callsign.c_str());
			break;
		case TL_NORSK:
			text.Printf(wxT("Tilkoblet %s"), callsign.c_str());
			break;
		case TL_PORTUGUES:
			text.Printf(wxT("Conectado a %s"), callsign.c_str());
			break;
		default:
			text.Printf(wxT("Linked to %s"), callsign.c_str());
			break;
	}

	CTextData textData(m_linkStatus, callsign, text, m_address, m_port);
	m_repeaterHandler->writeText(textData);

	m_audio->setStatus(m_linkStatus, m_linkRepeater, text);
	triggerInfo();

	m_ccsHandler->setReflector(callsign);
}

void CRepeaterHandler::writeNotLinked()
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
		case TL_NEDERLANDS_NL:
		case TL_NEDERLANDS_BE:
			text = wxT("Niet gelinkt");
			break;
		case TL_NORSK:
			text = wxT("Ikke linket");
			break;
		case TL_PORTUGUES:
			text = wxT("Desconectado");
			break;
		default:
			text = wxT("Not linked");
			break;
	}

	CTextData textData(LS_NONE, wxEmptyString, text, m_address, m_port);
	m_repeaterHandler->writeText(textData);

	m_audio->setStatus(m_linkStatus, m_linkRepeater, text);
	triggerInfo();

	m_ccsHandler->setReflector();
}

void CRepeaterHandler::writeIsBusy(const wxString& callsign)
{
	wxString tempText;
	wxString text;

	switch (m_language) {
		case TL_DEUTSCH:
			text = wxT("Nicht verbunden");
			tempText.Printf(wxT("%s ist belegt"), callsign.c_str());
			break;
		case TL_DANSK:
			text = wxT("Ikke forbundet");
			tempText.Printf(wxT("Optaget fra %s"), callsign.c_str());
			break;
		case TL_FRANCAIS:
			text = wxT("Non connecte");
			tempText.Printf(wxT("Occupe par %s"), callsign.c_str());
			break;
		case TL_ITALIANO:
			text = wxT("Non connesso");
			tempText.Printf(wxT("Occupado da%s"), callsign.c_str());
			break;
		case TL_POLSKI:
			text = wxT("Nie polaczony");
			tempText.Printf(wxT("%s jest zajety"), callsign.c_str());
			break;
		case TL_ESPANOL:
			text = wxT("No enlazado");
			tempText.Printf(wxT("%s ocupado"), callsign.c_str());
			break;
		case TL_SVENSKA:
			text = wxT("Ej lankad");
			tempText.Printf(wxT("%s ar upptagen"), callsign.c_str());
			break;
		case TL_NEDERLANDS_NL:
		case TL_NEDERLANDS_BE:
			text = wxT("Niet gelinkt");
			tempText.Printf(wxT("%s is bezet"), callsign.c_str());
			break;
		case TL_NORSK:
			text = wxT("Ikke linket");
			tempText.Printf(wxT("%s er opptatt"), callsign.c_str());
			break;
		case TL_PORTUGUES:
			text = wxT("Desconectado");
			tempText.Printf(wxT("%s, ocupado"), callsign.c_str());
			break;
		default:
			text = wxT("Not linked");
			tempText.Printf(wxT("%s is busy"), callsign.c_str());
			break;
	}

	CTextData textData1(m_linkStatus, m_linkRepeater, tempText, m_address, m_port, true);
	m_repeaterHandler->writeText(textData1);

	CTextData textData2(m_linkStatus, m_linkRepeater, text, m_address, m_port);
	m_repeaterHandler->writeText(textData2);

	m_audio->setStatus(m_linkStatus, m_linkRepeater, text);
	m_audio->setTempStatus(m_linkStatus, m_linkRepeater, tempText);
	triggerInfo();

	m_ccsHandler->setReflector();
}

void CRepeaterHandler::ccsLinkMade(const wxString& callsign, DIRECTION direction)
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
			text.Printf(wxT("Lankad till %s"), callsign.c_str());
			break;
		case TL_NEDERLANDS_NL:
		case TL_NEDERLANDS_BE:
			text.Printf(wxT("Gelinkt met %s"), callsign.c_str());
			break;
		case TL_NORSK:
			text.Printf(wxT("Tilkoblet %s"), callsign.c_str());
			break;
		case TL_PORTUGUES:
			text.Printf(wxT("Conectado a %s"), callsign.c_str());
			break;
		default:
			text.Printf(wxT("Linked to %s"), callsign.c_str());
			break;
	}

	if (direction == DIR_OUTGOING) {
		suspendLinks();

		m_linkStatus   = LS_LINKED_CCS;
		m_linkRepeater = callsign;
		m_queryTimer.stop();

		CTextData textData(m_linkStatus, callsign, text, m_address, m_port);
		m_repeaterHandler->writeText(textData);

		m_audio->setStatus(m_linkStatus, m_linkRepeater, text);
		triggerInfo();
	} else {
		CTextData textData(m_linkStatus, m_linkRepeater, text, m_address, m_port, true);
		m_repeaterHandler->writeText(textData);

		m_audio->setTempStatus(LS_LINKED_CCS, callsign, text);
		triggerInfo();
	}
}

void CRepeaterHandler::ccsLinkEnded(const wxString&, DIRECTION direction)
{
	wxString tempText;
	wxString text;

	switch (m_language) {
		case TL_DEUTSCH:
			text = wxT("Nicht verbunden");
			tempText = wxT("CCS ist beendet");
			break;
		case TL_DANSK:
			text = wxT("Ikke forbundet");
			tempText = wxT("CCS er afsluttet");
			break;
		case TL_FRANCAIS:
			text = wxT("Non connecte");
			tempText = wxT("CCS a pris fin");
			break;
		case TL_ITALIANO:
			text = wxT("Non connesso");
			tempText = wxT("CCS e finita");
			break;
		case TL_POLSKI:
			text = wxT("Nie polaczony");
			tempText = wxT("CCS zakonczyl");
			break;
		case TL_ESPANOL:
			text = wxT("No enlazado");
			tempText = wxT("CCS ha terminado");
			break;
		case TL_SVENSKA:
			text = wxT("Ej lankad");
			tempText = wxT("CCS har upphort");
			break;
		case TL_NEDERLANDS_NL:
		case TL_NEDERLANDS_BE:
			text = wxT("Niet gelinkt");
			tempText = wxT("CCS is afgelopen");
			break;
		case TL_NORSK:
			text = wxT("Ikke linket");
			tempText = wxT("CCS er avsluttet");
			break;
		case TL_PORTUGUES:
			text = wxT("Desconectado");
			tempText = wxT("CCS terminou");
			break;
		default:
			text = wxT("Not linked");
			tempText = wxT("CCS has ended");
			break;
	}

	if (direction == DIR_OUTGOING) {
		m_linkStatus = LS_NONE;
		m_linkRepeater.Clear();
		m_queryTimer.stop();

		bool res = restoreLinks();
		if (!res) {
			CTextData textData1(m_linkStatus, m_linkRepeater, tempText, m_address, m_port, true);
			m_repeaterHandler->writeText(textData1);

			CTextData textData2(m_linkStatus, m_linkRepeater, text, m_address, m_port);
			m_repeaterHandler->writeText(textData2);

			m_audio->setStatus(m_linkStatus, m_linkRepeater, text);
			m_audio->setTempStatus(m_linkStatus, m_linkRepeater, tempText);
			triggerInfo();
		}
	} else {
		CTextData textData(m_linkStatus, m_linkRepeater, tempText, m_address, m_port, true);
		m_repeaterHandler->writeText(textData);

		m_audio->setTempStatus(m_linkStatus, m_linkRepeater, tempText);
		triggerInfo();
	}
}

void CRepeaterHandler::ccsLinkFailed(const wxString& dtmf, DIRECTION direction)
{
	wxString tempText;
	wxString text;

	switch (m_language) {
		case TL_DEUTSCH:
			text = wxT("Nicht verbunden");
			tempText.Printf(wxT("%s unbekannt"), dtmf.c_str());
			break;
		case TL_DANSK:
			text = wxT("Ikke forbundet");
			tempText.Printf(wxT("%s unknown"), dtmf.c_str());
			break;
		case TL_FRANCAIS:
			text = wxT("Non connecte");
			tempText.Printf(wxT("%s inconnu"), dtmf.c_str());
			break;
		case TL_ITALIANO:
			text = wxT("Non connesso");
			tempText.Printf(wxT("Sconosciuto %s"), dtmf.c_str());
			break;
		case TL_POLSKI:
			text = wxT("Nie polaczony");
			tempText.Printf(wxT("%s nieznany"), dtmf.c_str());
			break;
		case TL_ESPANOL:
			text = wxT("No enlazado");
			tempText.Printf(wxT("Desconocido %s"), dtmf.c_str());
			break;
		case TL_SVENSKA:
			text = wxT("Ej lankad");
			tempText.Printf(wxT("%s okand"), dtmf.c_str());
			break;
		case TL_NEDERLANDS_NL:
		case TL_NEDERLANDS_BE:
			text = wxT("Niet gelinkt");
			tempText.Printf(wxT("%s bekend"), dtmf.c_str());
			break;
		case TL_NORSK:
			text = wxT("Ikke linket");
			tempText.Printf(wxT("%s ukjent"), dtmf.c_str());
			break;
		case TL_PORTUGUES:
			text = wxT("Desconectado");
			tempText.Printf(wxT("%s desconhecido"), dtmf.c_str());
			break;
		default:
			text = wxT("Not linked");
			tempText.Printf(wxT("%s unknown"), dtmf.c_str());
			break;
	}

	if (direction == DIR_OUTGOING) {
		m_linkStatus = LS_NONE;
		m_linkRepeater.Clear();
		m_queryTimer.stop();

		bool res = restoreLinks();
		if (!res) {
			CTextData textData1(m_linkStatus, m_linkRepeater, tempText, m_address, m_port, true);
			m_repeaterHandler->writeText(textData1);

			CTextData textData2(m_linkStatus, m_linkRepeater, text, m_address, m_port);
			m_repeaterHandler->writeText(textData2);

			m_audio->setStatus(m_linkStatus, m_linkRepeater, text);
			m_audio->setTempStatus(m_linkStatus, m_linkRepeater, tempText);
			triggerInfo();
		}
	} else {
		CTextData textData(m_linkStatus, m_linkRepeater, tempText, m_address, m_port, true);
		m_repeaterHandler->writeText(textData);

		m_audio->setTempStatus(m_linkStatus, m_linkRepeater, tempText);
		triggerInfo();
	}
}

void CRepeaterHandler::writeStatus(CStatusData& statusData)
{
	for (unsigned int i = 0U; i < m_maxRepeaters; i++) {
		if (m_repeaters[i] != NULL) {
			statusData.setDestination(m_repeaters[i]->m_address, m_repeaters[i]->m_port);
			m_repeaters[i]->m_repeaterHandler->writeStatus(statusData);
		}
	}
}

void CRepeaterHandler::sendHeard(const wxString& text)
{
	if (m_irc == NULL)
		return;

	wxString destination;

	if (m_g2Status == G2_OK) {
		destination = m_g2Repeater;
	} else if (m_g2Status == G2_NONE && (m_linkStatus == LS_LINKED_DPLUS || m_linkStatus == LS_LINKED_DEXTRA || m_linkStatus == LS_LINKED_DCS)) {
		if (m_linkRepeater.Left(3U).IsSameAs(wxT("REF")) || m_linkRepeater.Left(3U).IsSameAs(wxT("XRF")) || m_linkRepeater.Left(3U).IsSameAs(wxT("DCS")))
			destination = m_linkRepeater;
	}

	m_irc->sendHeardWithTXMsg(m_myCall1, m_myCall2, m_yourCall, m_rptCall1, m_rptCall2, m_flag1, m_flag2, m_flag3, destination, text);
}

void CRepeaterHandler::sendStats()
{
	if (m_irc != NULL)
		m_irc->sendHeardWithTXStats(m_myCall1, m_myCall2, m_yourCall, m_rptCall1, m_rptCall2, m_flag1, m_flag2, m_flag3, m_frames, m_silence, m_errors / 2U);
}

void CRepeaterHandler::suspendLinks()
{
	if (m_linkStatus == LS_LINKING_DCS      || m_linkStatus == LS_LINKED_DCS    ||
        m_linkStatus == LS_LINKING_DEXTRA   || m_linkStatus == LS_LINKED_DEXTRA ||
	    m_linkStatus == LS_LINKING_DPLUS    || m_linkStatus == LS_LINKED_DPLUS  ||
		m_linkStatus == LS_LINKING_LOOPBACK || m_linkStatus == LS_LINKED_LOOPBACK) {
		m_lastReflector = m_linkRepeater;
		m_lastReflector.Trim();
	}

	CDPlusHandler::unlink(this);
	CDExtraHandler::unlink(this);
	CDCSHandler::unlink(this);

	m_linkStatus = LS_NONE;
	m_linkRepeater.Clear();
	m_linkReconnectTimer.stop();

	m_ccsHandler->setReflector();
}

bool CRepeaterHandler::restoreLinks()
{
	if (m_linkReconnect == RECONNECT_FIXED) {
		if (!m_lastReflector.IsEmpty()) {
			linkInt(m_linkStartup);
			m_lastReflector.Clear();
			return true;
		}
	} else if (m_linkReconnect == RECONNECT_NEVER) {
		if (!m_lastReflector.IsEmpty()) {
			linkInt(m_lastReflector);
			m_lastReflector.Clear();
			return true;
		}
	} else {
		m_linkReconnectTimer.start();
		if (!m_lastReflector.IsEmpty()) {
			linkInt(m_lastReflector);
			m_lastReflector.Clear();
			return true;
		}
	}

	m_lastReflector.Clear();
	return false;
}

void CRepeaterHandler::triggerInfo()
{
	if (!m_infoEnabled)
		return;

	// Either send the audio now, or queue it until the end of the transmission
	if (m_repeaterId != 0x00U || m_busyId != 0x00U) {
		m_infoNeeded = true;
	} else {
		m_audio->sendStatus();
		m_infoNeeded = false;
	}
}
