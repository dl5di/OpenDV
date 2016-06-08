/*
 * Header here
 *
 */

#include <wiringPi.h>

#include "UDRCController.h"

#define ARRAY_SIZE(a) (sizeof((a)) / sizeof((a)[0]))

//  Pin definitions
//  These use the BCM numbering system, *NOT* physical pin numbers or WiringPi
//  "we need to be special" pin numbering.
static const unsigned int BASE_PIN = 6;
static const unsigned int PTT_PIN = 12;
static const unsigned int PKSQL_PIN = 5;
static const unsigned int SQL_PIN = 25;
static const unsigned int EXT1_PIN = 24;
static const unsigned int EXT2_PIN = 22;
static const unsigned int EXT3_PIN = 23;
static const unsigned int EXT4_PIN = 27;

static const unsigned int input_pins[] = {
	SQL_PIN,
	PKSQL_PIN,
};

static const unsigned int output_pins[] = {
	BASE_PIN,
	EXT1_PIN,
	EXT2_PIN,
	EXT3_PIN,
	EXT4_PIN,
};

//  XXX This is ugly, we should have a better parent class
CUDRCController::CUDRCController(enum repeater_modes mode) :
CExternalController(NULL, false),
m_mode(mode)
{
}

void CUDRCController::switchMode(enum repeater_modes mode) {

	if(mode == HOTSPOT)
		return;

	switch(mode) {
		case AUTO_FM:	// Auto/FM
			::digitalWrite(EXT1_PIN, HIGH);
			::digitalWrite(EXT2_PIN, HIGH);
			break;
		case DIGITAL_DIGITAL:	//  Digital/Digital
			::digitalWrite(EXT1_PIN, HIGH);
			::digitalWrite(EXT2_PIN, LOW);
			break;
		case FM_FM: //  FM/FM Fixed
			::digitalWrite(EXT1_PIN, LOW);
			::digitalWrite(EXT2_PIN, HIGH);
			break;
		case AUTO_AUTO:	//  Auto/Auto
			::digitalWrite(EXT1_PIN, LOW);
			::digitalWrite(EXT2_PIN, LOW);
			break;
	}
}

bool CUDRCController::open()
{
	if(::wiringPiSetupGpio() != 0) {
		wxLogError("Unable to initialize the wiringPi library");
		return false;
	}

	::pinMode(PTT_PIN, OUTPUT);

	if(m_mode != HOTSPOT) {
		for(int i = 0; i < ARRAY_SIZE(input_pins); ++i) {
			::pinMode(input_pins[i], INPUT);
			::pullUpDnControl(input_pins[i], PUD_UP);
		}

		for(int i = 0; i < ARRAY_SIZE(output_pins); ++i) {
			::pinMode(output_pins[i], OUTPUT);
			::digitalWrite(output_pins[i], HIGH);
		}

		::digitalWrite(PTT_PIN, HIGH);

		::digitalWrite(BASE_PIN, LOW);
		switchMode(m_mode);
		::digitalWrite(BASE_PIN, HIGH);
	} else {
		::digitalWrite(PTT_PIN, LOW);
	}

	return true;
}

bool CUDRCController::getDisable() const
{
	if(m_mode == HOTSPOT)
		return false;

	return ::digitalRead(PKSQL_PIN) == LOW;
}

void CUDRCController::setRadioTransmit(bool value)
{
	//  If we're in hotspot mode, we're using the miniDIN connector that is
	//  reversed from the normal sense of PTT.
	if(m_mode == HOTSPOT)
		value = !value;

	::digitalWrite(PTT_PIN, value ? LOW : HIGH);
}

void CUDRCController::setActive(bool value)
{
	if(m_mode == HOTSPOT)
		return;

	if(value == HIGH) {
		::digitalWrite(BASE_PIN, LOW);
		switchMode(FM_FM);
	} else {
		switchMode(m_mode);
		::digitalWrite(BASE_PIN, HIGH);
	}
}

//  This needs to be overridden for now because the base class, ExternalController
//  will try to dork around with threads that we're not using anymore.
void CUDRCController::close()
{
}

