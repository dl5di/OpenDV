/*
 *	Copyright (C) 2012,2013 by Jonathan Naylor, G4KLX
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

#include "RaspberryController.h"

CRaspberryController::CRaspberryController() :
m_outp1(false),
m_outp2(false),
m_outp3(false),
m_outp4(false),
m_outp5(false),
m_outp6(false),
m_outp7(false),
m_outp8(false)
{
}

CRaspberryController::~CRaspberryController()
{
}

#if !defined(RASPBERRY_PI)

bool CRaspberryController::open()
{
	return false;
}

void CRaspberryController::getDigitalInputs(bool&, bool&, bool&, bool&, bool&)
{
}

void CRaspberryController::setDigitalOutputs(bool, bool, bool, bool, bool, bool, bool, bool)
{
}

void CRaspberryController::close()
{
}

#else

#include <wiringPi.h>

bool CRaspberryController::open()
{
	bool ret = ::wiringPiSetup() != -1;
	if (!ret) {
		wxLogError(wxT("Unable to initialise wiringPi"));
		return false;
	}

	::pinMode(8, INPUT);
	::pinMode(9, INPUT);
	::pinMode(7, INPUT);
	::pinMode(0, INPUT);
	::pinMode(2, INPUT);

	// Set pull ups on the input pins
	::pullUpDnControl(8, PUD_UP);
	::pullUpDnControl(9, PUD_UP);
	::pullUpDnControl(7, PUD_UP);
	::pullUpDnControl(0, PUD_UP);
	::pullUpDnControl(2, PUD_UP);

	::pinMode(12, OUTPUT);
	::pinMode(13, OUTPUT);
	::pinMode(14, OUTPUT);
	::pinMode(11, OUTPUT);
	::pinMode(10, OUTPUT);
	::pinMode(6,  OUTPUT);
	::pinMode(5,  OUTPUT);
	::pinMode(4,  OUTPUT);

	setDigitalOutputs(false, false, false, false, false, false, false, false);

	return true;
}

void CRaspberryController::getDigitalInputs(bool& inp1, bool& inp2, bool& inp3, bool& inp4, bool& inp5)
{
	inp1 = ::digitalRead(8) == LOW;

	inp2 = ::digitalRead(9) == LOW;

	inp3 = ::digitalRead(7) == LOW;

	inp4 = ::digitalRead(0) == LOW;

	inp5 = ::digitalRead(2) == LOW;
}

void CRaspberryController::setDigitalOutputs(bool outp1, bool outp2, bool outp3, bool outp4, bool outp5, bool outp6, bool outp7, bool outp8)
{
	if (outp1 != m_outp1) {
		::digitalWrite(12, outp1 ? HIGH : LOW);
		m_outp1 = outp1;
	}

	if (outp2 != m_outp2) {
		::digitalWrite(13, outp2 ? HIGH : LOW);
		m_outp2 = outp2;
	}

	if (outp3 != m_outp3) {
		::digitalWrite(14, outp3 ? HIGH : LOW);
		m_outp3 = outp3;
	}

	if (outp4 != m_outp4) {
		::digitalWrite(11, outp4 ? HIGH : LOW);
		m_outp4 = outp4;
	}

	if (outp5 != m_outp5) {
		::digitalWrite(10, outp5 ? HIGH : LOW);
		m_outp5 = outp5;
	}

	if (outp6 != m_outp6) {
		::digitalWrite(6, outp6 ? HIGH : LOW);
		m_outp6 = outp6;
	}

	if (outp7 != m_outp7) {
		::digitalWrite(5, outp7 ? HIGH : LOW);
		m_outp7 = outp7;
	}

	if (outp8 != m_outp8) {
		::digitalWrite(4, outp8 ? HIGH : LOW);
		m_outp8 = outp8;
	}
}

void CRaspberryController::close()
{
}

#endif
