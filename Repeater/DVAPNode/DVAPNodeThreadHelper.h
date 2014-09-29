/*
 *   Copyright (C) 2012 by Jonathan Naylor G4KLX
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

#ifndef	DVAPNodeThreadHelper_H
#define	DVAPNodeThreadHelper_H

#include "DVAPNodeStatusData.h"
#include "DVAPNodeThread.h"

#include <wx/wx.h>

class CDVAPNodeThreadHelper : public wxThread {

public:
	CDVAPNodeThreadHelper(IDVAPNodeThread* thread);
	virtual ~CDVAPNodeThreadHelper();

	virtual void start();

	virtual void* Entry();

	virtual void kill();

	virtual void setLogging(bool logging, const wxString& dir);

	virtual CDVAPNodeStatusData* getStatus();

private:
	IDVAPNodeThread* m_thread;
};

#endif
