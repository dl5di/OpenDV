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

#ifndef	SplitRepeaterThreadHelper_H
#define	SplitRepeaterThreadHelper_H

#include "SplitRepeaterStatusData.h"
#include "SplitRepeaterThread.h"

#include <wx/wx.h>

class CSplitRepeaterThreadHelper : public wxThread {

public:
	CSplitRepeaterThreadHelper(CSplitRepeaterThread* thread);
	virtual ~CSplitRepeaterThreadHelper();

	virtual void start();

	virtual void* Entry();

	virtual void kill();

	virtual void shutdown();
	virtual void startup();

	virtual void command1();
	virtual void command2();
	virtual void command3();
	virtual void command4();

	virtual CSplitRepeaterStatusData* getStatus();

private:
	CSplitRepeaterThread* m_thread;
};

#endif
