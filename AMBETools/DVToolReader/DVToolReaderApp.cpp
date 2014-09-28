/*
 *   Copyright (C) 2009,2010,2012,2014 by Jonathan Naylor G4KLX
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

#include "DVToolReaderApp.h"

#include "DVDongleController.h"
#include "DV3000Controller.h"
#include "AMBE3000Thread.h"
#include "DVDongleThread.h"
#include "DStarDefines.h"
#include "Version.h"
#include "Logger.h"

#include <wx/config.h>
#include <wx/filename.h>

IMPLEMENT_APP(CDVToolReaderApp)

const wxString KEY_SOUND_DEVICE   = wxT("/soundDevice");
const wxString KEY_DONGLE_TYPE    = wxT("/dongleType");
const wxString KEY_DONGLE_DEVICE  = wxT("/dongleDevice");
const wxString KEY_DONGLE_ADDRESS = wxT("/dongleAddress");
const wxString KEY_DONGLE_PORT    = wxT("/donglePort");
const wxString KEY_BLEEP          = wxT("/bleep");

const wxString     DEFAULT_SOUND_DEVICE   = wxEmptyString;
const DONGLE_TYPE  DEFAULT_DONGLE_TYPE    = DT_DVDONGLE;
const wxString     DEFAULT_DONGLE_DEVICE  = wxEmptyString;
const wxString     DEFAULT_DONGLE_ADDRESS = wxEmptyString;
const unsigned int DEFAULT_DONGLE_PORT    = 2460L;
const bool         DEFAULT_BLEEP          = true;


CDVToolReaderApp::CDVToolReaderApp() :
wxApp(),
m_frame(NULL),
m_thread(NULL)
{
}

CDVToolReaderApp::~CDVToolReaderApp()
{
}

bool CDVToolReaderApp::OnInit()
{
	SetVendorName(VENDOR_NAME);

	wxFileName loggerName(wxFileName::GetHomeDir(), LOG_FILE_NAME, wxT("log"));
	wxLog* log = new CLogger(loggerName.GetFullPath());
	wxLog::SetActiveTarget(log);

	m_frame = new CDVToolReaderFrame(APPLICATION_NAME + wxT(" - ") + VERSION);
	m_frame->Show();

	SetTopWindow(m_frame);

	wxLogInfo(wxT("Starting ") + APPLICATION_NAME + wxT(" - ") + VERSION);

	// Log the SVN revsion and the version of wxWidgets and the Operating System
	wxLogInfo(SVNREV);
	wxLogInfo(wxT("Using wxWidgets %d.%d.%d on %s"), wxMAJOR_VERSION, wxMINOR_VERSION, wxRELEASE_NUMBER, ::wxGetOsDescription().c_str());

	createThread();

	return wxApp::OnInit();
}

int CDVToolReaderApp::OnExit()
{
	wxLogInfo(APPLICATION_NAME + wxT(" is exiting"));

	m_thread->kill();
	m_thread->Wait();
	delete m_thread;

	return 0;
}

#if defined(__WXDEBUG__)
void CDVToolReaderApp::OnAssertFailure(const wxChar* file, int line, const wxChar* func, const wxChar* cond, const wxChar* msg)
{
	wxLogFatalError(wxT("Assertion failed on line %d in file %s and function %s: %s %s"), line, file, func, cond, msg);
}
#endif

void CDVToolReaderApp::showMessage(CMessageData* message) const
{
	wxASSERT(message != NULL);

	m_frame->showMessage(message);
}

void CDVToolReaderApp::showHeader(CHeaderData* header) const
{
	m_frame->showHeader(header);
}

void CDVToolReaderApp::error(const wxString& text) const
{
	m_frame->error(text);
}

void CDVToolReaderApp::getSoundcard(wxString& device) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Read(KEY_SOUND_DEVICE, &device, DEFAULT_SOUND_DEVICE);

	wxLogInfo(wxT("Soundcard set to %s"), device.c_str());

	delete profile;
}

void CDVToolReaderApp::setSoundcard(const wxString& device)
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Write(KEY_SOUND_DEVICE, device);
	profile->Flush();

	wxLogInfo(wxT("Soundcard set to %s"), device.c_str());

	delete profile;
}

void CDVToolReaderApp::getDongle(DONGLE_TYPE& type, wxString& device, wxString& address, unsigned int& port) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	long temp;
	profile->Read(KEY_DONGLE_TYPE, &temp, long(DEFAULT_DONGLE_TYPE));
	type = DONGLE_TYPE(temp);

	profile->Read(KEY_DONGLE_DEVICE, &device, DEFAULT_DONGLE_DEVICE);

	profile->Read(KEY_DONGLE_ADDRESS, &address, DEFAULT_DONGLE_ADDRESS);

	profile->Read(KEY_DONGLE_PORT, &temp, long(DEFAULT_DONGLE_PORT));
	port = (unsigned int)temp;

	wxLogInfo(wxT("Dongle type: %d, device: %s, address: %s:%u"), int(type), device.c_str(), address.c_str(), port);

	delete profile;
}

void CDVToolReaderApp::setDongle(DONGLE_TYPE type, const wxString& device, const wxString& address, unsigned int port)
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Write(KEY_DONGLE_TYPE, long(type));
	profile->Write(KEY_DONGLE_DEVICE, device);
	profile->Write(KEY_DONGLE_ADDRESS, address);
	profile->Write(KEY_DONGLE_PORT, long(port));
	profile->Flush();

	wxLogInfo(wxT("Dongle type: %d, device: %s, address: %s:%u"), int(type), device.c_str(), address.c_str(), port);

	delete profile;
}

void CDVToolReaderApp::getBleep(bool& on) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Read(KEY_BLEEP, &on, DEFAULT_BLEEP);

	wxLogInfo(wxT("End bleep set to %d"), int(on));

	delete profile;
}

void CDVToolReaderApp::setBleep(bool on) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Write(KEY_BLEEP, on);
	profile->Flush();

	wxLogInfo(wxT("End bleep set to %d"), int(on));

	m_thread->setBleep(on);

	delete profile;
}

bool CDVToolReaderApp::setReader(IFileReader* reader)
{
	wxASSERT(reader != NULL);

	return m_thread->setReader(reader);
}

void CDVToolReaderApp::createThread()
{
	m_thread = new CDVToolReaderThread;
	m_thread->Create();
	m_thread->Run();

	DONGLE_TYPE type;
	wxString device, address;
	unsigned int port;
	getDongle(type, device, address, port);

	CDongleThread* dongle = NULL;

	switch (type) {
		case DT_DVDONGLE:
			if (!device.IsEmpty())
				dongle = new CDVDongleThread(new CDVDongleController(device));
			break;
		case DT_DV3000:
			if (!address.IsEmpty() && port > 0U)
				dongle = new CAMBE3000Thread(new CDV3000Controller(address, port));
			break;
		default:
			error(_("Invalid Dongle type specified"));
			break;
	}

	if (dongle != NULL) {
		dongle->setDecodeCallback(m_thread);

		bool res = dongle->open();
		if (!res)
			error(_("Can't find the AMBE Dongle on the port specified"));
		else
			m_thread->setDongle(dongle);
	}

	getSoundcard(device);

	if (!device.IsEmpty()) {
#if defined(__WINDOWS__)
		CSoundCardReaderWriter* soundcard = new CSoundCardReaderWriter(wxEmptyString, device, DSTAR_RADIO_SAMPLE_RATE, DSTAR_RADIO_BLOCK_SIZE);
#else
		CSoundCardReaderWriter* soundcard = new CSoundCardReaderWriter(wxEmptyString, device, DSTAR_RADIO_SAMPLE_RATE, 64U);
#endif
		soundcard->setCallback(m_thread, 0);

		bool res = soundcard->open();
		if (!res)
			error(_("Cannot open the sound card"));
		else
			m_thread->setSoundCard(soundcard);
	}

	bool bleep;
	getBleep(bleep);
	m_thread->setBleep(bleep);
}
