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

#include "DVToolWriterApp.h"

#include "DVDongleController.h"
#include "DV3000Controller.h"
#include "AMBE3000Thread.h"
#include "DVDongleThread.h"
#include "DStarDefines.h"
#include "HeaderData.h"
#include "Version.h"
#include "Logger.h"

#include <wx/config.h>
#include <wx/filename.h>

IMPLEMENT_APP(CDVToolWriterApp)

const wxString KEY_CALLSIGN1      = wxT("/callsign1");
const wxString KEY_CALLSIGN2      = wxT("/callsign2");
const wxString KEY_SOUND_DEVICE   = wxT("/soundDevice");
const wxString KEY_DONGLE_TYPE    = wxT("/dongleType");
const wxString KEY_DONGLE_DEVICE  = wxT("/dongleDevice");
const wxString KEY_DONGLE_ADDRESS = wxT("/dongleAddress");
const wxString KEY_DONGLE_PORT    = wxT("/donglePort");

const wxString     DEFAULT_CALLSIGN1      = wxEmptyString;
const wxString     DEFAULT_CALLSIGN2      = wxEmptyString;
const wxString     DEFAULT_SOUND_DEVICE   = wxEmptyString;
const DONGLE_TYPE  DEFAULT_DONGLE_TYPE    = DT_DVDONGLE;
const wxString     DEFAULT_DONGLE_DEVICE  = wxEmptyString;
const wxString     DEFAULT_DONGLE_ADDRESS = wxEmptyString;
const unsigned int DEFAULT_DONGLE_PORT    = 2460L;


CDVToolWriterApp::CDVToolWriterApp() :
wxApp(),
m_frame(NULL),
m_thread(NULL),
m_callsign1(),
m_callsign2()
{
}

CDVToolWriterApp::~CDVToolWriterApp()
{
}

bool CDVToolWriterApp::OnInit()
{
	SetVendorName(VENDOR_NAME);

	wxFileName loggerName(wxFileName::GetHomeDir(), LOG_FILE_NAME, wxT("log"));
	wxLog* log = new CLogger(loggerName.GetFullPath());
	wxLog::SetActiveTarget(log);

	m_frame = new CDVToolWriterFrame(APPLICATION_NAME + wxT(" - ") + VERSION);
	m_frame->Show();

	SetTopWindow(m_frame);

	wxLogInfo(wxT("Starting ") + APPLICATION_NAME + wxT(" - ") + VERSION);

	// Log the SVN revsion and the version of wxWidgets and the Operating System
	wxLogInfo(SVNREV);
	wxLogInfo(wxT("Using wxWidgets %d.%d.%d on %s"), wxMAJOR_VERSION, wxMINOR_VERSION, wxRELEASE_NUMBER, ::wxGetOsDescription().c_str());

	createThread();

	return wxApp::OnInit();
}

int CDVToolWriterApp::OnExit()
{
	wxLogInfo(APPLICATION_NAME + wxT(" is exiting"));

	m_thread->kill();
	m_thread->Wait();
	delete m_thread;

	return 0;
}

#if defined(__WXDEBUG__)
void CDVToolWriterApp::OnAssertFailure(const wxChar* file, int line, const wxChar* func, const wxChar* cond, const wxChar* msg)
{
	wxLogFatalError(wxT("Assertion failed on line %d in file %s and function %s: %s %s"), line, file, func, cond, msg);
}
#endif

void CDVToolWriterApp::error(const wxString& text) const
{
	m_frame->error(text);
}

void CDVToolWriterApp::showFinished() const
{
	m_frame->showFinished();
}

void CDVToolWriterApp::getCallsign(wxString& callsign1, wxString& callsign2) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Read(KEY_CALLSIGN1, &callsign1, DEFAULT_CALLSIGN1);
	profile->Read(KEY_CALLSIGN2, &callsign2, DEFAULT_CALLSIGN2);

	wxLogInfo(wxT("Callsign set to %s/%s"), callsign1.c_str(), callsign2.c_str());

	delete profile;
}

void CDVToolWriterApp::setCallsign(const wxString& callsign1, const wxString& callsign2)
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Write(KEY_CALLSIGN1, callsign1);
	profile->Write(KEY_CALLSIGN2, callsign2);
	profile->Flush();

	wxLogInfo(wxT("Callsign set to %s/%s"), callsign1.c_str(), callsign2.c_str());

	delete profile;
}

void CDVToolWriterApp::getSoundcard(wxString& device) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Read(KEY_SOUND_DEVICE, &device, DEFAULT_SOUND_DEVICE);

	wxLogInfo(wxT("Soundcard set to %s"), device.c_str());

	delete profile;
}

void CDVToolWriterApp::setSoundcard(const wxString& device)
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Write(KEY_SOUND_DEVICE, device);
	profile->Flush();

	wxLogInfo(wxT("Soundcard set to %s"), device.c_str());

	delete profile;
}

void CDVToolWriterApp::getDongle(DONGLE_TYPE& type, wxString& device, wxString& address, unsigned int& port) const
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

void CDVToolWriterApp::setDongle(DONGLE_TYPE type, const wxString& device, const wxString& address, unsigned int port)
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

bool CDVToolWriterApp::processFile(CWAVFileReader* reader, const wxString& your, const wxString& rpt1, const wxString& rpt2, const wxString& text, IFileWriter* writer)
{
	wxASSERT(writer != NULL);

	if (m_callsign1.IsEmpty() || m_callsign2.IsEmpty() || your.IsEmpty() || rpt1.IsEmpty() || rpt2.IsEmpty())
		return false;

	CHeaderData* header = new CHeaderData(m_callsign1, m_callsign2, your, rpt1, rpt2);

	return m_thread->processFile(reader, header, text, writer);
}

bool CDVToolWriterApp::processAudio(const wxString& your, const wxString& rpt1, const wxString& rpt2, const wxString& text, IFileWriter* writer)
{
	wxASSERT(writer != NULL);

	if (m_callsign1.IsEmpty() || m_callsign2.IsEmpty() || your.IsEmpty() || rpt1.IsEmpty() || rpt2.IsEmpty())
		return false;

	CHeaderData* header = new CHeaderData(m_callsign1, m_callsign2, your, rpt1, rpt2);

	return m_thread->processAudio(header, text, writer);
}

void CDVToolWriterApp::processAudio()
{
	m_thread->processAudio();
}

void CDVToolWriterApp::createThread()
{
	m_thread = new CDVToolWriterThread;
	m_thread->Create();
	m_thread->Run();

	getCallsign(m_callsign1, m_callsign2);

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
		dongle->setEncodeCallback(m_thread);
		dongle->setBleep(false);

		bool res = dongle->open();
		if (!res)
			error(_("Can't find the AMBE Dongle on the port specified"));
		else
			m_thread->setDongle(dongle);
	}

	getSoundcard(device);

	if (!device.IsEmpty()) {
#if defined(__WINDOWS__)
		CSoundCardReaderWriter* soundcard = new CSoundCardReaderWriter(device, wxEmptyString, DSTAR_RADIO_SAMPLE_RATE, DSTAR_RADIO_BLOCK_SIZE);
#else
		CSoundCardReaderWriter* soundcard = new CSoundCardReaderWriter(device, wxEmptyString, DSTAR_RADIO_SAMPLE_RATE, 64U);
#endif
		soundcard->setCallback(m_thread, 0);

		bool res = soundcard->open();
		if (!res)
			error(_("Cannot open the sound card"));
		else
			m_thread->setSoundCard(soundcard);
	}
}
