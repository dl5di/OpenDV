/*
 *   Copyright (C) 2010,2011,2012,2014 by Jonathan Naylor G4KLX
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

#ifndef	DummyRepeaterPreferences_H
#define	DummyRepeaterPreferences_H

#include <wx/wx.h>
#include <wx/notebook.h>

#include "DummyRepeaterControllerSet.h"
#include "DummyRepeaterSoundcardSet.h"
#include "DummyRepeaterCallsignSet.h"
#include "DummyRepeaterMessageSet.h"
#include "DummyRepeaterTimeoutSet.h"
#include "DummyRepeaterNetworkSet.h"
#include "DummyRepeaterDongleSet.h"
#include "DummyRepeaterBleepSet.h"

class CDummyRepeaterPreferences : public wxDialog {
public:
	CDummyRepeaterPreferences(wxWindow* parent, int id, const wxString& callsign1, const wxString& callsign2,
		const wxString& readDevice, const wxString& writeDevice, DONGLE_TYPE dongleType, const wxString& dongleDevice,
		const wxString& dongleAddress, unsigned int donglePort, const wxString& gwyAddress, unsigned int gwyPort,
		const wxString& localAddress, unsigned int localPort, const wxString& type, unsigned int config, bool pttInvert,
		bool squelchInvert, unsigned int timeout, const wxString& message, bool bleep);
	virtual ~CDummyRepeaterPreferences();

	virtual bool Validate();

	virtual wxString     getCallsign1() const;
	virtual wxString     getCallsign2() const;

	virtual wxString     getSoundcardReadDevice() const;
	virtual wxString     getSoundcardWriteDevice() const;

	virtual DONGLE_TYPE  getDongleType() const;
	virtual wxString     getDongleDevice() const;
	virtual wxString     getDongleAddress() const;
	virtual unsigned int getDonglePort() const;

	virtual wxString     getGatewayAddress() const;
	virtual unsigned int getGatewayPort() const;
	virtual wxString     getLocalAddress() const;
	virtual unsigned int getLocalPort() const;

	virtual wxString     getType() const;
	virtual unsigned int getConfig() const;
	virtual bool         getPTTInvert() const;
	virtual bool         getSquelchInvert() const;

	virtual unsigned int getTimeout() const;

	virtual wxString     getMessage() const;

	virtual bool         getBleep() const;

private:
	CDummyRepeaterCallsignSet*   m_callsign;
	CDummyRepeaterSoundcardSet*  m_soundcard;
	CDummyRepeaterDongleSet*     m_dongle;
	CDummyRepeaterNetworkSet*    m_network;
	CDummyRepeaterControllerSet* m_controller;
	CDummyRepeaterTimeoutSet*    m_timeout;
	CDummyRepeaterMessageSet*    m_message;
	CDummyRepeaterBleepSet*      m_bleep;
};

#endif
