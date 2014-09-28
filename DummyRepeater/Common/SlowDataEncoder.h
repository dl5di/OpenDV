/*
 *	Copyright (C) 2009,2010,2011,2014 by Jonathan Naylor, G4KLX
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

#ifndef	SlowDataEncoder_H
#define	SlowDataEncoder_H

#include "MessageData.h"
#include "HeaderData.h"

#include <vector>
using namespace std;

#include <wx/wx.h>

class CSlowDataEncoder {
public:
	CSlowDataEncoder();
	~CSlowDataEncoder();

	void setMessageData(const wxString& message);
	void setHeaderData(const CHeaderData& header);
	void setCodeSquelch(unsigned char value);

	void getData(unsigned char* data);

	void reset();

private:
	vector<unsigned char>                 m_initial;
	vector<unsigned char>                 m_body;
	vector<unsigned char>::const_iterator m_it;
	vector<unsigned char>::const_iterator m_end;
	vector<unsigned char>                 m_header;
	vector<unsigned char>                 m_message;
	vector<unsigned char>                 m_codeSquelch;

	void buildData();
};

#endif
