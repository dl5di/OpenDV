/*
 *   Copyright (C) 2011,2012,2013 by Jonathan Naylor G4KLX
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

#ifndef	DVAPNodeApp_H
#define	DVAPNodeApp_H

#include "DVAPNodeThreadHelper.h"
#include "DVAPNodeStatusData.h"
#include "DVAPNodeConfig.h"
#include "DVAPNodeFrame.h"
#include "DVAPNodeDefs.h"

#include <wx/wx.h>

class CDVAPNodeApp : public wxApp {

public:
	CDVAPNodeApp();
	virtual ~CDVAPNodeApp();

	virtual bool OnInit();
	virtual int  OnExit();
	
	virtual void OnInitCmdLine(wxCmdLineParser& parser);
	virtual bool OnCmdLineParsed(wxCmdLineParser& parser);

	// This is overridden because dialog boxes from threads are bad news
#if defined(__WXDEBUG__)
	virtual void OnAssertFailure(const wxChar* file, int line, const wxChar* func, const wxChar* cond, const wxChar* msg);
#endif

	virtual CDVAPNodeStatusData* getStatus() const;

	virtual void showLog(const wxString& text);

	virtual void getCallsign(wxString& callsign, wxString& gateway, DSTAR_MODE& mode, ACK_TYPE& ack, bool& restriction, bool& rpt1Validation) const;
	virtual void setCallsign(const wxString& callsign, const wxString& gateway, DSTAR_MODE mode, ACK_TYPE ack, bool restriction, bool rpt1Validation);

	virtual void getNetwork(wxString& gatewayAddress, unsigned int& gatewayPort, wxString& localAddress, unsigned int& localPort) const;
	virtual void setNetwork(const wxString& gatewayAddress, unsigned int gatewayPort, const wxString& localAddress, unsigned int localPort);

	virtual void getDVAP(wxString& port, unsigned int& frequency, int& power, int& squelch) const;
	virtual void setDVAP(const wxString& port, unsigned int frequency, int power, int squelch);

	virtual void getTimes(unsigned int& timeout, unsigned int& ackTime) const;
	virtual void setTimes(unsigned int timeout, unsigned int ackTime);

	virtual void getBeacon(unsigned int& time, wxString& text, bool& voice, TEXT_LANG& language) const;
	virtual void setBeacon(unsigned int time, const wxString& text, bool voice, TEXT_LANG language);

	virtual void getAnnouncement(bool& enabled, unsigned int& time, wxString& recordRPT1, wxString& recordRPT2, wxString& deleteRPT1, wxString& deleteRPT2) const;
	virtual void setAnnouncement(bool enabled, unsigned int time, const wxString& recordRPT1, const wxString& recordRPT2, const wxString& deleteRPT1, const wxString& deleteRPT2);

	virtual void getLogging(bool& logging) const;
	virtual void setLogging(bool logging);

	virtual void getPosition(int& x, int& y) const;
	virtual void setPosition(int x, int y);

	virtual bool writeConfig();

private:
	wxString               m_name;
	bool                   m_nolog;
	bool                   m_gui;
	wxString               m_logDir;
	wxString               m_confDir;
	wxString               m_audioDir;
	CDVAPNodeFrame*        m_frame;
	CDVAPNodeThreadHelper* m_thread;
	CDVAPNodeConfig*       m_config;
	wxLogChain*            m_logChain;

	void createThread();
};

DECLARE_APP(CDVAPNodeApp)

#endif
