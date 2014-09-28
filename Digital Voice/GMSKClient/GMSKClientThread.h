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

#ifndef	GMSKClientThread_H
#define	GMSKClientThread_H

#include "SoundCardReaderWriter.h"
#include "DVDongleController.h"
#include "DVTOOLFileWriter.h"
#include "SlowDataDecoder.h"
#include "SlowDataEncoder.h"
#include "GMSKClientDefs.h"
#include "DVDongleThread.h"
#include "AudioCallback.h"
#include "RingBuffer.h"
#include "HeaderData.h"
#include "GMSKModem.h"

#include <wx/wx.h>

enum RX_STATE {
	RXS_LISTENING,
	RXS_PROCESS_DATA
};

enum CLIENT_STATE {
	CS_RECEIVE,
	CS_TRANSMIT,
	CS_WANT_RECEIVE
};

class CGMSKClientThread : public wxThread, public IAudioCallback {
public:
	CGMSKClientThread();
	virtual ~CGMSKClientThread();

	virtual void callback(const wxFloat32* input, wxFloat32* output, unsigned int nSamples, int id);

	virtual void setCallsign(const wxString& callsign1, const wxString& callsign2);
	virtual void setSoundCard(CSoundCardReaderWriter* soundcard);
	virtual void setDVDongle(CDVDongleController* dongle);
	virtual void setMessage(const wxString& text);
	virtual void setModem(IGMSKModem* modem);
	virtual void setBleep(bool on);

	virtual void setRecording(bool record);

	virtual void setYour(const wxString& your);
	virtual void setRpt1(const wxString& rpt1);
	virtual void setRpt2(const wxString& rpt2);
	virtual bool setTransmit(bool transmit);

	virtual void* Entry();
	virtual void  kill();

private:
	CSoundCardReaderWriter*    m_soundcard;
	CDVDongleThread*           m_dongle;
	CHeaderData*               m_header;
	IGMSKModem*                m_modem;
	CSlowDataDecoder           m_slowDataDecoder;
	CSlowDataEncoder           m_slowDataEncoder;
	bool                       m_stopped;
	CRingBuffer<wxFloat32>     m_decodeAudio;
	CRingBuffer<unsigned char> m_decodeData;
	CRingBuffer<wxFloat32>     m_encodeAudio;
	CRingBuffer<unsigned char> m_encodeData;
	CLIENT_STATE               m_transmit;
	bool                       m_record;
	CDVTOOLFileWriter*         m_recorder;
	wxString                   m_callsign1;
	wxString                   m_callsign2;
	wxString                   m_your;
	wxString                   m_rpt1;
	wxString                   m_rpt2;
	wxString                   m_message;
	unsigned char*             m_ambeBuffer;
	unsigned int               m_ambeLength;
	unsigned int               m_syncCount;
	RX_STATE                   m_state;
	bool                       m_killed;
	bool                       m_tx;

	void receive();
	void transmit();

	void resetReceiver();
	bool processHeader(CHeaderData* header);
	void processFrame(const unsigned char* buffer, bool sync);
	void endOfData();
	void setState(RX_STATE state);
	void processSlowData(const unsigned char* buffer);
};

#endif
