/*
 *   Copyright (C) 2012,2013 by Jonathan Naylor G4KLX
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

#ifndef	TimeServerThread_H
#define	TimeServerThread_H

#include "SlowDataEncoder.h"
#include "UDPReaderWriter.h"
#include "TimeServerDefs.h"
#include "HeaderData.h"
#include "AMBEData.h"

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

class CTimeServerThread {
public:
	CTimeServerThread();
	virtual ~CTimeServerThread();

	virtual bool setGateway(const wxString& callsign, bool sendA, bool sendB, bool sendC, bool sendD, bool sendE, const wxString& address);
	virtual void setAnnouncements(LANGUAGE language, FORMAT format, INTERVAL interval);

	virtual void run();
	virtual void kill();

private:
	CUDPReaderWriter m_socket;
	wxString         m_callsign;
	wxString         m_callsignA;
	wxString         m_callsignB;
	wxString         m_callsignC;
	wxString         m_callsignD;
	wxString         m_callsignE;
	wxString         m_callsignG;
	in_addr          m_address;
	LANGUAGE         m_language;
	FORMAT           m_format;
	INTERVAL         m_interval;
	unsigned char*   m_ambe;
	unsigned int     m_ambeLength;
	CIndexList_t     m_index;
	unsigned int     m_seqNo;
	unsigned int     m_in;
	CSlowDataEncoder m_encoder;
	CAMBEData**      m_data;
	bool             m_killed;

	void sendTime(unsigned int hour, unsigned int min);

	wxArrayString sendTimeEnGB1(unsigned int hour, unsigned int min);
	wxArrayString sendTimeEnGB2(unsigned int hour, unsigned int min);
	wxArrayString sendTimeEnUS1(unsigned int hour, unsigned int min);
	wxArrayString sendTimeEnUS2(unsigned int hour, unsigned int min);
	wxArrayString sendTimeDeDE1(unsigned int hour, unsigned int min);
	wxArrayString sendTimeDeDE2(unsigned int hour, unsigned int min);
	wxArrayString sendTimeFrFR(unsigned int hour, unsigned int min);
	wxArrayString sendTimeNlNL(unsigned int hour, unsigned int min);
	wxArrayString sendTimeSeSE(unsigned int hour, unsigned int min);
	wxArrayString sendTimeEsES(unsigned int hour, unsigned int min);
	wxArrayString sendTimeNoNO(unsigned int hour, unsigned int min);
	wxArrayString sendTimePtPT(unsigned int hour, unsigned int min);

	bool send(const wxArrayString& words, unsigned int hour, unsigned int min);
	bool sendHeader(const CHeaderData& header);
	bool sendData(const CAMBEData& data);

	bool loadAMBE();
	bool readAMBE(const wxString& name);
	bool readIndex(const wxString& name);

	bool lookup(const wxString& id);
	void end();
};

#endif
