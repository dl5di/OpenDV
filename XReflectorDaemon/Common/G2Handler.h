/*
 *   Copyright (C) 2010,2012 by Jonathan Naylor G4KLX
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

#ifndef	G2Handler_H
#define	G2Handler_H

#include "G2ProtocolHandler.h"
#include "RepeaterHandler.h"
#include "DStarDefines.h"
#include "HeaderLogger.h"
#include "HeaderData.h"
#include "AMBEData.h"
#include "Timer.h"

#if defined(__WINDOWS__)
#include "Inaddr.h"
#else
#include <netinet/in.h>
#endif

#include <wx/wx.h>

class CG2Handler {
public:
	static void initialise(unsigned int maxRoutes);

	static void setG2ProtocolHandler(CG2ProtocolHandler* handler);
	static void setHeaderLogger(CHeaderLogger* logger);

	static void process(CHeaderData& header);
	static void process(CAMBEData& header);

	static void clock(unsigned int ms);

	static void finalise();

protected:
	CG2Handler(CRepeaterHandler* repeater, const in_addr& address, unsigned int id);
	~CG2Handler();

	bool clockInt(unsigned int ms);

private:
	static unsigned int        m_maxRoutes;
	static CG2Handler**        m_routes;

	static CG2ProtocolHandler* m_handler;

	static CHeaderLogger*      m_headerLogger;

	CRepeaterHandler* m_repeater;
	in_addr           m_address;
	unsigned int      m_id;
	CTimer            m_inactivityTimer;
};

#endif
