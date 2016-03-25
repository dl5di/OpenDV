/*
 *	UDRCController Copyright (C) 2016 by John Hays, K7VE
 *	based on GPIOController which is
 *	Copyright (C) 2012,2013,2015 by Jonathan Naylor, G4KLX
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

#include "UDRCController.h"
//#include <iostream>
////using namespace std;

CUDRCController::CUDRCController(unsigned int config) :
	m_config(config), 
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

CUDRCController::~CUDRCController() {
}

#if !defined(GPIO)

bool CUDRCController::open() {
	return false;
}

void CUDRCController::getDigitalInputs(bool&, bool&, bool&, bool&, bool&) {
}

void CUDRCController::setDigitalOutputs(bool, bool, bool, bool, bool, bool,
		bool, bool) {
}

void CUDRCController::close() {
}

void CUDRCController::switchMode() {
}

#else

#include <wiringPi.h>

// Pins
#define BASEPIN 22
#define PTTPIN 26
#define PKSQLPIN 21
#define SQLPIN 6
#define EXT1PIN 5
#define EXT2PIN 3
#define EXT3PIN 4
#define EXT4PIN 2

/*
#define BASEPIN 4
#define PTTPIN 3
#define PKSQLPIN 5
#define SQLPIN 6
#define EXT1PIN 2
#define EXT2PIN 0
#define EXT3PIN 22
#define EXT4PIN 21
*/
void CUDRCController::switchMode() {

	switch (m_config) {
		case 1U:// AUTO/FM
				::digitalWrite(EXT1PIN,HIGH);
				::digitalWrite(EXT2PIN,HIGH);
				break;
		case 2U:// AUTO/AUTO
				::digitalWrite(EXT1PIN,LOW);
				::digitalWrite(EXT2PIN,LOW);
				break;
		case 3U:// Digital/Digital
				::digitalWrite(EXT1PIN,HIGH);
				::digitalWrite(EXT2PIN,LOW);
				break;
		case 4U:// FM/FM Fixed
				::digitalWrite(EXT1PIN,LOW);
				::digitalWrite(EXT2PIN,HIGH);
				break;
		default:// AUTO/AUTO
				::digitalWrite(EXT1PIN,LOW);
				::digitalWrite(EXT2PIN,LOW);
	}
	::digitalWrite(BASEPIN,HIGH); // Release Control
}

bool CUDRCController::open()
{
	bool ret = ::wiringPiSetup() != -1;
	if (!ret) {
		wxLogError(wxT("Unable to initialise wiringPi"));
		return false;
	}
	
	::pinMode(PKSQLPIN, INPUT); // Tone Squelch
	::pinMode(SQLPIN, INPUT);// Noise Squelch

	// Set pull ups on the input pins
	::pullUpDnControl(PKSQLPIN, PUD_UP);
	::pullUpDnControl(SQLPIN, PUD_UP);

	::pinMode(BASEPIN, OUTPUT);// Take control as external repeater controller
	::pinMode(PTTPIN, OUTPUT);// PTT
	::pinMode(EXT1PIN, OUTPUT);// EXT 1
	::pinMode(EXT2PIN, OUTPUT);// EXT 2
	::pinMode(EXT3PIN, OUTPUT);// EXT 3
	::pinMode(EXT4PIN, OUTPUT);// EXT 4
	::pullUpDnControl(PTTPIN, PUD_UP);
	::pullUpDnControl(EXT1PIN, PUD_UP);
	::pullUpDnControl(EXT2PIN, PUD_UP);


	setDigitalOutputs(false, false, false, false, false, false, false, false); // Start in FM/FM Fixed
	::pinMode(BASEPIN, LOW);
	switchMode();
	::pinMode(BASEPIN, HIGH);
	return true;
}

void CUDRCController::getDigitalInputs(bool& inp1, bool& inp2, bool& inp3, bool& inp4, bool& inp5)
{

	inp1 = LOW;

	inp2 = LOW;

	inp3 = LOW;

	inp4 = LOW;

	bool pksql = ::digitalRead(PKSQLPIN);
//	if (pksql == LOW) {
//		m_udrc_timer.Start(0);
//	} else {
//		if (m_udrc_timer.Time() < 200) {
//			pksql = LOW; // if 200 ms have not 
//				     // passed simulate a low
//			// cerr << "simulate pksql" << endl;
//		}
//	}

	inp5 = pksql == LOW;
}

void CUDRCController::setDigitalOutputs(bool outp1, bool outp2, bool outp3, bool outp4, bool outp5, bool outp6, bool outp7, bool outp8)
{

	::digitalWrite(EXT3PIN, HIGH);
	::digitalWrite(EXT4PIN, HIGH);

	if (outp1 != m_outp1) { // PTT
		::digitalWrite(PTTPIN, outp1 ? HIGH : LOW);
		m_outp1 = outp1;
	}

	if (outp2 != m_outp2) {
	//	::digitalWrite(3, outp2 ? HIGH : LOW);
		m_outp2 = outp2;
	}

	if (outp3 != m_outp3) { // Heartbeat
	//	::digitalWrite(2, outp3 ? HIGH : LOW);
		m_outp3 = outp3;
	}

	if (outp4 != m_outp4) { // Active (Repeater Takeover)
		if (outp4 == HIGH) { // FM/FM Fixed
			::digitalWrite(BASEPIN,LOW);
			::digitalWrite(EXT1PIN,LOW);
			::digitalWrite(EXT2PIN,HIGH);
//			::digitalWrite(0,HIGH);
		} else {
			// ::digitalWrite(4,HIGH); // Release Control
		//	usleep(150000); // 100 ms
		//	cerr << "Sleeping 150 ms" << endl;
			switchMode();
		}
		m_outp4 = outp4;
	}

	if (outp5 != m_outp5) { // EXT1
	//	::digitalWrite(2, outp5 ? HIGH : LOW);
		m_outp5 = outp5;
	}

	if (outp6 != m_outp6) { // EXT2
	//	::digitalWrite(0, outp6 ? HIGH : LOW);
		m_outp6 = outp6;
	}

	if (outp7 != m_outp7) {
	//	::digitalWrite(22, outp7 ? HIGH : LOW);
		m_outp7 = outp7;
	}

	if (outp8 != m_outp8) {
	//	::digitalWrite(21, outp8 ? HIGH : LOW);
		m_outp8 = outp8;
	}

}

void CUDRCController::close()
{
}

#endif
