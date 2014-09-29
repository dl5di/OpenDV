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

#ifndef	SoundCardRepeaterTXRXThread_H
#define	SoundCardRepeaterTXRXThread_H

#include "SoundCardRepeaterThread.h"
#include "DStarGMSKDemodulator.h"
#include "LimitedLengthBuffer.h"
#include "DStarGMSKModulator.h"
#include "SlowDataDecoder.h"
#include "SlowDataEncoder.h"
#include "PatternMatcher.h"
#include "WAVFileWriter.h"
#include "HeaderData.h"
#include "AudioDelay.h"
#include "RingBuffer.h"
#include "PTTDelay.h"
#include "Goertzel.h"
#include "AMBEFEC.h"
#include "Timer.h"

#include <wx/wx.h>

class CSoundCardRepeaterTXRXThread : public ISoundCardRepeaterThread {
public:
	CSoundCardRepeaterTXRXThread();
	virtual ~CSoundCardRepeaterTXRXThread();

	virtual void callback(const wxFloat32* input, wxFloat32* output, unsigned int nSamples, int id);

	virtual void setCallsign(const wxString& callsign, const wxString& gateway, DSTAR_MODE mode, ACK_TYPE ack, bool restriction, bool rpt1Validation, bool dtmfBlanking);
	virtual void setProtocolHandler(CRepeaterProtocolHandler* handler);
	virtual void setSoundCard(CSoundCardReaderWriter* soundcard, wxFloat32 rxLevel, wxFloat32 txLevel, SQUELCH_MODE squelchMode, wxFloat32 squelchLevel, bool rxInvert, bool txInvert);
	virtual void setController(CExternalController* controller, int pttDelay);
	virtual void setTimes(unsigned int timeout, unsigned int ackTime, unsigned int hangTime);
	virtual void setBeacon(unsigned int time, const wxString& text, bool voice, TEXT_LANG language);
	virtual void setAnnouncement(bool enabled, unsigned int time, const wxString& recordRPT1, const wxString& recordRPT2, const wxString& deleteRPT1, const wxString& deleteRPT2);
	virtual void setControl(bool enabled, const wxString& rpt1Callsign, const wxString& rpt2Callsign, const wxString& shutdown, const wxString& startup, const wxString& status1, const wxString& status2, const wxString& status3, const wxString& status4, const wxString& status5, const wxString& command1, const wxString& command1Line, const wxString& command2, const wxString& command2Line, const wxString& command3, const wxString& command3Line, const wxString& command4, const wxString& command4Line, const wxString& output1, const wxString& output2, const wxString& output3, const wxString& output4);
	virtual void setActiveHang(unsigned int time);
	virtual void setOutputs(bool out1, bool out2, bool out3, bool out4);
	virtual void setLogging(bool logging, const wxString& dir);
	virtual void setReader(CWAVFileReader* reader);
	virtual void setBlackList(CCallsignList* list);
	virtual void setGreyList(CCallsignList* list);

	virtual void setRXLevel(wxFloat32 level);
	virtual void setTXLevel(wxFloat32 level);
	virtual void setSquelchLevel(wxFloat32 level);

	virtual void shutdown();
	virtual void startup();

	virtual void command1();
	virtual void command2();
	virtual void command3();
	virtual void command4();

	virtual CSoundCardRepeaterStatusData* getStatus();

	virtual void run();
	virtual void kill();

private:
	CSoundCardReaderWriter*   m_soundcard;
	CRepeaterProtocolHandler* m_protocolHandler;
	CExternalController*      m_controller;
	CGoertzel                 m_goertzel;
	CPTTDelay*                m_pttDelay;
	CAudioDelay*              m_audioDelay;
	bool                      m_stopped;
	CRingBuffer<wxFloat32>    m_inBuffer;
	CRingBuffer<wxFloat32>    m_outBuffer;
	CRingBuffer<bool>         m_networkBuffer;
	unsigned int              m_networkRun;
	bool                      m_networkStarted;
	wxString                  m_rptCallsign;
	CWAVFileReader*           m_reader;
	CDStarGMSKDemodulator     m_demodulator;
	CDStarGMSKModulator       m_modulator;
	CLimitedLengthBuffer      m_bitBuffer;
	DSTAR_RX_STATE            m_rxState;
	CPatternMatcher           m_frameMatcher;
	CPatternMatcher           m_dataMatcher;
	CPatternMatcher           m_endMatcher;
	CWAVFileWriter*           m_writer;
	CHeaderData*              m_rxHeader;
	CHeaderData*              m_txHeader;
	unsigned int              m_headerBER;
	unsigned char             m_radioSeqNo;
	unsigned int              m_radioSyncsLost;
	unsigned char             m_networkSeqNo;
	CTimer                    m_watchdogTimer;
	CTimer                    m_pollTimer;
	CTimer                    m_hangTimer;
	DSTAR_RPT_STATE           m_rptState;
	CSlowDataDecoder          m_slowDataDecoder;
	bool                      m_tx;
	bool                      m_squelch;
	unsigned int              m_squelchCount;
	wxFloat32                 m_noise;
	unsigned int              m_noiseCount;
	bool                      m_killed;
	wxFloat32                 m_rxLevel;
	wxFloat32                 m_txLevel;
	SQUELCH_MODE              m_squelchMode;
	CTimer                    m_activeHangTimer;
	bool                      m_disable;
	unsigned char*            m_lastData;
	CAMBEFEC                  m_ambe;
	unsigned int              m_ambeFrames;
	unsigned int              m_ambeSilence;
	unsigned int              m_ambeBits;
	unsigned int              m_ambeErrors;
	unsigned int              m_lastAMBEBits;
	unsigned int              m_lastAMBEErrors;
	unsigned int              m_radioCount;
	unsigned int              m_networkCount;
	unsigned int              m_transmitCount;
	unsigned int              m_timeCount;
	unsigned int              m_lastHour;
	wxStopWatch               m_headerTime;
	wxStopWatch               m_packetTime;
	unsigned int              m_packetCount;
	unsigned int              m_packetSilence;

	void receiveRadio();
	void receiveNetwork();
	void transmitNetworkHeader(CHeaderData& header);
	void transmit();
	void transmitNetwork();
	void transmitBits(const bool* bits, unsigned int length);

	void radioStateMachine(bool bit);
	void repeaterStateMachine();
	bool processRadioHeader(CHeaderData* header);
	void processNetworkHeader(CHeaderData* header);
	void processRadioFrame(FRAME_TYPE type);
	unsigned int processNetworkFrame(unsigned char* data, unsigned int length, unsigned char seqNo);
	void endOfRadioData();
	void endOfNetworkData();
	CHeaderData* processFECHeader();
	CHeaderData* processSlowData(bool sync);
	void setRadioState(DSTAR_RX_STATE state);
	bool setRepeaterState(DSTAR_RPT_STATE state);
	void readRadioData(const wxFloat32* buffer, unsigned int nSamples);
	void getStatistics();
	void writeStatistics();
	void clock(unsigned int ms);
};

#endif
