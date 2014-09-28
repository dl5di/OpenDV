/*
 *	Copyright (C) 2009,2010,2013 by Jonathan Naylor, G4KLX
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

#ifndef	ExternalController_H
#define	ExternalController_H

#include "HardwareController.h"

#include <wx/wx.h>

class CExternalController : public wxThread {
public:
	CExternalController(IHardwareController* controller, bool pttInvert = false, bool squelchInvert = false);
	virtual ~CExternalController();

	virtual bool open();

	virtual bool getRadioSquelch1() const;
	virtual bool getRadioSquelch2() const;
	virtual bool getExternalSquelch() const;
	virtual bool getBattery() const;
	virtual bool getDisable() const;

	virtual void setRadioTransmit(bool value);
	virtual void setExternalTransmit(bool value);
	virtual void setHeartbeat();
	virtual void setActive(bool value);
	virtual void setOutput1(bool value);
	virtual void setOutput2(bool value);
	virtual void setOutput3(bool value);
	virtual void setOutput4(bool value);

	virtual void close();

	virtual void* Entry();

private:
	IHardwareController* m_controller;
	bool                 m_pttInvert;
	bool                 m_squelchInvert;
	bool                 m_radioSquelch1;
	bool                 m_radioSquelch2;
	bool                 m_externalSquelch;
	bool                 m_battery;
	bool                 m_disable;
	bool                 m_heartbeat;
	bool                 m_active;
	bool                 m_radioTX;
	bool                 m_externalTX;
	bool                 m_out1;
	bool                 m_out2;
	bool                 m_out3;
	bool                 m_out4;
	bool                 m_kill;
};

#endif
