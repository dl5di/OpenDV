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

#ifndef	DCSGatewayAMBEData_H
#define	DCSGatewayAMBEData_H

#include <wx/wx.h>

class CDCSGatewayAMBEData {
public:
	CDCSGatewayAMBEData();
	CDCSGatewayAMBEData(const CDCSGatewayAMBEData& data);
	~CDCSGatewayAMBEData();

	bool setRepeaterData(const unsigned char* data, unsigned int length);
	bool setDCSData(const unsigned char* data, unsigned int length);
	void setData(const unsigned char* data, unsigned int length);

	unsigned int getRepeaterData(unsigned char* data, unsigned int length) const;
	unsigned int getDCSData(unsigned char* data, unsigned int length) const;
	unsigned int getData(unsigned char* data, unsigned int length) const;

	unsigned int getId() const;
	void setId(unsigned int id);

	unsigned int getSeq() const;
	void setSeq(unsigned int seqNo);

	unsigned int getRptSeq() const;
	void setRptSeq(unsigned int seqNo);

	bool isEnd() const;
	void setEnd(bool end);

	bool isSync() const;

	static void setText(const wxString& text);

	static void initialise();
	static void finalise();

private:
	static unsigned char* m_text;

	unsigned char  m_outSeq;
	unsigned int   m_rptSeq;
	unsigned int   m_id;
	unsigned char* m_data;
};

#endif
