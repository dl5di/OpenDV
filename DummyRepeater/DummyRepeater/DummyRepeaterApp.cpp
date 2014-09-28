/*
 *   Copyright (C) 2010-2014 by Jonathan Naylor G4KLX
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

#include "RepeaterProtocolHandler.h"
#include "SoundCardReaderWriter.h"
#include "SerialLineController.h"
#if defined(RASPBERRY_PI)
#include "RaspberryController.h"
#endif
#include "ExternalController.h"
#include "DVDongleController.h"
#include "ArduinoController.h"
#include "DummyRepeaterApp.h"
#include "URIUSBController.h"
#include "DV3000Controller.h"
#include "K8055Controller.h"
#include "DummyController.h"
#include "DVDongleThread.h"
#include "AMBE3000Thread.h"
#include "DStarDefines.h"
#include "Version.h"
#include "Logger.h"

#include <wx/cmdline.h>
#include <wx/tokenzr.h>
#include <wx/config.h>
#include <wx/filename.h>

IMPLEMENT_APP(CDummyRepeaterApp)

const wxString KEY_CALLSIGN1              = wxT("/callsign1");
const wxString KEY_CALLSIGN2              = wxT("/callsign2");
const wxString KEY_MESSAGE_TYPE           = wxT("/msgType");
const wxString KEY_MESSAGE_TEXT           = wxT("/msgText");
const wxString KEY_SOUND_READ_DEVICE      = wxT("/readDevice");
const wxString KEY_SOUND_WRITE_DEVICE     = wxT("/writeDevice");
const wxString KEY_GATEWAY_ADDRESS        = wxT("/gwyAddress");
const wxString KEY_GATEWAY_PORT           = wxT("/gwyPort");
const wxString KEY_LOCAL_ADDRESS          = wxT("/localAddress");
const wxString KEY_LOCAL_PORT             = wxT("/localPort");
const wxString KEY_IFACE_TYPE             = wxT("/interfaceType");
const wxString KEY_IFACE_CONFIG           = wxT("/interfaceConfig");
const wxString KEY_PTT_INVERT             = wxT("/pttInvert");
const wxString KEY_SQUELCH_INVERT         = wxT("/squelchInvert");
const wxString KEY_DONGLE_TYPE            = wxT("/dongleType");
const wxString KEY_DONGLE_DEVICE          = wxT("/dongleDevice");
const wxString KEY_DONGLE_ADDRESS         = wxT("/dongleAddress");
const wxString KEY_DONGLE_PORT            = wxT("/donglePort");
const wxString KEY_YOUR_CALL              = wxT("/yourCall");
const wxString KEY_YOUR_LIST              = wxT("/yourList");
const wxString KEY_RPT1_CALL              = wxT("/rpt1Call");
const wxString KEY_RPT1_LIST              = wxT("/rpt1List");
const wxString KEY_RPT2_CALL              = wxT("/rpt2Call");
const wxString KEY_RPT2_LIST              = wxT("/rpt2List");
const wxString KEY_TIMEOUT                = wxT("/timeout");
const wxString KEY_BLEEP                  = wxT("/bleep");
const wxString KEY_WINDOW_X               = wxT("/windowX");
const wxString KEY_WINDOW_Y               = wxT("/windowY");

const wxString DEFAULT_CALLSIGN1          = wxEmptyString;
const wxString DEFAULT_CALLSIGN2          = wxEmptyString;
const wxString DEFAULT_MESSAGE_TEXT       = wxEmptyString;
const wxString DEFAULT_SOUND_READ_DEVICE  = wxEmptyString;
const wxString DEFAULT_SOUND_WRITE_DEVICE = wxEmptyString;
const wxString DEFAULT_GATEWAY_ADDRESS    = wxT("127.0.0.1");
const long     DEFAULT_GATEWAY_PORT       = 20010L;
const wxString DEFAULT_LOCAL_ADDRESS      = wxT("127.0.0.1");
const long     DEFAULT_LOCAL_PORT         = 20011L;
const wxString DEFAULT_IFACE_TYPE         = wxEmptyString;
const long     DEFAULT_IFACE_CONFIG       = 1L;
const bool     DEFAULT_PTT_INVERT         = false;
const bool     DEFAULT_SQUELCH_INVERT     = false;
const long     DEFAULT_DONGLE_TYPE        = 0L;
const wxString DEFAULT_DONGLE_DEVICE      = wxEmptyString;
const wxString DEFAULT_DONGLE_ADDRESS     = wxEmptyString;
const long     DEFAULT_DONGLE_PORT        = 2460L;
const wxString DEFAULT_YOUR_CALL          = wxEmptyString;
const wxString DEFAULT_YOUR_LIST          = wxEmptyString;
const wxString DEFAULT_RPT1_CALL          = wxEmptyString;
const wxString DEFAULT_RPT1_LIST          = wxEmptyString;
const wxString DEFAULT_RPT2_CALL          = wxEmptyString;
const wxString DEFAULT_RPT2_LIST          = wxEmptyString;
const long     DEFAULT_TIMEOUT            = 180L;
const bool     DEFAULT_BLEEP              = true;
const long     DEFAULT_WINDOW_X           = -1L;
const long     DEFAULT_WINDOW_Y           = -1L;


const wxChar* NOLOGGING_SWITCH = wxT("nolog");


CDummyRepeaterApp::CDummyRepeaterApp() :
wxApp(),
m_noLog(false),
m_frame(NULL),
m_thread(NULL)
{
}

CDummyRepeaterApp::~CDummyRepeaterApp()
{
}

bool CDummyRepeaterApp::OnInit()
{
	SetVendorName(VENDOR_NAME);

	if (!wxApp::OnInit())
		return false;

	if (!m_noLog) {
		wxLog* log = new CLogger(wxFileName::GetHomeDir(), LOG_BASE_NAME);
		wxLog::SetActiveTarget(log);
	} else {
		new wxLogNull;
	}

	wxPoint position = wxDefaultPosition;

	int x, y;
	getPosition(x, y);
	if (x >= 0 && y >= 0)
		position = wxPoint(x, y);

	m_frame = new CDummyRepeaterFrame(APPLICATION_NAME + wxT(" - ") + VERSION, position);
	m_frame->Show();

	SetTopWindow(m_frame);

	wxLogInfo(wxT("Starting ") + APPLICATION_NAME + wxT(" - ") + VERSION);

	// Log the SVN revsion and the version of wxWidgets and the Operating System
	wxLogInfo(SVNREV);
	wxLogInfo(wxT("Using wxWidgets %d.%d.%d on %s"), wxMAJOR_VERSION, wxMINOR_VERSION, wxRELEASE_NUMBER, ::wxGetOsDescription().c_str());

	createThread();

	return wxApp::OnInit();
}

int CDummyRepeaterApp::OnExit()
{
	wxLogInfo(APPLICATION_NAME + wxT(" is exiting"));

	m_thread->kill();
	m_thread->Wait();

	return 0;
}

void CDummyRepeaterApp::OnInitCmdLine(wxCmdLineParser& parser)
{
	parser.AddSwitch(NOLOGGING_SWITCH, wxEmptyString, wxEmptyString, wxCMD_LINE_PARAM_OPTIONAL);

	wxApp::OnInitCmdLine(parser);
}

bool CDummyRepeaterApp::OnCmdLineParsed(wxCmdLineParser& parser)
{
	if (!wxApp::OnCmdLineParsed(parser))
		return false;

	m_noLog = parser.Found(NOLOGGING_SWITCH);

	return true;
}

#if defined(__WXDEBUG__)
void CDummyRepeaterApp::OnAssertFailure(const wxChar* file, int line, const wxChar* func, const wxChar* cond, const wxChar* msg)
{
	wxLogFatalError(wxT("Assertion failed on line %d in file %s and function %s: %s %s"), line, file, func, cond, msg);
}
#endif

void CDummyRepeaterApp::showMessage(CMessageData* message) const
{
	wxASSERT(message != NULL);

	m_frame->showMessage(message);
}

void CDummyRepeaterApp::showHeader(CHeaderData* header) const
{
	m_frame->showHeader(header);
}

void CDummyRepeaterApp::showSlowData(const wxString& text) const
{
	m_frame->showSlowData(text);
}

void CDummyRepeaterApp::showStatus1(const wxString& text) const
{
	m_frame->showStatus1(text);
}

void CDummyRepeaterApp::showStatus2(const wxString& text) const
{
	m_frame->showStatus2(text);
}

void CDummyRepeaterApp::showStatus3(const wxString& text) const
{
	m_frame->showStatus3(text);
}

void CDummyRepeaterApp::showStatus4(const wxString& text) const
{
	m_frame->showStatus4(text);
}

void CDummyRepeaterApp::showStatus5(const wxString& text) const
{
	m_frame->showStatus5(text);
}

void CDummyRepeaterApp::error(const wxString& text) const
{
	m_frame->error(text);
}

void CDummyRepeaterApp::setYour(const wxString& your)
{
	m_thread->setYour(your);
}

void CDummyRepeaterApp::setRpt1(const wxString& rpt1)
{
	m_thread->setRpt1(rpt1);
}

void CDummyRepeaterApp::setRpt2(const wxString& rpt2)
{
	m_thread->setRpt2(rpt2);
}

bool CDummyRepeaterApp::setTransmit(bool on)
{
	return m_thread->setTransmit(on);
}

void CDummyRepeaterApp::getCallsign(wxString& callsign1, wxString& callsign2) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Read(KEY_CALLSIGN1, &callsign1, DEFAULT_CALLSIGN1);
	profile->Read(KEY_CALLSIGN2, &callsign2, DEFAULT_CALLSIGN2);

	wxLogInfo(wxT("Callsign set to %s/%s"), callsign1.c_str(), callsign2.c_str());

	delete profile;
}

void CDummyRepeaterApp::setCallsign(const wxString& callsign1, const wxString& callsign2) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Write(KEY_CALLSIGN1, callsign1);
	profile->Write(KEY_CALLSIGN2, callsign2);
	profile->Flush();

	wxLogInfo(wxT("Callsign set to %s/%s"), callsign1.c_str(), callsign2.c_str());

	delete profile;
}

void CDummyRepeaterApp::getMessage(wxString& message) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Read(KEY_MESSAGE_TEXT, &message, DEFAULT_MESSAGE_TEXT);

	wxLogInfo(wxT("Message set to %s"), message.c_str());

	delete profile;
}

void CDummyRepeaterApp::setMessage(const wxString& message) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Write(KEY_MESSAGE_TEXT, message);
	profile->Flush();

	wxLogInfo(wxT("Message set to %s"), message.c_str());

	m_thread->setMessage(message);

	delete profile;
}

void CDummyRepeaterApp::getSoundcard(wxString& readDevice, wxString& writeDevice) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Read(KEY_SOUND_READ_DEVICE,  &readDevice,  DEFAULT_SOUND_READ_DEVICE);
	profile->Read(KEY_SOUND_WRITE_DEVICE, &writeDevice, DEFAULT_SOUND_WRITE_DEVICE);

	wxLogInfo(wxT("Soundcard set to %s:%s"), readDevice.c_str(), writeDevice.c_str());

	delete profile;
}

void CDummyRepeaterApp::setSoundcard(const wxString& readDevice, const wxString& writeDevice) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Write(KEY_SOUND_READ_DEVICE,  readDevice);
	profile->Write(KEY_SOUND_WRITE_DEVICE, writeDevice);
	profile->Flush();

	wxLogInfo(wxT("Soundcard set to %s:%s"), readDevice.c_str(), writeDevice.c_str());

	delete profile;
}

void CDummyRepeaterApp::getDongle(DONGLE_TYPE& type, wxString& device, wxString& address, unsigned int& port) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	long temp;
	profile->Read(KEY_DONGLE_TYPE, &temp, DEFAULT_DONGLE_TYPE);
	type = DONGLE_TYPE(temp);

	profile->Read(KEY_DONGLE_DEVICE, &device, DEFAULT_DONGLE_DEVICE);

	profile->Read(KEY_DONGLE_ADDRESS, &address, DEFAULT_DONGLE_ADDRESS);

	profile->Read(KEY_DONGLE_PORT, &temp, DEFAULT_DONGLE_PORT);
	port = (unsigned int)temp;

	wxLogInfo(wxT("AMBE Dongle type: %d, device: %s, address: %s:%u"), int(type), device.c_str(), address.c_str(), port);

	delete profile;
}

void CDummyRepeaterApp::setDongle(DONGLE_TYPE type, const wxString& device, const wxString& address, unsigned int port) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Write(KEY_DONGLE_TYPE, long(type));
	profile->Write(KEY_DONGLE_DEVICE, device);
	profile->Write(KEY_DONGLE_ADDRESS, address);
	profile->Write(KEY_DONGLE_PORT, long(port));
	profile->Flush();

	wxLogInfo(wxT("AMBE Dongle type: %d, device: %s, address: %s:%u"), int(type), device.c_str(), address.c_str(), port);

	delete profile;
}

void CDummyRepeaterApp::getNetwork(wxString& gwyAddress, unsigned int& gwyPort, wxString& localAddress, unsigned int& localPort) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Read(KEY_GATEWAY_ADDRESS, &gwyAddress, DEFAULT_GATEWAY_ADDRESS);

	long temp;
	profile->Read(KEY_GATEWAY_PORT,    &temp,       DEFAULT_GATEWAY_PORT);
	gwyPort = (unsigned int)temp;

	profile->Read(KEY_LOCAL_ADDRESS, &localAddress, DEFAULT_LOCAL_ADDRESS);

	profile->Read(KEY_LOCAL_PORT,    &temp,         DEFAULT_LOCAL_PORT);
	localPort = (unsigned int)temp;

	wxLogInfo(wxT("Gateway set to %s:%u, local set to %s:%u"), gwyAddress.c_str(), gwyPort, localAddress.c_str(), localPort);

	delete profile;
}

void CDummyRepeaterApp::setNetwork(const wxString& gwyAddress, unsigned int gwyPort, const wxString& localAddress, unsigned int localPort) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Write(KEY_GATEWAY_ADDRESS, gwyAddress);
	profile->Write(KEY_GATEWAY_PORT,    long(gwyPort));
	profile->Write(KEY_LOCAL_ADDRESS,   localAddress);
	profile->Write(KEY_LOCAL_PORT,      long(localPort));

	wxLogInfo(wxT("Gateway set to %s:%u, local set to %s:%u"), gwyAddress.c_str(), gwyPort, localAddress.c_str(), localPort);

	delete profile;
}

void CDummyRepeaterApp::getController(wxString& type, unsigned int& config, bool& pttInvert, bool& squelchInvert) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Read(KEY_IFACE_TYPE,     &type,          DEFAULT_IFACE_TYPE);

	long temp;
	profile->Read(KEY_IFACE_CONFIG,   &temp,          DEFAULT_IFACE_CONFIG);
	config = (unsigned int)temp;

	profile->Read(KEY_PTT_INVERT,     &pttInvert,     DEFAULT_PTT_INVERT);
	profile->Read(KEY_SQUELCH_INVERT, &squelchInvert, DEFAULT_SQUELCH_INVERT);

	wxLogInfo(wxT("Controller set to %s, config: %u, PTT Inversion set to %d, Squelch Inversion set to %d"), type.c_str(), config, pttInvert, squelchInvert);

	delete profile;
}

void CDummyRepeaterApp::setController(const wxString& type, unsigned int config, bool pttInvert, bool squelchInvert) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Write(KEY_IFACE_TYPE,     type);
	profile->Write(KEY_IFACE_CONFIG,   long(config));
	profile->Write(KEY_PTT_INVERT,     pttInvert);
	profile->Write(KEY_SQUELCH_INVERT, squelchInvert);
	profile->Flush();

	wxLogInfo(wxT("Controller set to %s, config: %u, PTT Inversion set to %d, Squelch Inversion set to %d"), type.c_str(), config, pttInvert, squelchInvert);

	delete profile;
}

void CDummyRepeaterApp::getYourCalls(wxString& call, wxSortedArrayString& list) const
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

void CDummyRepeaterApp::setYourCalls(const wxString& call, const wxSortedArrayString& list) const
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

void CDummyRepeaterApp::getRpt1Calls(wxString& call, wxSortedArrayString& list) const
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

void CDummyRepeaterApp::setRpt1Calls(const wxString& call, const wxSortedArrayString& list) const
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

void CDummyRepeaterApp::getRpt2Calls(wxString& call, wxSortedArrayString& list) const
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

void CDummyRepeaterApp::setRpt2Calls(const wxString& call, const wxSortedArrayString& list) const
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

void CDummyRepeaterApp::getBleep(bool& on) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Read(KEY_BLEEP, &on, DEFAULT_BLEEP);

	wxLogInfo(wxT("End bleep set to %d"), int(on));

	delete profile;
}

void CDummyRepeaterApp::setBleep(bool on) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Write(KEY_BLEEP, on);
	profile->Flush();

	wxLogInfo(wxT("End bleep set to %d"), int(on));

	m_thread->setBleep(on);

	delete profile;
}

void CDummyRepeaterApp::getTimeout(unsigned int& timeout) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	long temp;
	profile->Read(KEY_TIMEOUT, &temp, DEFAULT_TIMEOUT);
	timeout = (unsigned int)temp;

	wxLogInfo(wxT("Timeout set to %u"), timeout);

	delete profile;
}

void CDummyRepeaterApp::setTimeout(unsigned int timeout) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Write(KEY_TIMEOUT, long(timeout));
	profile->Flush();

	wxLogInfo(wxT("Timeout set to %u"), timeout);

	delete profile;
}

void CDummyRepeaterApp::getPosition(int& x, int& y) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	long temp;
	profile->Read(KEY_WINDOW_X, &temp, DEFAULT_WINDOW_X);
	x = int(temp);

	profile->Read(KEY_WINDOW_Y, &temp, DEFAULT_WINDOW_Y);
	y = int(temp);

	delete profile;
}

void CDummyRepeaterApp::setPosition(int x, int y) const
{
	wxConfigBase* profile = new wxConfig(APPLICATION_NAME);
	wxASSERT(profile != NULL);

	profile->Write(KEY_WINDOW_X, long(x));
	profile->Write(KEY_WINDOW_Y, long(y));
	profile->Flush();

	delete profile;
}

void CDummyRepeaterApp::createThread()
{
	m_thread = new CDummyRepeaterThread;
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

	DONGLE_TYPE dongleType;
	wxString dongleDevice, dongleAddress;
	unsigned int donglePort;
	getDongle(dongleType, dongleDevice, dongleAddress, donglePort);

	CDongleThread* dongle = NULL;

	switch (dongleType) {
		case DT_DVDONGLE:
			if (!dongleDevice.IsEmpty())
				dongle = new CDVDongleThread(new CDVDongleController(dongleDevice));
			break;
		case DT_DV3000:
			if (!dongleAddress.IsEmpty() && donglePort > 0U)
				dongle = new CAMBE3000Thread(new CDV3000Controller(dongleAddress, donglePort));
			break;
		default:
			wxLogError(wxT("Invalid Dongle type specified - %d"), int(dongleType));
			error(_("Invalid Dongle type specified"));
			break;
	}

	if (dongle != NULL) {
		dongle->setEncodeCallback(m_thread);
		dongle->setDecodeCallback(m_thread);

		bool res = dongle->open();
		if (!res) {
			wxLogError(wxT("Can't find the AMBE Dongle on the port specified"));
			error(_("Can't find the AMBE Dongle on the port specified"));
		} else {
			m_thread->setDongle(dongle);
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

	wxString gwyAddress, localAddress;
	unsigned int gwyPort, localPort;
	getNetwork(gwyAddress, gwyPort, localAddress, localPort);

	if (!gwyAddress.IsEmpty()) {
		CRepeaterProtocolHandler* protocol = new CRepeaterProtocolHandler(gwyAddress, gwyPort, localAddress, localPort);

		bool res = protocol->open();
		if (!res) {
			wxLogError(wxT("Cannot open the protocol handler"));
			error(_("Cannot open the protocol handler"));
		} else {
			m_thread->setProtocol(protocol);
		}
	}

	wxString type;
	unsigned int config;
	bool pttInvert, squelchInvert;
	getController(type, config, pttInvert, squelchInvert);

	CExternalController* controller = NULL;

	wxString port;
	if (type.StartsWith(wxT("Velleman K8055 - "), &port)) {
		unsigned long num;
		port.ToULong(&num);
		controller = new CExternalController(new CK8055Controller(num), pttInvert, squelchInvert);
	} else if (type.StartsWith(wxT("URI USB - "), &port)) {
		unsigned long num;
		port.ToULong(&num);
		controller = new CExternalController(new CURIUSBController(num, true), pttInvert, squelchInvert);
	} else if (type.StartsWith(wxT("Serial - "), &port)) {
		controller = new CExternalController(new CSerialLineController(port, config), pttInvert, squelchInvert);
	} else if (type.StartsWith(wxT("Arduino - "), &port)) {
		controller = new CExternalController(new CArduinoController(port), pttInvert, squelchInvert);
#if defined(RASPBERRY_PI)
	} else if (type.IsSameAs(wxT("Raspberry Pi"))) {
		controller = new CExternalController(new CRaspberryController, pttInvert, squelchInvert);
#endif
	} else {
		controller = new CExternalController(new CDummyController, pttInvert, squelchInvert);
	}

	bool res = controller->open();
	if (!res) {
		wxLogError(wxT("Cannot open the hardware interface - %s"), type.c_str());
		error(_("Cannot open the hardware interface"));
	} else {
		m_thread->setController(controller);
	}

	bool bleep;
	getBleep(bleep);
	m_thread->setBleep(bleep);
}

