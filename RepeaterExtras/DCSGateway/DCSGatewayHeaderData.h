/*
 *   Copyright (C) 2010-2013 by Jonathan Naylor G4KLX
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

#ifndef	DCSGatewayHeaderData_H
#define	DCSGatewayHeaderData_H

#include <wx/wx.h>

class CDCSGatewayHeaderData {
public:
	CDCSGatewayHeaderData();
	CDCSGatewayHeaderData(const CDCSGatewayHeaderData& header);
	~CDCSGatewayHeaderData();

	bool setRepeaterData(const unsigned char* data, unsigned int length, bool check);
	bool setDCSData(const unsigned char* data, unsigned int length);

	unsigned int getRepeaterData(unsigned char* data, unsigned int length, bool check) const;
	unsigned int getDCSData(unsigned char* data, unsigned int length) const;

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

	void setMyCall1(const wxString& callsign);
	void setMyCall2(const wxString& callsign);
	void setYourCall(const wxString& callsign);
	void setRptCall1(const wxString& callsign);
	void setRptCall2(const wxString& callsign);

	void setReflector(const wxString& reflector);

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
};

#endif
