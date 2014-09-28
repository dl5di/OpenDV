/*
 *   Copyright (C) 2009,2010,2012 by Jonathan Naylor G4KLX
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

#include "SerialDataController.h"
#include "DStarDefines.h"
#include "Version.h"
#include "Logger.h"

#include <wx/config.h>
#include <wx/filename.h>

IMPLEMENT_APP(CDVToolReaderApp)

static const wxString KEY_SOUND_DEVICE = wxT("/sndDevice");
static const wxString KEY_DVD_DEVICE   = wxT("/dvdDevice");
static const wxString KEY_BLEEP        = wxT("/bleep");

static const wxString DEFAULT_SOUND_DEVICE = wxEmptyString;
static const wxString DEFAULT_DVD_DEVICE   = wxEmptyString;
static const bool     DEFAULT_BLEEP        = true;


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

void CDVToolReaderApp::getDVDongle(wxString& device) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Read(KEY_DVD_DEVICE, &device, DEFAULT_DVD_DEVICE);

	wxLogInfo(wxT("DV Dongle set to: %s"), device.c_str());

	delete profile;
}

void CDVToolReaderApp::setDVDongle(const wxString& device)
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Write(KEY_DVD_DEVICE, device);
	profile->Flush();

	wxLogInfo(wxT("DV Dongle set to: %s"), device.c_str());

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

bool CDVToolReaderApp::setReader(CDVTOOLFileReader* reader)
{
	wxASSERT(reader != NULL);

	return m_thread->setReader(reader);
}

void CDVToolReaderApp::createThread()
{
	m_thread = new CDVToolReaderThread;
	m_thread->Create();
	m_thread->Run();

	wxString device;
	getDVDongle( device);

	if (!device.IsEmpty()) {
		CDVDongleController* dongle = new CDVDongleController(new CSerialDataController(device, SERIAL_230400));

		bool res = dongle->open();
		if (!res)
			error(_("Can't find the DV-Dongle on the port specified"));
		else
			m_thread->setDVDongle(dongle);
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
