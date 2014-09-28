/*
 *   Copyright (C) 2010,2011,2012 by Jonathan Naylor G4KLX
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

#include "DExtraClientApp.h"

#include "SerialDataController.h"
#include "ExternalController.h"
#include "URIUSBController.h"
#include "DummyController.h"
#include "K8055Controller.h"
#include "SerialController.h"
#include "DStarDefines.h"
#include "Version.h"
#include "Logger.h"

#include <wx/tokenzr.h>
#include <wx/config.h>
#include <wx/filename.h>

IMPLEMENT_APP(CDExtraClientApp)

static const wxString KEY_CALLSIGN       = wxT("/callsign");
static const wxString KEY_MESSAGE_TYPE   = wxT("/msgType");
static const wxString KEY_MESSAGE_TEXT   = wxT("/msgText");
static const wxString KEY_READ_DEVICE    = wxT("/readDevice");
static const wxString KEY_WRITE_DEVICE   = wxT("/writeDevice");
static const wxString KEY_DVD_DEVICE     = wxT("/dvdDevice");
static const wxString KEY_LAST_REFLECTOR = wxT("/lastReflector");
static const wxString KEY_LAST_MODULE    = wxT("/lastModule");
static const wxString KEY_BLEEP          = wxT("/bleep");
static const wxString KEY_IFACE_TYPE     = wxT("/interfaceType");
static const wxString KEY_AUDIO_DELAY    = wxT("/audioDelay");
static const wxString KEY_SQUELCH_INVERT = wxT("/squelchInvert");
static const wxString KEY_PTT_INVERT     = wxT("/pttInvert");
static const wxString KEY_STARTUP_REFLECTOR = wxT("/startupReflector");
static const wxString KEY_STARTUP_MODULE    = wxT("/startupModule");

static const wxString DEFAULT_CALLSIGN       = wxEmptyString;
static const wxString DEFAULT_MESSAGE_TEXT   = wxEmptyString;
static const wxString DEFAULT_READ_DEVICE    = wxEmptyString;
static const wxString DEFAULT_WRITE_DEVICE   = wxEmptyString;
static const wxString DEFAULT_DVD_DEVICE     = wxEmptyString;
static const wxString DEFAULT_LAST_REFLECTOR = wxEmptyString;
static const long     DEFAULT_LAST_MODULE    = 0L;
static const bool     DEFAULT_BLEEP          = true;
static const wxString DEFAULT_IFACE_TYPE     = wxEmptyString;
static const long     DEFAULT_AUDIO_DELAY    = 0L;
static const bool     DEFAULT_SQUELCH_INVERT = false;
static const bool     DEFAULT_PTT_INVERT     = false;
static const wxString DEFAULT_STARTUP_REFLECTOR = wxEmptyString;
static const long     DEFAULT_STARTUP_MODULE    = 0L;

CDExtraClientApp::CDExtraClientApp() :
wxApp(),
m_frame(NULL),
m_thread(NULL)
{
}

CDExtraClientApp::~CDExtraClientApp()
{
}

bool CDExtraClientApp::OnInit()
{
	SetVendorName(VENDOR_NAME);

	wxFileName loggerName(wxFileName::GetHomeDir(), LOG_FILE_NAME, wxT("log"));
	wxLog* log = new CLogger(loggerName.GetFullPath());
	wxLog::SetActiveTarget(log);

	m_frame = new CDExtraClientFrame(APPLICATION_NAME + wxT(" - ") + VERSION);
	m_frame->Show();

	SetTopWindow(m_frame);

	wxLogInfo(wxT("Starting ") + APPLICATION_NAME + wxT(" - ") + VERSION);

	// Log the SVN revsion and the version of wxWidgets and the Operating System
	wxLogInfo(SVNREV);
	wxLogInfo(wxT("Using wxWidgets %d.%d.%d on %s"), wxMAJOR_VERSION, wxMINOR_VERSION, wxRELEASE_NUMBER, ::wxGetOsDescription().c_str());

	createThread();

	return wxApp::OnInit();
}

int CDExtraClientApp::OnExit()
{
	wxLogInfo(APPLICATION_NAME + wxT(" is exiting"));

	m_thread->kill();
	m_thread->Wait();
	delete m_thread;

	return 0;
}

#if defined(__WXDEBUG__)
void CDExtraClientApp::OnAssertFailure(const wxChar* file, int line, const wxChar* func, const wxChar* cond, const wxChar* msg)
{
	wxLogFatalError(wxT("Assertion failed on line %d in file %s and function %s: %s %s"), line, file, func, cond, msg);
}
#endif

void CDExtraClientApp::showMessage(CMessageData* message) const
{
	wxASSERT(message != NULL);

	m_frame->showMessage(message);
}

void CDExtraClientApp::showHeader(CHeaderData* header) const
{
	m_frame->showHeader(header);
}

void CDExtraClientApp::error(const wxString& text) const
{
	m_frame->error(text);
}

bool CDExtraClientApp::connect(bool connect, const wxString& reflector, const wxString& address)
{
	return m_thread->connect(connect, reflector, address);
}

bool CDExtraClientApp::transmit(bool transmit)
{
	return m_thread->transmit(transmit);
}

void CDExtraClientApp::setRecording(bool recording)
{
	m_thread->setRecording(recording);
}

void CDExtraClientApp::setModule(wxChar module)
{
	m_thread->setModule(module);
}

bool CDExtraClientApp::isConnected()
{
	return m_thread->isConnected();
}

void CDExtraClientApp::getCallsign(wxString& callsign) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Read(KEY_CALLSIGN, &callsign, DEFAULT_CALLSIGN);

	wxLogInfo(wxT("Callsign set to %s"), callsign.c_str());

	delete profile;
}

void CDExtraClientApp::setCallsign(const wxString& callsign) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Write(KEY_CALLSIGN, callsign);
	profile->Flush();

	wxLogInfo(wxT("Callsign set to %s"), callsign.c_str());

	m_thread->setCallsign(callsign);

	delete profile;
}

void CDExtraClientApp::getMessage(wxString& message) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Read(KEY_MESSAGE_TEXT, &message, DEFAULT_MESSAGE_TEXT);

	wxLogInfo(wxT("Message set to %s"), message.c_str());

	delete profile;
}

void CDExtraClientApp::setMessage(const wxString& message) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Write(KEY_MESSAGE_TEXT, message);
	profile->Flush();

	wxLogInfo(wxT("Message set to %s"), message.c_str());

	m_thread->setMessage(message);

	delete profile;
}

void CDExtraClientApp::getSoundcard(wxString& readDevice, wxString& writeDevice) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Read(KEY_READ_DEVICE,  &readDevice,  DEFAULT_READ_DEVICE);
	profile->Read(KEY_WRITE_DEVICE, &writeDevice, DEFAULT_WRITE_DEVICE);

	wxLogInfo(wxT("Soundcard set to %s:%s"), readDevice.c_str(), writeDevice.c_str());

	delete profile;
}

void CDExtraClientApp::setSoundcard(const wxString& readDevice, const wxString& writeDevice) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Write(KEY_READ_DEVICE,  readDevice);
	profile->Write(KEY_WRITE_DEVICE, writeDevice);
	profile->Flush();

	wxLogInfo(wxT("Soundcard set to %s:%s"), readDevice.c_str(), writeDevice.c_str());

	delete profile;
}

void CDExtraClientApp::getDVDongle(wxString& device) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Read(KEY_DVD_DEVICE, &device, DEFAULT_DVD_DEVICE);

	wxLogInfo(wxT("DV Dongle set to: %s"), device.c_str());

	delete profile;
}

void CDExtraClientApp::setDVDongle(const wxString& device) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Write(KEY_DVD_DEVICE, device);

	wxLogInfo(wxT("DV Dongle set to: %s"), device.c_str());

	delete profile;
}

void CDExtraClientApp::getLastReflector(wxString& reflector) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Read(KEY_LAST_REFLECTOR, &reflector, DEFAULT_LAST_REFLECTOR);

	wxLogInfo(wxT("Last Reflector set to: %s"), reflector.c_str());

	delete profile;
}

void CDExtraClientApp::setLastReflector(const wxString& reflector) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Write(KEY_LAST_REFLECTOR, reflector);

	wxLogInfo(wxT("Last Reflector set to: %s"), reflector.c_str());

	delete profile;
}

void CDExtraClientApp::getLastModule(unsigned int& module) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	long dummy;
	profile->Read(KEY_LAST_MODULE, &dummy, DEFAULT_LAST_MODULE);
	module = (unsigned int)dummy;

	wxLogInfo(wxT("Last Module set to: %u"), module);

	delete profile;
}

void CDExtraClientApp::setLastModule(unsigned int module) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Write(KEY_LAST_MODULE, long(module));

	wxLogInfo(wxT("Last Module set to: %u"), module);

	delete profile;
}

void CDExtraClientApp::getBleep(bool& on) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Read(KEY_BLEEP, &on, DEFAULT_BLEEP);

	wxLogInfo(wxT("End bleep set to %d"), int(on));

	delete profile;
}

void CDExtraClientApp::setBleep(bool on) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Write(KEY_BLEEP, on);
	profile->Flush();

	wxLogInfo(wxT("End bleep set to %d"), int(on));

	m_thread->setBleep(on);

	delete profile;
}

void CDExtraClientApp::getController(wxString& type, unsigned int& audioDelay, bool& squelchInvert, bool& pttInvert) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Read(KEY_IFACE_TYPE,     &type,          DEFAULT_IFACE_TYPE);

	long temp;
	profile->Read(KEY_AUDIO_DELAY,    &temp,          DEFAULT_AUDIO_DELAY);
	audioDelay = (unsigned int)temp;

	profile->Read(KEY_SQUELCH_INVERT, &squelchInvert, DEFAULT_SQUELCH_INVERT);
	profile->Read(KEY_PTT_INVERT,     &pttInvert,     DEFAULT_PTT_INVERT);

	wxLogInfo(wxT("Interface type set to: %s, audio delay: %u ms, ptt invert: %d, squelch invert: %d"), type.c_str(), audioDelay * FRAME_TIME_MS, pttInvert, squelchInvert);

	delete profile;
}

void CDExtraClientApp::setController(const wxString& type, unsigned int audioDelay, bool squelchInvert, bool pttInvert) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Write(KEY_IFACE_TYPE,     type);
	profile->Write(KEY_AUDIO_DELAY,    long(audioDelay));
	profile->Write(KEY_SQUELCH_INVERT, squelchInvert);
	profile->Write(KEY_PTT_INVERT,     pttInvert);

	wxLogInfo(wxT("Interface type set to: %s, audio delay: %u ms, ptt invert: %d, squelch invert: %d"), type.c_str(), audioDelay * FRAME_TIME_MS, pttInvert, squelchInvert);

	delete profile;
}

void CDExtraClientApp::getStartup(wxString& reflector, unsigned int& module) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Read(KEY_STARTUP_REFLECTOR, &reflector, DEFAULT_STARTUP_REFLECTOR);

	long dummy;
	profile->Read(KEY_STARTUP_MODULE,    &dummy,     DEFAULT_STARTUP_MODULE);
	module = (unsigned int)dummy;

	wxLogInfo(wxT("Startup Reflector set to: %s:%u"), reflector.c_str(), module);

	delete profile;
}

void CDExtraClientApp::setStartup(const wxString& reflector, unsigned int module) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Write(KEY_STARTUP_REFLECTOR, reflector);
	profile->Write(KEY_STARTUP_MODULE,    int(module));

	wxLogInfo(wxT("Startup Reflector set to: %s:%u"), reflector.c_str(), module);

	delete profile;
}

void CDExtraClientApp::createThread()
{
	m_thread = new CDExtraClientThread;
	m_thread->Create();
	m_thread->Run();

	wxString callsign;
	getCallsign(callsign);
	m_thread->setCallsign(callsign);

	wxString message;
	getMessage(message);
	m_thread->setMessage(message);

	wxString device;
	getDVDongle(device);

	if (!device.IsEmpty()) {
		CDVDongleController* dongle = new CDVDongleController(new CSerialDataController(device, SERIAL_230400));

		bool res = dongle->open();
		if (!res) {
			wxLogError(wxT("Can't find the DV-Dongle on the port specified"));
			error(_("Can't find the DV-Dongle on the port specified"));
		} else {
			m_thread->setDVDongle(dongle);
		}
	}

	wxString readDevice, writeDevice;
	getSoundcard(readDevice, writeDevice);

	if (!readDevice.IsEmpty() && !writeDevice.IsEmpty()) {
#if defined(__WINDOWS__)
		CSoundCardReaderWriter* soundcard = new CSoundCardReaderWriter(readDevice, writeDevice, DSTAR_RADIO_SAMPLE_RATE, DSTAR_RADIO_BLOCK_SIZE);
#else
		CSoundCardReaderWriter* soundcard = new CSoundCardReaderWriter(readDevice, writeDevice, DSTAR_RADIO_SAMPLE_RATE, 64U);
#endif
		soundcard->setCallback(m_thread, 0);

		bool res = soundcard->open();
		if (!res) {
			wxLogError(wxT("Cannot open the sound card"));
			error(_("Cannot open the sound card"));
		} else {
			m_thread->setSoundCard(soundcard);
		}
	}

	bool bleep;
	getBleep(bleep);
	m_thread->setBleep(bleep);

	wxString type;
	unsigned int audioDelay;
	bool squelchInvert, pttInvert;
	getController(type, audioDelay, squelchInvert, pttInvert);

	IExternalController* controller = NULL;

	wxString port;
	if (type.StartsWith(wxT("Velleman K8055 - "), &port)) {
		unsigned long num;
		port.ToULong(&num);
		controller = new CK8055Controller(num);
	} else if (type.StartsWith(wxT("URI USB - "), &port)) {
		unsigned long num;
		port.ToULong(&num);
		controller = new CURIUSBController(num);
	} else if (type.StartsWith(wxT("Serial - "), &port)) {
		controller = new CSerialController(port);
	} else {
		controller = new CDummyController;
	}

	bool res = controller->open();
	if (!res)
		wxLogError(wxT("Cannot open the hardware interface - %s"), type.c_str());
	else
		m_thread->setController(controller, audioDelay, pttInvert, squelchInvert);
}
