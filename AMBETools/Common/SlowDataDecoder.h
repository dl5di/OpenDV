/*
 *	Copyright (C) 2009,2010 by Jonathan Naylor, G4KLX
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; version 2 of the License.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 */

#ifndef	SlowDataDecoder_H
#define	SlowDataDecoder_H

#include "MessageData.h"
#include "HeaderData.h"

#include <vector>
using namespace std;

#include <wx/wx.h>

enum SDD_STATE {
	SDD_FIRST,
	SDD_SECOND
};

class CSlowDataDecoder {
public:
	CSlowDataDecoder();
	~CSlowDataDecoder();

	void addData(const unsigned char* data);

//	CGPSMessageData* getGPSMessageData();

	CMessageData* getMessageData();

	unsigned int getHeader(unsigned char* data, unsigned int length);
	CHeaderData* getHeaderData();

	bool getCodeSquelchData(unsigned int& value);

	void sync();
	void reset();

private:
	unsigned char*        m_buffer;
	SDD_STATE             m_state;
	vector<unsigned char> m_header;
	vector<unsigned char> m_message;
	unsigned int          m_messageExp;
//	vector<unsigned char> m_gpsMessage;
	CHeaderData*          m_headerData;
	CMessageData*         m_messageData;
//	CGPSMessageData*      m_gpsMessageData;
	unsigned char         m_codeSquelchData;
	bool                  m_hasCodeSquelch;

	void processHeader();
	void processMessage();
//	void processGPSMessage();
	void processCodeSquelch();
};

#endif
