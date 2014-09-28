/*
 *	Copyright (C) 2011,2013 by Jonathan Naylor, G4KLX
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

#ifndef	WAVFileStore_H
#define	WAVFileStore_H

#include "FixedAudioSource.h"

#include <wx/wx.h>

class CWAVFileStore : public IFixedAudioSource {
public:
	CWAVFileStore();
	virtual ~CWAVFileStore();

	virtual bool load(const wxString& fileName, unsigned int sampleRate);

	virtual unsigned int getAudio(wxFloat32* audio, unsigned int length, wxFloat32 amplitude);

	virtual bool isEmpty() const;

	virtual void reset();

private:
	wxFloat32*   m_data;
	unsigned int m_length;
	unsigned int m_current;
};

#endif
