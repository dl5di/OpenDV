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

#include "DTMFController.h"

CDTMFController::CDTMFController(unsigned int sampleRate, wxFloat32 threshold, const wxString& shutdown,
								 const wxString& startup, const wxString& timeout, const wxString& timeReset,
								 const wxString& command1, const wxString& command2, const wxString& output1,
								 const wxString& output2, const wxString& output3, const wxString& output4) :
m_decoder(sampleRate, threshold),
m_shutdown(shutdown),
m_startup(startup),
m_timeout(timeout),
m_timeReset(timeReset),
m_command1(command1),
m_command2(command2),
m_output1(output1),
m_output2(output2),
m_output3(output3),
m_output4(output4),
m_command(),
m_lastChar(wxT(' '))
{
	wxASSERT(sampleRate > 0U);
	wxASSERT(threshold >= 0.0F);

	if (!m_shutdown.IsEmpty())
		m_shutdown.Append(wxT('#'));
	if (!m_startup.IsEmpty())
		m_startup.Append(wxT('#'));
	if (!m_timeout.IsEmpty())
		m_timeout.Append(wxT('#'));
	if (!m_timeReset.IsEmpty())
		m_timeReset.Append(wxT('#'));
	if (!m_command1.IsEmpty())
		m_command1.Append(wxT('#'));
	if (!m_command2.IsEmpty())
		m_command2.Append(wxT('#'));
	if (!m_output1.IsEmpty())
		m_output1.Append(wxT('#'));
	if (!m_output2.IsEmpty())
		m_output2.Append(wxT('#'));
	if (!m_output3.IsEmpty())
		m_output3.Append(wxT('#'));
	if (!m_output4.IsEmpty())
		m_output4.Append(wxT('#'));
}

CDTMFController::~CDTMFController()
{
}

void CDTMFController::setThreshold(wxFloat32 threshold)
{
	wxASSERT(threshold >= 0.0F);

	m_decoder.setThreshold(threshold);
}

DTMF_COMMAND CDTMFController::process(const wxFloat32* audio, unsigned int length)
{
	wxASSERT(audio != NULL);

	// If no audio, then don't do any processing
	if (length == 0U)
		return DTMF_NONE;

	m_decoder.process(audio, length);

	wxChar c = m_decoder.getCharacter();

	// If the same as the last decode or an unknown....
	if (c == m_lastChar || c == wxT('-'))
		return DTMF_NONE;

	switch (c) {
		case wxT(' '):
			break;
		case wxT('*'):
			m_command.Clear();
			break;
		default:
			m_command.Append(c);
			break;
	}

	m_lastChar = c;

	// Is it the end of the command?
	if (c != wxT('#'))
		return DTMF_NONE;

	if (!m_shutdown.IsEmpty() && m_shutdown.IsSameAs(m_command)) {
		m_command.Clear();
		return DTMF_SHUTDOWN;
	}

	if (!m_startup.IsEmpty() && m_startup.IsSameAs(m_command)) {
		m_command.Clear();
		return DTMF_STARTUP;
	}

	if (!m_timeout.IsEmpty() && m_timeout.IsSameAs(m_command)) {
		m_command.Clear();
		return DTMF_TIMEOUT;
	}

	if (!m_timeReset.IsEmpty() && m_timeReset.IsSameAs(m_command)) {
		m_command.Clear();
		return DTMF_TIMERESET;
	}

	if (!m_command1.IsEmpty() && m_command1.IsSameAs(m_command)) {
		m_command.Clear();
		return DTMF_COMMAND1;
	}

	if (!m_command2.IsEmpty() && m_command2.IsSameAs(m_command)) {
		m_command.Clear();
		return DTMF_COMMAND2;
	}

	if (!m_output1.IsEmpty() && m_output1.IsSameAs(m_command)) {
		m_command.Clear();
		return DTMF_OUTPUT1;
	}

	if (!m_output2.IsEmpty() && m_output2.IsSameAs(m_command)) {
		m_command.Clear();
		return DTMF_OUTPUT2;
	}

	if (!m_output3.IsEmpty() && m_output3.IsSameAs(m_command)) {
		m_command.Clear();
		return DTMF_OUTPUT3;
	}

	if (!m_output4.IsEmpty() && m_output4.IsSameAs(m_command)) {
		m_command.Clear();
		return DTMF_OUTPUT4;
	}

	m_command.Clear();
	return DTMF_NONE;
}

wxString CDTMFController::getBuffer() const
{
	return m_command;
}

void CDTMFController::reset()
{
	m_lastChar = wxT(' ');
	m_command.Clear();
}
