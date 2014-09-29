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

#include "DVAPNodePreferences.h"
#include "DStarDefines.h"

const unsigned int BORDER_SIZE   = 5U;

#include <wx/gbsizer.h>
#include <wx/notebook.h>

CDVAPNodePreferences::CDVAPNodePreferences(wxWindow* parent, int id, const wxString& callsign, const wxString& gateway,
	DSTAR_MODE mode, ACK_TYPE ack, bool restriction, bool rpt1Validation, const wxString& gatewayAddress,
	unsigned int gatewayPort, const wxString& localAddress, unsigned int localPort, unsigned int timeout, unsigned int ackTime,
	unsigned int beaconTime, const wxString& beaconText, bool beaconVoice, TEXT_LANG language, bool announcementEnabled,
	unsigned int announcementTime, const wxString& announcementRecordRPT1, const wxString& announcementRecordRPT2,
	const wxString& announcementDeleteRPT1, const wxString& announcementDeleteRPT2, const wxString& port, unsigned int frequency,
	int power, int squelch) :
wxDialog(parent, id, wxString(_("DVAP Node Preferences"))),
m_callsign(NULL),
m_network(NULL),
m_times(NULL),
m_beacon(NULL),
m_announcement(NULL),
m_dvap(NULL)
{
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	wxNotebook* noteBook = new wxNotebook(this, -1);

	m_callsign = new CDVAPNodeCallsignSet(noteBook, -1, APPLICATION_NAME, callsign, gateway, mode, ack, restriction, rpt1Validation);
	noteBook->AddPage(m_callsign, _("Callsign"), true);

	m_network = new CNetworkSet(noteBook, -1, APPLICATION_NAME, gatewayAddress, gatewayPort, localAddress, localPort);
	noteBook->AddPage(m_network, _("Network"), false);

	m_times = new CDVAPNodeTimesSet(noteBook, -1, APPLICATION_NAME, timeout, ackTime);
	noteBook->AddPage(m_times, _("Timers"), false);

	m_beacon = new CBeaconSet(noteBook, -1, APPLICATION_NAME, beaconTime, beaconText, beaconVoice, language);
	noteBook->AddPage(m_beacon, _("Beacon"), false);

	m_announcement = new CAnnouncementSet(noteBook, -1, APPLICATION_NAME, announcementEnabled, announcementTime, announcementRecordRPT1, announcementRecordRPT2, announcementDeleteRPT1, announcementDeleteRPT2);
	noteBook->AddPage(m_announcement, _("Announcement"), false);

	m_dvap = new CDVAPNodeDVAPSet(noteBook, -1, APPLICATION_NAME, port, frequency, power, squelch);
	noteBook->AddPage(m_dvap, wxT("DVAP"), false);

	mainSizer->Add(noteBook, 1, wxALL | wxGROW, BORDER_SIZE);

	mainSizer->Add(CreateButtonSizer(wxOK | wxCANCEL), 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	SetAutoLayout(true);
	Layout();

	mainSizer->Fit(this);
	mainSizer->SetSizeHints(this);

	SetSizer(mainSizer);
}

CDVAPNodePreferences::~CDVAPNodePreferences()
{
}

bool CDVAPNodePreferences::Validate()
{
	if (!m_callsign->Validate() || !m_network->Validate() || !m_times->Validate() || !m_beacon->Validate() || !m_announcement->Validate() || !m_dvap->Validate())
		return false;

	return true;	
}

wxString CDVAPNodePreferences::getCallsign() const
{
	return m_callsign->getCallsign();
}

wxString CDVAPNodePreferences::getGateway() const
{
	return m_callsign->getGateway();
}

DSTAR_MODE CDVAPNodePreferences::getMode() const
{
	return m_callsign->getMode();
}

ACK_TYPE CDVAPNodePreferences::getAck() const
{
	return m_callsign->getAck();
}

bool CDVAPNodePreferences::getRestriction() const
{
	return m_callsign->getRestriction();
}

bool CDVAPNodePreferences::getRPT1Validation() const
{
	return m_callsign->getRPT1Validation();
}

wxString CDVAPNodePreferences::getGatewayAddress() const
{
	return m_network->getGatewayAddress();
}

unsigned int CDVAPNodePreferences::getGatewayPort() const
{
	return m_network->getGatewayPort();
}

wxString CDVAPNodePreferences::getLocalAddress() const
{
	return m_network->getLocalAddress();
}

unsigned int CDVAPNodePreferences::getLocalPort() const
{
	return m_network->getLocalPort();
}

unsigned int CDVAPNodePreferences::getTimeout() const
{
	return m_times->getTimeout();
}

unsigned int CDVAPNodePreferences::getAckTime() const
{
	return m_times->getAckTime();
}

unsigned int CDVAPNodePreferences::getBeaconTime() const
{
	return m_beacon->getTime();
}

wxString CDVAPNodePreferences::getBeaconText() const
{
	return m_beacon->getText();
}

bool CDVAPNodePreferences::getBeaconVoice() const
{
	return m_beacon->getVoice();
}

TEXT_LANG CDVAPNodePreferences::getLanguage() const
{
	return m_beacon->getLanguage();
}

bool CDVAPNodePreferences::getAnnouncementEnabled() const
{
	return m_announcement->getEnabled();
}

unsigned int CDVAPNodePreferences::getAnnouncementTime() const
{
	return m_announcement->getTime();
}

wxString CDVAPNodePreferences::getAnnouncementRecordRPT1() const
{
	return m_announcement->getRecordRPT1();
}

wxString CDVAPNodePreferences::getAnnouncementRecordRPT2() const
{
	return m_announcement->getRecordRPT2();
}

wxString CDVAPNodePreferences::getAnnouncementDeleteRPT1() const
{
	return m_announcement->getDeleteRPT1();
}

wxString CDVAPNodePreferences::getAnnouncementDeleteRPT2() const
{
	return m_announcement->getDeleteRPT2();
}

wxString CDVAPNodePreferences::getPort() const
{
	return m_dvap->getPort();
}

unsigned int CDVAPNodePreferences::getFrequency() const
{
	return m_dvap->getFrequency();
}

int CDVAPNodePreferences::getPower() const
{
	return m_dvap->getPower();
}

int CDVAPNodePreferences::getSquelch() const
{
	return m_dvap->getSquelch();
}
