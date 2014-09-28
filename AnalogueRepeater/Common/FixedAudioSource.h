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

#ifndef	FixedAudioSource_H
#define	FixedAudioSource_H

#include <wx/wx.h>

class IFixedAudioSource {
public:
	virtual ~IFixedAudioSource() = 0;

	virtual unsigned int getAudio(wxFloat32* audio, unsigned int length, wxFloat32 amplitude) = 0;

	virtual bool isEmpty() const = 0;

	virtual void reset() = 0;

private:
};

#endif
