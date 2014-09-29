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

#ifndef	DExtraGatewayAMBEData_H
#define	DExtraGatewayAMBEData_H

#include <wx/wx.h>

class CDExtraGatewayAMBEData {
public:
	CDExtraGatewayAMBEData();
	CDExtraGatewayAMBEData(const CDExtraGatewayAMBEData& data);
	~CDExtraGatewayAMBEData();

	bool setRepeaterData(const unsigned char* data, unsigned int length);
	bool setDExtraData(const unsigned char* data, unsigned int length);
	void setData(const unsigned char* data, unsigned int length);

	unsigned int getRepeaterData(unsigned char* data, unsigned int length) const;
	unsigned int getDExtraData(unsigned char* data, unsigned int length) const;

	unsigned int getId() const;
	void setId(unsigned int id);

	unsigned int getSeq() const;
	void setSeq(unsigned int seqNo);

	bool isEnd() const;
	void setEnd(bool end);

private:
	unsigned char  m_outSeq;
	unsigned int   m_id;
	unsigned char* m_data;
};

#endif
