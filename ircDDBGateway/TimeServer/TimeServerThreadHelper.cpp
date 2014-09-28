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

#include "TimeServerThreadHelper.h"

CTimeServerThreadHelper::CTimeServerThreadHelper(CTimeServerThread* thread) :
wxThread(wxTHREAD_JOINABLE),
m_thread(thread)
{
	wxASSERT(thread != NULL);
}

CTimeServerThreadHelper::~CTimeServerThreadHelper()
{
	delete m_thread;
}

void CTimeServerThreadHelper::start()
{
	Create();

	SetPriority(100U);

	Run();
}

void* CTimeServerThreadHelper::Entry()
{
	wxASSERT(m_thread != NULL);

	m_thread->run();

	return NULL;
}

void CTimeServerThreadHelper::kill()
{
	wxASSERT(m_thread != NULL);

	m_thread->kill();

	Wait();
}
