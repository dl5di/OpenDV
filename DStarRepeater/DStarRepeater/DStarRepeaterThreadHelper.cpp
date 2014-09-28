/*
 *   Copyright (C) 2012,2014 by Jonathan Naylor G4KLX
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

#include "DStarRepeaterThreadHelper.h"

CDStarRepeaterThreadHelper::CDStarRepeaterThreadHelper(IDStarRepeaterThread* thread) :
wxThread(wxTHREAD_JOINABLE),
m_thread(thread)
{
	wxASSERT(thread != NULL);
}

CDStarRepeaterThreadHelper::~CDStarRepeaterThreadHelper()
{
	delete m_thread;
}

void CDStarRepeaterThreadHelper::start()
{
	Create();

	SetPriority(100U);

	Run();
}

void* CDStarRepeaterThreadHelper::Entry()
{
	wxASSERT(m_thread != NULL);

	m_thread->run();

	return NULL;
}

void CDStarRepeaterThreadHelper::kill()
{
	wxASSERT(m_thread != NULL);

	m_thread->kill();

	Wait();
}

void CDStarRepeaterThreadHelper::setOutputs(bool out1, bool out2, bool out3, bool out4)
{
	wxASSERT(m_thread != NULL);

	m_thread->setOutputs(out1, out2, out3, out4);
}

void CDStarRepeaterThreadHelper::setLogging(bool logging, const wxString& dir)
{
	wxASSERT(m_thread != NULL);

	m_thread->setLogging(logging, dir);
}

void CDStarRepeaterThreadHelper::shutdown()
{
	wxASSERT(m_thread != NULL);

	m_thread->shutdown();
}

void CDStarRepeaterThreadHelper::startup()
{
	wxASSERT(m_thread != NULL);

	m_thread->startup();
}

void CDStarRepeaterThreadHelper::command1()
{
	wxASSERT(m_thread != NULL);

	m_thread->command1();
}

void CDStarRepeaterThreadHelper::command2()
{
	wxASSERT(m_thread != NULL);

	m_thread->command2();
}

void CDStarRepeaterThreadHelper::command3()
{
	wxASSERT(m_thread != NULL);

	m_thread->command3();
}

void CDStarRepeaterThreadHelper::command4()
{
	wxASSERT(m_thread != NULL);

	m_thread->command4();
}

void CDStarRepeaterThreadHelper::command5()
{
	wxASSERT(m_thread != NULL);

	m_thread->command5();
}

void CDStarRepeaterThreadHelper::command6()
{
	wxASSERT(m_thread != NULL);

	m_thread->command6();
}

CDStarRepeaterStatusData* CDStarRepeaterThreadHelper::getStatus()
{
	wxASSERT(m_thread != NULL);

	return m_thread->getStatus();
}
