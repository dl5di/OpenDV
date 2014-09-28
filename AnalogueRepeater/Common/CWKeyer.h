/*
 *	Copyright (C) 2009,2013 by Jonathan Naylor, G4KLX
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

#ifndef	CWKeyer_H
#define	CWKeyer_H

#include "FixedAudioSource.h"

#include <wx/wx.h>

class CCWKeyer : public IFixedAudioSource {
public:
	CCWKeyer(const wxString& text, unsigned int speed, unsigned int freq, unsigned int sampleRate);
	virtual ~CCWKeyer();

	virtual unsigned int getAudio(wxFloat32* audio, unsigned int length, wxFloat32 amplitude);

	virtual bool isEmpty() const;

	virtual void reset();

private:
	wxFloat32*   m_data;
	unsigned int m_length;
	unsigned int m_current;

	wxString charToCW(wxChar c) const;
};

#endif
