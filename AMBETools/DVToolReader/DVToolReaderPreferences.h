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

#ifndef	DVToolReaderPreferences_H
#define	DVToolReaderPreferences_H

#include <wx/wx.h>

#include "SoundcardSet.h"
#include "DongleSet.h"
#include "BleepSet.h"

class CDVToolReaderPreferences : public wxDialog {
public:
	CDVToolReaderPreferences(wxWindow* parent, int id, DONGLE_TYPE type, const wxString& device, const wxString& address, unsigned int port, const wxString& soundcard, bool bleep);
	virtual ~CDVToolReaderPreferences();

	virtual bool Validate();

	virtual DONGLE_TYPE getType() const;
	virtual wxString  getDevice() const;
	virtual wxString  getAddress() const;
	virtual unsigned int getPort() const;

	virtual wxString getSoundcard() const;

	virtual bool     getBleep() const;

private:
	CSoundcardSet* m_soundcard;
	CDongleSet*    m_dongle;
	CBleepSet*     m_bleep;
};

#endif
