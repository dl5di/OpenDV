/*
 *   Copyright (C) 2010,2012,2014 by Jonathan Naylor G4KLX
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

#ifndef	DummyRepeaterDefs_H
#define	DummyRepeaterDefs_H

#include <wx/wx.h>

#if defined(__WINDOWS__)
const unsigned int BORDER_SIZE   = 5U;
const unsigned int LABEL_WIDTH   = 80U;
const unsigned int CONTROL_WIDTH = 130U;

const unsigned int HEARD_WIDTH   = 690U;
const unsigned int HEARD_HEIGHT  = 300U;

const unsigned int DATETIME_WIDTH   = 135U;
const unsigned int CALLSIGN_WIDTH   = 100U;
const unsigned int MYCALLSIGN_WIDTH = 100U;
const unsigned int MESSAGE_WIDTH    = 250U;
#else
const unsigned int BORDER_SIZE   = 5U;
const unsigned int LABEL_WIDTH   = 80U;
const unsigned int CONTROL_WIDTH = 150U;

const unsigned int HEARD_WIDTH   = 750U;
const unsigned int HEARD_HEIGHT  = 350U;

const unsigned int DATETIME_WIDTH   = 150U;
const unsigned int CALLSIGN_WIDTH   = 100U;
const unsigned int MYCALLSIGN_WIDTH = 120U;
const unsigned int MESSAGE_WIDTH    = 250U;
#endif

const wxString APPLICATION_NAME = wxT("Dummy Repeater");

const wxString LOG_BASE_NAME    = wxT("DummyRepeater");

const wxString CQCQCQ = wxT("CQCQCQ");
const wxString DIRECT = wxT("DIRECT  ");
const wxString UNUSED = wxT("<unused>");

enum DONGLE_TYPE {
	DT_DVDONGLE,
	DT_DV3000
};

#endif
