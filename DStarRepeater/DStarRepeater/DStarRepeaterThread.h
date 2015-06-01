/*
 *   Copyright (C) 2011-2015 by Jonathan Naylor G4KLX
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

#ifndef	DStarRepeaterThread_H
#define	DStarRepeaterThread_H

#include "DStarRepeaterStatusData.h"
#include "RepeaterProtocolHandler.h"
#include "ExternalController.h"
#include "DStarRepeaterWindowsefs.h"
#include "CallsignList.h"
#include "Modem.h"

#include <wx/wx.h>

enum FRAME_TYPE {
	FRAME_NORMAL,
	FRAME_SYNC,
	FRAME_END
};

class IDStarRepeaterThread {
public:
	virtual ~IDStarRepeaterThread() = 0;

	virtual void setCallsign(const std::string& callsign, const std::string& gateway, DSTAR_MODE mode, ACK_TYPE ack, bool restriction, bool rpt1Validation, bool dtmfBlanking, bool errorReply) = 0;
	virtual void setProtocolHandler(CRepeaterProtocolHandler* handler, bool local) = 0;
	virtual void setModem(CModem* modem) = 0;
	virtual void setController(CExternalController* controller, unsigned int activeHangTime) = 0;
	virtual void setTimes(unsigned int timeout, unsigned int ackTime) = 0;
	virtual void setBeacon(unsigned int time, const std::string& text, bool voice, TEXT_LANG language) = 0;
	virtual void setAnnouncement(bool enabled, unsigned int time, const std::string& recordRPT1, const std::string& recordRPT2, const std::string& deleteRPT1, const std::string& deleteRPT2) = 0;
	virtual void setControl(bool enabled, const std::string& rpt1Callsign, const std::string& rpt2Callsign, const std::string& shutdown, const std::string& startup, const std::string& status1, const std::string& status2, const std::string& status3, const std::string& status4, const std::string& status5, const std::string& command1, const std::string& command1Line, const std::string& command2, const std::string& command2Line, const std::string& command3, const std::string& command3Line, const std::string& command4, const std::string& command4Line, const std::string& command5, const std::string& command5Line, const std::string& command6, const std::string& command6Line, const std::string& output1, const std::string& output2, const std::string& output3, const std::string& output4) = 0;
	virtual void setOutputs(bool out1, bool out2, bool out3, bool out4) = 0;
	virtual void setLogging(bool logging, const std::string& dir) = 0;
	virtual void setWhiteList(CCallsignList* list) = 0;
	virtual void setBlackList(CCallsignList* list) = 0;
	virtual void setGreyList(CCallsignList* list) = 0;

	virtual void shutdown() = 0;
	virtual void startup() = 0;

	virtual void command1() = 0;
	virtual void command2() = 0;
	virtual void command3() = 0;
	virtual void command4() = 0;
	virtual void command5() = 0;
	virtual void command6() = 0;

	virtual CDStarRepeaterStatusData* getStatus() = 0;

	virtual void run() = 0;
	virtual void kill() = 0;

private:
};

#endif
