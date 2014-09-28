/*
 *   Copyright (C) 2009,2010,2014 by Jonathan Naylor G4KLX
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

#ifndef	DVToolWriterPreferences_H
#define	DVToolWriterPreferences_H

#include <wx/wx.h>

#include "SoundcardSet.h"
#include "CallsignSet.h"
#include "DongleSet.h"

class CDVToolWriterPreferences : public wxDialog {
public:
	CDVToolWriterPreferences(wxWindow* parent, int id, const wxString& callsign1, const wxString& callsign2, DONGLE_TYPE type, const wxString& device, const wxString& address, unsigned int port, const wxString& soundcard);
	virtual ~CDVToolWriterPreferences();

	virtual bool Validate();

	virtual wxString     getCallsign1() const;
	virtual wxString     getCallsign2() const;
	virtual DONGLE_TYPE  getType() const;
	virtual wxString     getDevice() const;
	virtual wxString     getAddress() const;
	virtual unsigned int getPort() const;

	virtual wxString getSoundcard() const;

private:
	CCallsignSet*  m_callsign;
	CSoundcardSet* m_soundcard;
	CDongleSet*    m_dongle;
};

#endif
