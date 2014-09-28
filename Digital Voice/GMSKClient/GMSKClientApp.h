/*
 *   Copyright (C) 2010,2012 by Jonathan Naylor G4KLX
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

#ifndef	GMSKClientApp_H
#define	GMSKClientApp_H

#include "GMSKClientThread.h"
#include "GMSKClientFrame.h"
#include "GMSKClientDefs.h"
#include "MessageData.h"
#include "DVDongleSet.h"
#include "HeaderData.h"
#include "BusyData.h"

#include <wx/wx.h>

class CGMSKClientApp : public wxApp {

public:
	CGMSKClientApp();
	virtual ~CGMSKClientApp();

	virtual bool OnInit();
	virtual int  OnExit();
	
	// This is overridden because dialog boxes from threads are bad news
#if defined(__WXDEBUG__)
	virtual void OnAssertFailure(const wxChar* file, int line, const wxChar* func, const wxChar* cond, const wxChar* msg);
#endif
	virtual void showHeader(CHeaderData* header) const;
	virtual void showMessage(CMessageData* message) const;
	virtual void showBusy(CBusyData* busy) const;

	virtual void error(const wxString& text) const;

	virtual void setYour(const wxString& your);
	virtual void setRpt1(const wxString& rpt1);
	virtual void setRpt2(const wxString& rpt2);

	virtual bool setTransmit(bool on);

	virtual void setRecording(bool recording);

	virtual void getCallsign(wxString& callsign1, wxString& callsign2) const;
	virtual void setCallsign(const wxString& callsign1, const wxString& callsign2) const;

	virtual void getModem(GMSK_MODEM_TYPE& type, unsigned int& address) const;
	virtual void setModem(GMSK_MODEM_TYPE type, unsigned int address) const;

	virtual void getSoundcard(wxString& readDevice, wxString& writeDevice) const;
	virtual void setSoundcard(const wxString& readDevice, const wxString& writeDevice) const;

	virtual void getMessage(wxString& message) const;
	virtual void setMessage(const wxString& message) const;

	virtual void getDVDongle(wxString& device) const;
	virtual void setDVDongle(const wxString& device) const;

	virtual void getYourCalls(wxString& your, wxSortedArrayString& list) const;
	virtual void setYourCalls(const wxString& your, const wxSortedArrayString& list) const;

	virtual void getRpt1Calls(wxString& your, wxSortedArrayString& list) const;
	virtual void setRpt1Calls(const wxString& your, const wxSortedArrayString& list) const;

	virtual void getRpt2Calls(wxString& your, wxSortedArrayString& list) const;
	virtual void setRpt2Calls(const wxString& your, const wxSortedArrayString& list) const;

	virtual void getBleep(bool& on) const;
	virtual void setBleep(bool on) const;

private:
	CGMSKClientFrame*  m_frame;
	CGMSKClientThread* m_thread;

	void createThread();
};

DECLARE_APP(CGMSKClientApp)

#endif
