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

#ifndef	SoundCardRepeaterThread_H
#define	SoundCardRepeaterThread_H

#include "SoundCardRepeaterStatusData.h"
#include "RepeaterProtocolHandler.h"
#include "SoundCardRepeaterDefs.h"
#include "SoundCardReaderWriter.h"
#include "ExternalController.h"
#include "AudioCallback.h"
#include "WAVFileReader.h"
#include "CallsignList.h"

#include <wx/wx.h>

enum FRAME_TYPE {
	FRAME_NORMAL,
	FRAME_SYNC,
	FRAME_END
};

class ISoundCardRepeaterThread : public IAudioCallback {
public:
	virtual void setCallsign(const wxString& callsign, const wxString& gateway, DSTAR_MODE mode, ACK_TYPE ack, bool restriction, bool rpt1Validation, bool dtmfBlanking) = 0;
	virtual void setProtocolHandler(CRepeaterProtocolHandler* handler) = 0;
	virtual void setSoundCard(CSoundCardReaderWriter* soundcard, wxFloat32 rxLevel, wxFloat32 txLevel, SQUELCH_MODE squelchMode, wxFloat32 squelchLevel, bool rxInvert, bool txInvert) = 0;
	virtual void setController(CExternalController* controller, int pttDelay) = 0;
	virtual void setTimes(unsigned int timeout, unsigned int ackTime, unsigned int hangTime) = 0;
	virtual void setBeacon(unsigned int time, const wxString& text, bool voice, TEXT_LANG language) = 0;
	virtual void setAnnouncement(bool enabled, unsigned int time, const wxString& recordRPT1, const wxString& recordRPT2, const wxString& deleteRPT1, const wxString& deleteRPT2) = 0;
	virtual void setControl(bool enabled, const wxString& rpt1Callsign, const wxString& rpt2Callsign, const wxString& shutdown, const wxString& startup, const wxString& status1, const wxString& status2, const wxString& status3, const wxString& status4, const wxString& status5, const wxString& command1, const wxString& command1Line, const wxString& command2, const wxString& command2Line, const wxString& command3, const wxString& command3Line, const wxString& command4, const wxString& command4Line, const wxString& output1, const wxString& output2, const wxString& output3, const wxString& output4) = 0;
	virtual void setActiveHang(unsigned int time) = 0;
	virtual void setOutputs(bool out1, bool out2, bool out3, bool out4) = 0;
	virtual void setLogging(bool logging, const wxString& dir) = 0;
	virtual void setReader(CWAVFileReader* reader) = 0;
	virtual void setBlackList(CCallsignList* list) = 0;
	virtual void setGreyList(CCallsignList* list) = 0;

	virtual void setRXLevel(wxFloat32 level) = 0;
	virtual void setTXLevel(wxFloat32 level) = 0;
	virtual void setSquelchLevel(wxFloat32 level) = 0;

	virtual void shutdown() = 0;
	virtual void startup() = 0;

	virtual void command1() = 0;
	virtual void command2() = 0;
	virtual void command3() = 0;
	virtual void command4() = 0;

	virtual CSoundCardRepeaterStatusData* getStatus() = 0;

	virtual void run() = 0;
	virtual void kill() = 0;

private:
};

#endif
