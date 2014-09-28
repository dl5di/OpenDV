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

#include "APRSWriter.h"

#include "DStarDefines.h"
#include "Defs.h"

CAPRSEntry::CAPRSEntry(const wxString& callsign, const wxString& band, double frequency, double offset, double range, double latitude, double longitude, double agl) :
m_callsign(callsign),
m_band(band),
m_frequency(frequency),
m_offset(offset),
m_range(range),
m_latitude(latitude),
m_longitude(longitude),
m_agl(agl),
m_timer(1000U, 10U),
m_first(true),
m_collector(NULL)
{
	m_callsign.Trim();

	m_collector = new CAPRSCollector;
}

CAPRSEntry::~CAPRSEntry()
{
	delete m_collector;
}

wxString CAPRSEntry::getCallsign() const
{
	return m_callsign;
}

wxString CAPRSEntry::getBand() const
{
	return m_band;
}

double CAPRSEntry::getFrequency() const
{
	return m_frequency;
}

double CAPRSEntry::getOffset() const
{
	return m_offset;
}

double CAPRSEntry::getRange() const
{
	return m_range;
}

double CAPRSEntry::getLatitude() const
{
	return m_latitude;
}

double CAPRSEntry::getLongitude() const
{
	return m_longitude;
}

double CAPRSEntry::getAGL() const
{
	return m_agl;
}

CAPRSCollector* CAPRSEntry::getCollector() const
{
	return m_collector;
}

void CAPRSEntry::reset()
{
	m_first = true;
	m_timer.stop();
	m_collector->reset();
}

void CAPRSEntry::clock(unsigned int ms)
{
	m_timer.clock(ms);
}

bool CAPRSEntry::isOK()
{
	if (m_first) {
		m_first = false;
		m_timer.start();
		return true;
	}

	if (m_timer.hasExpired()) {
		m_timer.start();
		return true;
	} else {
		m_timer.start();
		return false;
	}
}

CAPRSWriter::CAPRSWriter(const wxString& hostname, unsigned int port, const wxString& gateway, const wxString& address) :
m_thread(NULL),
m_enabled(false),
m_idTimer(1000U, 20U * 60U),		// 20 minutes
m_gateway(),
m_array()
{
	wxASSERT(!hostname.IsEmpty());
	wxASSERT(port > 0U);
	wxASSERT(!gateway.IsEmpty());

	m_thread = new CAPRSWriterThread(gateway, address, hostname, port);

	m_gateway = gateway;
	m_gateway.Truncate(LONG_CALLSIGN_LENGTH - 1U);
	m_gateway.Trim();
}

CAPRSWriter::~CAPRSWriter()
{
	for (CEntry_t::iterator it = m_array.begin(); it != m_array.end(); ++it)
		delete it->second;

	m_array.clear();
}

void CAPRSWriter::setPort(const wxString& callsign, const wxString& band, double frequency, double offset, double range, double latitude, double longitude, double agl)
{
	wxString temp = callsign;
	temp.resize(LONG_CALLSIGN_LENGTH - 1U, wxT(' '));
	temp.Append(band);

	m_array[temp] = new CAPRSEntry(callsign, band, frequency, offset, range, latitude, longitude, agl);
}

bool CAPRSWriter::open()
{
	return m_thread->start();
}

void CAPRSWriter::writeData(const wxString& callsign, const CAMBEData& data)
{
	if (data.isEnd())
		return;

	CAPRSEntry* entry = m_array[callsign];
	if (entry == NULL) {
		wxLogError(wxT("Cannot find the callsign \"%s\" in the APRS array"), callsign.c_str());
		return;
	}

	CAPRSCollector* collector = entry->getCollector();

	if (data.isSync()) {
		collector->sync();
		return;
	}

	unsigned char buffer[400U];
	data.getData(buffer, DV_FRAME_MAX_LENGTH_BYTES);

	bool complete = collector->writeData(buffer + VOICE_FRAME_LENGTH_BYTES);
	if (!complete)
		return;

	if (!m_enabled) {
		collector->reset();
		return;
	}

	if (!m_thread->isConnected()) {
		collector->reset();
		return;
	}

	// Check the transmission timer
	bool ok = entry->isOK();
	if (!ok) {
		collector->reset();
		return;
	}

	unsigned int length = collector->getData(buffer, 400U);
	wxString text((char*)buffer, wxConvLocal, length);

	int n = text.Find(wxT(':'));
	if (n == wxNOT_FOUND) {
		collector->reset();
		return;
	}

	wxString header = text.Left(n);
	wxString body   = text.Mid(n + 1U);

	// If we already have a q-construct, don't send it on
	n = header.Find(wxT('q'));
	if (n != wxNOT_FOUND)
		return;

	// Remove the trailing \r
	n = body.Find(wxT('\r'));
	if (n != wxNOT_FOUND)
		body = body.Left(n);

	wxString output;
	output.Printf(wxT("%s,qAR,%s-%s:%s"), header.c_str(), entry->getCallsign().c_str(), entry->getBand().c_str(), body.c_str());

	char ascii[500U];
	::memset(ascii, 0x00, 500U);
	for (unsigned int i = 0U; i < output.Len(); i++)
		ascii[i] = output.GetChar(i);

	m_thread->write(ascii);

	collector->reset();
}

void CAPRSWriter::reset(const wxString& callsign)
{
	CAPRSEntry* entry = m_array[callsign];
	if (entry == NULL) {
		wxLogError(wxT("Cannot find the callsign \"%s\" in the APRS array"), callsign.c_str());
		return;
	}

	entry->reset();
}

void CAPRSWriter::setEnabled(bool enabled)
{
	m_enabled = enabled;

	if (m_enabled) {
		sendIdFrames();
		m_idTimer.start();
	}
}

void CAPRSWriter::clock(unsigned int ms)
{
	m_idTimer.clock(ms);

	if (m_idTimer.hasExpired()) {
		sendIdFrames();
		m_idTimer.start();
	}

	for (CEntry_t::iterator it = m_array.begin(); it != m_array.end(); ++it)
		it->second->clock(ms);
}

bool CAPRSWriter::isConnected() const
{
	return m_thread->isConnected();
}

void CAPRSWriter::close()
{
	m_thread->stop();
}

void CAPRSWriter::sendIdFrames()
{
	if (!m_thread->isConnected())
		return;

	time_t now;
	::time(&now);
	struct tm* tm = ::gmtime(&now);

	for (CEntry_t::iterator it = m_array.begin(); it != m_array.end(); ++it) {
		CAPRSEntry* entry = it->second;
		if (entry == NULL)
			continue;

		// Default values aren't passed on
		if (entry->getLatitude() == 0.0 && entry->getLongitude() == 0.0)
			continue;

		wxString desc;
		if (entry->getBand().Len() > 1U) {
			if (entry->getFrequency() != 0.0)
				desc.Printf(wxT("Data %.5lfMHz"), entry->getFrequency());
			else
				desc = wxT("Data");
		} else {
			if (entry->getFrequency() != 0.0)
				desc.Printf(wxT("Voice %.5lfMHz %c%.4lfMHz"),
						entry->getFrequency(),
						entry->getOffset() < 0.0 ? wxT('-') : wxT('+'),
						::fabs(entry->getOffset()));
			else
				desc = wxT("Voice");
		}

		wxString band;
		if (entry->getFrequency() >= 1200.0)
			band = wxT("1.2");
		else if (entry->getFrequency() >= 420.0)
			band = wxT("440");
		else if (entry->getFrequency() >= 144.0)
			band = wxT("2m");
		else if (entry->getFrequency() >= 50.0)
			band = wxT("6m");
		else if (entry->getFrequency() >= 28.0)
			band = wxT("10m");

		double tempLat  = ::fabs(entry->getLatitude());
		double tempLong = ::fabs(entry->getLongitude());

		double latitude  = ::floor(tempLat);
		double longitude = ::floor(tempLong);

		latitude  = (tempLat  - latitude)  * 60.0 + latitude  * 100.0;
		longitude = (tempLong - longitude) * 60.0 + longitude * 100.0;

		wxString lat;
		if (latitude >= 1000.0F)
			lat.Printf(wxT("%.2lf"), latitude);
		else if (latitude >= 100.0F)
			lat.Printf(wxT("0%.2lf"), latitude);
		else if (latitude >= 10.0F)
			lat.Printf(wxT("00%.2lf"), latitude);
		else
			lat.Printf(wxT("000%.2lf"), latitude);

		wxString lon;
		if (longitude >= 10000.0F)
			lon.Printf(wxT("%.2lf"), longitude);
		else if (longitude >= 1000.0F)
			lon.Printf(wxT("0%.2lf"), longitude);
		else if (longitude >= 100.0F)
			lon.Printf(wxT("00%.2lf"), longitude);
		else if (longitude >= 10.0F)
			lon.Printf(wxT("000%.2lf"), longitude);
		else
			lon.Printf(wxT("0000%.2lf"), longitude);

		// Convert commas to periods in the latitude and longitude
		lat.Replace(wxT(","), wxT("."));
		lon.Replace(wxT(","), wxT("."));

		wxString output;
		output.Printf(wxT("%s-S>APDG01,TCPIP*,qAC,%s-GS:;%-7s%-2s*%02d%02d%02dz%s%cD%s%caRNG%04.0lf %s %s"),
			m_gateway.c_str(), m_gateway.c_str(), entry->getCallsign().c_str(), entry->getBand().c_str(),
			tm->tm_mday, tm->tm_hour, tm->tm_min,
			lat.c_str(), (entry->getLatitude() < 0.0F)  ? wxT('S') : wxT('N'),
			lon.c_str(), (entry->getLongitude() < 0.0F) ? wxT('W') : wxT('E'),
			entry->getRange() * 0.6214, band.c_str(), desc.c_str());

		char ascii[300U];
		::memset(ascii, 0x00, 300U);
		for (unsigned int i = 0U; i < output.Len(); i++)
			ascii[i] = output.GetChar(i);

		m_thread->write(ascii);

		if (entry->getBand().Len() == 1U) {
			output.Printf(wxT("%s-%s>APDG02,TCPIP*,qAC,%s-%sS:!%s%cD%s%c&RNG%04.0lf %s %s"),
				entry->getCallsign().c_str(), entry->getBand().c_str(), entry->getCallsign().c_str(), entry->getBand().c_str(),
				lat.c_str(), (entry->getLatitude() < 0.0F)  ? wxT('S') : wxT('N'),
				lon.c_str(), (entry->getLongitude() < 0.0F) ? wxT('W') : wxT('E'),
				entry->getRange() * 0.6214, band.c_str(), desc.c_str());

			char ascii[300U];
			::memset(ascii, 0x00, 300U);
			for (unsigned int i = 0U; i < output.Len(); i++)
				ascii[i] = output.GetChar(i);

			m_thread->write(ascii);
		}
	}

	m_idTimer.start();
}
