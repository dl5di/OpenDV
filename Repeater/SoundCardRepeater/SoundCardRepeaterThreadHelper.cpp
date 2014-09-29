/*
 *   Copyright (C) 2012 by Jonathan Naylor G4KLX
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

#include "SoundCardRepeaterThreadHelper.h"

CSoundCardRepeaterThreadHelper::CSoundCardRepeaterThreadHelper(ISoundCardRepeaterThread* thread) :
wxThread(wxTHREAD_JOINABLE),
m_thread(thread)
{
	wxASSERT(thread != NULL);
}

CSoundCardRepeaterThreadHelper::~CSoundCardRepeaterThreadHelper()
{
	delete m_thread;
}

void CSoundCardRepeaterThreadHelper::start()
{
	Create();

	SetPriority(100U);

	Run();
}

void* CSoundCardRepeaterThreadHelper::Entry()
{
	wxASSERT(m_thread != NULL);

	m_thread->run();

	return NULL;
}

void CSoundCardRepeaterThreadHelper::kill()
{
	wxASSERT(m_thread != NULL);

	m_thread->kill();

	Wait();
}

void CSoundCardRepeaterThreadHelper::setOutputs(bool out1, bool out2, bool out3, bool out4)
{
	wxASSERT(m_thread != NULL);

	m_thread->setOutputs(out1, out2, out3, out4);
}

void CSoundCardRepeaterThreadHelper::setLogging(bool logging, const wxString& dir)
{
	wxASSERT(m_thread != NULL);

	m_thread->setLogging(logging, dir);
}

void CSoundCardRepeaterThreadHelper::setReader(CWAVFileReader* reader)
{
	wxASSERT(m_thread != NULL);

	m_thread->setReader(reader);
}

void CSoundCardRepeaterThreadHelper::setRXLevel(wxFloat32 level)
{
	wxASSERT(m_thread != NULL);

	m_thread->setRXLevel(level);
}

void CSoundCardRepeaterThreadHelper::setTXLevel(wxFloat32 level)
{
	wxASSERT(m_thread != NULL);

	m_thread->setTXLevel(level);
}

void CSoundCardRepeaterThreadHelper::setSquelchLevel(wxFloat32 level)
{
	wxASSERT(m_thread != NULL);

	m_thread->setSquelchLevel(level);
}

void CSoundCardRepeaterThreadHelper::shutdown()
{
	wxASSERT(m_thread != NULL);

	m_thread->shutdown();
}

void CSoundCardRepeaterThreadHelper::startup()
{
	wxASSERT(m_thread != NULL);

	m_thread->startup();
}

void CSoundCardRepeaterThreadHelper::command1()
{
	wxASSERT(m_thread != NULL);

	m_thread->command1();
}

void CSoundCardRepeaterThreadHelper::command2()
{
	wxASSERT(m_thread != NULL);

	m_thread->command2();
}

void CSoundCardRepeaterThreadHelper::command3()
{
	wxASSERT(m_thread != NULL);

	m_thread->command3();
}

void CSoundCardRepeaterThreadHelper::command4()
{
	wxASSERT(m_thread != NULL);

	m_thread->command4();
}

CSoundCardRepeaterStatusData* CSoundCardRepeaterThreadHelper::getStatus()
{
	wxASSERT(m_thread != NULL);

	return m_thread->getStatus();
}
