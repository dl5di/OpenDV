/*
 *   Copyright (C) 2009,2010 by Jonathan Naylor G4KLX
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

#ifndef	HeaderData_H
#define	HeaderData_H

#include <wx/wx.h>

class CHeaderData {
public:
	CHeaderData();
	CHeaderData(const CHeaderData& header);
	CHeaderData(const unsigned char* data, unsigned int length, bool check);
	CHeaderData(const wxString& myCall1,  const wxString& myCall2, const wxString& yourCall,
				const wxString& rptCall1, const wxString& rptCall2, unsigned char flag1 = 0x00,
				unsigned char flag2 = 0x00, unsigned char flag3 = 0x00);
	~CHeaderData();

	wxDateTime getTime() const;
	wxString   getMyCall1() const;
	wxString   getMyCall2() const;
	wxString   getYourCall() const;
	wxString   getRptCall1() const;
	wxString   getRptCall2() const;

	unsigned char getFlag1() const;
	unsigned char getFlag2() const;
	unsigned char getFlag3() const;

	bool isRepeaterMode() const;
	bool isDataPacket() const;
	bool isInterrupted() const;
	bool isControlSignal() const;
	bool isUrgent() const;
	unsigned char getRepeaterFlags() const;

	void setRepeaterMode(bool set);
	void setDataPacket(bool set);
	void setInterrupted(bool set);
	void setControlSignal(bool set);
	void setUrgent(bool set);
	void setRepeaterFlags(unsigned char set);

	bool isValid() const;

	void reset();

private:
	wxDateTime    m_time;
	wxString      m_myCall1;
	wxString      m_myCall2;
	wxString      m_yourCall;
	wxString      m_rptCall1;
	wxString      m_rptCall2;
	unsigned char m_flag1;
	unsigned char m_flag2;
	unsigned char m_flag3;
	bool          m_valid;
};

#endif
