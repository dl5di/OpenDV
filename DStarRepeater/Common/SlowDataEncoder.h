/*
 *	Copyright (C) 2010 by Jonathan Naylor, G4KLX
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

#include "HeaderData.h"

#include <wx/wx.h>

class CSlowDataEncoder {
public:
	CSlowDataEncoder();
	~CSlowDataEncoder();

	void setHeaderData(const CHeaderData& header);
	void setTextData(const wxString& text);

	void getHeaderData(unsigned char* data);
	void getTextData(unsigned char* data);

	void reset();
	void sync();

private:
	unsigned char* m_headerData;
	unsigned char* m_textData;
	unsigned int   m_headerPtr;
	unsigned int   m_textPtr;
};

#endif
