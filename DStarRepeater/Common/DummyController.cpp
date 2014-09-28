/*
 *	Copyright (C) 2009,2013 by Jonathan Naylor, G4KLX
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

#include "DummyController.h"

#include <wx/wx.h>

CDummyController::CDummyController()
{
}

CDummyController::~CDummyController()
{
}

bool CDummyController::open()
{
	return true;
}

void CDummyController::getDigitalInputs(bool& inp1, bool& inp2, bool& inp3, bool& inp4, bool& inp5)
{
	inp1 = false;
	inp2 = false;
	inp3 = false;
	inp4 = false;
	inp5 = false;
}

void CDummyController::setDigitalOutputs(bool, bool, bool, bool, bool, bool, bool, bool)
{
}

void CDummyController::close()
{
}
