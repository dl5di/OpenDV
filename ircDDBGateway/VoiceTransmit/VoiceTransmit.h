/*
 *   Copyright (C) 2014 by Jonathan Naylor G4KLX
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

#ifndef	VoiceTransmit_H
#define	VoiceTransmit_H

#include "UDPReaderWriter.h"
#include "VoiceStore.h"
#include "HeaderData.h"
#include "AMBEData.h"

class CVoiceTransmit {
public:
	CVoiceTransmit(const wxString& callsign, CVoiceStore* store);
	~CVoiceTransmit();

	bool run();

private:
	CUDPReaderWriter m_socket;
	wxString         m_callsign;
	CVoiceStore*     m_store;

	bool sendHeader(CHeaderData* header);
	bool sendData(CAMBEData* data);
};

#endif
