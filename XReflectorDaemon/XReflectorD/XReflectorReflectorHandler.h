/*
 *   Copyright (C) 2011,2012 by Jonathan Naylor G4KLX
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

#ifndef	XReflectorReflectorHandler_H
#define	XReflectorReflectorHandler_H

#include "XReflectorReflectorCallback.h"
#include "DExtraProtocolHandler.h"
#include "DPlusProtocolHandler.h"
#include "TextCollector.h"
#include "Timer.h"
#include "Defs.h"

#if defined(__WINDOWS__)
#include "Inaddr.h"
#else
#include <netinet/in.h>
#endif

#include <wx/wx.h>
#include <wx/ffile.h>

class CXReflectorReflectorHandler : public IXReflectorReflectorCallback {
public:
	static void initialise(unsigned int maxReflectors, const wxString& name);

	static void setLogDirectory(const wxString& directory);

	static void add(const wxString& callsign);

	static CXReflectorReflectorHandler* findReflector(const wxString& callsign);

	static void finalise();

	static void clock(unsigned long ms);

	virtual bool process(unsigned int id, CHeaderData& header);
	virtual bool process(unsigned int id, CAMBEData& data);
	virtual void timeout(unsigned int id);

protected:
	CXReflectorReflectorHandler(const wxString& callsign);
	~CXReflectorReflectorHandler();

	void clockInt(unsigned long ms);

private:
	static unsigned int                  m_maxReflectors;
	static CXReflectorReflectorHandler** m_reflectors;
	static bool                          m_logEnabled;
	static wxString                      m_name;
	static wxFFile                       m_file;

	wxString       m_callsign;
	wxString       m_gateway;
	unsigned int   m_id;
	wxString       m_user;
	CTextCollector m_text;
	CTimer         m_inactivityTimer;

	void sendToReflectors(const CHeaderData& header);
	void sendToReflectors(const CAMBEData& data);
	void sendToDongles(const CHeaderData& header);
	void sendToDongles(const CAMBEData& data);

	static bool openTextFile(const wxString& directory);
	static void writeTextFile(const wxString& user, const wxString& text);
	static void closeTextFile();
};

#endif
