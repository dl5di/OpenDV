/*
 *	Copyright (C) 2013 by Jonathan Naylor, G4KLX
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

#ifndef	APRSRX_H
#define	APRSRX_H

#include <wx/wx.h>

class CAPRSRX {
public:
	CAPRSRX();
	~CAPRSRX();

	void process(const wxFloat32* audio, unsigned int length);

private:
	wxFloat32*     m_corrMarkI;
	wxFloat32*     m_corrMarkQ;
	wxFloat32*     m_corrSpaceI;
	wxFloat32*     m_corrSpaceQ;
	unsigned int   m_dcd;
	unsigned int   m_phase;
	unsigned int   m_last;
	unsigned int   m_subsamp;

	unsigned char* m_buf;
	unsigned char* m_ptr;
	bool           m_state;
	unsigned int   m_bitStream;
	unsigned int   m_bitBuf;

	bool           m_dump;
	wxString       m_dumper;

	void rxbit(bool bit);
	void decodeMicE(const unsigned char* packet, unsigned int length);
};

#endif
