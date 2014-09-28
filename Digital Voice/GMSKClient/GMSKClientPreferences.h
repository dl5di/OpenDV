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

#ifndef	GMSKClientPreferences_H
#define	GMSKClientPreferences_H

#include <wx/wx.h>
#include <wx/notebook.h>

#include "GMSKClientCallsignSet.h"
#include "GMSKClientModemSet.h"
#include "GMSKClientDefs.h"
#include "SoundcardSet.h"
#include "DVDongleSet.h"
#include "MessageSet.h"
#include "BleepSet.h"

class CGMSKClientPreferences : public wxDialog {
public:
	CGMSKClientPreferences(wxWindow* parent, int id, const wxString& callsign1, const wxString& callsign2,
								   const wxString& readDevice, const wxString& writeDevice, GMSK_MODEM_TYPE modemType,
								   unsigned int modemAddress, const wxString& dvdDevice,
								   const wxString& message, bool bleep);
	virtual ~CGMSKClientPreferences();

	virtual bool Validate();

	virtual wxString getCallsign1() const;
	virtual wxString getCallsign2() const;

	virtual wxString getSoundcardReadDevice() const;
	virtual wxString getSoundcardWriteDevice() const;

	virtual GMSK_MODEM_TYPE getModemType() const;
	virtual unsigned int getModemAddress() const;

	virtual wxString getDVDDevice() const;

	virtual wxString getMessage() const;

	virtual bool     getBleep() const;

private:
	CGMSKClientCallsignSet* m_callsign;
	CSoundcardSet*          m_soundcard;
	CGMSKClientModemSet*    m_modem;
	CDVDongleSet*           m_dongle;
	CMessageSet*            m_message;
	CBleepSet*              m_bleep;
};

#endif
