/*
 *	Copyright (C) 2009,2015 by Jonathan Naylor, G4KLX
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

#ifndef	AudioCallback_H
#define	AudioCallback_H

class IAudioCallback {
public:
	virtual void readCallback(const float* input, unsigned int nSamples, int id) = 0;
	virtual void writeCallback(float* output, unsigned int& nSamples, int id) = 0;

private:
};

#endif
