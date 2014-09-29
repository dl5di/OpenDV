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

#ifndef	BeaconUnit_H
#define	BeaconUnit_H

#include "SlowDataEncoder.h"
#include "BeaconCallback.h"
#include "DStarDefines.h"

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

class CBeaconUnit {
public:
	CBeaconUnit(IBeaconCallback* handler, const wxString& callsign, const wxString& text, bool voice, TEXT_LANG language);
	~CBeaconUnit();

	void sendBeacon();

	void clock();

private:
	unsigned char*   m_ambe;
	unsigned int     m_ambeLength;
	unsigned char*   m_data;
	unsigned int     m_dataLength;
	CIndexList_t     m_index;
	TEXT_LANG        m_language;
	IBeaconCallback* m_handler;
	wxString         m_callsign;
	CSlowDataEncoder m_encoder;
	unsigned int     m_in;
	unsigned int     m_out;
	unsigned int     m_seqNo;
	wxStopWatch      m_time;
	bool             m_sending;

	bool lookup(const wxString& id);
	void spellCallsign(const wxString& callsign);

	bool readAMBE(const wxString& name);
	bool readIndex(const wxString& name);
};

#endif
