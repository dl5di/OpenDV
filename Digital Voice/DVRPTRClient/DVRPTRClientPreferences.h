/*
 *   Copyright (C) 2010,2012 by Jonathan Naylor G4KLX
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

#ifndef	DVRPTRClientPreferences_H
#define	DVRPTRClientPreferences_H

#include <wx/wx.h>
#include <wx/notebook.h>

#include "DVRPTRClientCallsignSet.h"
#include "DVRPTRClientModemSet.h"
#include "SoundcardSet.h"
#include "DVDongleSet.h"
#include "MessageSet.h"
#include "BleepSet.h"

class CDVRPTRClientPreferences : public wxDialog {
public:
	CDVRPTRClientPreferences(wxWindow* parent, int id, const wxString& callsign1, const wxString& callsign2,
								   const wxString& readDevice, const wxString& writeDevice, DVRPTR_VERSION version,
								   const wxString& port, bool rxInvert, bool txInvert, bool channel,
								   unsigned int modLevel, unsigned int txDelay, const wxString& dvdDevice,
								   const wxString& message, bool bleep);
	virtual ~CDVRPTRClientPreferences();

	virtual bool Validate();

	virtual wxString     getCallsign1() const;
	virtual wxString     getCallsign2() const;

	virtual wxString     getSoundcardReadDevice() const;
	virtual wxString     getSoundcardWriteDevice() const;

	virtual DVRPTR_VERSION getVersion() const;
	virtual wxString       getPort() const;
	virtual bool           getRXInvert() const;
	virtual bool           getTXInvert() const;
	virtual bool           getChannel() const;
	virtual unsigned int   getModLevel() const;
	virtual unsigned int   getTXDelay() const;

	virtual wxString     getDVDDevice() const;

	virtual wxString     getMessage() const;

	virtual bool         getBleep() const;

private:
	CDVRPTRClientCallsignSet* m_callsign;
	CSoundcardSet*            m_soundcard;
	CDVRPTRClientModemSet*    m_modem;
	CDVDongleSet*             m_dongle;
	CMessageSet*              m_message;
	CBleepSet*                m_bleep;
};

#endif
