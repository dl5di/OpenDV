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

#ifndef	DVAPNodePreferences_H
#define	DVAPNodePreferences_H

#include "DVAPNodeCallsignSet.h"
#include "DVAPNodeTimesSet.h"
#include "AnnouncementSet.h"
#include "DVAPNodeDVAPSet.h"
#include "DVAPNodeDefs.h"
#include "DStarDefines.h"
#include "NetworkSet.h"
#include "BeaconSet.h"

#include <wx/wx.h>

class CDVAPNodePreferences : public wxDialog {
public:
	CDVAPNodePreferences(wxWindow* parent, int id, const wxString& callsign, const wxString& gateway,
		DSTAR_MODE mode, ACK_TYPE ack, bool restriction, bool rpt1Validation, const wxString& gatewayAddress, 
		unsigned int gatewayPort, const wxString& localAddress, unsigned int localPort, unsigned int timeout,
		unsigned int ackTime, unsigned int beaconTime, const wxString& beaconText, bool beaconVoice,
		TEXT_LANG language, bool announcementEnabled, unsigned int announcementTime, const wxString& announcementRecordRPT1,
		const wxString& announcementRecordRPT2, const wxString& announcementDeleteRPT1, const wxString& announcementDeleteRPT2,
		const wxString& port, unsigned int frequency, int power, int squelch);
	virtual ~CDVAPNodePreferences();

	virtual bool Validate();

	virtual wxString     getCallsign() const;
	virtual wxString     getGateway() const;
	virtual DSTAR_MODE   getMode() const;
	virtual ACK_TYPE     getAck() const;
	virtual bool         getRestriction() const;
	virtual bool         getRPT1Validation() const;

	virtual wxString     getGatewayAddress() const;
	virtual unsigned int getGatewayPort() const;
	virtual wxString     getLocalAddress() const;
	virtual unsigned int getLocalPort() const;

	virtual unsigned int getTimeout() const;
	virtual unsigned int getAckTime() const;

	virtual unsigned int getBeaconTime() const;
	virtual wxString     getBeaconText() const;
	virtual bool         getBeaconVoice() const;
	virtual TEXT_LANG    getLanguage() const;

	virtual bool         getAnnouncementEnabled() const;
	virtual unsigned int getAnnouncementTime() const;
	virtual wxString     getAnnouncementRecordRPT1() const;
	virtual wxString     getAnnouncementRecordRPT2() const;
	virtual wxString     getAnnouncementDeleteRPT1() const;
	virtual wxString     getAnnouncementDeleteRPT2() const;

	virtual wxString     getPort() const;
	virtual unsigned int getFrequency() const;
	virtual int          getPower() const;
	virtual int          getSquelch() const;

private:
	CDVAPNodeCallsignSet* m_callsign;
	CNetworkSet*          m_network;
	CDVAPNodeTimesSet*    m_times;
	CBeaconSet*           m_beacon;
	CAnnouncementSet*     m_announcement;
	CDVAPNodeDVAPSet*     m_dvap;
};

#endif
