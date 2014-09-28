/*
 *   Copyright (C) 2012 by Jonathan Naylor G4KLX
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

#ifndef	DCSClientPreferences_H
#define	DCSClientPreferences_H

#include <wx/wx.h>
#include <wx/notebook.h>

#include "DCSClientControllerSet.h"
#include "DCSClientReflectorSet.h"
#include "DCSClientCallsignSet.h"
#include "SoundcardSet.h"
#include "DVDongleSet.h"
#include "MessageSet.h"
#include "BleepSet.h"

class CDCSClientPreferences : public wxDialog {
public:
	CDCSClientPreferences(wxWindow* parent, int id, const wxString& callsign, const wxString& readDevice,
								   const wxString& writeDevice, const wxString& dvdDevice, const wxString& message,
								   bool bleep, const wxString& ifaceType, unsigned int audioDelay, bool pttInvert,
								   bool squelchInvert, const wxString& startupReflector);
	virtual ~CDCSClientPreferences();

	virtual bool Validate();

	virtual wxString getCallsign() const;

	virtual wxString getReadDevice() const;
	virtual wxString getWriteDevice() const;

	virtual wxString getDVDDevice() const;

	virtual wxString getMessage() const;

	virtual bool     getBleep() const;

	virtual wxString     getIfaceType() const;
	virtual unsigned int getAudioDelay() const;
	virtual bool         getPTTInvert() const;
	virtual bool         getSquelchInvert() const;

	virtual wxString     getStartupReflector() const;

private:
	CDCSClientCallsignSet*   m_callsign;
	CSoundcardSet*              m_soundcard;
	CDVDongleSet*               m_dongle;
	CMessageSet*                m_message;
	CBleepSet*                  m_bleep;
	CDCSClientControllerSet* m_controller;
	CDCSClientReflectorSet*  m_reflector;
};

#endif
