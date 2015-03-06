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

#ifndef	AnalogueRepeaterThread_H
#define	AnalogueRepeaterThread_H

#include "AnalogueRepeaterStatusData.h"
#include "SoundCardReaderWriter.h"
#include "AnalogueRepeaterDefs.h"
#include "ExternalController.h"
#include "NetworkController.h"
#include "FixedAudioSource.h"
#include "AnalogueDefines.h"
#include "DTMFController.h"
#include "FirstOrderIIR.h"
#include "WAVFileReader.h"
#include "AudioCallback.h"
#include "TimeoutTones.h"
#include "RingBuffer.h"
#include "AudioDelay.h"
#include "FIRFilter.h"
#include "Goertzel.h"
#include "PTTDelay.h"
#include "Timer.h"
#include "VOGAD.h"
#include "NCO.h"

#include <wx/wx.h>

enum ACK_TYPE {
	ACK_NONE,
	ACK_RADIO_WAITING,
	ACK_RADIO_SENDING,
	ACK_EXTERNAL_WAITING,
	ACK_EXTERNAL_SENDING,
	ACK_BATTERY_WAITING,
	ACK_BATTERY_SENDING
};

class CAnalogueRepeaterThread : public IAudioCallback {
public:
	CAnalogueRepeaterThread();
	virtual ~CAnalogueRepeaterThread();

	virtual void readCallback(const wxFloat32* input, unsigned int nSamples, int id);
	virtual void writeCallback(wxFloat32* output, unsigned int& nSamples, int id);

	virtual void setCallsign(IFixedAudioSource* openId, IFixedAudioSource* closeId, IFixedAudioSource* beacon1, IFixedAudioSource* beacon2, wxFloat32 level1, wxFloat32 level2);
	virtual void setAck(IFixedAudioSource* radioAck, IFixedAudioSource* extAck, IFixedAudioSource* batteryAck, wxFloat32 level, unsigned int ack, unsigned int minimum);
	virtual void setTimes(unsigned int callsignTime, unsigned int timeout, unsigned int lockoutTime, unsigned int hangTime, unsigned int& latchTime);
	virtual void setTones(bool tbEnable, wxFloat32 tbThreshold, wxFloat32 ctcssFreq, bool ctcssInternal, wxFloat32 ctcssThresh, wxFloat32 ctcssLevel, unsigned int ctcssHangTime, ANALOGUE_CTCSS_OUTPUT ctcssOutput);
	virtual void setFeel(ANALOGUE_CALLSIGN_START callsignAtStart, unsigned int callsignStartDelay, bool callsignAtEnd, ANALOGUE_TIMEOUT_TYPE timeoutType, ANALOGUE_CALLSIGN_HOLDOFF callsignHoldoff);
	virtual void setRadio(CSoundCardReaderWriter* soundcard, unsigned int audioDelay, bool deEmphasis, bool preEmphasis, bool vogad);
	virtual void setExternal(ANALOGUE_EXTERNAL_MODE mode, CSoundCardReaderWriter* soundcard, unsigned int audioDelay, bool deEmphasis, bool preEmphasis, bool vogad, CNetworkController* controller, bool background);
	virtual void setController(CExternalController* controller, unsigned int pttDelay, unsigned int squelchDelay);
	virtual void setOutputs(bool out1, bool out2, bool out3, bool out4);
	virtual void setDTMF(bool radio, bool external, const wxString& shutdown, const wxString& startup, const wxString& timeout,
		const wxString& timeReset, const wxString& command1, const wxString& command1Line, const wxString& command2,
		const wxString& command2Line, const wxString& output1, const wxString& output2, const wxString& output3,
		const wxString& output4, wxFloat32 threshold);
	virtual void setActiveHang(unsigned int time);
#if defined(__WXDEBUG__)
	virtual void setReader(CWAVFileReader* reader);
#endif

	virtual void setTbThreshold(wxFloat32 threshold);
	virtual void setCTCSSThreshold(wxFloat32 threshold);
	virtual void setDTMFThreshold(wxFloat32 threshold);

	virtual void setId1Level(wxFloat32 level);
	virtual void setId2Level(wxFloat32 level);
	virtual void setAckLevel(wxFloat32 level);
	virtual void setCTCSSLevel(wxFloat32 level);

	virtual void shutdown();
	virtual void startup();
	virtual void timeout();
	virtual void timeReset();
	virtual void command1();
	virtual void command2();

	virtual CAnalogueRepeaterStatusData* getStatus();

	virtual void run();
	virtual void kill();

private:
	CSoundCardReaderWriter*   m_radioSoundcard;
	CSoundCardReaderWriter*   m_extSoundcard;
	CExternalController*      m_controller;
	CNetworkController*       m_extController;
	ANALOGUE_EXTERNAL_MODE    m_extMode;
	CPTTDelay*                m_pttDelay;
	CPTTDelay*                m_squelch1Delay;
	CPTTDelay*                m_squelch2Delay;
	bool                      m_stopped;
	bool                      m_battery;
	ANALOGUE_RPT_STATE        m_state;
	CRingBuffer<wxFloat32>    m_radioInBuffer;
	CRingBuffer<wxFloat32>    m_radioOutBuffer;
	CRingBuffer<wxFloat32>    m_extInBuffer;
	CRingBuffer<wxFloat32>    m_extOutBuffer;
	CGoertzel*                m_1750In;
	CGoertzel*                m_ctcssIn;
	bool                      m_ctcssInternal;
	CNCO*                     m_ctcssOut;
	ANALOGUE_CTCSS_OUTPUT     m_ctcssOutput;
	CTimeoutTones*            m_timeoutTones;
	IFixedAudioSource*        m_openId;
	IFixedAudioSource*        m_closeId;
	IFixedAudioSource*        m_beacon1;
	IFixedAudioSource*        m_beacon2;
	IFixedAudioSource*        m_radioAck;
	IFixedAudioSource*        m_extAck;
	IFixedAudioSource*        m_batteryAck;
	CTimer                    m_timeoutTimer;
	CTimer                    m_lockoutTimer;
	CTimer                    m_callsignTimer;
	CTimer                    m_callsignStartTimer;
	CTimer                    m_hangTimer;
	CTimer                    m_latchTimer;
	CTimer                    m_minimumTimer;
	CTimer                    m_ackTimer;
	wxFloat32                 m_idLevel1;
	wxFloat32                 m_idLevel2;
	wxFloat32                 m_ackLevel;
	wxFloat32                 m_ctcssLevel;
	CAudioDelay*              m_radioAudioDelay;
	CFirstOrderIIR*           m_radioDeEmphasis;
	CFirstOrderIIR*           m_radioPreEmphasis;
	CFIRFilter                m_audioFilter;
	wxFloat32                 m_audioLevel;
	CVOGAD                    m_radioAudioVOGAD;
	CVOGAD                    m_extAudioVOGAD;
	CAudioDelay*              m_extAudioDelay;
	CFirstOrderIIR*           m_extDeEmphasis;
	CFirstOrderIIR*           m_extPreEmphasis;
	bool                      m_extBackground;
	ANALOGUE_CALLSIGN_START   m_callsignAtStart;
	bool                      m_callsignAtEnd;
	ANALOGUE_CALLSIGN_HOLDOFF m_callsignHoldoff;
	ANALOGUE_SQUELCH          m_squelch;
	bool                      m_radioTransmit;
	bool                      m_extTransmit;
	bool                      m_firstTime;
	bool                      m_sendOpen;
	bool                      m_sendClose;
	bool                      m_sendBeacon1;
	bool                      m_sendBeacon2;
	ACK_TYPE                  m_sendAck;
	CDTMFController*          m_radioDTMF;
	CDTMFController*          m_extDTMF;
	bool                      m_output1;
	bool                      m_output2;
	bool                      m_output3;
	bool                      m_output4;
	CWAVFileReader*           m_reader;
	bool                      m_killed;
	wxString                  m_command1Line;
	wxString                  m_command2Line;
	CTimer                    m_activeHangTimer;
	CTimer                    m_ctcssHangTimer;
	bool                      m_shutdown;
	unsigned int              m_radioCount;
	unsigned int              m_externalCount;
	unsigned int              m_relayCount;
	unsigned int              m_transmitCount;
	unsigned int              m_timeCount;
	unsigned int              m_lastHour;

	void getAudio(wxFloat32* radioAudio, wxFloat32* extAudio, unsigned int& nRadio, unsigned int& nExt);
	ANALOGUE_SQUELCH checkRadioSquelch(const wxFloat32* audio, unsigned int length, ANALOGUE_SQUELCH squelch);
	ANALOGUE_SQUELCH checkExternalSquelch();
	void sendTones(wxFloat32* audio, unsigned int length);
	void setState(ANALOGUE_RPT_STATE state);
	void stateMachine();
	void feedRadio(const wxFloat32* audio, unsigned int length);
	void feedExternal(const wxFloat32* audio, unsigned int length, bool transmit);
	void calculateAudioLevel(const wxFloat32* audio, unsigned int length);
	void insertCTCSS(wxFloat32* audio, unsigned int length);
	void filterAudio(wxFloat32* audio, unsigned int length);
	void processDTMF(const wxFloat32* radioAudio, const wxFloat32* extAudio, unsigned int nRadio, unsigned int nExt);
	void sendOpen();
	void sendClose();
	void sendCallsign();
	void getStatistics();
	void writeStatistics();
#if defined(__WXDEBUG__)
	void getRadioAudio(const wxFloat32* input, unsigned int nSamples);
#endif
	void clock(unsigned int ms);
};

#endif
