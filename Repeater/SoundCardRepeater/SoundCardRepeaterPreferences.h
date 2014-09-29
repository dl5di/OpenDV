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

#ifndef	SoundCardRepeaterPreferences_H
#define	SoundCardRepeaterPreferences_H

#include "SoundCardRepeaterControllerSet.h"
#include "SoundCardRepeaterCallsignSet.h"
#include "SoundCardRepeaterRadioSet.h"
#include "SoundCardRepeaterTimesSet.h"
#include "SoundCardRepeaterDefs.h"
#include "AnnouncementSet.h"
#include "ActiveHangSet.h"
#include "DStarDefines.h"
#include "Control1Set.h"
#include "Control2Set.h"
#include "NetworkSet.h"
#include "BeaconSet.h"

#include <wx/wx.h>

class CSoundCardRepeaterPreferences : public wxDialog {
public:
	CSoundCardRepeaterPreferences(wxWindow* parent, int id, const wxString& callsign, const wxString& gateway,
		DSTAR_MODE mode, ACK_TYPE ack, bool restriction, bool rpt1Validation, bool dtmfBlanking,
		const wxString& gatewayAddress, unsigned int gatewayPort, const wxString& localAddress, unsigned int localPort,
		unsigned int timeout, unsigned int ackTime, unsigned int hangTime, unsigned int beaconTime,
		const wxString& beaconText, bool beaconVoice, TEXT_LANG language, bool announcementEnabled,
		unsigned int announcementTime, const wxString& announcementRecordRPT1, const wxString& announcementRecordRPT2,
		const wxString& announcementDeleteRPT1, const wxString& announcementDeleteRPT2, const wxString& readDevice,
		const wxString& writeDevice, bool rxInvert, bool txInvert, wxFloat32 rxLevel, wxFloat32 txLevel,
		SQUELCH_MODE squelchMode, wxFloat32 squelchLevel, const wxString& interfaceType, unsigned int interfaceConfig,
		int pttDelay, bool pttInvert, bool enabled, const wxString& rpt1Callsign, const wxString& rpt2Callsign,
		const wxString& shutdown, const wxString& startup, const wxString& status1, const wxString& status2,
		const wxString& status3, const wxString& status4,const wxString& status5, const wxString& command1,
		const wxString& command1Line, const wxString& command2, const wxString& command2Line,
		const wxString& command3, const wxString& command3Line, const wxString& command4,
		const wxString& command4Line, const wxString& output1, const wxString& output2,
		const wxString& output3, const wxString& output4, unsigned int activeHangTime);
	virtual ~CSoundCardRepeaterPreferences();

	virtual bool Validate();

	virtual wxString     getCallsign() const;
	virtual wxString     getGateway() const;
	virtual DSTAR_MODE   getMode() const;
	virtual ACK_TYPE     getAck() const;
	virtual bool         getRestriction() const;
	virtual bool         getRPT1Validation() const;
	virtual bool         getDTMFBlanking() const;

	virtual wxString     getGatewayAddress() const;
	virtual unsigned int getGatewayPort() const;
	virtual wxString     getLocalAddress() const;
	virtual unsigned int getLocalPort() const;

	virtual unsigned int getTimeout() const;
	virtual unsigned int getAckTime() const;
	virtual unsigned int getHangTime() const;

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

	virtual wxString     getReadDevice() const;
	virtual wxString     getWriteDevice() const;
	virtual bool         getRXInvert() const;
	virtual bool         getTXInvert() const;
	virtual wxFloat32    getRXLevel() const;
	virtual wxFloat32    getTXLevel() const;
	virtual SQUELCH_MODE getSquelchMode() const;
	virtual wxFloat32    getSquelchLevel() const;

	virtual wxString     getType() const;
	virtual unsigned int getConfig() const;
	virtual int          getPTTDelay() const;
	virtual bool         getPTTInvert() const;

	virtual bool         getEnabled() const;
	virtual wxString     getRPT1Callsign() const;
	virtual wxString     getRPT2Callsign() const;
	virtual wxString     getShutdown() const;
	virtual wxString     getStartup() const;
	virtual wxString     getStatus1() const;
	virtual wxString     getStatus2() const;
	virtual wxString     getStatus3() const;
	virtual wxString     getStatus4() const;
	virtual wxString     getStatus5() const;
	virtual wxString     getOutput1() const;
	virtual wxString     getOutput2() const;
	virtual wxString     getOutput3() const;
	virtual wxString     getOutput4() const;

	virtual wxString     getCommand1() const;
	virtual wxString     getCommand1Line() const;
	virtual wxString     getCommand2() const;
	virtual wxString     getCommand2Line() const;
	virtual wxString     getCommand3() const;
	virtual wxString     getCommand3Line() const;
	virtual wxString     getCommand4() const;
	virtual wxString     getCommand4Line() const;

	virtual unsigned int getActiveHangTime() const;

private:
	CSoundCardRepeaterCallsignSet*   m_callsign;
	CNetworkSet*                     m_network;
	CSoundCardRepeaterTimesSet*      m_times;
	CBeaconSet*                      m_beacon;
	CAnnouncementSet*                m_announcement;
	CSoundCardRepeaterRadioSet*      m_radio;
	CSoundCardRepeaterControllerSet* m_controller;
	CControl1Set*                    m_control1;
	CControl2Set*                    m_control2;
	CActiveHangSet*                  m_active;
};

#endif
