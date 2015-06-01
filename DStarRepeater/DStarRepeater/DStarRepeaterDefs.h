/*
 *   Copyright (C) 2011-2015 by Jonathan Naylor G4KLX
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

#ifndef	DStarRepeaterDefs_H
#define	DStarRepeaterDefs_H

#include <string>

const std::string APPLICATION_NAME = "D-Star Repeater";
const std::string LOG_BASE_NAME    = "DStarRepeater";
const std::string CONFIG_FILE_NAME = "dstarrepeater";

const std::string WHITELIST_FILE_NAME = "whitelist.dat";
const std::string BLACKLIST_FILE_NAME = "blacklist.dat";
const std::string GREYLIST_FILE_NAME  = "greylist.dat";

enum DSTAR_RX_STATE {
	DSRXS_LISTENING,
	DSRXS_PROCESS_DATA,
	DSRXS_PROCESS_SLOW_DATA
};

#endif
