/*
 *   Copyright (C) 2010,2011,2012 by Jonathan Naylor G4KLX
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

#ifndef	SplitRepeaterHeaderData_H
#define	SplitRepeaterHeaderData_H

#include <wx/wx.h>

#if defined(__WINDOWS__)
#include "Inaddr.h"
#else
#include <netinet/in.h>
#endif

class CSplitRepeaterHeaderData {
public:
	CSplitRepeaterHeaderData();
	CSplitRepeaterHeaderData(const CSplitRepeaterHeaderData& header);
	CSplitRepeaterHeaderData(const wxString& myCall1,  const wxString& myCall2, const wxString& yourCall,
				const wxString& rptCall1, const wxString& rptCall2, unsigned char flag1 = 0x00,
				unsigned char flag2 = 0x00, unsigned char flag3 = 0x00);
	~CSplitRepeaterHeaderData();

	bool setRepeaterData(const unsigned char* data, unsigned int length, bool check, const in_addr& address, unsigned int port);

	unsigned int getRepeaterData(unsigned char* data, unsigned int length, bool check) const;

	unsigned int getId() const;
	void setId(unsigned int id);

	unsigned char getFlag1() const;
	unsigned char getFlag2() const;
	unsigned char getFlag3() const;

	wxString getMyCall1() const;
	wxString getMyCall2() const;
	wxString getYourCall() const;
	wxString getRptCall1() const;
	wxString getRptCall2() const;

	void setFlag1(unsigned char flag);
	void setFlag2(unsigned char flag);
	void setFlag3(unsigned char flag);
	void setFlags(unsigned char flag1, unsigned char flag2, unsigned char flag3);

	bool isAck() const;
	bool isNoResponse() const;
	bool isRelayUnavailable() const;
	bool isRepeaterMode() const;
	bool isDataPacket() const;
	bool isInterrupted() const;
	bool isControlSignal() const;
	bool isUrgent() const;

	void setRepeaterMode(bool set);
	void setDataPacket(bool set);
	void setInterrupted(bool set);
	void setControlSignal(bool set);
	void setUrgent(bool set);
	void setRepeaterFlags(unsigned char set);

	void setMyCall1(const wxString& callsign);
	void setMyCall2(const wxString& callsign);
	void setYourCall(const wxString& callsign);
	void setRptCall1(const wxString& callsign);
	void setRptCall2(const wxString& callsign);
	void setCQCQCQ();

	void setRepeaters(const wxString& rpt1, const wxString& rpt2);
	void setDestination(const in_addr& address, unsigned int port);

	bool setData(const unsigned char* data, unsigned int length);
	unsigned int getData(unsigned char* data, unsigned int length) const;

	in_addr      getAddress() const;
	unsigned int getPort() const;

	unsigned int getErrors() const;

	static void initialise();
	static void finalise();
	static unsigned int createId();

private:
	unsigned int   m_id;
	unsigned char  m_flag1;
	unsigned char  m_flag2;
	unsigned char  m_flag3;
	unsigned char* m_myCall1;
	unsigned char* m_myCall2;
	unsigned char* m_yourCall;
	unsigned char* m_rptCall1;
	unsigned char* m_rptCall2;
	in_addr        m_address;
	unsigned int   m_port;
	unsigned int   m_errors;
};

#endif
