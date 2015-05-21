/*
 *   Copyright (C) 2011,2012,2013,2015 by Jonathan Naylor G4KLX
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

#ifndef	DStarRepeaterWindows_H
#define	DStarRepeaterWindows_H

#include "DStarRepeaterThread.h"
#include "DStarRepeaterDefs.h"

#include <string>

class CDStarRepeaterWindows {
public:
	CDStarRepeaterWindows(bool nolog, const std::string& logDir, const std::string& audioDir, const std::string& name);
	~CDStarRepeaterWindows();

	bool init();

	void run();

	void kill();

private:
	std::string           m_name;
	bool                  m_nolog;
	std::string           m_logDir;
	std::string           m_audioDir;
	IDStarRepeaterThread* m_thread;

	bool createThread();
};

#endif
