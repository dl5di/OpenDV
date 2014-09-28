/*
 *   Copyright (C) 2009-2014 by Jonathan Naylor G4KLX
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

#ifndef	AnalogueRepeaterPreferences_H
#define	AnalogueRepeaterPreferences_H

#include "AnalogueRepeaterControllerSet.h"
#include "AnalogueRepeaterCallsignSet.h"
#include "AnalogueRepeaterExternalSet.h"
#include "AnalogueRepeaterRadioSet.h"
#include "AnalogueRepeaterDTMF1Set.h"
#include "AnalogueRepeaterDTMF2Set.h"
#include "AnalogueRepeaterTonesSet.h"
#include "AnalogueRepeaterTimesSet.h"
#include "AnalogueRepeaterFeelSet.h"
#include "AnalogueRepeaterAPRSSet.h"
#include "AnalogueRepeaterAckSet.h"
#include "AnalogueRepeaterDefs.h"
#include "AnalogueDefines.h"
#include "ActiveHangSet.h"

#include <wx/wx.h>

class CAnalogueRepeaterPreferences : public wxDialog {
public:
	CAnalogueRepeaterPreferences(wxWindow* parent, int id, AUDIO_SOURCE openIdSource, const wxString& openId,
		AUDIO_SOURCE closeIdSource, const wxString& closeId, AUDIO_SOURCE beacon1Source, const wxString& beacon1,
		AUDIO_SOURCE beacon2Source, const wxString& beacon2, unsigned int idSpeed, unsigned int idFreq,
		wxFloat32 idLevel1, wxFloat32 idLevel2, AUDIO_SOURCE ackRadioSource, const wxString& ackRadio,
		AUDIO_SOURCE ackExternalSource, const wxString& ackExternal, AUDIO_SOURCE ackBatterySource,
		const wxString& ackBattery, unsigned int ackSpeed, unsigned int ackFreq, unsigned int ackDelay,
		unsigned int ackMinimum, wxFloat32 ackLevel, unsigned int callsignTime, unsigned int timeout,
		unsigned int lockoutTime, unsigned int hangTime, unsigned int latchTime, bool tbEnable,
		bool ctcssInternal, wxFloat32 tbThreshold, wxFloat32 ctcssFreq, wxFloat32 ctcssThresh,
		wxFloat32 ctcssLevel, unsigned int ctcssHangTime, ANALOGUE_CTCSS_OUTPUT ctcssOutput,
		ANALOGUE_CALLSIGN_START callsignAtStart, unsigned int callsignStartDelay, ANALOGUE_TIMEOUT_TYPE timeoutType,
		bool callsignAtEnd, ANALOGUE_CALLSIGN_HOLDOFF callsignHoldoff, const wxString& radioReadDevice,
		const wxString& radioWriteDevice, unsigned int radioAudioDelay, bool radioDeEmphasis,
		bool radioPreEmphasis, bool radioVOGAD, ANALOGUE_EXTERNAL_MODE externalMode, bool externalVOGAD,
		const wxString& externalReadDevice, const wxString& externalWriteDevice, unsigned int externalAudioDelay,
		bool externalDeEmphasis, bool externalPreEmphasis, const wxString& externalDevice,
		SERIALPIN externalTXPin, SERIALPIN externalRXPin, bool background, const wxString& interfaceType,
		unsigned int interfaceConfig, unsigned int pttDelay, unsigned int squelchDelay, bool pttInvert,
		bool squelchInvert, bool dtmfRadio, bool dtmfExternal, const wxString& dtmfShutdown,
		const wxString& dtmfStartup, const wxString& dtmfTimeout, const wxString& dtmfTimeReset,
		const wxString& dtmfOutput1, const wxString& dtmfOutput2, const wxString& dtmfOutput3,
		const wxString& dtmfOutput4, const wxString& dtmfCommand1, const wxString& dtmfCommand1Line,
		const wxString& dtmfCommand2, const wxString& dtmfCommand2Line, wxFloat32 dtmfThreshold,
		bool aprsTxEnabled, const wxString& aprsCallsign, wxFloat32 aprsLatitude, wxFloat32 aprsLongitude,
		int aprsHeight, const wxString& aprsDescription, unsigned int activeHangTime);
	virtual ~CAnalogueRepeaterPreferences();

	virtual bool Validate();

	virtual AUDIO_SOURCE getOpenIdSource() const;
	virtual wxString     getOpenId() const;
	virtual AUDIO_SOURCE getCloseIdSource() const;
	virtual wxString     getCloseId() const;
	virtual AUDIO_SOURCE getBeacon1Source() const;
	virtual wxString     getBeacon1() const;
	virtual AUDIO_SOURCE getBeacon2Source() const;
	virtual wxString     getBeacon2() const;
	virtual unsigned int getIdSpeed() const;
	virtual unsigned int getIdFreq() const;
	virtual wxFloat32    getIdLevel1() const;
	virtual wxFloat32    getIdLevel2() const;

	virtual AUDIO_SOURCE getRadioAckSource() const;
	virtual wxString     getRadioAck() const;
	virtual AUDIO_SOURCE getExternalAckSource() const;
	virtual wxString     getExternalAck() const;
	virtual AUDIO_SOURCE getBatteryAckSource() const;
	virtual wxString     getBatteryAck() const;
	virtual unsigned int getAckSpeed() const;
	virtual unsigned int getAckFreq() const;
	virtual wxFloat32    getAckLevel() const;
	virtual unsigned int getAckDelay() const;
	virtual unsigned int getAckMinimum() const;

	virtual unsigned int getCallsignTime() const;
	virtual unsigned int getTimeout() const;
	virtual unsigned int getLockoutTime() const;
	virtual unsigned int getHangTime() const;
	virtual unsigned int getLatchTime() const;

	virtual bool                  getTbEnable() const;
	virtual wxFloat32             getTbThreshold() const;
	virtual wxFloat32             getCTCSSFreq() const;
	virtual bool                  getCTCSSInternal() const;
	virtual wxFloat32             getCTCSSThresh() const;
	virtual wxFloat32             getCTCSSLevel() const;
	virtual unsigned int          getCTCSSHangTime() const;
	virtual ANALOGUE_CTCSS_OUTPUT getCTCSSOutput() const;

	virtual ANALOGUE_CALLSIGN_START   getCallAtStart() const;
	virtual unsigned int              getCallStartDelay() const;
	virtual bool                      getCallAtEnd() const;
	virtual ANALOGUE_TIMEOUT_TYPE     getTimeoutType() const;
	virtual ANALOGUE_CALLSIGN_HOLDOFF getHoldoff() const;

	virtual wxString     getRadioReadDevice() const;
	virtual wxString     getRadioWriteDevice() const;
	virtual unsigned int getRadioAudioDelay() const;
	virtual bool         getRadioDeEmphasis() const;
	virtual bool         getRadioPreEmphasis() const;
	virtual bool         getRadioVOGAD() const;

	virtual ANALOGUE_EXTERNAL_MODE getExternalMode() const;
	virtual wxString               getExternalReadDevice() const;
	virtual wxString               getExternalWriteDevice() const;
	virtual unsigned int           getExternalAudioDelay() const;
	virtual bool                   getExternalDeEmphasis() const;
	virtual bool                   getExternalPreEmphasis() const;
	virtual bool                   getExternalVOGAD() const;
	virtual wxString               getExternalDevice() const;
	virtual SERIALPIN              getExternalTXPin() const;
	virtual SERIALPIN              getExternalRXPin() const;
	virtual bool                   getExternalBackground() const;

	virtual wxString     getInterfaceType() const;
	virtual unsigned int getInterfaceConfig() const;
	virtual unsigned int getPTTDelay() const;
	virtual unsigned int getSquelchDelay() const;
	virtual bool         getPTTInvert() const;
	virtual bool         getSquelchInvert() const;

	virtual bool      getDTMFRadio() const;
	virtual bool      getDTMFExternal() const;
	virtual wxString  getDTMFShutdown() const;
	virtual wxString  getDTMFStartup() const;
	virtual wxString  getDTMFTimeout() const;
	virtual wxString  getDTMFTimeReset() const;
	virtual wxString  getDTMFOutput1() const;
	virtual wxString  getDTMFOutput2() const;
	virtual wxString  getDTMFOutput3() const;
	virtual wxString  getDTMFOutput4() const;
	virtual wxFloat32 getDTMFThreshold() const;
	virtual wxString  getDTMFCommand1() const;
	virtual wxString  getDTMFCommand1Line() const;
	virtual wxString  getDTMFCommand2() const;
	virtual wxString  getDTMFCommand2Line() const;

	virtual bool      getAPRSTXEnabled() const;
	virtual wxString  getAPRSCallsign() const;
	virtual wxFloat32 getAPRSLatitude() const;
	virtual wxFloat32 getAPRSLongitude() const;
	virtual int       getAPRSHeight() const;
	virtual wxString  getAPRSDescription() const;

	virtual unsigned int getActiveHangTime() const;

private:
	CAnalogueRepeaterCallsignSet*    m_callsign;
	CAnalogueRepeaterAckSet*         m_ack;
	CAnalogueRepeaterTimesSet*       m_times;
	CAnalogueRepeaterTonesSet*       m_tones;
	CAnalogueRepeaterFeelSet*        m_feel;
	CAnalogueRepeaterRadioSet*       m_radio;
	CAnalogueRepeaterExternalSet*    m_external;
	CAnalogueRepeaterControllerSet*  m_controller;
	CAnalogueRepeaterDTMF1Set*       m_dtmf1;
	CAnalogueRepeaterDTMF2Set*       m_dtmf2;
	CAnalogueRepeaterAPRSSet*        m_aprs;
	CActiveHangSet*                  m_active;
};

#endif
