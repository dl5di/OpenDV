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

#ifndef	AnalogueRepeaterApp_H
#define	AnalogueRepeaterApp_H

#include "AnalogueRepeaterThreadHelper.h"
#include "AnalogueRepeaterStatusData.h"
#include "AnalogueRepeaterConfig.h"
#include "AnalogueRepeaterFrame.h"
#include "AnalogueRepeaterDefs.h"
#include "AnalogueDefines.h"

#include <wx/wx.h>
#include <wx/config.h>

class CAnalogueRepeaterApp : public wxApp {

public:
	CAnalogueRepeaterApp();
	virtual ~CAnalogueRepeaterApp();

	virtual bool OnInit();
	virtual int  OnExit();
	
	virtual void OnInitCmdLine(wxCmdLineParser& parser);
	virtual bool OnCmdLineParsed(wxCmdLineParser& parser);

	// This is overridden because dialog boxes from threads are bad news
#if defined(__WXDEBUG__)
	virtual void OnAssertFailure(const wxChar* file, int line, const wxChar* func, const wxChar* cond, const wxChar* msg);
	virtual void setReader(CWAVFileReader* reader);
#endif

	virtual void setTbThreshold(wxFloat32 threshold);
	virtual void setCTCSSThreshold(wxFloat32 threshold);
	virtual void setDTMFThreshold(wxFloat32 threshold);

	virtual void setId1Level(wxFloat32 level);
	virtual void setId2Level(wxFloat32 level);
	virtual void setAckLevel(wxFloat32 level);
	virtual void setCTCSSLevel(wxFloat32 level);

	virtual CAnalogueRepeaterStatusData* getStatus() const;

	virtual void showLog(const wxString& text);

	virtual void getCallsign(AUDIO_SOURCE& openIdSource, wxString& openId, AUDIO_SOURCE& closeIdSource, wxString& closeId, AUDIO_SOURCE& beacon1Source, wxString& beacon1, AUDIO_SOURCE& beacon2Source, wxString& beacon2, unsigned int& speed, unsigned int& freq, wxFloat32& level1, wxFloat32& level2) const;
	virtual void setCallsign(AUDIO_SOURCE openIdSource, const wxString& openId, AUDIO_SOURCE closeIdSource, const wxString& closeId, AUDIO_SOURCE beacon1Source, const wxString& beacon1, AUDIO_SOURCE beacon2Source, const wxString& beacon2, unsigned int speed, unsigned int freq, wxFloat32 level1, wxFloat32 level2);

	virtual void getAck(AUDIO_SOURCE& radioAckSource, wxString& radioAck, AUDIO_SOURCE& extAckSource, wxString& extAck, AUDIO_SOURCE& batteryAckSource, wxString& batteryAck, unsigned int& speed, unsigned int& freq, wxFloat32& level, unsigned int& ack, unsigned int& minimum) const;
	virtual void setAck(AUDIO_SOURCE radioAckSource, const wxString& radioAck, AUDIO_SOURCE extAckSource, const wxString& extAck, AUDIO_SOURCE batteryAckSource, const wxString& batteryAck, unsigned int speed, unsigned int freq, wxFloat32 level, unsigned int ack, unsigned int minimum);

	virtual void getTimes(unsigned int& callsignTime, unsigned int& timeout, unsigned int& lockoutTime, unsigned int& hangTime, unsigned int& latchTime) const;
	virtual void setTimes(unsigned int callsignTime, unsigned int timeout, unsigned int lockoutTime, unsigned int hangTime, unsigned int latchTime);

	virtual void getTones(bool& tbEnable, wxFloat32& tbThreshold, wxFloat32& ctcssFreq, bool& ctcssInternal, wxFloat32& ctcssThresh, wxFloat32& ctcssLevel, unsigned int& ctcssHangTime, ANALOGUE_CTCSS_OUTPUT& ctcssOutput) const;
	virtual void setTones(bool tbEnable, wxFloat32 tbThreshold, wxFloat32 ctcssFreq, bool ctcssInternal, wxFloat32 ctcssThresh, wxFloat32 ctcssLevel, unsigned int ctcssHangTime, ANALOGUE_CTCSS_OUTPUT ctcssOutput);

	virtual void getFeel(ANALOGUE_CALLSIGN_START& callsignAtStart, unsigned int& callsignStartDelay, bool& callsignAtEnd, ANALOGUE_TIMEOUT_TYPE& timeoutType, ANALOGUE_CALLSIGN_HOLDOFF& callsignHoldoff) const;
	virtual void setFeel(ANALOGUE_CALLSIGN_START callsignAtStart, unsigned int callsignStartDelay, bool callsignAtEnd, ANALOGUE_TIMEOUT_TYPE timeoutType, ANALOGUE_CALLSIGN_HOLDOFF callsignHoldoff);

	virtual void getRadio(wxString& readDevice, wxString& writeDevice, unsigned int& delay, bool& deEmphasis, bool& preEmphasis, bool& vogad) const;
	virtual void setRadio(const wxString& readDevice, const wxString& writeDevice, unsigned int delay, bool deEmphasis, bool preEmphasis, bool vogad);

	virtual void getExternal(ANALOGUE_EXTERNAL_MODE& mode, wxString& readDevice, wxString& writeDevice, unsigned int& delay, bool& deEmphasis, bool& preEmphasis, bool& vogad, wxString& device, SERIALPIN& txPin, SERIALPIN& rxPin, bool& background) const;
	virtual void setExternal(ANALOGUE_EXTERNAL_MODE mode, const wxString& readDevice, const wxString& writeDevice, unsigned int delay, bool deEmphasis, bool preEmphasis, bool vogad, const wxString& device, SERIALPIN txPin, SERIALPIN rxPin, bool background);

	virtual void getController(wxString& type, unsigned int& config, unsigned int& pttDelay, unsigned int& squelchDelay, bool& pttInvert, bool& squelchInvert) const;
	virtual void setController(const wxString& type, unsigned int config, unsigned int pttDelay, unsigned int squelchDelay, bool pttInvert, bool squelchInvert);

	virtual void getDTMF(bool& radio, bool& external, wxString& shutdown, wxString& startup, wxString& timeout,
		wxString& timeReset, wxString& command1, wxString& command1Line, wxString& command2, wxString& command2Line,
		wxString& output1, wxString& output2, wxString& output3, wxString& output4, wxFloat32& threshold) const;
	virtual void setDTMF(bool radio, bool external, const wxString& shutdown, const wxString& startup, const wxString& timeout,
		const wxString& timeReset, const wxString& command1, const wxString& command1Line, const wxString& command2,
		const wxString& command2Line, const wxString& output1, const wxString& output2, const wxString& output3,
		const wxString& output4, wxFloat32 threshold);

	virtual void getActiveHang(unsigned int& time) const;
	virtual void setActiveHang(unsigned int time);

	virtual void getOutputs(bool& out1, bool& out2, bool& out3, bool& out4) const;
	virtual void setOutputs(bool out1, bool out2, bool out3, bool out4);

	virtual void getPosition(int& x, int& y) const;
	virtual void setPosition(int x, int y);

	virtual bool writeConfig();

	virtual void shutdown();
	virtual void startup();
	virtual void timeout();
	virtual void timeReset();
	virtual void command1();
	virtual void command2();

private:
	wxString                       m_name;
	bool                           m_nolog;
	bool                           m_gui;
	wxString                       m_logDir;
	wxString                       m_confDir;
	CAnalogueRepeaterFrame*        m_frame;
	CAnalogueRepeaterThreadHelper* m_thread;
	CAnalogueRepeaterConfig*       m_config;
	wxLogChain*                    m_logChain;

	void createThread();
};

DECLARE_APP(CAnalogueRepeaterApp)

#endif
