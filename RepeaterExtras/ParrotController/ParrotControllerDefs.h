/*
 *   Copyright (C) 2009-2012 by Jonathan Naylor G4KLX
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

#ifndef	ParrotControllerDefs_H
#define	ParrotControllerDefs_H

#include <wx/wx.h>

const wxString APPLICATION_NAME = wxT("Parrot Controller");
const wxString LOG_BASE_NAME    = wxT("ParrotController");

#if !defined(__WINDOWS__)
const wxString CONFIG_FILE_NAME = wxT("parrotcontroller");
const wxString LOG_DIR  = wxT("/var/log");
const wxString CONF_DIR = wxT("/etc");
#endif

enum PARROT_MODE {
	PM_PARROT,
	PM_BEACON
};

enum PARROT_STATE {
	PS_WAITING,
	PS_RECEIVING,
	PS_TRANSMITTING
};

#endif
