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

#include "DVRPTRClientApp.h"

#include "SerialDataController.h"
#include "DVRPTRControllerV2.h"
#include "DVRPTRControllerV1.h"
#include "DVRPTRController.h"
#include "DStarDefines.h"
#include "Version.h"
#include "Logger.h"

#include <wx/tokenzr.h>
#include <wx/config.h>
#include <wx/filename.h>

IMPLEMENT_APP(CDVRPTRClientApp)

static const wxString KEY_CALLSIGN1          = wxT("/callsign1");
static const wxString KEY_CALLSIGN2          = wxT("/callsign2");
static const wxString KEY_MESSAGE_TYPE       = wxT("/msgType");
static const wxString KEY_MESSAGE_TEXT       = wxT("/msgText");
static const wxString KEY_MODEM_VERSION      = wxT("/modemVersion");
static const wxString KEY_MODEM_PORT         = wxT("/modemPort");
static const wxString KEY_MODEM_RXINVERT     = wxT("/modemFrequency");
static const wxString KEY_MODEM_TXINVERT     = wxT("/modemTXInvert");
static const wxString KEY_MODEM_CHANNEL      = wxT("/modemRXInvert");
static const wxString KEY_MODEM_MODLEVEL     = wxT("/modemModLevel");
static const wxString KEY_MODEM_TXDELAY      = wxT("/modemTXDelay");
static const wxString KEY_SOUND_READ_DEVICE  = wxT("/readDevice");
static const wxString KEY_SOUND_WRITE_DEVICE = wxT("/writeDevice");
static const wxString KEY_DVD_DEVICE         = wxT("/dvdDevice");
static const wxString KEY_YOUR_CALL          = wxT("/yourCall");
static const wxString KEY_YOUR_LIST          = wxT("/yourList");
static const wxString KEY_RPT1_CALL          = wxT("/rpt1Call");
static const wxString KEY_RPT1_LIST          = wxT("/rpt1List");
static const wxString KEY_RPT2_CALL          = wxT("/rpt2Call");
static const wxString KEY_RPT2_LIST          = wxT("/rpt2List");
static const wxString KEY_BLEEP              = wxT("/bleep");

static const wxString   DEFAULT_CALLSIGN1          = wxEmptyString;
static const wxString   DEFAULT_CALLSIGN2          = wxEmptyString;
static const wxString   DEFAULT_MESSAGE_TEXT       = wxEmptyString;
static const DVRPTR_VERSION DEFAULT_MODEM_VERSION  = DVRPTR_V1;
static const wxString   DEFAULT_MODEM_PORT         = wxEmptyString;
static const bool       DEFAULT_MODEM_RXINVERT     = false;
static const bool       DEFAULT_MODEM_TXINVERT     = false;
static const bool       DEFAULT_MODEM_CHANNEL      = false;
static const long       DEFAULT_MODEM_MODLEVEL     = 20L;
static const long       DEFAULT_MODEM_TXDELAY      = 150L;
static const wxString   DEFAULT_SOUND_READ_DEVICE  = wxEmptyString;
static const wxString   DEFAULT_SOUND_WRITE_DEVICE = wxEmptyString;
static const wxString   DEFAULT_DVD_DEVICE         = wxEmptyString;
static const wxString   DEFAULT_YOUR_CALL          = wxEmptyString;
static const wxString   DEFAULT_YOUR_LIST          = wxEmptyString;
static const wxString   DEFAULT_RPT1_CALL          = wxEmptyString;
static const wxString   DEFAULT_RPT1_LIST          = wxEmptyString;
static const wxString   DEFAULT_RPT2_CALL          = wxEmptyString;
static const wxString   DEFAULT_RPT2_LIST          = wxEmptyString;
static const bool       DEFAULT_BLEEP              = true;


CDVRPTRClientApp::CDVRPTRClientApp() :
wxApp(),
m_frame(NULL),
m_thread(NULL)
{
}

CDVRPTRClientApp::~CDVRPTRClientApp()
{
}

bool CDVRPTRClientApp::OnInit()
{
	SetVendorName(VENDOR_NAME);

	wxFileName loggerName(wxFileName::GetHomeDir(), LOG_FILE_NAME, wxT("log"));
	wxLog* log = new CLogger(loggerName.GetFullPath());
	wxLog::SetActiveTarget(log);

	m_frame = new CDVRPTRClientFrame(APPLICATION_NAME + wxT(" - ") + VERSION);
	m_frame->Show();

	SetTopWindow(m_frame);

	wxLogInfo(wxT("Starting ") + APPLICATION_NAME + wxT(" - ") + VERSION);

	// Log the SVN revsion and the version of wxWidgets and the Operating System
	wxLogInfo(SVNREV);
	wxLogInfo(wxT("Using wxWidgets %d.%d.%d on %s"), wxMAJOR_VERSION, wxMINOR_VERSION, wxRELEASE_NUMBER, ::wxGetOsDescription().c_str());

	createThread();

	return wxApp::OnInit();
}

int CDVRPTRClientApp::OnExit()
{
	wxLogInfo(APPLICATION_NAME + wxT(" is exiting"));

	m_thread->kill();
	m_thread->Wait();
	delete m_thread;

	return 0;
}

#if defined(__WXDEBUG__)
void CDVRPTRClientApp::OnAssertFailure(const wxChar* file, int line, const wxChar* func, const wxChar* cond, const wxChar* msg)
{
	wxLogFatalError(wxT("Assertion failed on line %d in file %s and function %s: %s %s"), line, file, func, cond, msg);
}
#endif

void CDVRPTRClientApp::showMessage(CMessageData* message) const
{
	wxASSERT(message != NULL);

	m_frame->showMessage(message);
}

void CDVRPTRClientApp::showHeader(CHeaderData* header) const
{
	wxASSERT(header != NULL);

	m_frame->showHeader(header);
}

void CDVRPTRClientApp::showBusy(CBusyData* busy) const
{
	wxASSERT(busy != NULL);

	m_frame->showBusy(busy);
}

void CDVRPTRClientApp::error(const wxString& text) const
{
	m_frame->error(text);
}

void CDVRPTRClientApp::setYour(const wxString& your)
{
	m_thread->setYour(your);
}

void CDVRPTRClientApp::setRpt1(const wxString& rpt1)
{
	m_thread->setRpt1(rpt1);
}

void CDVRPTRClientApp::setRpt2(const wxString& rpt2)
{
	m_thread->setRpt2(rpt2);
}

bool CDVRPTRClientApp::setTransmit(bool on)
{
	return m_thread->setTransmit(on);
}

void CDVRPTRClientApp::setRecording(bool recording)
{
	m_thread->setRecording(recording);
}

void CDVRPTRClientApp::getCallsign(wxString& callsign1, wxString& callsign2) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Read(KEY_CALLSIGN1, &callsign1, DEFAULT_CALLSIGN1);
	profile->Read(KEY_CALLSIGN2, &callsign2, DEFAULT_CALLSIGN2);

	wxLogInfo(wxT("Callsign set to %s/%s"), callsign1.c_str(), callsign2.c_str());

	delete profile;
}

void CDVRPTRClientApp::setCallsign(const wxString& callsign1, const wxString& callsign2) const
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

void CDVRPTRClientApp::getMessage(wxString& message) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Read(KEY_MESSAGE_TEXT, &message, DEFAULT_MESSAGE_TEXT);

	wxLogInfo(wxT("Message set to %s"), message.c_str());

	delete profile;
}

void CDVRPTRClientApp::setMessage(const wxString& message) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Write(KEY_MESSAGE_TEXT, message);
	profile->Flush();

	wxLogInfo(wxT("Message set to %s"), message.c_str());

	m_thread->setMessage(message);

	delete profile;
}

void CDVRPTRClientApp::getModem(DVRPTR_VERSION& version, wxString& port, bool& rxInvert, bool& txInvert, bool& channel, unsigned int& modLevel, unsigned int& txDelay) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	long temp;
	profile->Read(KEY_MODEM_VERSION,  &temp,     long(DEFAULT_MODEM_VERSION));
	version = DVRPTR_VERSION(temp);

	profile->Read(KEY_MODEM_PORT,     &port,     DEFAULT_MODEM_PORT);
	profile->Read(KEY_MODEM_RXINVERT, &rxInvert, DEFAULT_MODEM_RXINVERT);
	profile->Read(KEY_MODEM_TXINVERT, &txInvert, DEFAULT_MODEM_TXINVERT);
	profile->Read(KEY_MODEM_CHANNEL,  &channel,  DEFAULT_MODEM_CHANNEL);

	profile->Read(KEY_MODEM_MODLEVEL, &temp,     DEFAULT_MODEM_MODLEVEL);
	modLevel = (unsigned int)temp;

	profile->Read(KEY_MODEM_TXDELAY,  &temp,     DEFAULT_MODEM_TXDELAY);
	txDelay = (unsigned int)temp;

	wxLogInfo(wxT("DV-RPTR modem: version: %d, port: %s, RX invert: %d, TX invert: %d, channel: %s, mod level: %u%%, TX delay: %u ms"), int(version), port.c_str(), int(rxInvert), int(txInvert), channel ? wxT("B") : wxT("A"), modLevel, txDelay);

	delete profile;
}

void CDVRPTRClientApp::setModem(DVRPTR_VERSION version, const wxString& port, bool rxInvert, bool txInvert, bool channel, unsigned int modLevel, unsigned int txDelay) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Write(KEY_MODEM_VERSION,  long(version));
	profile->Write(KEY_MODEM_PORT,     port);
	profile->Write(KEY_MODEM_RXINVERT, rxInvert);
	profile->Write(KEY_MODEM_TXINVERT, txInvert);
	profile->Write(KEY_MODEM_CHANNEL,  channel);
	profile->Write(KEY_MODEM_MODLEVEL, long(modLevel));
	profile->Write(KEY_MODEM_TXDELAY,  long(txDelay));
	profile->Flush();

	wxLogInfo(wxT("DV-RPTR modem: version: %d, port: %s, RX invert: %d, TX invert: %d, channel: %s, mod level: %u%%, TX delay: %u ms"), int(version), port.c_str(), int(rxInvert), int(txInvert), channel ? wxT("B") : wxT("A"), modLevel, txDelay);

	delete profile;
}

void CDVRPTRClientApp::getSoundcard(wxString& readDevice, wxString& writeDevice) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Read(KEY_SOUND_READ_DEVICE,  &readDevice,  DEFAULT_SOUND_READ_DEVICE);
	profile->Read(KEY_SOUND_WRITE_DEVICE, &writeDevice, DEFAULT_SOUND_WRITE_DEVICE);

	wxLogInfo(wxT("Soundcard set to %s:%s"), readDevice.c_str(), writeDevice.c_str());

	delete profile;
}

void CDVRPTRClientApp::setSoundcard(const wxString& readDevice, const wxString& writeDevice) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Write(KEY_SOUND_READ_DEVICE,  readDevice);
	profile->Write(KEY_SOUND_WRITE_DEVICE, writeDevice);
	profile->Flush();

	wxLogInfo(wxT("Soundcard set to %s:%s"), readDevice.c_str(), writeDevice.c_str());

	delete profile;
}

void CDVRPTRClientApp::getDVDongle(wxString& device) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Read(KEY_DVD_DEVICE, &device, DEFAULT_DVD_DEVICE);

	wxLogInfo(wxT("DV Dongle set to: %s"), device.c_str());

	delete profile;
}

void CDVRPTRClientApp::setDVDongle(const wxString& device) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Write(KEY_DVD_DEVICE, device);
	profile->Flush();

	wxLogInfo(wxT("DV Dongle set to: %s"), device.c_str());

	delete profile;
}

void CDVRPTRClientApp::getYourCalls(wxString& call, wxSortedArrayString& list) const
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

void CDVRPTRClientApp::setYourCalls(const wxString& call, const wxSortedArrayString& list) const
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

void CDVRPTRClientApp::getRpt1Calls(wxString& call, wxSortedArrayString& list) const
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

void CDVRPTRClientApp::setRpt1Calls(const wxString& call, const wxSortedArrayString& list) const
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

void CDVRPTRClientApp::getRpt2Calls(wxString& call, wxSortedArrayString& list) const
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

void CDVRPTRClientApp::setRpt2Calls(const wxString& call, const wxSortedArrayString& list) const
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

void CDVRPTRClientApp::getBleep(bool& on) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Read(KEY_BLEEP, &on, DEFAULT_BLEEP);

	wxLogInfo(wxT("End bleep set to %d"), int(on));

	delete profile;
}

void CDVRPTRClientApp::setBleep(bool on) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Write(KEY_BLEEP, on);
	profile->Flush();

	wxLogInfo(wxT("End bleep set to %d"), int(on));

	m_thread->setBleep(on);

	delete profile;
}

void CDVRPTRClientApp::createThread()
{
	m_thread = new CDVRPTRClientThread;
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

	wxString modemPort;
	DVRPTR_VERSION version;
	bool rxInvert, txInvert, channel;
	unsigned int modLevel, txDelay;
	getModem(version, modemPort, rxInvert, txInvert, channel, modLevel, txDelay);

	if (!modemPort.IsEmpty()) {
		IDVRPTRController* controller = NULL;
		switch (version) {
			case DVRPTR_V1:
				controller = new CDVRPTRControllerV1(modemPort, rxInvert, txInvert, channel, modLevel, txDelay);
				break;
			case DVRPTR_V2:
				controller = new CDVRPTRControllerV2(modemPort, txInvert, modLevel);
				break;
			default:
				wxLogError(wxT("Unknown DV-RPTR modem version - %d"), int(version));
				break;
		}

		if (controller != NULL) {
			bool res = controller->open();
			if (!res) {
				wxLogError(wxT("Cannot open the DV-RPTR modem"));
				error(_("Cannot open the DV-RPTR modem"));
			} else {
				m_thread->setModem(controller);
			}
		}
	}

	bool bleep;
	getBleep(bleep);
	m_thread->setBleep(bleep);
}
