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

#include "AnalogueRepeaterThreadHelper.h"

CAnalogueRepeaterThreadHelper::CAnalogueRepeaterThreadHelper(CAnalogueRepeaterThread* thread) :
wxThread(wxTHREAD_JOINABLE),
m_thread(thread)
{
	wxASSERT(thread != NULL);
}

CAnalogueRepeaterThreadHelper::~CAnalogueRepeaterThreadHelper()
{
	delete m_thread;
}

void CAnalogueRepeaterThreadHelper::start()
{
	Create();

	SetPriority(100U);

	Run();
}

void* CAnalogueRepeaterThreadHelper::Entry()
{
	wxASSERT(m_thread != NULL);

	m_thread->run();

	return NULL;
}

void CAnalogueRepeaterThreadHelper::kill()
{
	wxASSERT(m_thread != NULL);

	m_thread->kill();

	Wait();
}

void CAnalogueRepeaterThreadHelper::setOutputs(bool out1, bool out2, bool out3, bool out4)
{
	wxASSERT(m_thread != NULL);

	m_thread->setOutputs(out1, out2, out3, out4);
}

#if defined(__WXDEBUG__)
void CAnalogueRepeaterThreadHelper::setReader(CWAVFileReader* reader)
{
	wxASSERT(m_thread != NULL);

	m_thread->setReader(reader);
}
#endif

void CAnalogueRepeaterThreadHelper::setTbThreshold(wxFloat32 threshold)
{
	wxASSERT(m_thread != NULL);

	m_thread->setTbThreshold(threshold);
}

void CAnalogueRepeaterThreadHelper::setCTCSSThreshold(wxFloat32 threshold)
{
	wxASSERT(m_thread != NULL);

	m_thread->setCTCSSThreshold(threshold);
}

void CAnalogueRepeaterThreadHelper::setDTMFThreshold(wxFloat32 threshold)
{
	wxASSERT(m_thread != NULL);

	m_thread->setDTMFThreshold(threshold);
}

void CAnalogueRepeaterThreadHelper::setId1Level(wxFloat32 level)
{
	wxASSERT(m_thread != NULL);

	m_thread->setId1Level(level);
}

void CAnalogueRepeaterThreadHelper::setId2Level(wxFloat32 level)
{
	wxASSERT(m_thread != NULL);

	m_thread->setId2Level(level);
}

void CAnalogueRepeaterThreadHelper::setAckLevel(wxFloat32 level)
{
	wxASSERT(m_thread != NULL);

	m_thread->setAckLevel(level);
}

void CAnalogueRepeaterThreadHelper::setCTCSSLevel(wxFloat32 level)
{
	wxASSERT(m_thread != NULL);

	m_thread->setCTCSSLevel(level);
}

void CAnalogueRepeaterThreadHelper::shutdown()
{
	wxASSERT(m_thread != NULL);

	m_thread->shutdown();
}

void CAnalogueRepeaterThreadHelper::startup()
{
	wxASSERT(m_thread != NULL);

	m_thread->startup();
}

void CAnalogueRepeaterThreadHelper::timeout()
{
	wxASSERT(m_thread != NULL);

	m_thread->timeout();
}

void CAnalogueRepeaterThreadHelper::timeReset()
{
	wxASSERT(m_thread != NULL);

	m_thread->timeReset();
}

void CAnalogueRepeaterThreadHelper::command1()
{
	wxASSERT(m_thread != NULL);

	m_thread->command1();
}

void CAnalogueRepeaterThreadHelper::command2()
{
	wxASSERT(m_thread != NULL);

	m_thread->command2();
}

CAnalogueRepeaterStatusData* CAnalogueRepeaterThreadHelper::getStatus()
{
	wxASSERT(m_thread != NULL);

	return m_thread->getStatus();
}
