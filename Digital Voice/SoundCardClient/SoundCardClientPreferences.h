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

#ifndef	SoundCardClientPreferences_H
#define	SoundCardClientPreferences_H

#include <wx/wx.h>
#include <wx/notebook.h>

#include "SoundCardClientControllerSet.h"
#include "SoundCardClientCallsignSet.h"
#include "SoundcardSet.h"
#include "InversionSet.h"
#include "DVDongleSet.h"
#include "MessageSet.h"
#include "BleepSet.h"

class CSoundCardClientPreferences : public wxDialog {
public:
	CSoundCardClientPreferences(wxWindow* parent, int id, const wxString& callsign1, const wxString& callsign2,
								   const wxString& audioReadDevice, const wxString& audioWriteDevice,
								   const wxString& radioReadDevice, const wxString& radioWriteDevice,
								   bool rxInvert, bool txInvert, const wxString& ifaceType, bool ptt,
								   unsigned int delay, bool invert, const wxString& dvdDevice,
								   const wxString& message, bool bleep);
	virtual ~CSoundCardClientPreferences();

	virtual bool Validate();

	virtual wxString getCallsign1() const;
	virtual wxString getCallsign2() const;

	virtual wxString getAudioSoundcardReadDevice() const;
	virtual wxString getAudioSoundcardWriteDevice() const;

	virtual wxString getRadioSoundcardReadDevice() const;
	virtual wxString getRadioSoundcardWriteDevice() const;

	virtual bool     getRXGMSKInvert() const;
	virtual bool     getTXGMSKInvert() const;

	virtual wxString getDVDDevice() const;

	virtual wxString     getIfaceType() const;
	virtual bool         getPTTEnable() const;
	virtual unsigned int getTXDelay() const;
	virtual bool         getTXInvert() const;

	virtual wxString getMessage() const;

	virtual bool     getBleep() const;

private:
	CSoundCardClientCallsignSet*   m_callsign;
	CSoundcardSet*             m_audioSoundcard;
	CSoundcardSet*             m_radioSoundcard;
	CInversionSet*             m_inversion;
	CSoundCardClientControllerSet* m_controller;
	CDVDongleSet*              m_dongle;
	CMessageSet*               m_message;
	CBleepSet*                 m_bleep;
};

#endif
