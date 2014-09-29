/*
 *   Copyright (C) 2010,2011,2012 by Jonathan Naylor G4KLX
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

#ifndef	DCSGatewayThread_H
#define	DCSGatewayThread_H

#include "DCSGatewayRepeaterProtocolHandler.h"
#include "DCSGatewayDCSProtocolHandler.h"
#include "DCSGatewayTextCollector.h"
#include "DCSGatewayStatusData.h"
#include "DCSGatewayAudioUnit.h"
#include "DCSGatewayEchoUnit.h"
#include "DCSGatewayCallback.h"
#include "DCSGatewayDefs.h"
#include "Timer.h"

#include <wx/wx.h>

class CDCSGatewayThread : public IDCSGatewayCallback, public wxThread {
public:
	CDCSGatewayThread();
	virtual ~CDCSGatewayThread();

	virtual void setReflector(const wxString& callsign, const wxString& reflector, bool atStartup, RECONNECT reconnect, TEXT_LANG language);
	virtual void setRepeater(const wxString& repeaterCallsign, CDCSGatewayRepeaterProtocolHandler* repaterHandler);

	virtual CDCSGatewayStatusData* getStatus();

	virtual void* Entry();
	virtual void  kill();

	virtual bool  writeHeader(CDCSGatewayHeaderData& header);
	virtual bool  writeData(CDCSGatewayAMBEData& data);

private:
	bool                                m_killed;
	bool                                m_stopped;
	wxArrayString                       m_reflectors;
	wxArrayString                       m_hostnames;
	CDCSGatewayRepeaterProtocolHandler* m_repeaterHandler;
	CDCSGatewayDCSProtocolHandler       m_dcsHandler;
	CDCSGatewayAudioUnit*               m_audioUnit;
	CDCSGatewayEchoUnit*                m_echoUnit;
	CDCSGatewayHeaderData               m_header;
	CDCSGatewayTextCollector            m_textCollector;
	wxString                            m_text;
	wxString                            m_myCall;
	wxString                            m_yourCall;
	wxString                            m_rptCall1;
	wxString                            m_rptCall2;
	wxString                            m_repeaterCall;
	LINK_STATE                          m_state;
	wxString                            m_reflector;
	wxString                            m_startupReflector;
	bool                                m_atStartup;
	RECONNECT                           m_reconnect;
	TEXT_LANG                           m_language;
	CTimer                              m_reconnectTimer;
	CTimer                              m_watchdogTimer;
	unsigned int                        m_id;
	unsigned int                        m_seq;
	bool                                m_echo;
	bool                                m_info;

	void commandHandler(const wxString& yourCall);
	void unlink();
	bool link(const wxString& reflector);
	void showHeader(const CDCSGatewayHeaderData& header);
	void clearHeader();

	void writeLinkedTo(const wxString& callsign);
	void writeLinkingTo(const wxString& callsign);
	void writeNotLinked();
};

#endif
