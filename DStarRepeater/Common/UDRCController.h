#ifndef UDRCController_H
#define UDRCController_H

#include "ExternalController.h"

enum repeater_modes {
	HOTSPOT = 0,
	AUTO_FM = 1,
	DIGITAL_DIGITAL = 3,
	FM_FM = 4,
	AUTO_AUTO = 2,
};

class CUDRCController: public CExternalController {
public:
	CUDRCController(enum repeater_modes mode);

	bool getDisable() const;

	void setRadioTransmit(bool value);
	void setActive(bool value);

	bool open();
	void close();

private:
	const enum repeater_modes m_mode;

	void switchMode(enum repeater_modes mode);
};

#endif // UDRCController_H
