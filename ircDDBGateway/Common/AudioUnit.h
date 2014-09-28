/*
 *   Copyright (C) 2011,2012,2013 by Jonathan Naylor G4KLX
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

#ifndef	AudioUnit_H
#define	AudioUnit_H

#include "RepeaterCallback.h"
#include "SlowDataEncoder.h"
#include "AMBEData.h"
#include "Timer.h"
#include "Defs.h"

#include <wx/wx.h>

class CIndexRecord {
public:
	CIndexRecord(const wxString& name, unsigned int start, unsigned int length) :
	m_name(name),
	m_start(start),
	m_length(length)
	{
	}

	wxString getName() const
	{
		return m_name;
	}

	unsigned int getStart() const
	{
		return m_start;
	}

	unsigned int getLength() const
	{
		return m_length;
	}

private:
	wxString     m_name;
	unsigned int m_start;
	unsigned int m_length;
};

WX_DECLARE_STRING_HASH_MAP(CIndexRecord*, CIndexList_t);

enum AUDIO_STATUS {
	AS_IDLE,
	AS_WAIT,
	AS_TRANSMIT
};

class CAudioUnit {
public:
	CAudioUnit(IRepeaterCallback* handler, const wxString& callsign);
	~CAudioUnit();

	void sendStatus();

	void setStatus(LINK_STATUS status, const wxString& reflector, const wxString& text);
	void setTempStatus(LINK_STATUS status, const wxString& reflector, const wxString& text);

	void cancel();

	void clock(unsigned int ms);

	static void initialise();

	static void setLanguage(TEXT_LANG language);

	static void finalise();

private:
	static unsigned char* m_ambe;
	static unsigned int   m_ambeLength;
	static CIndexList_t   m_index;
	static TEXT_LANG      m_language;

	IRepeaterCallback* m_handler;
	wxString           m_callsign;
	CSlowDataEncoder   m_encoder;
	AUDIO_STATUS       m_status;
	LINK_STATUS        m_linkStatus;
	LINK_STATUS        m_tempLinkStatus;
	wxString           m_text;
	wxString           m_tempText;
	wxString           m_reflector;
	wxString           m_tempReflector;
	bool               m_hasTemporary;
	CTimer             m_timer;
	CAMBEData**        m_data;
	unsigned int       m_in;
	unsigned int       m_out;
	unsigned int       m_seqNo;
	wxStopWatch        m_time;

	bool lookup(unsigned int id, const wxString& name);
	void spellReflector(unsigned int id, const wxString& reflector);
	void sendStatus(LINK_STATUS status, const wxString& reflector, const wxString& text);

	static bool readAMBE(const wxString& name);
	static bool readIndex(const wxString& name);
};

#endif
