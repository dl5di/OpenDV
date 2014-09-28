/*
 *	Audio FSK modem for AX25 (1200 Baud, 1200/2200Hz).
 * 
 *	Copyright (C) Sivan Toledo, 2012
 *	Copyright (C) Jonathan Naylor, 2013
 * 
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef AX25TX_H
#define	AX25TX_H

#include <wx/wx.h>

class CAX25TX
{
public:
	CAX25TX();
	~CAX25TX();

	unsigned int getAudio(wxFloat32* audio, unsigned char* packet, unsigned int length);

private:
	wxFloat32    m_symbolPhase;
	wxFloat32	 m_ddsPhase;
	bool         m_lastSymbol;
	unsigned int m_stuffCount;

	unsigned int generateSymbolSamples(bool symbol, wxFloat32* audio, unsigned int position);
	unsigned int byteToSymbols(unsigned char bits, bool stuff, wxFloat32* audio);
};

#endif
