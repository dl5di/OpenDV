/*
 *   Copyright (C) 2010 by Jonathan Naylor G4KLX
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

#ifndef	DExtraClientThread_H
#define	DExtraClientThread_H

#include "SoundCardReaderWriter.h"
#include "DExtraProtocolHandler.h"
#include "DVDongleController.h"
#include "ExternalController.h"
#include "DVTOOLFileWriter.h"
#include "DExtraClientDefs.h"
#include "SlowDataEncoder.h"
#include "SlowDataDecoder.h"
#include "DVDongleThread.h"
#include "AudioCallback.h"
#include "RingBuffer.h"
#include "HeaderData.h"
#include "AudioDelay.h"
#include "Timer.h"

#include <wx/wx.h>

enum CLIENT_STATE {
	CLIENT_RECEIVE,
	CLIENT_TRANSMIT,
	CLIENT_WANT_RECEIVE
};

class CDExtraClientThread : public wxThread, public IAudioCallback {
public:
	CDExtraClientThread();
	virtual ~CDExtraClientThread();

	virtual void callback(const wxFloat32* input, wxFloat32* output, unsigned int nSamples, int id);

	virtual void setCallsign(const wxString& callsign);
	virtual void setSoundCard(CSoundCardReaderWriter* soundcard);
	virtual void setDVDongle(CDVDongleController* dongle);
	virtual void setMessage(const wxString& text);
	virtual void setRecording(bool record);
	virtual void setBleep(bool on);
	virtual void setController(IExternalController* controller, unsigned int audioDelay, bool pttInvert, bool squelchInvert);

	virtual bool connect(bool connect, const wxString& reflector, const wxString& address);
	virtual bool transmit(bool transmit);
	virtual void setModule(wxChar module);
	virtual bool isConnected();

	virtual void* Entry();
	virtual void  kill();

private:
	CSoundCardReaderWriter*    m_soundcard;
	CDVDongleThread*           m_dongle;
	IExternalController*       m_controller;
	bool                       m_pttInvert;
	bool                       m_squelchInvert;
	CAudioDelay*               m_audioDelay;
	CSlowDataEncoder           m_slowDataEncoder;
	CSlowDataDecoder           m_slowDataDecoder;
	CDExtraProtocolHandler     m_protocol;
	bool                       m_stopped;
	CRingBuffer<wxFloat32>     m_decodeAudio;
	CRingBuffer<unsigned char> m_decodeData;
	CRingBuffer<wxFloat32>     m_encodeAudio;
	CRingBuffer<unsigned char> m_encodeData;
	CLIENT_STATE               m_transmit;
	bool                       m_record;
	CDVTOOLFileWriter*         m_recorder;
	wxString                   m_callsign;
	wxString                   m_message;
	unsigned int               m_frameCount;
	bool                       m_killed;
	wxString                   m_reflector;
	wxChar                     m_module;
	bool                       m_started;
	CTimer                     m_watchdog;
	CDVTOOLFileWriter*         m_dumper;
	bool                       m_tx;

	void receive();
	void transmit();

	void resetReceiver();
	void processHeader(CHeaderData* header);
	void processFrame(const unsigned char* buffer, bool sync, bool end);
	void processSlowData(const unsigned char* buffer);
};

#endif
