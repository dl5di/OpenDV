/*
 *   Copyright (C) 2011,2012,2013 by Jonathan Naylor G4KLX
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

#ifndef	DVAPNodeThread_H
#define	DVAPNodeThread_H

#include "RepeaterProtocolHandler.h"
#include "DVAPNodeStatusData.h"
#include "DVAPController.h"
#include "DVAPNodeDefs.h"
#include "CallsignList.h"

#include <wx/wx.h>

enum FRAME_TYPE {
	FRAME_NORMAL,
	FRAME_SYNC,
	FRAME_END
};

class IDVAPNodeThread {
public:
	virtual void setCallsign(const wxString& callsign, const wxString& gateway, DSTAR_MODE mode, ACK_TYPE ack, bool restriction, bool rpt1Validation) = 0;
	virtual void setProtocolHandler(CRepeaterProtocolHandler* handler) = 0;
	virtual void setDVAP(CDVAPController* dvap) = 0;
	virtual void setTimes(unsigned int timeout, unsigned int ackTime) = 0;
	virtual void setBeacon(unsigned int time, const wxString& text, bool voice, TEXT_LANG language) = 0;
	virtual void setAnnouncement(bool enabled, unsigned int time, const wxString& recordRPT1, const wxString& recordRPT2, const wxString& deleteRPT1, const wxString& deleteRPT2) = 0;
	virtual void setLogging(bool logging, const wxString& dir) = 0;
	virtual void setBlackList(CCallsignList* list) = 0;
	virtual void setGreyList(CCallsignList* list) = 0;

	virtual CDVAPNodeStatusData* getStatus() = 0;

	virtual void run() = 0;
	virtual void kill() = 0;

private:
};

#endif
