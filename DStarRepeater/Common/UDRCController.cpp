/*
 * Header here
 *
 */

#include <wiringPi.h>
#include <errno.h>
#include <stdlib.h>

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

static const char vendor_string_path[] =     "/proc/device-tree/hat/vendor";
static const char product_id_string_path[] = "/proc/device-tree/hat/product_id";
static const char nwdr_vendor_string[] =     "NW Digital Radio";

#define UDRC_PRODUCT_ID		2
#define UDRC_II_PRODUCT_ID	3

static int getNWDRProductID() {
	int proc_file = -1;
	ssize_t read_size;
	char buffer[255];
	long product_id = 0;

	if((proc_file = open(vendor_string_path, O_RDONLY|O_NONBLOCK)) == -1) {
		wxLogError("Open %s: %s\n", vendor_string_path, strerror(errno));
	}

	read_size = read(proc_file, buffer, sizeof(buffer));
	if(read_size < 1) {
		wxLogError("Read %s: %s\n", vendor_string_path, strerror(errno));
		close(proc_file);
		return -1;
	}

	close(proc_file);

	if(strncmp(nwdr_vendor_string, buffer, sizeof(nwdr_vendor_string))) {
		wxLogError("HAT is not a NW Digital Radio Product: %.255s\n", buffer);
		return -1;
	}

	if((proc_file = open(product_id_string_path, O_RDONLY|O_NONBLOCK)) == -1) {
		wxLogError("Open %s: %s\n", product_id_string_path, strerror(errno));
	}

	read_size = read(proc_file, buffer, sizeof(buffer));
	if(read_size < 1) {
		wxLogError("Read %s: %s\n", product_id_string_path, strerror(errno));
		close(proc_file);
		return -1;
	}

	close(proc_file);

	product_id = strtol(buffer, NULL, 16);
	if(product_id == EINVAL || product_id == ERANGE) {
		wxLogError("Invalid value for product id: %.255s\n", buffer);
		return -1;
	}

	return product_id;
}

//  XXX This is ugly, we should have a better parent class
CUDRCController::CUDRCController(enum repeater_modes mode) :
CExternalController(NULL, false),
m_mode(mode),
m_pttPin(PTT_PIN)
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

	if(m_mode != HOTSPOT) {
		::pinMode(m_pttPin, OUTPUT);

		for(int i = 0; i < ARRAY_SIZE(input_pins); ++i) {
			::pinMode(input_pins[i], INPUT);
			::pullUpDnControl(input_pins[i], PUD_UP);
		}

		for(int i = 0; i < ARRAY_SIZE(output_pins); ++i) {
			::pinMode(output_pins[i], OUTPUT);
			::digitalWrite(output_pins[i], HIGH);
		}

		::digitalWrite(m_pttPin, HIGH);

		::digitalWrite(BASE_PIN, LOW);
		switchMode(m_mode);
		::digitalWrite(BASE_PIN, HIGH);
	} else {
		//  On a UDRC II in Hotspot mode, the PTT is on EXT3_PIN instead
		switch(getNWDRProductID()) {
			case UDRC_PRODUCT_ID:
				m_pttPin = PTT_PIN;
				break;
			case UDRC_II_PRODUCT_ID:
				m_pttPin = EXT3_PIN;
				break;
			case -1:
				return false;
			default:
				wxLogError("Unknown NW Digital Radio Product");
				return false;
		}

		::pinMode(m_pttPin, OUTPUT);
		::digitalWrite(m_pttPin, LOW);
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

	::digitalWrite(m_pttPin, value ? LOW : HIGH);
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

