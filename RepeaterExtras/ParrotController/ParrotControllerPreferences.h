/*
 *   Copyright (C) 2009 by Jonathan Naylor G4KLX
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

#ifndef	ParrotControllerPreferences_H
#define	ParrotControllerPreferences_H

#include <wx/wx.h>
#include <wx/notebook.h>

#include "ParrotControllerNetworkSet.h"
#include "ParrotControllerBeaconSet.h"
#include "ParrotControllerParrotSet.h"
#include "ParrotControllerModeSet.h"


class CParrotControllerPreferences : public wxDialog {
public:
	CParrotControllerPreferences(wxWindow* parent, int id, PARROT_MODE mode, unsigned int beaconTime,
		const wxString& beaconFileName, unsigned int turnaroundTime, bool keepFile, const wxString& repeaterAddress,
		unsigned int repeaterPort, const wxString& localAddress, unsigned int localPort);
	virtual ~CParrotControllerPreferences();

	virtual bool Validate();

	virtual PARROT_MODE  getMode() const;
	virtual unsigned int getBeaconTime() const;
	virtual wxString     getBeaconFileName() const;
	virtual unsigned int getTurnaroundTime() const;
	virtual bool         getKeepFile() const;

	virtual wxString     getRepeaterAddress() const;
	virtual unsigned int getRepeaterPort() const;
	virtual wxString     getLocalAddress() const;
	virtual unsigned int getLocalPort() const;

private:
	CParrotControllerModeSet*    m_mode;
	CParrotControllerParrotSet*  m_parrot;
	CParrotControllerBeaconSet*  m_beacon;
	CParrotControllerNetworkSet* m_network;
};

#endif
