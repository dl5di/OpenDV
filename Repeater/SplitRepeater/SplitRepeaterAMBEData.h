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

#ifndef	SplitRepeaterAMBEData_H
#define	SplitRepeaterAMBEData_H

#include <wx/wx.h>

#if defined(__WINDOWS__)
#include "Inaddr.h"
#else
#include <netinet/in.h>
#endif

class CSplitRepeaterAMBEData {
public:
	CSplitRepeaterAMBEData();
	CSplitRepeaterAMBEData(const CSplitRepeaterAMBEData& data);
	~CSplitRepeaterAMBEData();

	bool setRepeaterData(const unsigned char* data, unsigned int length, const in_addr& address, unsigned int port);

	unsigned int getRepeaterData(unsigned char* data, unsigned int length) const;

	unsigned long getTime() const;
	void setTime(unsigned long ms);

	unsigned int getId() const;
	void setId(unsigned int id);

	unsigned int getSeq() const;
	void setSeq(unsigned int seqNo);

	bool isEnd() const;
	void setEnd(bool end);

	bool isSync() const;

	void setData(const unsigned char* data, unsigned int length);
	unsigned int getData(unsigned char* data, unsigned int length) const;

	void setDestination(const in_addr& address, unsigned int port);

	in_addr      getAddress() const;
	unsigned int getPort() const;

	unsigned int getErrors() const;

private:
	unsigned char  m_outSeq;
	unsigned int   m_id;
	unsigned char* m_data;
	in_addr        m_address;
	unsigned int   m_port;
	unsigned int   m_errors;
	unsigned long  m_time;
};

#endif
