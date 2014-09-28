/*
 *   Copyright (C) 2009,2010,2014 by Jonathan Naylor G4KLX
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

#ifndef	DVToolWriterApp_H
#define	DVToolWriterApp_H

#include "DVToolWriterThread.h"
#include "DVToolWriterFrame.h"
#include "HeaderData.h"
#include "FileWriter.h"
#include "DongleSet.h"

#include <wx/wx.h>

class CDVToolWriterApp : public wxApp {

public:
	CDVToolWriterApp();
	virtual ~CDVToolWriterApp();

	virtual bool OnInit();
	virtual int  OnExit();
	
	// This is overridden because dialog boxes from threads are bad news
#if defined(__WXDEBUG__)
	virtual void OnAssertFailure(const wxChar* file, int line, const wxChar* func, const wxChar* cond, const wxChar* msg);
#endif
	virtual void error(const wxString& text) const;

	virtual void showFinished() const;

	virtual void setCallsign(const wxString& call1, const wxString& call2);
	virtual void getCallsign(wxString& call1, wxString& call2) const;

	virtual void setSoundcard(const wxString& device);
	virtual void getSoundcard(wxString& device) const;

	virtual void setDongle(DONGLE_TYPE type, const wxString& device, const wxString& address, unsigned int port);
	virtual void getDongle(DONGLE_TYPE& type, wxString& device, wxString& address, unsigned int& port) const;

	virtual bool processFile(CWAVFileReader* reader, const wxString& your, const wxString& rpt1, const wxString& rpt2, const wxString& text, IFileWriter* writer);

	virtual bool processAudio(const wxString& your, const wxString& rpt1, const wxString& rpt2, const wxString& text, IFileWriter* writer);
	virtual void processAudio();

private:
	CDVToolWriterFrame*  m_frame;
	CDVToolWriterThread* m_thread;
	wxString             m_callsign1;
	wxString             m_callsign2;

	void createThread();
};

DECLARE_APP(CDVToolWriterApp)

#endif
