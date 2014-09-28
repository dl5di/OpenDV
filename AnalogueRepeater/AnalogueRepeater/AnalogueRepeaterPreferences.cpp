/*
 *   Copyright (C) 2009-2014 by Jonathan Naylor G4KLX
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

#include "AnalogueRepeaterPreferences.h"

const unsigned int BORDER_SIZE  = 5U;

#include <wx/notebook.h>

CAnalogueRepeaterPreferences::CAnalogueRepeaterPreferences(wxWindow* parent, int id, AUDIO_SOURCE openIdSource,
		const wxString& openId, AUDIO_SOURCE closeIdSource, const wxString& closeId, AUDIO_SOURCE beacon1Source,
		const wxString& beacon1, AUDIO_SOURCE beacon2Source, const wxString& beacon2, unsigned int idSpeed,
		unsigned int idFreq, wxFloat32 idLevel1, wxFloat32 idLevel2, AUDIO_SOURCE ackRadioSource,
		const wxString& ackRadio, AUDIO_SOURCE ackExternalSource, const wxString& ackExternal,
		AUDIO_SOURCE ackBatterySource, const wxString& ackBattery, unsigned int ackSpeed, unsigned int ackFreq,
		unsigned int ackDelay, unsigned int ackMinimum, wxFloat32 ackLevel, unsigned int callsignTime,
		unsigned int timeout, unsigned int lockoutTime, unsigned int hangTime, unsigned int latchTime,
		bool tbEnable, bool ctcssInternal, wxFloat32 tbThreshold, wxFloat32 ctcssFreq, wxFloat32 ctcssThresh,
		wxFloat32 ctcssLevel, unsigned int ctcssHangTime, ANALOGUE_CTCSS_OUTPUT ctcssOutput,
		ANALOGUE_CALLSIGN_START callsignAtStart, unsigned int callsignStartDelay, ANALOGUE_TIMEOUT_TYPE timeoutType,
		bool callsignAtEnd, ANALOGUE_CALLSIGN_HOLDOFF callsignHoldoff, const wxString& radioReadDevice,
		const wxString& radioWriteDevice, unsigned int radioAudioDelay, bool radioDeEmphasis,
		bool radioPreEmphasis, bool radioVOGAD, ANALOGUE_EXTERNAL_MODE externalMode,
		bool externalVOGAD, const wxString& externalReadDevice, const wxString& externalWriteDevice,
		unsigned int externalAudioDelay, bool externalDeEmphasis, bool externalPreEmphasis, 
		const wxString& externalDevice, SERIALPIN externalTXPin, SERIALPIN externalRXPin, bool background,
		const wxString& interfaceType, unsigned int interfaceConfig, unsigned int pttDelay,
		unsigned int squelchDelay, bool pttInvert, bool squelchInvert, bool dtmfRadio, bool dtmfExternal,
		const wxString& dtmfShutdown, const wxString& dtmfStartup, const wxString& dtmfTimeout,
		const wxString& dtmfTimeReset, const wxString& dtmfOutput1, const wxString& dtmfOutput2,
		const wxString& dtmfOutput3, const wxString& dtmfOutput4, const wxString& dtmfCommand1,
		const wxString& dtmfCommand1Line, const wxString& dtmfCommand2, const wxString& dtmfCommand2Line,
		wxFloat32 dtmfThreshold, bool aprsTxEnabled, const wxString& aprsCallsign, wxFloat32 aprsLatitude,
		wxFloat32 aprsLongitude, int aprsHeight, const wxString& aprsDescription, unsigned int activeHangTime) :
wxDialog(parent, id, wxString(_("Analogue Repeater Preferences"))),
m_callsign(NULL),
m_ack(NULL),
m_times(NULL),
m_tones(NULL),
m_feel(NULL),
m_radio(NULL),
m_external(NULL),
m_controller(NULL),
m_dtmf1(NULL),
m_dtmf2(NULL),
m_aprs(NULL),
m_active(NULL)
{
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	wxNotebook* noteBook = new wxNotebook(this, -1);

	m_callsign = new CAnalogueRepeaterCallsignSet(noteBook, -1, APPLICATION_NAME, openIdSource, openId, closeIdSource, closeId, beacon1Source, beacon1, beacon2Source, beacon2, idSpeed, idFreq, idLevel1, idLevel2);
	noteBook->AddPage(m_callsign, _("Callsign"), true);

	m_ack = new CAnalogueRepeaterAckSet(noteBook, -1, APPLICATION_NAME, ackRadioSource, ackRadio, ackExternalSource, ackExternal, ackBatterySource, ackBattery, ackSpeed, ackFreq, ackLevel, ackDelay, ackMinimum);
	noteBook->AddPage(m_ack, _("Acknowldegements"), false);

	m_times = new CAnalogueRepeaterTimesSet(noteBook, -1, APPLICATION_NAME, callsignTime, timeout, lockoutTime, hangTime, latchTime);
	noteBook->AddPage(m_times, _("Timers"), false);

	m_tones = new CAnalogueRepeaterTonesSet(noteBook, -1, APPLICATION_NAME, tbEnable, tbThreshold, ctcssFreq, ctcssInternal, ctcssThresh, ctcssLevel, ctcssHangTime, ctcssOutput);
	noteBook->AddPage(m_tones, _("Access Tones"), false);

	m_feel = new CAnalogueRepeaterFeelSet(noteBook, -1, APPLICATION_NAME, callsignAtStart, callsignStartDelay, callsignAtEnd, timeoutType, callsignHoldoff);
	noteBook->AddPage(m_feel, _("Feel"), false);

	m_radio = new CAnalogueRepeaterRadioSet(noteBook, -1, APPLICATION_NAME, radioReadDevice, radioWriteDevice, radioAudioDelay, radioDeEmphasis, radioPreEmphasis, radioVOGAD);
	noteBook->AddPage(m_radio, _("Radio"), false);

	m_external = new CAnalogueRepeaterExternalSet(noteBook, -1, APPLICATION_NAME, externalMode, externalReadDevice, externalWriteDevice, externalAudioDelay, externalDeEmphasis, externalPreEmphasis, externalVOGAD, externalDevice, externalTXPin, externalRXPin, background);
	noteBook->AddPage(m_external, _("External"), false);

	m_controller = new CAnalogueRepeaterControllerSet(noteBook, -1, APPLICATION_NAME, interfaceType, interfaceConfig, pttDelay, squelchDelay, pttInvert, squelchInvert);
	noteBook->AddPage(m_controller, _("Controller"), false);

	m_dtmf1 = new CAnalogueRepeaterDTMF1Set(noteBook, -1, APPLICATION_NAME, dtmfRadio, dtmfExternal, dtmfShutdown, dtmfStartup, dtmfTimeout, dtmfTimeReset, dtmfOutput1, dtmfOutput2, dtmfOutput3, dtmfOutput4, dtmfThreshold);
	noteBook->AddPage(m_dtmf1, _("DTMF Commands 1"), false);

	m_dtmf2 = new CAnalogueRepeaterDTMF2Set(noteBook, -1, APPLICATION_NAME, dtmfCommand1, dtmfCommand1Line, dtmfCommand2, dtmfCommand2Line);
	noteBook->AddPage(m_dtmf2, _("DTMF Commands 2"), false);

	m_aprs = new CAnalogueRepeaterAPRSSet(noteBook, -1, APPLICATION_NAME, aprsTxEnabled, aprsCallsign, aprsLatitude, aprsLongitude, aprsHeight, aprsDescription);
	noteBook->AddPage(m_aprs, _("APRS"), false);

	m_active = new CActiveHangSet(noteBook, -1, APPLICATION_NAME, activeHangTime);
	noteBook->AddPage(m_active, _("Active Output"), false);

	mainSizer->Add(noteBook, 1, wxALL | wxGROW, BORDER_SIZE);

	mainSizer->Add(CreateButtonSizer(wxOK | wxCANCEL), 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	SetAutoLayout(true);
	Layout();

	mainSizer->Fit(this);
	mainSizer->SetSizeHints(this);

	SetSizer(mainSizer);
}

CAnalogueRepeaterPreferences::~CAnalogueRepeaterPreferences()
{
}

bool CAnalogueRepeaterPreferences::Validate()
{
	if (!m_callsign->Validate() || !m_ack->Validate()   || !m_times->Validate()    || !m_tones->Validate() ||
		!m_feel->Validate()     || !m_radio->Validate() || !m_external->Validate() || !m_controller->Validate() ||
		!m_dtmf1->Validate()    || !m_dtmf2->Validate() || !m_active->Validate())
		return false;

	return true;
}

AUDIO_SOURCE CAnalogueRepeaterPreferences::getOpenIdSource() const
{
	return m_callsign->getOpenIdSource();
}

wxString CAnalogueRepeaterPreferences::getOpenId() const
{
	return m_callsign->getOpenId();
}

AUDIO_SOURCE CAnalogueRepeaterPreferences::getCloseIdSource() const
{
	return m_callsign->getCloseIdSource();
}

wxString CAnalogueRepeaterPreferences::getCloseId() const
{
	return m_callsign->getCloseId();
}

AUDIO_SOURCE CAnalogueRepeaterPreferences::getBeacon1Source() const
{
	return m_callsign->getBeacon1Source();
}

wxString CAnalogueRepeaterPreferences::getBeacon1() const
{
	return m_callsign->getBeacon1();
}

AUDIO_SOURCE CAnalogueRepeaterPreferences::getBeacon2Source() const
{
	return m_callsign->getBeacon2Source();
}

wxString CAnalogueRepeaterPreferences::getBeacon2() const
{
	return m_callsign->getBeacon2();
}

unsigned int CAnalogueRepeaterPreferences::getIdSpeed() const
{
	return m_callsign->getSpeed();
}

unsigned int CAnalogueRepeaterPreferences::getIdFreq() const
{
	return m_callsign->getFreq();
}

wxFloat32 CAnalogueRepeaterPreferences::getIdLevel1() const
{
	return m_callsign->getLevel1();
}

wxFloat32 CAnalogueRepeaterPreferences::getIdLevel2() const
{
	return m_callsign->getLevel2();
}

AUDIO_SOURCE CAnalogueRepeaterPreferences::getRadioAckSource() const
{
	return m_ack->getRadioAckSource();
}

wxString CAnalogueRepeaterPreferences::getRadioAck() const
{
	return m_ack->getRadioAck();
}

AUDIO_SOURCE CAnalogueRepeaterPreferences::getExternalAckSource() const
{
	return m_ack->getExternalAckSource();
}

wxString CAnalogueRepeaterPreferences::getExternalAck() const
{
	return m_ack->getExternalAck();
}

AUDIO_SOURCE CAnalogueRepeaterPreferences::getBatteryAckSource() const
{
	return m_ack->getBatteryAckSource();
}

wxString CAnalogueRepeaterPreferences::getBatteryAck() const
{
	return m_ack->getBatteryAck();
}

unsigned int CAnalogueRepeaterPreferences::getAckSpeed() const
{
	return m_ack->getSpeed();
}

unsigned int CAnalogueRepeaterPreferences::getAckFreq() const
{
	return m_ack->getFreq();
}

wxFloat32 CAnalogueRepeaterPreferences::getAckLevel() const
{
	return m_ack->getLevel();
}

unsigned int CAnalogueRepeaterPreferences::getAckDelay() const
{
	return m_ack->getDelay();
}

unsigned int CAnalogueRepeaterPreferences::getAckMinimum() const
{
	return m_ack->getMinimum();
}

unsigned int CAnalogueRepeaterPreferences::getCallsignTime() const
{
	return m_times->getCallsignTime();
}

unsigned int CAnalogueRepeaterPreferences::getTimeout() const
{
	return m_times->getTimeout();
}

unsigned int CAnalogueRepeaterPreferences::getLockoutTime() const
{
	return m_times->getLockoutTime();
}

unsigned int CAnalogueRepeaterPreferences::getHangTime() const
{
	return m_times->getHangTime();
}

unsigned int CAnalogueRepeaterPreferences::getLatchTime() const
{
	return m_times->getLatchTime();
}

bool CAnalogueRepeaterPreferences::getTbEnable() const
{
	return m_tones->getTbEnable();
}

wxFloat32 CAnalogueRepeaterPreferences::getTbThreshold() const
{
	return m_tones->getTbThreshold();
}

bool CAnalogueRepeaterPreferences::getCTCSSInternal() const
{
	return m_tones->getCTCSSInternal();
}

wxFloat32 CAnalogueRepeaterPreferences::getCTCSSFreq() const
{
	return m_tones->getCTCSSFreq();
}

wxFloat32 CAnalogueRepeaterPreferences::getCTCSSThresh() const
{
	return m_tones->getCTCSSThresh();
}

wxFloat32 CAnalogueRepeaterPreferences::getCTCSSLevel() const
{
	return m_tones->getCTCSSLevel();
}

unsigned int CAnalogueRepeaterPreferences::getCTCSSHangTime() const
{
	return m_tones->getCTCSSHangTime();
}

ANALOGUE_CTCSS_OUTPUT CAnalogueRepeaterPreferences::getCTCSSOutput() const
{
	return m_tones->getCTCSSOutput();
}

ANALOGUE_CALLSIGN_START CAnalogueRepeaterPreferences::getCallAtStart() const
{
	return m_feel->getCallAtStart();
}

unsigned int CAnalogueRepeaterPreferences::getCallStartDelay() const
{
	return m_feel->getStartDelay();
}

bool CAnalogueRepeaterPreferences::getCallAtEnd() const
{
	return m_feel->getCallAtEnd();
}

ANALOGUE_TIMEOUT_TYPE CAnalogueRepeaterPreferences::getTimeoutType() const
{
	return m_feel->getTimeoutType();
}

ANALOGUE_CALLSIGN_HOLDOFF  CAnalogueRepeaterPreferences::getHoldoff() const
{
	return m_feel->getHoldoff();
}

wxString CAnalogueRepeaterPreferences::getRadioReadDevice() const
{
	return m_radio->getReadDevice();
}

wxString CAnalogueRepeaterPreferences::getRadioWriteDevice() const
{
	return m_radio->getWriteDevice();
}

unsigned int CAnalogueRepeaterPreferences::getRadioAudioDelay() const
{
	return m_radio->getAudioDelay();
}

bool CAnalogueRepeaterPreferences::getRadioDeEmphasis() const
{
	return m_radio->getDeEmphasis();
}

bool CAnalogueRepeaterPreferences::getRadioPreEmphasis() const
{
	return m_radio->getPreEmphasis();
}

bool CAnalogueRepeaterPreferences::getRadioVOGAD() const
{
	return m_radio->getVOGAD();
}

ANALOGUE_EXTERNAL_MODE CAnalogueRepeaterPreferences::getExternalMode() const
{
	 return m_external->getMode();
}

wxString CAnalogueRepeaterPreferences::getExternalReadDevice() const
{
	return m_external->getReadDevice();
}

wxString CAnalogueRepeaterPreferences::getExternalWriteDevice() const
{
	return m_external->getWriteDevice();
}

unsigned int CAnalogueRepeaterPreferences::getExternalAudioDelay() const
{
	return m_external->getAudioDelay();
}

bool CAnalogueRepeaterPreferences::getExternalDeEmphasis() const
{
	return m_external->getDeEmphasis();
}

bool CAnalogueRepeaterPreferences::getExternalPreEmphasis() const
{
	return m_external->getPreEmphasis();
}

bool CAnalogueRepeaterPreferences::getExternalVOGAD() const
{
	return m_external->getVOGAD();
}

wxString CAnalogueRepeaterPreferences::getExternalDevice() const
{
	return m_external->getDevice();
}

SERIALPIN CAnalogueRepeaterPreferences::getExternalTXPin() const
{
	return m_external->getTXPin();
}

SERIALPIN CAnalogueRepeaterPreferences::getExternalRXPin() const
{
	return m_external->getRXPin();
}

bool CAnalogueRepeaterPreferences::getExternalBackground() const
{
	return m_external->getBackground();
}

wxString CAnalogueRepeaterPreferences::getInterfaceType() const
{
	return m_controller->getType();
}

unsigned int CAnalogueRepeaterPreferences::getInterfaceConfig() const
{
	return m_controller->getConfig();
}

unsigned int CAnalogueRepeaterPreferences::getPTTDelay() const
{
	return m_controller->getPTTDelay();
}

unsigned int CAnalogueRepeaterPreferences::getSquelchDelay() const
{
	return m_controller->getSquelchDelay();
}

bool CAnalogueRepeaterPreferences::getPTTInvert() const
{
	return m_controller->getPTTInvert();
}

bool CAnalogueRepeaterPreferences::getSquelchInvert() const
{
	return m_controller->getSquelchInvert();
}

bool CAnalogueRepeaterPreferences::getDTMFRadio() const
{
	return m_dtmf1->getRadio();
}

bool CAnalogueRepeaterPreferences::getDTMFExternal() const
{
	return m_dtmf1->getExternal();
}

wxString CAnalogueRepeaterPreferences::getDTMFShutdown() const
{
	return m_dtmf1->getShutdown();
}

wxString CAnalogueRepeaterPreferences::getDTMFStartup() const
{
	return m_dtmf1->getStartup();
}

wxString CAnalogueRepeaterPreferences::getDTMFTimeout() const
{
	return m_dtmf1->getTimeout();
}

wxString CAnalogueRepeaterPreferences::getDTMFTimeReset() const
{
	return m_dtmf1->getTimeReset();
}

wxString CAnalogueRepeaterPreferences::getDTMFCommand1() const
{
	return m_dtmf2->getCommand1();
}

wxString CAnalogueRepeaterPreferences::getDTMFCommand1Line() const
{
	return m_dtmf2->getCommand1Line();
}

wxString CAnalogueRepeaterPreferences::getDTMFCommand2() const
{
	return m_dtmf2->getCommand2();
}

wxString CAnalogueRepeaterPreferences::getDTMFCommand2Line() const
{
	return m_dtmf2->getCommand2Line();
}

wxString CAnalogueRepeaterPreferences::getDTMFOutput1() const
{
	return m_dtmf1->getOutput1();
}

wxString CAnalogueRepeaterPreferences::getDTMFOutput2() const
{
	return m_dtmf1->getOutput2();
}

wxString CAnalogueRepeaterPreferences::getDTMFOutput3() const
{
	return m_dtmf1->getOutput3();
}

wxString CAnalogueRepeaterPreferences::getDTMFOutput4() const
{
	return m_dtmf1->getOutput4();
}

wxFloat32 CAnalogueRepeaterPreferences::getDTMFThreshold() const
{
	return m_dtmf1->getThreshold();
}

bool CAnalogueRepeaterPreferences::getAPRSTXEnabled() const
{
	return m_aprs->getTXEnabled();
}

wxString CAnalogueRepeaterPreferences::getAPRSCallsign() const
{
	return m_aprs->getCallsign();
}

wxFloat32 CAnalogueRepeaterPreferences::getAPRSLatitude() const
{
	return m_aprs->getLatitude();
}

wxFloat32 CAnalogueRepeaterPreferences::getAPRSLongitude() const
{
	return m_aprs->getLongitude();
}

int CAnalogueRepeaterPreferences::getAPRSHeight() const
{
	return m_aprs->getHeight();
}

wxString CAnalogueRepeaterPreferences::getAPRSDescription() const
{
	return m_aprs->getDescription();
}

unsigned int CAnalogueRepeaterPreferences::getActiveHangTime() const
{
	return m_active->getTime();
}
