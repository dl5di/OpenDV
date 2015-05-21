/*
 *	Copyright (C) 2009,2010,2014,2015 by Jonathan Naylor, G4KLX
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; version 2 of the License.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 */

#include "ExternalController.h"

#include "DStarDefines.h"

#include <cassert>

CExternalController::CExternalController(IHardwareController* controller, bool pttInvert) :
CThread(),
m_controller(controller),
m_pttInvert(pttInvert),
m_disable(false),
m_heartbeat(false),
m_active(false),
m_radioTX(false),
m_out1(false),
m_out2(false),
m_out3(false),
m_out4(false),
m_kill(false)
{
	assert(controller != NULL);

	if (m_pttInvert)
		m_radioTX = true;
}

CExternalController::~CExternalController()
{
	delete m_controller;
}

bool CExternalController::open()
{
	bool res = m_controller->open();
	if (!res)
		return false;

	run();

	return true;
}

void CExternalController::entry()
{
	assert(m_controller != NULL);

	bool dummy1, dummy2, dummy3, dummy4;

	while (!m_kill) {
		m_controller->setDigitalOutputs(m_radioTX, false, m_heartbeat, m_active, m_out1, m_out2, m_out3, m_out4);
		m_controller->getDigitalInputs(dummy1, dummy2, dummy3, dummy4, m_disable);

		Sleep(DSTAR_FRAME_TIME_MS / 2U);
	}

	if (m_pttInvert)
		m_controller->setDigitalOutputs(true, false, false, false, false, false, false, false);
	else
		m_controller->setDigitalOutputs(false, false, false, false, false, false, false, false);
	m_controller->getDigitalInputs(dummy1, dummy2, dummy3, dummy4, m_disable);

	Sleep(DSTAR_FRAME_TIME_MS * 3U);

	if (m_pttInvert)
		m_controller->setDigitalOutputs(true, false, false, false, false, false, false, false);
	else
		m_controller->setDigitalOutputs(false, false, false, false, false, false, false, false);
	m_controller->getDigitalInputs(dummy1, dummy2, dummy3, dummy4, m_disable);

	m_controller->close();
}

bool CExternalController::getDisable() const
{
	return m_disable;
}

void CExternalController::setRadioTransmit(bool value)
{
	if (m_pttInvert)
		value = !value;

	m_radioTX = value;
}

void CExternalController::setHeartbeat()
{
	m_heartbeat = !m_heartbeat;
}

void CExternalController::setActive(bool value)
{
	m_active = value;
}

void CExternalController::setOutput1(bool value)
{
	m_out1 = value;
}

void CExternalController::setOutput2(bool value)
{
	m_out2 = value;
}

void CExternalController::setOutput3(bool value)
{
	m_out3 = value;
}

void CExternalController::setOutput4(bool value)
{
	m_out4 = value;
}

void CExternalController::close()
{
	m_kill = true;

	wait();
}
