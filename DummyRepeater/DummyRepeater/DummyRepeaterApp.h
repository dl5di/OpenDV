/*
 *   Copyright (C) 2010,2011,2012,2014 by Jonathan Naylor G4KLX
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

#ifndef	DummyRepeaterApp_H
#define	DummyRepeaterApp_H

#include "DummyRepeaterDongleSet.h"
#include "DummyRepeaterThread.h"
#include "DummyRepeaterFrame.h"
#include "DummyRepeaterDefs.h"
#include "MessageData.h"
#include "HeaderData.h"

#include <wx/wx.h>

class CDummyRepeaterApp : public wxApp {

public:
	CDummyRepeaterApp();
	virtual ~CDummyRepeaterApp();

	virtual bool OnInit();
	virtual int  OnExit();

	virtual void OnInitCmdLine(wxCmdLineParser& parser);
	virtual bool OnCmdLineParsed(wxCmdLineParser& parser);

	// This is overridden because dialog boxes from threads are bad news
#if defined(__WXDEBUG__)
	virtual void OnAssertFailure(const wxChar* file, int line, const wxChar* func, const wxChar* cond, const wxChar* msg);
#endif
	virtual void showHeader(CHeaderData* header) const;
	virtual void showMessage(CMessageData* message) const;
	virtual void showSlowData(const wxString& text) const;
	virtual void showStatus1(const wxString& text) const;
	virtual void showStatus2(const wxString& text) const;
	virtual void showStatus3(const wxString& text) const;
	virtual void showStatus4(const wxString& text) const;
	virtual void showStatus5(const wxString& text) const;

	virtual void error(const wxString& text) const;

	virtual void setYour(const wxString& your);
	virtual void setRpt1(const wxString& rpt1);
	virtual void setRpt2(const wxString& rpt2);

	virtual bool setTransmit(bool on);

	virtual void getCallsign(wxString& callsign1, wxString& callsign2) const;
	virtual void setCallsign(const wxString& callsign1, const wxString& callsign2) const;

	virtual void getSoundcard(wxString& readDevice, wxString& writeDevice) const;
	virtual void setSoundcard(const wxString& readDevice, const wxString& writeDevice) const;

	virtual void getMessage(wxString& message) const;
	virtual void setMessage(const wxString& message) const;

	virtual void getDongle(DONGLE_TYPE& type, wxString& device, wxString& address, unsigned int& port) const;
	virtual void setDongle(DONGLE_TYPE type, const wxString& device, const wxString& address, unsigned int port) const;

	virtual void getNetwork(wxString& gwyAddress, unsigned int& gwyPort, wxString& localAddress, unsigned int& localPort) const;
	virtual void setNetwork(const wxString& gwyAddress, unsigned int gwyPort, const wxString& localAddress, unsigned int localPort) const;

	virtual void getController(wxString& type, unsigned int& config, bool& pttInvert, bool& squelchInvert) const;
	virtual void setController(const wxString& type, unsigned int config, bool pttInvert, bool squelchInvert) const;

	virtual void getYourCalls(wxString& your, wxSortedArrayString& list) const;
	virtual void setYourCalls(const wxString& your, const wxSortedArrayString& list) const;

	virtual void getRpt1Calls(wxString& your, wxSortedArrayString& list) const;
	virtual void setRpt1Calls(const wxString& your, const wxSortedArrayString& list) const;

	virtual void getRpt2Calls(wxString& your, wxSortedArrayString& list) const;
	virtual void setRpt2Calls(const wxString& your, const wxSortedArrayString& list) const;

	virtual void getTimeout(unsigned int& timeout) const;
	virtual void setTimeout(unsigned int timeout) const;

	virtual void getBleep(bool& on) const;
	virtual void setBleep(bool on) const;

	virtual void getPosition(int& x, int& y) const;
	virtual void setPosition(int x, int y) const;

private:
	bool                  m_noLog;
	CDummyRepeaterFrame*  m_frame;
	CDummyRepeaterThread* m_thread;

	void createThread();
};

DECLARE_APP(CDummyRepeaterApp)

#endif
