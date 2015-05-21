/*
 *   Copyright (C) 2012,2014,2015 by Jonathan Naylor G4KLX
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

#include <cstdio>
#include <assert>

CDStarRepeaterThreadHelper::CDStarRepeaterThreadHelper(IDStarRepeaterThread* thread) :
CThread(),
m_thread(thread)
{
	assert(thread != NULL);
}

CDStarRepeaterThreadHelper::~CDStarRepeaterThreadHelper()
{
	delete m_thread;
}

void CDStarRepeaterThreadHelper::start()
{
	run();
}

void CDStarRepeaterThreadHelper::entry()
{
	assert(m_thread != NULL);

	m_thread->run();
}

void CDStarRepeaterThreadHelper::kill()
{
	assert(m_thread != NULL);

	m_thread->kill();

	wait();
}

void CDStarRepeaterThreadHelper::setOutputs(bool out1, bool out2, bool out3, bool out4)
{
	assert(m_thread != NULL);

	m_thread->setOutputs(out1, out2, out3, out4);
}

void CDStarRepeaterThreadHelper::setLogging(bool logging, const std::string& dir)
{
	assert(m_thread != NULL);

	m_thread->setLogging(logging, dir);
}

void CDStarRepeaterThreadHelper::shutdown()
{
	assert(m_thread != NULL);

	m_thread->shutdown();
}

void CDStarRepeaterThreadHelper::startup()
{
	assert(m_thread != NULL);

	m_thread->startup();
}

void CDStarRepeaterThreadHelper::command1()
{
	assert(m_thread != NULL);

	m_thread->command1();
}

void CDStarRepeaterThreadHelper::command2()
{
	assert(m_thread != NULL);

	m_thread->command2();
}

void CDStarRepeaterThreadHelper::command3()
{
	assert(m_thread != NULL);

	m_thread->command3();
}

void CDStarRepeaterThreadHelper::command4()
{
	assert(m_thread != NULL);

	m_thread->command4();
}

void CDStarRepeaterThreadHelper::command5()
{
	assert(m_thread != NULL);

	m_thread->command5();
}

void CDStarRepeaterThreadHelper::command6()
{
	assert(m_thread != NULL);

	m_thread->command6();
}

CDStarRepeaterStatusData* CDStarRepeaterThreadHelper::getStatus()
{
	assert(m_thread != NULL);

	return m_thread->getStatus();
}
