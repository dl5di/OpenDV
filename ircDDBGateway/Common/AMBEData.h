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

#ifndef	AMBEData_H
#define	AMBEData_H

#include "HeaderData.h"

#include <wx/wx.h>

#if defined(__WINDOWS__)
#include "Inaddr.h"
#else
#include <netinet/in.h>
#endif

class CAMBEData {
public:
	CAMBEData();
	CAMBEData(const CAMBEData& data);
	~CAMBEData();

	bool setIcomRepeaterData(const unsigned char* data, unsigned int length, const in_addr& yourAddress, unsigned int yourPort);
	bool setHBRepeaterData(const unsigned char* data, unsigned int length, const in_addr& yourAddress, unsigned int yourPort);
	bool setG2Data(const unsigned char* data, unsigned int length, const in_addr& yourAddress, unsigned int yourPort);
	bool setDExtraData(const unsigned char* data, unsigned int length, const in_addr& yourAddress, unsigned int yourPort, unsigned int myPort);
	bool setDPlusData(const unsigned char* data, unsigned int length, const in_addr& yourAddress, unsigned int yourPort, unsigned int myPort);
	bool setDCSData(const unsigned char* data, unsigned int length, const in_addr& yourAddress, unsigned int yourPort, unsigned int myPort);
	bool setCCSData(const unsigned char* data, unsigned int length, const in_addr& yourAddress, unsigned int yourPort, unsigned int myPort);

	unsigned int getIcomRepeaterData(unsigned char* data, unsigned int length) const;
	unsigned int getHBRepeaterData(unsigned char* data, unsigned int length) const;
	unsigned int getDExtraData(unsigned char* data, unsigned int length) const;
	unsigned int getDPlusData(unsigned char* data, unsigned int length) const;
	unsigned int getDCSData(unsigned char* data, unsigned int length) const;
	unsigned int getCCSData(unsigned char* data, unsigned int length) const;
	unsigned int getG2Data(unsigned char* data, unsigned int length) const;

	unsigned int getId() const;
	void setId(unsigned int id);

	unsigned char getBand1() const;
	unsigned char getBand2() const;
	unsigned char getBand3() const;
	void setBand1(unsigned char band);
	void setBand2(unsigned char band);
	void setBand3(unsigned char band);

	unsigned int getRptSeq() const;
	void setRptSeq(unsigned int seqNo);

	unsigned int getSeq() const;
	void setSeq(unsigned int seqNo);

	bool isEnd() const;
	void setEnd(bool end);

	bool isSync() const;

	void setData(const unsigned char* data, unsigned int length);
	unsigned int getData(unsigned char* data, unsigned int length) const;

	void setDestination(const in_addr& address, unsigned int port);

	void setText(const wxString& text);

	in_addr      getYourAddress() const;
	unsigned int getYourPort() const;
	unsigned int getMyPort() const;

	unsigned int getErrors() const;

	CHeaderData& getHeader();

	CAMBEData& operator=(const CAMBEData& data);

private:
	unsigned int   m_rptSeq;
	unsigned char  m_outSeq;
	unsigned int   m_id;
	unsigned char  m_band1;
	unsigned char  m_band2;
	unsigned char  m_band3;
	unsigned char* m_data;
	in_addr        m_yourAddress;
	unsigned int   m_yourPort;
	unsigned int   m_myPort;
	unsigned int   m_errors;
	wxString       m_text;
	CHeaderData    m_header;
};

#endif
