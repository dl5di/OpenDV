/*
 *   Copyright (C) 2012,2013 by Jonathan Naylor G4KLX
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

#ifndef	AnalogueRepeaterThreadHelper_H
#define	AnalogueRepeaterThreadHelper_H

#include "AnalogueRepeaterStatusData.h"
#include "AnalogueRepeaterThread.h"
#include "WAVFileReader.h"

#include <wx/wx.h>

class CAnalogueRepeaterThreadHelper : public wxThread {

public:
	CAnalogueRepeaterThreadHelper(CAnalogueRepeaterThread* thread);
	virtual ~CAnalogueRepeaterThreadHelper();

	virtual void start();

	virtual void* Entry();

	virtual void kill();

	virtual void setOutputs(bool out1, bool out2, bool out3, bool out4);
#if defined(__WXDEBUG__)
	virtual void setReader(CWAVFileReader* reader);
#endif

	virtual void setTbThreshold(wxFloat32 threshold);
	virtual void setCTCSSThreshold(wxFloat32 threshold);
	virtual void setDTMFThreshold(wxFloat32 threshold);

	virtual void setId1Level(wxFloat32 level);
	virtual void setId2Level(wxFloat32 level);
	virtual void setAckLevel(wxFloat32 level);
	virtual void setCTCSSLevel(wxFloat32 level);

	virtual void shutdown();
	virtual void startup();
	virtual void timeout();
	virtual void timeReset();
	virtual void command1();
	virtual void command2();

	virtual CAnalogueRepeaterStatusData* getStatus();

private:
	CAnalogueRepeaterThread* m_thread;
};

#endif
