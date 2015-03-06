/*
 *   Copyright (C) 2009-2015 by Jonathan Naylor G4KLX
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

#ifndef	AnalogueRepeaterConfig_H
#define	AnalogueRepeaterConfig_H

#include "SerialLineController.h"
#include "AnalogueDefines.h"
#include "TimeoutTones.h"

#include <wx/wx.h>
#include <wx/config.h>
#include <wx/filename.h>

class CAnalogueRepeaterConfig {
public:
#if defined(__WINDOWS__)
	CAnalogueRepeaterConfig(wxConfigBase* config, const wxString& name);
#else
	CAnalogueRepeaterConfig(const wxString& dir, const wxString& name);
#endif
	~CAnalogueRepeaterConfig();

	void getCallsign(AUDIO_SOURCE& openIdSource, wxString& openId, AUDIO_SOURCE& closeIdSource, wxString& closeId, AUDIO_SOURCE& beacon1Source, wxString& beacon1, AUDIO_SOURCE& beacon2Source, wxString& beacon2, unsigned int& speed, unsigned int& freq, wxFloat32& level1, wxFloat32& level2) const;
	void setCallsign(AUDIO_SOURCE openIdSource, const wxString& openId, AUDIO_SOURCE closeIdSource, const wxString& closeId, AUDIO_SOURCE beacon1Source, const wxString& beacon1, AUDIO_SOURCE beacon2Source, const wxString& beacon2, unsigned int speed, unsigned int freq, wxFloat32 level1, wxFloat32 level2);

	void getAck(AUDIO_SOURCE& radioAckSource, wxString& radioAck, AUDIO_SOURCE& extAckSource, wxString& extAck, AUDIO_SOURCE& batteryAckSource, wxString& batteryAck, unsigned int& speed, unsigned int& freq, wxFloat32& level, unsigned int& ack, unsigned int& minimum) const;
	void setAck(AUDIO_SOURCE radioAckSource, const wxString& radioAck, AUDIO_SOURCE extAckSource, const wxString& extAck, AUDIO_SOURCE batteryAckSource, const wxString& batteryAck, unsigned int speed, unsigned int freq, wxFloat32 level, unsigned int ack, unsigned int minimum);

	void getTimes(unsigned int& callsignTime, unsigned int& timeout, unsigned int& lockoutTime, unsigned int& hangTime, unsigned int& latchTime) const;
	void setTimes(unsigned int callsignTime, unsigned int timeout, unsigned int lockoutTime, unsigned int hangTime, unsigned int latchTime);

	void getTones(bool& tbEnable, wxFloat32& tbThreshold, wxFloat32& ctcssFreq, bool& ctcssInternal, wxFloat32& ctcssThresh, wxFloat32& ctcssLevel, unsigned int& ctcssHangTime, ANALOGUE_CTCSS_OUTPUT& ctcssOutput) const;
	void setTones(bool tbEnable, wxFloat32 tbThreshold, wxFloat32 ctcssFreq, bool ctcssInternal, wxFloat32 ctcssThresh, wxFloat32 ctcssLevel, unsigned int ctcssHangTime, ANALOGUE_CTCSS_OUTPUT ctcssOutput);

	void getFeel(ANALOGUE_CALLSIGN_START& callsignAtStart, unsigned int& callsignStartDelay, bool& callsignAtEnd, ANALOGUE_TIMEOUT_TYPE& timeoutType, ANALOGUE_CALLSIGN_HOLDOFF& callsignHoldoff) const;
	void setFeel(ANALOGUE_CALLSIGN_START callsignAtStart, unsigned int callsignStartDelay, bool callsignAtEnd, ANALOGUE_TIMEOUT_TYPE timeoutType, ANALOGUE_CALLSIGN_HOLDOFF callsignHoldoff);

	void getRadio(wxString& readDevice, wxString& writeDevice, unsigned int& delay, bool& deEmphasis, bool& preEmphasis, bool& vogad) const;
	void setRadio(const wxString& readDevice, const wxString& writeDevice, unsigned int delay, bool deEmphasis, bool preEmphasis, bool vogad);

	void getExternal(ANALOGUE_EXTERNAL_MODE& mode, wxString& readDevice, wxString& writeDevice, unsigned int& delay, bool& deEmphasis, bool& preEmphasis, bool& vogad, wxString& device, SERIALPIN& txPin, SERIALPIN& rxPin, bool& background) const;
	void setExternal(ANALOGUE_EXTERNAL_MODE mode, const wxString& readDevice, const wxString& writeDevice, unsigned int delay, bool deEmphasis, bool preEmphasis, bool vogad, const wxString& device, SERIALPIN txPin, SERIALPIN rxPin, bool background);

	void getController(wxString& type, unsigned int& config, unsigned int& pttDelay, unsigned int& squelchDelay, bool& pttInvert, bool& squelchInvert) const;
	void setController(const wxString& type, unsigned int config, unsigned int pttDelay, unsigned int squelchDelay, bool pttInvert, bool squelchInvert);

	void getDTMF(bool& radio, bool& external, wxString& shutdown, wxString& startup, wxString& timeout,
		wxString& timeReset, wxString& command1, wxString& command1Line, wxString& command2, wxString& command2Line,
		wxString& output1, wxString& output2, wxString& output3, wxString& output4, wxFloat32& threshold) const;
	void setDTMF(bool radio, bool external, const wxString& shutdown, const wxString& startup, const wxString& timeout,
		const wxString& timeReset, const wxString& command1, const wxString& command1Line, const wxString& command2,
		const wxString& command2Line, const wxString& output1, const wxString& output2, const wxString& output3,
		const wxString& output4, wxFloat32 threshold);

	void getActiveHang(unsigned int& time) const;
	void setActiveHang(unsigned int time);

	void getOutputs(bool& out1, bool& out2, bool& out3, bool& out4) const;
	void setOutputs(bool out1, bool out2, bool out3, bool out4);

	void getPosition(int& x, int& y) const;
	void setPosition(int x, int y);

	bool write();

private:
#if defined(__WINDOWS__)
	wxConfigBase* m_config;
	wxString      m_name;
#else
	wxFileName    m_fileName;
#endif
	AUDIO_SOURCE  m_openIdSource;
	wxString      m_openId;
	AUDIO_SOURCE  m_closeIdSource;
	wxString      m_closeId;
	AUDIO_SOURCE  m_beacon1Source;
	wxString      m_beacon1;
	AUDIO_SOURCE  m_beacon2Source;
	wxString      m_beacon2;
	unsigned int  m_idSpeed;
	unsigned int  m_idFreq;
	wxFloat32     m_idLevel1;
	wxFloat32     m_idLevel2;
	AUDIO_SOURCE  m_ackRadioSource;
	wxString      m_ackRadio;
	AUDIO_SOURCE  m_ackExternalSource;
	wxString      m_ackExternal;
	AUDIO_SOURCE  m_ackBatterySource;
	wxString      m_ackBattery;
	unsigned int  m_ackSpeed;
	unsigned int  m_ackFreq;
	wxFloat32     m_ackLevel;
	unsigned int  m_ackDelay;
	unsigned int  m_ackMin;
	unsigned int  m_callsignTime;
	unsigned int  m_timeout;
	unsigned int  m_lockoutTime;
	unsigned int  m_hangTime;
	unsigned int  m_latchTime;
	bool          m_toneburstEnable;
	wxFloat32     m_toneburstThreshold;
	wxFloat32     m_ctcssFreq;
	bool          m_ctcssInternal;
	wxFloat32     m_ctcssThresh;
	wxFloat32     m_ctcssLevel;
	unsigned int  m_ctcssHangTime;
	ANALOGUE_CTCSS_OUTPUT     m_ctcssOutput;
	ANALOGUE_CALLSIGN_START   m_callAtStart;
	unsigned int              m_callStartDelay;
	bool                      m_callAtEnd;
	ANALOGUE_TIMEOUT_TYPE     m_timeoutType;
	ANALOGUE_CALLSIGN_HOLDOFF m_callHoldoff;
	wxString      m_radioReadDevice;
	wxString      m_radioWriteDevice;
	unsigned int  m_radioDelay;
	bool          m_radioDeEmphasis;
	bool          m_radioPreEmphasis;
	bool          m_radioVOGAD;
	ANALOGUE_EXTERNAL_MODE m_externalMode;
	wxString      m_externalReadDevice;
	wxString      m_externalWriteDevice;
	unsigned int  m_externalDelay;
	bool          m_externalDeEmphasis;
	bool          m_externalPreEmphasis;
	bool          m_externalVOGAD;
	wxString      m_externalInterfaceDevice;
	SERIALPIN     m_externalInterfaceTXPin;
	SERIALPIN     m_externalInterfaceRXPin;
	bool          m_externalBackground;
	wxString      m_interfaceType;
	unsigned int  m_interfaceConfig;
	unsigned int  m_pttDelay;
	unsigned int  m_squelchDelay;
	bool          m_pttInvert;
	bool          m_squelchInvert;
	bool          m_dtmfRadio;
	bool          m_dtmfExternal;
	wxString      m_dtmfShutdown;
	wxString      m_dtmfStartup;
	wxString      m_dtmfTimeout;
	wxString      m_dtmfTimeReset;
	wxString      m_dtmfCommand1;
	wxString      m_dtmfCommand1Line;
	wxString      m_dtmfCommand2;
	wxString      m_dtmfCommand2Line;
	wxString      m_dtmfOutput1;
	wxString      m_dtmfOutput2;
	wxString      m_dtmfOutput3;
	wxString      m_dtmfOutput4;
	wxFloat32     m_dtmfThreshold;
	unsigned int  m_activeHangTime;
	bool          m_output1;
	bool          m_output2;
	bool          m_output3;
	bool          m_output4;
	int           m_x;
	int           m_y;
};

#endif
