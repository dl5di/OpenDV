/*
 *   Copyright (C) 2009,2010,2012 by Jonathan Naylor G4KLX
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

#ifndef	SoundCardClientThread_H
#define	SoundCardClientThread_H

#include "SoundCardReaderWriter.h"
#include "LimitedPatternMatcher.h"
#include "DStarGMSKDemodulator.h"
#include "LimitedLengthBuffer.h"
#include "DVDongleController.h"
#include "DStarGMSKModulator.h"
#include "ExternalController.h"
#include "DVTOOLFileWriter.h"
#include "SlowDataEncoder.h"
#include "SoundCardClientDefs.h"
#include "SlowDataDecoder.h"
#include "PatternMatcher.h"
#include "DVDongleThread.h"
#include "WAVFileReader.h"
#include "AudioCallback.h"
#include "WAVFileWriter.h"
#include "RingBuffer.h"
#include "HeaderData.h"
#include "PTTDelay.h"

#include <wx/wx.h>

enum RX_STATE {
	RX_STATE_LISTENING,
	RX_STATE_PROCESS_HEADER,
	RX_STATE_PROCESSING
};

enum CLIENT_STATE {
	CLIENT_RECEIVE,
	CLIENT_TRANSMIT,
	CLIENT_WANT_RECEIVE
};

class CSoundCardClientThread : public wxThread, public IAudioCallback {
public:
	CSoundCardClientThread();
	virtual ~CSoundCardClientThread();

	virtual void callback(const wxFloat32* input, wxFloat32* output, unsigned int nSamples, int id);

	virtual void setCallsign(const wxString& callsign1, const wxString& callsign2);
	virtual void setAudioSoundCard(CSoundCardReaderWriter* soundcard);
	virtual void setRadioSoundCard(CSoundCardReaderWriter* soundcard);
	virtual void setInversion(bool rxInvert, bool txInvert);
	virtual void setDVDongle(CDVDongleController* dongle);
	virtual void setMessage(const wxString& text);
	virtual void setController(IExternalController* controller, bool pttEnable, unsigned int txDelay, bool txInvert);
	virtual void setBleep(bool on);

	virtual void setReader(CWAVFileReader* reader);
	virtual void setRecording(bool record);

	virtual void setYour(const wxString& your);
	virtual void setRpt1(const wxString& rpt1);
	virtual void setRpt2(const wxString& rpt2);
	virtual bool setTransmit(bool transmit);

	virtual void* Entry();
	virtual void  kill();

private:
	CSoundCardReaderWriter*    m_audioSoundcard;
	CSoundCardReaderWriter*    m_radioSoundcard;
	CDVDongleThread*           m_dongle;
	CHeaderData*               m_header;
	IExternalController*       m_controller;
	bool                       m_pttEnable;
	bool                       m_txInvert;
	CPTTDelay*                 m_txDelay;
	CSlowDataEncoder           m_slowDataEncoder;
	CSlowDataDecoder           m_slowDataDecoder;
	bool                       m_stopped;
	CRingBuffer<wxFloat32>     m_radioBuffer;
	CRingBuffer<wxFloat32>     m_decodeAudio;
	CRingBuffer<unsigned char> m_decodeData;
	CRingBuffer<wxFloat32>     m_encodeAudio;
	CRingBuffer<unsigned char> m_encodeData;
	CRingBuffer<bool>          m_transmitBits;
	CLIENT_STATE               m_transmit;
	bool                       m_record;
	CDVTOOLFileWriter*         m_recorder;
	wxString                   m_callsign1;
	wxString                   m_callsign2;
	wxString                   m_your;
	wxString                   m_rpt1;
	wxString                   m_rpt2;
	wxString                   m_message;
	CWAVFileReader*            m_reader;
	CDStarGMSKDemodulator      m_demodulator;
	CDStarGMSKModulator        m_modulator;
	CLimitedLengthBuffer       m_bitBuffer;
	unsigned int               m_frameCount;
	RX_STATE                   m_state;
	CPatternMatcher            m_frameMatcher;
	CPatternMatcher            m_dataMatcher1;
	CLimitedPatternMatcher     m_dataMatcher2;
	CPatternMatcher            m_endMatcher;
	bool                       m_headerSeen;
	CWAVFileWriter*            m_writer;
	bool                       m_killed;
	bool                       m_tx;

	void receive();
	void transmit();

	void resetReceiver();
	void stateMachine(bool bit);
	bool processHeader();
	void processFrame(bool sync);
	void processSlowData(const unsigned char* buffer);
	void endOfData();
	void transmitBits();
	unsigned char bitsToByte(const bool* bits) const;
	void byteToBits(unsigned char byte, bool* data) const;
	void setState(RX_STATE state);
	void readRadioData(const wxFloat32* buffer, unsigned int nSamples);
};

#endif
