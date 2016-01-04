/*
 *   Copyright (C) 2014 by Jonathan Naylor G4KLX
 *   APRSTransmit Copyright (C) 2015 Geoffrey Merck F4FXL / KC3FRA
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

#ifndef	APRSTransmitAppD_H
#define	APRSTransmitAppD_H

#include "RingBuffer.h"
#include "APRSWriterThread.h"

#include <wx/wx.h>
#include <wx/config.h>
#include <wx/snglinst.h>


class CAPRSTransmitAppD {

public:
	CAPRSTransmitAppD(const wxString& repeater, const wxString& aprsHost, unsigned int aprsPort, const wxString& aprsFilter, bool daemon);
	~CAPRSTransmitAppD();

	CRingBuffer<wxString*>  * m_aprsFramesQueue;

	bool init();
	void run();
	void kill();

private:
	wxString m_repeater, m_aprsHost, m_aprsFilter;
	unsigned int m_aprsPort;
	CAPRSWriterThread *      m_aprsThread;
	bool m_run;
	wxSingleInstanceChecker * m_checker;
	bool m_daemon;

	void cleanup();
};

#endif
