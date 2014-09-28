/*
 *   Copyright (C) 2009,2010,2011 by Jonathan Naylor G4KLX
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

#ifndef DTMFController_H
#define	DTMFController_H

#include <wx/wx.h>

#include "DTMFDecoder.h"

enum DTMF_COMMAND {
	DTMF_NONE,
	DTMF_SHUTDOWN,
	DTMF_STARTUP,
	DTMF_TIMEOUT,
	DTMF_TIMERESET,
	DTMF_COMMAND1,
	DTMF_COMMAND2,
	DTMF_OUTPUT1,
	DTMF_OUTPUT2,
	DTMF_OUTPUT3,
	DTMF_OUTPUT4
};

class CDTMFController {
public:
	CDTMFController(unsigned int sampleRate, wxFloat32 threshold, const wxString& shutdown, const wxString& startup,
		const wxString& timeout, const wxString& timeReset, const wxString& command1, const wxString& command2,
		const wxString& output1, const wxString& output2, const wxString& output3, const wxString& output4);
	~CDTMFController();

	void setThreshold(wxFloat32 threshold);

	DTMF_COMMAND process(const wxFloat32* audio, unsigned int length);

	wxString getBuffer() const;

	void reset();

private:
	CDTMFDecoder m_decoder;
	wxString     m_shutdown;
	wxString     m_startup;
	wxString     m_timeout;
	wxString     m_timeReset;
	wxString     m_command1;
	wxString     m_command2;
	wxString     m_output1;
	wxString     m_output2;
	wxString     m_output3;
	wxString     m_output4;
	wxString     m_command;
	wxChar       m_lastChar;
};

#endif
