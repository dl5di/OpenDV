/*
 *   Copyright (C) 2010,2011 by Jonathan Naylor G4KLX
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

#ifndef	DExtraClientApp_H
#define	DExtraClientApp_H

#include "DExtraClientThread.h"
#include "DExtraClientFrame.h"
#include "DExtraClientDefs.h"
#include "MessageData.h"
#include "DVDongleSet.h"
#include "HeaderData.h"

#include <wx/wx.h>

class CDExtraClientApp : public wxApp {

public:
	CDExtraClientApp();
	virtual ~CDExtraClientApp();

	virtual bool OnInit();
	virtual int  OnExit();
	
	// This is overridden because dialog boxes from threads are bad news
#if defined(__WXDEBUG__)
	virtual void OnAssertFailure(const wxChar* file, int line, const wxChar* func, const wxChar* cond, const wxChar* msg);
#endif
	virtual void showHeader(CHeaderData* header) const;
	virtual void showMessage(CMessageData* message) const;

	virtual void error(const wxString& text) const;

	virtual void setRecording(bool recording);
	virtual bool connect(bool connect, const wxString& reflector = wxEmptyString, const wxString& address = wxEmptyString);
	virtual bool transmit(bool transmit);
	virtual void setModule(wxChar module);
	virtual bool isConnected();

	virtual void getCallsign(wxString& callsign) const;
	virtual void setCallsign(const wxString& callsign) const;

	virtual void getSoundcard(wxString& readDevice, wxString& writeDevice) const;
	virtual void setSoundcard(const wxString& readDevice, const wxString& writeDevice) const;

	virtual void getMessage(wxString& message) const;
	virtual void setMessage(const wxString& message) const;

	virtual void getDVDongle(wxString& device) const;
	virtual void setDVDongle(const wxString& device) const;

	virtual void getLastReflector(wxString& reflector) const;
	virtual void setLastReflector(const wxString& reflector) const;

	virtual void getLastModule(unsigned int& module) const;
	virtual void setLastModule(unsigned int module) const;

	virtual void getBleep(bool& on) const;
	virtual void setBleep(bool on) const;

	virtual void getController(wxString& type, unsigned int& audioDelay, bool& squelchInvert, bool& pttInvert) const;
	virtual void setController(const wxString& type, unsigned int audioDelay, bool squelchInvert, bool pttInvert) const;

	virtual void getStartup(wxString& reflector, unsigned int& module) const;
	virtual void setStartup(const wxString& reflector, unsigned int module) const;

private:
	CDExtraClientFrame*  m_frame;
	CDExtraClientThread* m_thread;

	void createThread();
};

DECLARE_APP(CDExtraClientApp)

#endif
