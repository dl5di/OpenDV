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

#include "GMSKRepeaterThreadHelper.h"

CGMSKRepeaterThreadHelper::CGMSKRepeaterThreadHelper(IGMSKRepeaterThread* thread) :
wxThread(wxTHREAD_JOINABLE),
m_thread(thread)
{
	wxASSERT(thread != NULL);
}

CGMSKRepeaterThreadHelper::~CGMSKRepeaterThreadHelper()
{
	delete m_thread;
}

void CGMSKRepeaterThreadHelper::start()
{
	Create();

	SetPriority(100U);

	Run();
}

void* CGMSKRepeaterThreadHelper::Entry()
{
	wxASSERT(m_thread != NULL);

	m_thread->run();

	return NULL;
}

void CGMSKRepeaterThreadHelper::kill()
{
	wxASSERT(m_thread != NULL);

	m_thread->kill();

	Wait();
}

void CGMSKRepeaterThreadHelper::setOutputs(bool out1, bool out2, bool out3, bool out4)
{
	wxASSERT(m_thread != NULL);

	m_thread->setOutputs(out1, out2, out3, out4);
}

void CGMSKRepeaterThreadHelper::setLogging(bool logging, const wxString& dir)
{
	wxASSERT(m_thread != NULL);

	m_thread->setLogging(logging, dir);
}

void CGMSKRepeaterThreadHelper::shutdown()
{
	wxASSERT(m_thread != NULL);

	m_thread->shutdown();
}

void CGMSKRepeaterThreadHelper::startup()
{
	wxASSERT(m_thread != NULL);

	m_thread->startup();
}

void CGMSKRepeaterThreadHelper::command1()
{
	wxASSERT(m_thread != NULL);

	m_thread->command1();
}

void CGMSKRepeaterThreadHelper::command2()
{
	wxASSERT(m_thread != NULL);

	m_thread->command2();
}

void CGMSKRepeaterThreadHelper::command3()
{
	wxASSERT(m_thread != NULL);

	m_thread->command3();
}

void CGMSKRepeaterThreadHelper::command4()
{
	wxASSERT(m_thread != NULL);

	m_thread->command4();
}

CGMSKRepeaterStatusData* CGMSKRepeaterThreadHelper::getStatus()
{
	wxASSERT(m_thread != NULL);

	return m_thread->getStatus();
}
