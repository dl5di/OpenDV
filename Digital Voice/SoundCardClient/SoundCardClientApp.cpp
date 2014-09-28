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

#include "SoundCardClientApp.h"

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

IMPLEMENT_APP(CSoundCardClientApp)

static const wxString KEY_CALLSIGN1                = wxT("/callsign1");
static const wxString KEY_CALLSIGN2                = wxT("/callsign2");
static const wxString KEY_MESSAGE_TYPE             = wxT("/msgType");
static const wxString KEY_MESSAGE_TEXT             = wxT("/msgText");
static const wxString KEY_IFACE_TYPE               = wxT("/interfaceType");
static const wxString KEY_PTT_ENABLE               = wxT("/pttEnable");
static const wxString KEY_TX_DELAY                 = wxT("/txDelay");
static const wxString KEY_TX_INVERT                = wxT("/txInvert");
static const wxString KEY_AUDIO_SOUND_READ_DEVICE  = wxT("/audioReadDevice");
static const wxString KEY_AUDIO_SOUND_WRITE_DEVICE = wxT("/audioWriteDevice");
static const wxString KEY_RADIO_SOUND_READ_DEVICE  = wxT("/radioReadDevice");
static const wxString KEY_RADIO_SOUND_WRITE_DEVICE = wxT("/radioWriteDevice");
static const wxString KEY_GMSK_TXINV               = wxT("/txGMSKInvert");
static const wxString KEY_GMSK_RXINV               = wxT("/rxGMSKInvert");
static const wxString KEY_DVD_DEVICE               = wxT("/dvdDevice");
static const wxString KEY_YOUR_CALL                = wxT("/yourCall");
static const wxString KEY_YOUR_LIST                = wxT("/yourList");
static const wxString KEY_RPT1_CALL                = wxT("/rpt1Call");
static const wxString KEY_RPT1_LIST                = wxT("/rpt1List");
static const wxString KEY_RPT2_CALL                = wxT("/rpt2Call");
static const wxString KEY_RPT2_LIST                = wxT("/rpt2List");
static const wxString KEY_BLEEP                    = wxT("/bleep");

static const wxString DEFAULT_CALLSIGN1                = wxEmptyString;
static const wxString DEFAULT_CALLSIGN2                = wxEmptyString;
static const wxString DEFAULT_MESSAGE_TEXT             = wxEmptyString;
static const wxString DEFAULT_IFACE_TYPE               = wxEmptyString;
static const bool     DEFAULT_PTT_ENABLE               = false;
static const long     DEFAULT_TX_DELAY                 = 0L;
static const bool     DEFAULT_TX_INVERT                = false;
static const wxString DEFAULT_AUDIO_SOUND_READ_DEVICE  = wxEmptyString;
static const wxString DEFAULT_AUDIO_SOUND_WRITE_DEVICE = wxEmptyString;
static const wxString DEFAULT_RADIO_SOUND_READ_DEVICE  = wxEmptyString;
static const wxString DEFAULT_RADIO_SOUND_WRITE_DEVICE = wxEmptyString;
static const bool     DEFAULT_GMSK_RXINV               = false;
static const bool     DEFAULT_GMSK_TXINV               = false;
static const wxString DEFAULT_DVD_DEVICE               = wxEmptyString;
static const wxString DEFAULT_YOUR_CALL                = wxEmptyString;
static const wxString DEFAULT_YOUR_LIST                = wxEmptyString;
static const wxString DEFAULT_RPT1_CALL                = wxEmptyString;
static const wxString DEFAULT_RPT1_LIST                = wxEmptyString;
static const wxString DEFAULT_RPT2_CALL                = wxEmptyString;
static const wxString DEFAULT_RPT2_LIST                = wxEmptyString;
static const bool     DEFAULT_BLEEP                    = true;


CSoundCardClientApp::CSoundCardClientApp() :
wxApp(),
m_frame(NULL),
m_thread(NULL)
{
}

CSoundCardClientApp::~CSoundCardClientApp()
{
}

bool CSoundCardClientApp::OnInit()
{
	SetVendorName(VENDOR_NAME);

	wxFileName loggerName(wxFileName::GetHomeDir(), LOG_FILE_NAME, wxT("log"));
	wxLog* log = new CLogger(loggerName.GetFullPath());
	wxLog::SetActiveTarget(log);

	m_frame = new CSoundCardClientFrame(APPLICATION_NAME + wxT(" - ") + VERSION);
	m_frame->Show();

	SetTopWindow(m_frame);

	wxLogInfo(wxT("Starting ") + APPLICATION_NAME + wxT(" - ") + VERSION);

	// Log the SVN revsion and the version of wxWidgets and the Operating System
	wxLogInfo(SVNREV);
	wxLogInfo(wxT("Using wxWidgets %d.%d.%d on %s"), wxMAJOR_VERSION, wxMINOR_VERSION, wxRELEASE_NUMBER, ::wxGetOsDescription().c_str());

	createThread();

	return wxApp::OnInit();
}

int CSoundCardClientApp::OnExit()
{
	wxLogInfo(APPLICATION_NAME + wxT(" is exiting"));

	m_thread->kill();
	m_thread->Wait();
	delete m_thread;

	return 0;
}

#if defined(__WXDEBUG__)
void CSoundCardClientApp::OnAssertFailure(const wxChar* file, int line, const wxChar* func, const wxChar* cond, const wxChar* msg)
{
	wxLogFatalError(wxT("Assertion failed on line %d in file %s and function %s: %s %s"), line, file, func, cond, msg);
}
#endif

void CSoundCardClientApp::showMessage(CMessageData* message) const
{
	wxASSERT(message != NULL);

	m_frame->showMessage(message);
}

void CSoundCardClientApp::showHeader(CHeaderData* header) const
{
	wxASSERT(header != NULL);

	m_frame->showHeader(header);
}

void CSoundCardClientApp::showBusy(CBusyData* busy) const
{
	wxASSERT(busy != NULL);

	m_frame->showBusy(busy);
}

void CSoundCardClientApp::error(const wxString& text) const
{
	m_frame->error(text);
}

void CSoundCardClientApp::setReader(CWAVFileReader* reader)
{
	return m_thread->setReader(reader);
}

void CSoundCardClientApp::setYour(const wxString& your)
{
	m_thread->setYour(your);
}

void CSoundCardClientApp::setRpt1(const wxString& rpt1)
{
	m_thread->setRpt1(rpt1);
}

void CSoundCardClientApp::setRpt2(const wxString& rpt2)
{
	m_thread->setRpt2(rpt2);
}

bool CSoundCardClientApp::setTransmit(bool on)
{
	return m_thread->setTransmit(on);
}

void CSoundCardClientApp::setRecording(bool recording)
{
	m_thread->setRecording(recording);
}

void CSoundCardClientApp::getCallsign(wxString& callsign1, wxString& callsign2) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Read(KEY_CALLSIGN1, &callsign1, DEFAULT_CALLSIGN1);
	profile->Read(KEY_CALLSIGN2, &callsign2, DEFAULT_CALLSIGN2);

	wxLogInfo(wxT("Callsign set to %s/%s"), callsign1.c_str(), callsign2.c_str());

	delete profile;
}

void CSoundCardClientApp::setCallsign(const wxString& callsign1, const wxString& callsign2) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Write(KEY_CALLSIGN1, callsign1);
	profile->Write(KEY_CALLSIGN2, callsign2);
	profile->Flush();

	wxLogInfo(wxT("Callsign set to %s/%s"), callsign1.c_str(), callsign2.c_str());

	m_thread->setCallsign(callsign1, callsign2);

	delete profile;
}

void CSoundCardClientApp::getMessage(wxString& message) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Read(KEY_MESSAGE_TEXT, &message, DEFAULT_MESSAGE_TEXT);

	wxLogInfo(wxT("Message set to %s"), message.c_str());

	delete profile;
}

void CSoundCardClientApp::setMessage(const wxString& message) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Write(KEY_MESSAGE_TEXT, message);
	profile->Flush();

	wxLogInfo(wxT("Message set to %s"), message.c_str());

	m_thread->setMessage(message);

	delete profile;
}

void CSoundCardClientApp::getController(wxString& type, bool& pttEnable, unsigned int& txDelay, bool& txInvert) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Read(KEY_IFACE_TYPE, &type,      DEFAULT_IFACE_TYPE);
	profile->Read(KEY_PTT_ENABLE, &pttEnable, DEFAULT_PTT_ENABLE);

	long temp;
	profile->Read(KEY_TX_DELAY,   &temp,      DEFAULT_TX_DELAY);
	txDelay = (unsigned int)temp;

	profile->Read(KEY_TX_INVERT,  &txInvert,  DEFAULT_TX_INVERT);

	wxLogInfo(wxT("Interface type set to: %s, ptt enabled: %d, tx delay: %u ms, tx invert: %d"), type.c_str(), pttEnable, txDelay * FRAME_TIME_MS, txInvert);

	delete profile;
}

void CSoundCardClientApp::setController(const wxString& type, bool pttEnable, unsigned int txDelay, bool txInvert) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Write(KEY_IFACE_TYPE, type);
	profile->Write(KEY_PTT_ENABLE, pttEnable);
	profile->Write(KEY_TX_DELAY,   long(txDelay));
	profile->Write(KEY_TX_INVERT,  txInvert);

	wxLogInfo(wxT("Interface type set to: %s, ptt enabled: %d, tx delay: %u ms, tx invert: %d"), type.c_str(), pttEnable, txDelay * FRAME_TIME_MS, txInvert);

	delete profile;
}

void CSoundCardClientApp::getRadioSoundcard(wxString& readDevice, wxString& writeDevice) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Read(KEY_RADIO_SOUND_READ_DEVICE,  &readDevice,  DEFAULT_RADIO_SOUND_READ_DEVICE);
	profile->Read(KEY_RADIO_SOUND_WRITE_DEVICE, &writeDevice, DEFAULT_RADIO_SOUND_WRITE_DEVICE);

	wxLogInfo(wxT("Radio soundcard set to %s:%s"), readDevice.c_str(), writeDevice.c_str());

	delete profile;
}

void CSoundCardClientApp::setRadioSoundcard(const wxString& readDevice, const wxString& writeDevice) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Write(KEY_RADIO_SOUND_READ_DEVICE,  readDevice);
	profile->Write(KEY_RADIO_SOUND_WRITE_DEVICE, writeDevice);
	profile->Flush();

	wxLogInfo(wxT("Radio soundcard set to %s:%s"), readDevice.c_str(), writeDevice.c_str());

	delete profile;
}

void CSoundCardClientApp::getInversion(bool& rxInvert, bool& txInvert) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Read(KEY_GMSK_RXINV, &rxInvert, DEFAULT_GMSK_RXINV);
	profile->Read(KEY_GMSK_TXINV, &txInvert, DEFAULT_GMSK_TXINV);

	wxLogInfo(wxT("GMSK Inversion set to %d:%d"), int(rxInvert), int(txInvert));

	delete profile;
}

void CSoundCardClientApp::setInversion(bool rxInvert, bool txInvert) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Write(KEY_GMSK_RXINV, rxInvert);
	profile->Write(KEY_GMSK_TXINV, txInvert);
	profile->Flush();

	wxLogInfo(wxT("GMSK Inversion set to %d:%d"), int(rxInvert), int(txInvert));

	m_thread->setInversion(rxInvert, txInvert);

	delete profile;
}

void CSoundCardClientApp::getAudioSoundcard(wxString& readDevice, wxString& writeDevice) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Read(KEY_AUDIO_SOUND_READ_DEVICE,  &readDevice,  DEFAULT_AUDIO_SOUND_READ_DEVICE);
	profile->Read(KEY_AUDIO_SOUND_WRITE_DEVICE, &writeDevice, DEFAULT_AUDIO_SOUND_WRITE_DEVICE);

	wxLogInfo(wxT("Audio soundcard set to %s:%s"), readDevice.c_str(), writeDevice.c_str());

	delete profile;
}

void CSoundCardClientApp::setAudioSoundcard(const wxString& readDevice, const wxString& writeDevice) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Write(KEY_AUDIO_SOUND_READ_DEVICE,  readDevice);
	profile->Write(KEY_AUDIO_SOUND_WRITE_DEVICE, writeDevice);
	profile->Flush();

	wxLogInfo(wxT("Audio soundcard set to %s:%s"), readDevice.c_str(), writeDevice.c_str());

	delete profile;
}

void CSoundCardClientApp::getDVDongle(wxString& device) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Read(KEY_DVD_DEVICE, &device, DEFAULT_DVD_DEVICE);

	wxLogInfo(wxT("DV Dongle set to: %s"), device.c_str());

	delete profile;
}

void CSoundCardClientApp::setDVDongle(const wxString& device) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Write(KEY_DVD_DEVICE, device);
	profile->Flush();

	wxLogInfo(wxT("DV Dongle set to: %s"), device.c_str());

	delete profile;
}

void CSoundCardClientApp::getYourCalls(wxString& call, wxSortedArrayString& list) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	wxString temp;

	profile->Read(KEY_YOUR_CALL, &call, DEFAULT_YOUR_CALL);
	profile->Read(KEY_YOUR_LIST, &temp, DEFAULT_YOUR_LIST);

	wxLogInfo(wxT("Your call set to %s"), call.c_str());
	wxLogInfo(wxT("Your list set to %s"), temp.c_str());

	// Convert the comma delimited list to an array
	list.Clear();
	wxStringTokenizer tokens(temp, wxT(","));
	while (tokens.HasMoreTokens()) {
		wxString item = tokens.GetNextToken();
		if (!item.IsEmpty())
			list.Add(item);
	}

	delete profile;
}

void CSoundCardClientApp::setYourCalls(const wxString& call, const wxSortedArrayString& list) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	// Convert the array into a comma delimited list
	wxString temp;
	size_t count = list.GetCount();
	for (unsigned int i = 0U; i < count; i++) {
		temp += list.Item(i);
		temp += wxT(',');
	}

	profile->Write(KEY_YOUR_CALL, call);
	profile->Write(KEY_YOUR_LIST, temp);
	profile->Flush();

	wxLogInfo(wxT("Your call set to %s"), call.c_str());
	wxLogInfo(wxT("Your list set to %s"), temp.c_str());

	delete profile;
}

void CSoundCardClientApp::getRpt1Calls(wxString& call, wxSortedArrayString& list) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	wxString temp;

	profile->Read(KEY_RPT1_CALL, &call, DEFAULT_RPT1_CALL);
	profile->Read(KEY_RPT1_LIST, &temp, DEFAULT_RPT1_LIST);

	wxLogInfo(wxT("RPT1 call set to %s"), call.c_str());
	wxLogInfo(wxT("RPT1 list set to %s"), temp.c_str());

	// Convert the comma delimited list to an array
	list.Clear();
	wxStringTokenizer tokens(temp, wxT(","));
	while (tokens.HasMoreTokens()) {
		wxString item = tokens.GetNextToken();
		if (!item.IsEmpty())
			list.Add(item);
	}

	delete profile;
}

void CSoundCardClientApp::setRpt1Calls(const wxString& call, const wxSortedArrayString& list) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	// Convert the array into a comma delimited list
	wxString temp;
	size_t count = list.GetCount();
	for (unsigned int i = 0U; i < count; i++) {
		temp += list.Item(i);
		temp += wxT(',');
	}

	profile->Write(KEY_RPT1_CALL, call);
	profile->Write(KEY_RPT1_LIST, temp);
	profile->Flush();

	wxLogInfo(wxT("RPT1 call set to %s"), call.c_str());
	wxLogInfo(wxT("RPT1 list set to %s"), temp.c_str());

	delete profile;
}

void CSoundCardClientApp::getRpt2Calls(wxString& call, wxSortedArrayString& list) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	wxString temp;

	profile->Read(KEY_RPT2_CALL, &call, DEFAULT_RPT2_CALL);
	profile->Read(KEY_RPT2_LIST, &temp, DEFAULT_RPT2_LIST);

	wxLogInfo(wxT("RPT2 call set to %s"), call.c_str());
	wxLogInfo(wxT("RPT2 list set to %s"), temp.c_str());

	// Convert the comma delimited list to an array
	list.Clear();
	wxStringTokenizer tokens(temp, wxT(","));
	while (tokens.HasMoreTokens()) {
		wxString item = tokens.GetNextToken();
		if (!item.IsEmpty())
			list.Add(item);
	}

	delete profile;
}

void CSoundCardClientApp::setRpt2Calls(const wxString& call, const wxSortedArrayString& list) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	// Convert the array into a comma delimited list
	wxString temp;
	size_t count = list.GetCount();
	for (unsigned int i = 0U; i < count; i++) {
		temp += list.Item(i);
		temp += wxT(',');
	}

	profile->Write(KEY_RPT2_CALL, call);
	profile->Write(KEY_RPT2_LIST, temp);
	profile->Flush();

	wxLogInfo(wxT("RPT2 call set to %s"), call.c_str());
	wxLogInfo(wxT("RPT2 list set to %s"), temp.c_str());

	delete profile;
}

void CSoundCardClientApp::getBleep(bool& on) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Read(KEY_BLEEP, &on, DEFAULT_BLEEP);

	wxLogInfo(wxT("End bleep set to %d"), int(on));

	delete profile;
}

void CSoundCardClientApp::setBleep(bool on) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Write(KEY_BLEEP, on);
	profile->Flush();

	wxLogInfo(wxT("End bleep set to %d"), int(on));

	m_thread->setBleep(on);

	delete profile;
}

void CSoundCardClientApp::createThread()
{
	m_thread = new CSoundCardClientThread;
	m_thread->Create();
	m_thread->Run();

	wxString callsign1, callsign2;
	getCallsign(callsign1, callsign2);
	m_thread->setCallsign(callsign1, callsign2);

	wxString call;
	wxSortedArrayString list;

	getYourCalls(call, list);
	m_thread->setYour(call);

	getRpt1Calls(call, list);
	m_thread->setRpt1(call);

	getRpt2Calls(call, list);
	m_thread->setRpt2(call);

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
	getAudioSoundcard(readDevice, writeDevice);

	if (!readDevice.IsEmpty() && !writeDevice.IsEmpty()) {
#if defined(__WINDOWS__)
		CSoundCardReaderWriter* soundcard = new CSoundCardReaderWriter(readDevice, writeDevice, DSTAR_RADIO_SAMPLE_RATE, DSTAR_RADIO_BLOCK_SIZE);
#else
		CSoundCardReaderWriter* soundcard = new CSoundCardReaderWriter(readDevice, writeDevice, DSTAR_RADIO_SAMPLE_RATE, 64U);
#endif
		soundcard->setCallback(m_thread, USER_AUDIO_ID);

		bool res = soundcard->open();
		if (!res) {
			wxLogError(wxT("Cannot open the audio sound card"));
			error(_("Cannot open the audio sound card"));
		} else {
			m_thread->setAudioSoundCard(soundcard);
		}
	}

	getRadioSoundcard(readDevice, writeDevice);

	if (!readDevice.IsEmpty() && !writeDevice.IsEmpty()) {
#if defined(__WINDOWS__)
		CSoundCardReaderWriter* soundcard = new CSoundCardReaderWriter(readDevice, writeDevice, DSTAR_RADIO_SAMPLE_RATE, DSTAR_RADIO_BLOCK_SIZE);
#else
		CSoundCardReaderWriter* soundcard = new CSoundCardReaderWriter(readDevice, writeDevice, DSTAR_RADIO_SAMPLE_RATE, 64U);
#endif
		soundcard->setCallback(m_thread, RADIO_AUDIO_ID);

		bool res = soundcard->open();
		if (!res) {
			wxLogError(wxT("Cannot open the radio sound card"));
			error(_("Cannot open the radio sound card"));
		} else {
			m_thread->setRadioSoundCard(soundcard);
		}
	}

	bool rxGMSKInvert, txGMSKInvert;
	getInversion(rxGMSKInvert, txGMSKInvert);
	m_thread->setInversion(rxGMSKInvert, txGMSKInvert);

	wxString type;
	unsigned int txDelay;
	bool pttEnable, txInvert;
	getController(type, pttEnable, txDelay, txInvert);

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
		m_thread->setController(controller, pttEnable, txDelay, txInvert);

	bool bleep;
	getBleep(bleep);
	m_thread->setBleep(bleep);
}
