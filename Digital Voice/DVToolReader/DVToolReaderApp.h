/*
 *   Copyright (C) 2009,2010 by Jonathan Naylor G4KLX
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

#ifndef	DVToolReaderApp_H
#define	DVToolReaderApp_H

#include "DVToolReaderFrame.h"
#include "DVToolReaderThread.h"
#include "DVTOOLFileReader.h"
#include "MessageData.h"
#include "DVDongleSet.h"
#include "HeaderData.h"

#include <wx/wx.h>

class CDVToolReaderApp : public wxApp {

public:
	CDVToolReaderApp();
	virtual ~CDVToolReaderApp();

	virtual bool OnInit();
	virtual int  OnExit();
	
	// This is overridden because dialog boxes from threads are bad news
#if defined(__WXDEBUG__)
	virtual void OnAssertFailure(const wxChar* file, int line, const wxChar* func, const wxChar* cond, const wxChar* msg);
#endif
	virtual void showMessage(CMessageData* message) const;
	virtual void showHeader(CHeaderData* header) const;
	virtual void error(const wxString& text) const;

	virtual void setSoundcard(const wxString& device);
	virtual void getSoundcard(wxString& device) const;

	virtual void setDVDongle(const wxString& device);
	virtual void getDVDongle(wxString& device) const;

	virtual bool setReader(CDVTOOLFileReader* reader);

	virtual void getBleep(bool& on) const;
	virtual void setBleep(bool on) const;

private:
	CDVToolReaderFrame*  m_frame;
	CDVToolReaderThread* m_thread;

	void createThread();
};

DECLARE_APP(CDVToolReaderApp)

#endif
