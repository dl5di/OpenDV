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

#ifndef	ParrotControllerThread_H
#define	ParrotControllerThread_H

#include "ParrotControllerProtocolHandler.h"
#include "ParrotControllerStatusData.h"
#include "ParrotControllerDefs.h"
#include "DVTOOLFileReader.h"
#include "DVTOOLFileWriter.h"
#include "Timer.h"

#include <wx/wx.h>

class CParrotControllerThread {
public:
	CParrotControllerThread();
	virtual ~CParrotControllerThread();

	virtual void setMode(PARROT_MODE mode, unsigned int beaconTime, const wxString& beaconFileName, unsigned int turnaroundTime, bool keepFile);
	virtual void setNetwork(CParrotControllerProtocolHandler* handler);

	virtual CParrotControllerStatusData* getStatus();

	virtual void run();
	virtual void kill();

private:
	PARROT_MODE                       m_mode;
	PARROT_STATE                      m_state;
	CTimer                            m_beaconTimer;
	wxString                          m_beaconFileName;
	bool                              m_keepFile;
	bool                              m_killed;
	bool                              m_stopped;
	CParrotControllerProtocolHandler* m_protocolHandler;
	CHeaderData*                      m_header;
	unsigned int                      m_dataTimer;
	unsigned int                      m_dataTimeout;
	CTimer                            m_turnaroundTimer;
	CTimer                            m_watchdogTimer;
	wxString                          m_fileName;
	CDVTOOLFileReader                 m_reader;
	CDVTOOLFileWriter                 m_writer;

	bool transmit(const wxString& fileName);
	bool transmit();
	bool receive(NETWORK_TYPE type);
};

#endif
