/*
 *   Copyright (C) 2009-2015 by Jonathan Naylor G4KLX
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

#include "AnalogueRepeaterThread.h"
#include "AnalogueRepeaterApp.h"
#include "FirstOrderIIR.h"
#include "VOGAD.h"

#include <cmath>

// A de-emphasis filter with 6dB drop off per octave
const wxFloat32 IIRDE_A1   = -0.97260F;
const wxFloat32 IIRDE_B0   = 0.013699F;
const wxFloat32 IIRDE_B1   = 0.013699F;
const wxFloat32 IIRDE_GAIN = 4.805613F;

// A pre-emphasis filter with 6dB gain per octave
const wxFloat32 IIRPE_A1   = 0.97260F;
const wxFloat32 IIRPE_B0   = 0.98630F;
const wxFloat32 IIRPE_B1   = -0.98630F;
const wxFloat32 IIRPE_GAIN = 15.31699F;

// A band pass filter created with
// b = fir1(200, [ 300 / 24000 3000 / 24000 ]);

const unsigned int FIRLP_LEN    = 201U;
const wxFloat32    FIRLP_TAPS[] = {
	 4.348089722683099e-004F,  4.135042495540001e-004F,  3.565734316721585e-004F,
	 2.697620683049211e-004F,  1.630208019098757e-004F,  4.963421717875377e-005F,
	-5.511664728362165e-005F, -1.357146691842085e-004F, -1.784720386611927e-004F,
	-1.736941634835989e-004F, -1.176523500371192e-004F, -1.402211853398311e-005F,
	 1.255561999118871e-004F,  2.820687925363604e-004F,  4.310038699806798e-004F,
	 5.454413649542267e-004F,  6.000529950196235e-004F,  5.754846676938578e-004F,
	 4.624257460107961e-004F,  2.646027935871826e-004F, -8.998878026856994e-019F,
	-3.001857568578321e-004F, -5.950169554549807e-004F, -8.394648920034268e-004F,
	-9.915596586982927e-004F, -1.019998307501092e-003F, -9.109416298854615e-004F,
	-6.727015058714779e-004F, -3.372223785104403e-004F,  4.231523099903672e-005F,
	 3.978432840308519e-004F,  6.560960944864116e-004F,  7.503714920446452e-004F,
	 6.325803609781686e-004F,  2.835429460363310e-004F, -2.804399678111389e-004F,
	-1.006291058992636e-003F, -1.808947908432476e-003F, -2.582095951203979e-003F,
	-3.213662476370489e-003F, -3.603952054699695e-003F, -3.683632046000646e-003F,
	-3.428514864134490e-003F, -2.868316566104280e-003F, -2.087306999884778e-003F,
	-1.215927593116136e-003F, -4.138814968899417e-004F,  1.533200345187720e-004F,
	 3.410866837681884e-004F,  5.162936712129394e-005F, -7.453774810594479e-004F,
	-2.001260159459314e-003F, -3.588693388062809e-003F, -5.314977400685459e-003F,
	-6.947255754887167e-003F, -8.246635510914082e-003F, -9.006443163121124e-003F,
	-9.088753480449461e-003F, -8.453095966632742e-003F, -7.171983753681457e-003F,
	-5.429581110071774e-003F, -3.502232559034098e-003F, -1.722388887843640e-003F,
	-4.302625426330871e-004F,  8.012281697951944e-005F, -3.876344919835156e-004F,
	-1.891805835793981e-003F, -4.330331749140149e-003F, -7.442040563048283e-003F,
	-1.083314186121884e-002F, -1.402704694735273e-002F, -1.653176933584757e-002F,
	-1.791595309144118e-002F, -1.788250347606054e-002F, -1.632825233260431e-002F,
	-1.337927103416548e-002F, -9.394287349331992e-003F, -4.932853682002187e-003F,
	-6.899140995522271e-004F,  2.596466785959089e-003F,  4.252970468674810e-003F,
	 3.780101653662344e-003F,  9.468283669403368e-004F, -4.143948225037351e-003F,
	-1.103074294656373e-002F, -1.891637020172199e-002F, -2.673890106758282e-002F,
	-3.328462589955223e-002F, -3.733082798472451e-002F, -3.780140989137119e-002F,
	-3.391573795286361e-002F, -2.531094064719834e-002F, -1.212043181384624e-002F,
	 5.003619980645199e-003F,  2.492938215129132e-002F,  4.614378276854996e-002F,
	 6.690036788706782e-002F,  8.540010514415053e-002F,  9.998405763108859e-002F,
	 1.093149075489733e-001F,  1.125253099317730e-001F,  1.093149075489733e-001F,
	 9.998405763108859e-002F,  8.540010514415053e-002F,  6.690036788706782e-002F,
	 4.614378276854996e-002F,  2.492938215129132e-002F,  5.003619980645199e-003F,
	-1.212043181384624e-002F, -2.531094064719834e-002F, -3.391573795286361e-002F,
	-3.780140989137119e-002F, -3.733082798472451e-002F, -3.328462589955223e-002F,
	-2.673890106758282e-002F, -1.891637020172199e-002F, -1.103074294656373e-002F,
	-4.143948225037351e-003F,  9.468283669403368e-004F,  3.780101653662344e-003F,
	 4.252970468674810e-003F,  2.596466785959089e-003F, -6.899140995522271e-004F,
	-4.932853682002187e-003F, -9.394287349331992e-003F, -1.337927103416548e-002F,
	-1.632825233260431e-002F, -1.788250347606054e-002F, -1.791595309144118e-002F,
	-1.653176933584757e-002F, -1.402704694735273e-002F, -1.083314186121884e-002F,
	-7.442040563048283e-003F, -4.330331749140149e-003F, -1.891805835793981e-003F,
	-3.876344919835156e-004F,  8.012281697951944e-005F, -4.302625426330871e-004F,
	-1.722388887843640e-003F, -3.502232559034098e-003F, -5.429581110071774e-003F,
	-7.171983753681457e-003F, -8.453095966632742e-003F, -9.088753480449461e-003F,
	-9.006443163121124e-003F, -8.246635510914082e-003F, -6.947255754887167e-003F,
	-5.314977400685459e-003F, -3.588693388062809e-003F, -2.001260159459314e-003F,
	-7.453774810594479e-004F,  5.162936712129394e-005F,  3.410866837681884e-004F,
	 1.533200345187720e-004F, -4.138814968899417e-004F, -1.215927593116136e-003F,
	-2.087306999884778e-003F, -2.868316566104280e-003F, -3.428514864134490e-003F,
	-3.683632046000646e-003F, -3.603952054699695e-003F, -3.213662476370489e-003F,
	-2.582095951203979e-003F, -1.808947908432476e-003F, -1.006291058992636e-003F,
	-2.804399678111389e-004F,  2.835429460363310e-004F,  6.325803609781686e-004F,
	 7.503714920446452e-004F,  6.560960944864116e-004F,  3.978432840308519e-004F,
	 4.231523099903672e-005F, -3.372223785104403e-004F, -6.727015058714779e-004F,
	-9.109416298854615e-004F, -1.019998307501092e-003F, -9.915596586982927e-004F,
	-8.394648920034268e-004F, -5.950169554549807e-004F, -3.001857568578321e-004F,
	-8.998878026856994e-019F,  2.646027935871826e-004F,  4.624257460107961e-004F,
	 5.754846676938578e-004F,  6.000529950196235e-004F,  5.454413649542267e-004F,
	 4.310038699806798e-004F,  2.820687925363604e-004F,  1.255561999118871e-004F,
	-1.402211853398311e-005F, -1.176523500371192e-004F, -1.736941634835989e-004F,
	-1.784720386611927e-004F, -1.357146691842085e-004F, -5.511664728362165e-005F,
	 4.963421717875377e-005F,  1.630208019098757e-004F,  2.697620683049211e-004F,
	 3.565734316721585e-004F,  4.135042495540001e-004F,  4.348089722683099e-004F};


CAnalogueRepeaterThread::CAnalogueRepeaterThread() :
m_radioSoundcard(NULL),
m_extSoundcard(NULL),
m_controller(NULL),
m_extController(NULL),
m_extMode(AEM_DISABLED),
m_pttDelay(NULL),
m_squelch1Delay(NULL),
m_squelch2Delay(NULL),
m_stopped(true),
m_battery(false),
m_state(ARS_LISTENING),
m_radioInBuffer(ANALOGUE_RADIO_BLOCK_SIZE * 5U),
m_radioOutBuffer(ANALOGUE_RADIO_BLOCK_SIZE * 5U),
m_extInBuffer(ANALOGUE_RADIO_BLOCK_SIZE * 5U),
m_extOutBuffer(ANALOGUE_RADIO_BLOCK_SIZE * 5U),
m_1750In(NULL),
m_ctcssIn(NULL),
m_ctcssInternal(true),
m_ctcssOut(NULL),
m_ctcssOutput(ACO_WHEN_OPEN),
m_timeoutTones(NULL),
m_openId(NULL),
m_closeId(NULL),
m_beacon1(NULL),
m_beacon2(NULL),
m_radioAck(NULL),
m_extAck(NULL),
m_batteryAck(NULL),
m_timeoutTimer(1000U),
m_lockoutTimer(1000U),
m_callsignTimer(1000U),
m_callsignStartTimer(1000U),
m_hangTimer(1000U),
m_latchTimer(1000U),
m_minimumTimer(1000U),
m_ackTimer(1000U),
m_idLevel1(0.0F),
m_idLevel2(0.0F),
m_ackLevel(0.0F),
m_ctcssLevel(0.0F),
m_radioAudioDelay(NULL),
m_radioDeEmphasis(NULL),
m_radioPreEmphasis(NULL),
m_audioFilter(FIRLP_TAPS, FIRLP_LEN),
m_audioLevel(0.0F),
m_radioAudioVOGAD(0.0F, MAX_AUDIO_LEVEL),
m_extAudioVOGAD(0.0F, MAX_AUDIO_LEVEL),
m_extAudioDelay(NULL),
m_extDeEmphasis(NULL),
m_extPreEmphasis(NULL),
m_extBackground(false),
m_callsignAtStart(ACS_NONE),
m_callsignAtEnd(false),
m_callsignHoldoff(ACH_NONE),
m_squelch(AS_CLOSED),
m_radioTransmit(false),
m_extTransmit(false),
m_firstTime(false),
m_sendOpen(false),
m_sendClose(false),
m_sendBeacon1(false),
m_sendBeacon2(false),
m_sendAck(ACK_NONE),
m_radioDTMF(NULL),
m_extDTMF(NULL),
m_output1(false),
m_output2(false),
m_output3(false),
m_output4(false),
m_reader(NULL),
m_killed(false),
m_command1Line(),
m_command2Line(),
m_activeHangTimer(1000U),
m_ctcssHangTimer(1000U),
m_shutdown(false),
m_radioCount(0U),
m_externalCount(0U),
m_relayCount(0U),
m_transmitCount(0U),
m_timeCount(0U),
m_lastHour(0U)
{
}

CAnalogueRepeaterThread::~CAnalogueRepeaterThread()
{
}

void CAnalogueRepeaterThread::run()
{
	// Wait here until we have the essentials to run
	while (!m_killed && (m_radioSoundcard == NULL || m_openId == NULL || m_closeId == NULL || m_beacon1 == NULL || m_beacon2 == NULL || m_radioAck == NULL || m_extAck == NULL || m_batteryAck == NULL || m_timeoutTones == NULL || m_controller == NULL))
		::wxMilliSleep(500UL);		// 1/2 sec

	if (m_killed)
		return;

	m_stopped = false;

	m_callsignTimer.start();
	m_controller->setExternalTransmit(false);
	m_controller->setRadioTransmit(false);
	m_controller->setActive(false);

	m_extInBuffer.clear();
	m_radioInBuffer.clear();

	wxDateTime dateTime = wxDateTime::Now();
	m_lastHour = dateTime.GetHour();

	wxLogMessage(wxT("Starting the analogue repeater thread"));

	unsigned int counter = 0U;

	wxStopWatch timer;

	ANALOGUE_SQUELCH radioSquelch = AS_CLOSED;
	ANALOGUE_SQUELCH extSquelch = AS_CLOSED;

	unsigned int nRadio = 0U;
	unsigned int nExternal = 0U;

	try {
		while (!m_killed) {
			timer.Start();

			wxFloat32 radioAudio[ANALOGUE_RADIO_BLOCK_SIZE];
			wxFloat32 extAudio[ANALOGUE_RADIO_BLOCK_SIZE];

			getAudio(radioAudio, extAudio, nRadio, nExternal);

			m_battery = m_controller->getBattery();

			// Perform de-emphasis if required
			if (m_radioDeEmphasis != NULL && nRadio > 0U)
				m_radioDeEmphasis->process(radioAudio, nRadio);
			if (m_extDeEmphasis != NULL && nExternal > 0U)
				m_extDeEmphasis->process(extAudio, nExternal);

			// Handle DTMF commands
			processDTMF(radioAudio, extAudio, nRadio, nExternal);

			// Set the watchdog port every one second
			counter++;
			if (counter == 50U) {
				m_controller->setHeartbeat();
				counter = 0U;
			}

			// Check the shutdown state
			bool disable = m_controller->getDisable();
			if (disable || m_shutdown) {
				if (m_state != ARS_SHUTDOWN)
					setState(ARS_SHUTDOWN);
			} else {
				if (m_state == ARS_SHUTDOWN)
					setState(ARS_LISTENING);
			}

			// If we're shutdown then don't go any further
			if (m_state == ARS_SHUTDOWN)
				continue;

			// Set the output state
			if (m_state != ARS_LISTENING || m_sendBeacon2 || m_sendOpen || m_sendClose || m_sendBeacon1 || m_callsignStartTimer.isRunning() || (m_activeHangTimer.isRunning() && !m_activeHangTimer.hasExpired())) {
				m_controller->setActive(true);
			} else {
				m_controller->setActive(false);
				m_activeHangTimer.stop();
			}

			// Detect a suitable access signal, 1750Hz, CTCSS, or carrier
			if (nRadio > 0U)
				radioSquelch = checkRadioSquelch(radioAudio, nRadio, radioSquelch);

			// Detect an incoming external transmission
			extSquelch = checkExternalSquelch();

			// Choose the correct squelch value
			if (m_state == ARS_RELAYING_EXTERNAL) {
				// If already receiving from the external source, give it priority
				if (extSquelch != AS_CLOSED)
					m_squelch = extSquelch;
				else
					m_squelch = radioSquelch;
			} else {
				// If already receiving from the radio, or none, give radio the priority
				if (radioSquelch != AS_CLOSED)
					m_squelch = radioSquelch;
				else
					m_squelch = extSquelch;
			}

			stateMachine();

			m_radioTransmit = m_state == ARS_RELAYING_RADIO  || m_state == ARS_RELAYING_EXTERNAL ||
							  m_state == ARS_WAITING_RADIO   || m_state == ARS_WAITING_EXTERNAL  ||
							  m_state == ARS_TIMEOUT_RADIO   || m_state == ARS_TIMEOUT_EXTERNAL  ||
							  m_sendBeacon1 || m_sendBeacon2 || m_sendOpen || m_sendClose || m_callsignStartTimer.isRunning();

			// The audio is chosen depending on the squelch and state
			unsigned int nAudio;
			wxFloat32 audio[ANALOGUE_RADIO_BLOCK_SIZE];
			if (m_state == ARS_RELAYING_RADIO && m_squelch != AS_CLOSED) {
				nAudio = nRadio;
				m_radioAudioVOGAD.process(radioAudio, audio, nAudio);
			} else if (m_state == ARS_RELAYING_EXTERNAL && m_squelch != AS_CLOSED) {
				nAudio = nExternal;
				m_extAudioVOGAD.process(extAudio, audio, nAudio);
			} else {
				// No open squelch, silence
				nAudio = ANALOGUE_RADIO_BLOCK_SIZE;
				::memset(audio, 0x00, ANALOGUE_RADIO_BLOCK_SIZE * sizeof(wxFloat32));
			}

			calculateAudioLevel(audio, nAudio);

			// XXX Pre-Emphasis

			if (m_state == ARS_RELAYING_RADIO) {
				// Key the external transmitter and send audio to the external program before tones are added
				if (m_extPreEmphasis != NULL) {
					wxFloat32 out[ANALOGUE_RADIO_BLOCK_SIZE];
					m_extPreEmphasis->process(audio, out, nAudio);
					feedExternal(out, nAudio, true);
					m_extTransmit = true;
				} else {
					feedExternal(audio, nAudio, true);
					m_extTransmit = true;
				}
			} else if (m_extBackground && m_state == ARS_RELAYING_EXTERNAL) {
				if (nRadio > 0U) {
					if (radioSquelch != AS_CLOSED) {
						// Special case, relay radio audio to the external port when relaying from the external port
						wxFloat32 audio[ANALOGUE_RADIO_BLOCK_SIZE];
						m_radioAudioVOGAD.process(radioAudio, audio, nRadio);
						if (m_extPreEmphasis != NULL)
							m_extPreEmphasis->process(audio, nRadio);
						feedExternal(audio, nRadio, true);
						m_extTransmit = true;
					} else {
						feedExternal(audio, nAudio, false);
						m_extTransmit = false;
					}
				}
			} else {
				feedExternal(audio, nAudio, false);
				m_extTransmit = false;
			}

			// From here onwards, the audio is destined for the radio only

			// Add tones
			sendTones(audio, nAudio);

			// Filter the audio
			filterAudio(audio, nAudio);

			// Perform pre-emphasis if required
			if (m_radioPreEmphasis != NULL && m_radioTransmit && nAudio > 0U)
				m_radioPreEmphasis->process(audio, nAudio);

			// Insert CTCSS here because of the role off in the pre-emphasis filter
			insertCTCSS(audio, nAudio);

			// Set the transmitter, and provide some audio
			feedRadio(audio, nAudio);

			getStatistics();

			unsigned int ms = timer.Time();
			clock(ms);
		}
	}
	catch (std::exception& e) {
		wxString message(e.what(), wxConvLocal);
		wxLogError(wxT("Exception raised - \"%s\""), message.c_str());
	}
	catch (...) {
		wxLogError(wxT("Unknown exception raised"));
	}

	writeStatistics();

	wxLogMessage(wxT("Stopping the analogue repeater thread"));

#if defined(__WXDEBUG__)
	if (m_reader != NULL) {
		m_reader->close();
		delete m_reader;
	}
#endif

	m_radioSoundcard->close();

	if (m_extSoundcard != NULL)
		m_extSoundcard->close();

	if (m_extController != NULL)
		m_extController->close();

	m_controller->setExternalTransmit(false);
	m_controller->setRadioTransmit(false);
	m_controller->setActive(false);
	m_controller->close();

	delete m_radioDTMF;
	delete m_extDTMF;
	delete m_timeoutTones;
	delete m_radioSoundcard;
	delete m_extSoundcard;
	delete m_controller;
	delete m_extController;
	delete m_radioAudioDelay;
	delete m_pttDelay;
	delete m_squelch1Delay;
	delete m_squelch2Delay;
	delete m_radioDeEmphasis;
	delete m_radioPreEmphasis;
	delete m_extAudioDelay;
	delete m_extDeEmphasis;
	delete m_extPreEmphasis;
	delete m_1750In;
	delete m_ctcssIn;
	delete m_ctcssOut;
	delete m_openId;
	delete m_closeId;
	delete m_beacon1;
	delete m_beacon2;
	delete m_radioAck;
	delete m_extAck;
	delete m_batteryAck;
}

void CAnalogueRepeaterThread::kill()
{
	m_killed = true;
}

void CAnalogueRepeaterThread::setCallsign(IFixedAudioSource* openId, IFixedAudioSource* closeId, IFixedAudioSource* beacon1, IFixedAudioSource* beacon2, wxFloat32 level1, wxFloat32 level2)
{
	m_openId  = openId;
	m_closeId = closeId;
	m_beacon1 = beacon1;
	m_beacon2 = beacon2;

	m_idLevel1 = level1 * MAX_AUDIO_LEVEL / 1.0F;		// Max of 100% of full deviation
	m_idLevel2 = level2 * MAX_AUDIO_LEVEL / 2.0F;		// Max of 50% of full deviation
}

void CAnalogueRepeaterThread::setAck(IFixedAudioSource* radioAck, IFixedAudioSource* extAck, IFixedAudioSource* batteryAck, wxFloat32 level, unsigned int ack, unsigned int minimum)
{
	m_radioAck   = radioAck;
	m_extAck     = extAck;
	m_batteryAck = batteryAck;

	// Old-style acks are in seconds, convert to ms
	if (ack < 5U)
		ack *= 1000U;

	// Old-style minimum times are in seconds, convert to ms
	if (minimum < 11U)
		minimum *= 1000U;

	// Add the ack delay to the minimum time, if it's set
	if (minimum > 0U)
		minimum += ack;

	m_ackTimer.setTimeout(0U, ack);
	m_minimumTimer.setTimeout(0U, minimum);
	m_ackLevel = level * MAX_AUDIO_LEVEL / 1.0F;			// Max of 100% of full deviation
}

void CAnalogueRepeaterThread::setTimes(unsigned int callsignTime, unsigned int timeout, unsigned int lockoutTime, unsigned int hangTime, unsigned int& latchTime)
{
	m_callsignTimer.setTimeout(callsignTime);
	m_timeoutTimer.setTimeout(timeout);
	m_lockoutTimer.setTimeout(lockoutTime);
	m_hangTimer.setTimeout(hangTime);
	m_latchTimer.setTimeout(latchTime);
}

void CAnalogueRepeaterThread::setTones(bool toneburst, wxFloat32 tbThreshold, wxFloat32 ctcssFreq, bool ctcssInternal, wxFloat32 ctcssThresh, wxFloat32 ctcssLevel, unsigned int ctcssHangTime, ANALOGUE_CTCSS_OUTPUT ctcssOutput)
{
	tbThreshold *= 10000.0F;			// Based on testing by Jon, G4TSN
	ctcssThresh *= 500000.0F;			// Based on testing by Jon, G4TSN

	m_ctcssInternal = ctcssInternal;
	m_ctcssOutput   = ctcssOutput;

	if (toneburst)
		m_1750In = new CGoertzel(ANALOGUE_RADIO_SAMPLE_RATE, 1750.0F, ANALOGUE_RADIO_SAMPLE_RATE / 25U, tbThreshold);	// 25 Hz B/W

	if (ctcssFreq != 0.0F && ctcssInternal)
		m_ctcssIn = new CGoertzel(ANALOGUE_RADIO_SAMPLE_RATE, ctcssFreq, ANALOGUE_RADIO_SAMPLE_RATE / 5U, ctcssThresh);	// 5 Hz B/W

	if (ctcssFreq != 0.0F && ctcssOutput != ACO_NONE)
		m_ctcssOut = new CNCO(ANALOGUE_RADIO_SAMPLE_RATE, ctcssFreq);

	// CTCSS deviation in the UK for 5 kHz total deviation is 500 Hz +-200 Hz
	m_ctcssLevel = ctcssLevel * MAX_AUDIO_LEVEL / 5.0F;			// Max of 20% of full deviation

	m_ctcssHangTimer.setTimeout(0U, ctcssHangTime * 20U);
}

void CAnalogueRepeaterThread::setFeel(ANALOGUE_CALLSIGN_START callsignAtStart, unsigned int callsignStartDelay, bool callsignAtEnd, ANALOGUE_TIMEOUT_TYPE timeoutType, ANALOGUE_CALLSIGN_HOLDOFF callsignHoldoff)
{
	m_callsignAtStart = callsignAtStart;
	m_callsignAtEnd   = callsignAtEnd;
	m_callsignHoldoff = callsignHoldoff;

	m_callsignStartTimer.setTimeout(callsignStartDelay);

	m_timeoutTones = new CTimeoutTones(ANALOGUE_RADIO_SAMPLE_RATE, timeoutType);
}

void CAnalogueRepeaterThread::setRadio(CSoundCardReaderWriter* soundcard, unsigned int audioDelay, bool deEmphasis, bool preEmphasis, bool vogad)
{
	wxASSERT(soundcard != NULL);

	m_radioSoundcard = soundcard;

	if (audioDelay > 0U)
		m_radioAudioDelay = new CAudioDelay(audioDelay * ANALOGUE_FRAME_TIME_MS * ANALOGUE_RADIO_SAMPLE_RATE / 1000U);

	if (deEmphasis)
		m_radioDeEmphasis = new CFirstOrderIIR(IIRDE_A1, IIRDE_B0, IIRDE_B1, IIRDE_GAIN);

	if (preEmphasis)
		m_radioPreEmphasis = new CFirstOrderIIR(IIRPE_A1, IIRPE_B0, IIRPE_B1, IIRPE_GAIN);

	if (vogad)
		m_radioAudioVOGAD.setMaxGain(MAX_VOGAD_GAIN);
}

void CAnalogueRepeaterThread::setExternal(ANALOGUE_EXTERNAL_MODE mode, CSoundCardReaderWriter* soundcard, unsigned int audioDelay, bool deEmphasis, bool preEmphasis, bool vogad, CNetworkController* controller, bool background)
{
	wxASSERT(soundcard != NULL);
	wxASSERT(controller != NULL);

	m_extMode       = mode;
	m_extSoundcard  = soundcard;
	m_extController = controller;
	m_extBackground = background;

	if (audioDelay > 0U)
		m_extAudioDelay = new CAudioDelay(audioDelay * ANALOGUE_FRAME_TIME_MS * ANALOGUE_RADIO_SAMPLE_RATE / 1000U);

	if (deEmphasis)
		m_extDeEmphasis = new CFirstOrderIIR(IIRDE_A1, IIRDE_B0, IIRDE_B1, IIRDE_GAIN);

	if (preEmphasis)
		m_extPreEmphasis = new CFirstOrderIIR(IIRPE_A1, IIRPE_B0, IIRPE_B1, IIRPE_GAIN);

	if (vogad)
		m_extAudioVOGAD.setMaxGain(MAX_VOGAD_GAIN);
}

void CAnalogueRepeaterThread::setController(CExternalController* controller, unsigned int pttDelay, unsigned int squelchDelay)
{
	wxASSERT(controller != NULL);

	m_controller = controller;

	if (pttDelay > 0U)
		m_pttDelay = new CPTTDelay(pttDelay);

	if (squelchDelay > 0U) {
		m_squelch1Delay = new CPTTDelay(squelchDelay);
		m_squelch2Delay = new CPTTDelay(squelchDelay);
	}
}

void CAnalogueRepeaterThread::setOutputs(bool out1, bool out2, bool out3, bool out4)
{
	if (m_controller == NULL)
		return;

	m_output1 = out1;
	m_output2 = out2;
	m_output3 = out3;
	m_output4 = out4;

	m_controller->setOutput1(m_output1);
	m_controller->setOutput2(m_output2);
	m_controller->setOutput3(m_output3);
	m_controller->setOutput4(m_output4);
}

void CAnalogueRepeaterThread::setDTMF(bool radio, bool external, const wxString& shutdown, const wxString& startup, const wxString& timeout, const wxString& timeReset, const wxString& command1, const wxString& command1Line, const wxString& command2, const wxString& command2Line, const wxString& output1, const wxString& output2, const wxString& output3, const wxString& output4, wxFloat32 threshold)
{
	threshold *= 5000.0F;		// Based on testing by Steve, G8ZMG

	m_command1Line = command1Line;
	m_command2Line = command2Line;

	if (radio)
		m_radioDTMF = new CDTMFController(ANALOGUE_RADIO_SAMPLE_RATE, threshold, shutdown, startup, timeout, timeReset, command1, command2, output1, output2, output3, output4);
	if (external)
		m_extDTMF = new CDTMFController(ANALOGUE_RADIO_SAMPLE_RATE, threshold, shutdown, startup, timeout, timeReset, command1, command2, output1, output2, output3, output4);
}

void CAnalogueRepeaterThread::setActiveHang(unsigned int time)
{
	m_activeHangTimer.setTimeout(time);
}

#if defined(__WXDEBUG__)
void CAnalogueRepeaterThread::setReader(CWAVFileReader* reader)
{
	wxASSERT(reader != NULL);

	// Already busy?
	if (m_reader != NULL) {
		reader->close();
		delete reader;
		return;
	}

	wxLogInfo(wxT("Reading from WAV file - %s"), reader->getFilename().c_str());

	m_reader = reader;
}
#endif

void CAnalogueRepeaterThread::setTbThreshold(wxFloat32 threshold)
{
	threshold *= 10000.0F;			// Based on testing by Jon, G4TSN

	if (m_1750In != NULL)
		m_1750In->setThreshold(threshold);
}

void CAnalogueRepeaterThread::setCTCSSThreshold(wxFloat32 threshold)
{
	threshold *= 500000.0F;			// Based on testing by Jon, G4TSN

	if (m_ctcssIn != NULL)
		m_ctcssIn->setThreshold(threshold);
}

void CAnalogueRepeaterThread::setDTMFThreshold(wxFloat32 threshold)
{
	threshold *= 5000.0F;		// Based on testing by Steve, G8ZMG

	if (m_radioDTMF != NULL)
		m_radioDTMF->setThreshold(threshold);
	if (m_extDTMF != NULL)
		m_extDTMF->setThreshold(threshold);
}

void CAnalogueRepeaterThread::setId1Level(wxFloat32 level)
{
	m_idLevel1 = level * MAX_AUDIO_LEVEL / 1.0F;		// Max of 100% of full deviation
}

void CAnalogueRepeaterThread::setId2Level(wxFloat32 level)
{
	m_idLevel2 = level * MAX_AUDIO_LEVEL / 2.0F;		// Max of 50% of full deviation
}

void CAnalogueRepeaterThread::setAckLevel(wxFloat32 level)
{
	m_ackLevel = level * MAX_AUDIO_LEVEL / 1.0F;			// Max of 100% of full deviation
}

void CAnalogueRepeaterThread::setCTCSSLevel(wxFloat32 level)
{
	m_ctcssLevel = level * MAX_AUDIO_LEVEL / 5.0F;			// Max of 20% of full deviation
}

void CAnalogueRepeaterThread::readCallback(const wxFloat32* input, unsigned int nSamples, int id)
{
	if (m_stopped)
		return;

	switch (id) {
		case SOUNDCARD_RADIO:
#if defined(__WXDEBUG__)
			getRadioAudio(input, nSamples);
#else
			m_radioInBuffer.addData(input, nSamples);
#endif
			break;

		case SOUNDCARD_EXTERNAL:
			m_extInBuffer.addData(input, nSamples);
			break;
	}
}

void CAnalogueRepeaterThread::writeCallback(wxFloat32* output, unsigned int& nSamples, int id)
{
	if (nSamples == 0U)
		return;

	::memset(output, 0x00, nSamples * sizeof(wxFloat32));

	if (m_stopped)
		return;

	switch (id) {
		case SOUNDCARD_RADIO:
			nSamples = m_radioOutBuffer.getData(output, nSamples);
			break;

		case SOUNDCARD_EXTERNAL:
			nSamples = m_extOutBuffer.getData(output, nSamples);
			break;
	}
}

void CAnalogueRepeaterThread::getAudio(wxFloat32* radioAudio, wxFloat32* externalAudio, unsigned int& nRadio, unsigned int& nExternal)
{
	wxASSERT(radioAudio != NULL);
	wxASSERT(externalAudio != NULL);

	nRadio    = 0U;
	nExternal = 0U;

	::memset(radioAudio,    0x00, ANALOGUE_RADIO_BLOCK_SIZE * sizeof(wxFloat32));
	::memset(externalAudio, 0x00, ANALOGUE_RADIO_BLOCK_SIZE * sizeof(wxFloat32));

	if (m_state == ARS_RELAYING_EXTERNAL || m_state == ARS_TIMEOUT_EXTERNAL || m_state == ARS_LOCKOUT_EXTERNAL || m_state == ARS_WAITING_EXTERNAL) {
		// Get external audio
		for (unsigned int n = 0U; nExternal < ANALOGUE_RADIO_BLOCK_SIZE && n < 20U; n++) {
			nExternal += m_extInBuffer.getData(externalAudio + nExternal, ANALOGUE_RADIO_BLOCK_SIZE - nExternal);

			if (nExternal < ANALOGUE_RADIO_BLOCK_SIZE)
				::wxMilliSleep(ANALOGUE_FRAME_TIME_MS / 4UL);
		}

		nRadio = m_radioInBuffer.getData(radioAudio, ANALOGUE_RADIO_BLOCK_SIZE);

		if (m_radioAudioDelay != NULL && nRadio > 0U)
			m_radioAudioDelay->delay(radioAudio, nRadio);

		if (m_extAudioDelay != NULL && nExternal > 0U)
			m_extAudioDelay->delay(externalAudio, nExternal);

		if (nExternal < ANALOGUE_RADIO_BLOCK_SIZE)
			wxLogError(wxT("No external audio is being received"));
	} else {
		// Get radio audio
		for (unsigned int n = 0U; nRadio < ANALOGUE_RADIO_BLOCK_SIZE && n < 20U; n++) {
			nRadio += m_radioInBuffer.getData(radioAudio + nRadio, ANALOGUE_RADIO_BLOCK_SIZE - nRadio);

			if (nRadio < ANALOGUE_RADIO_BLOCK_SIZE)
				::wxMilliSleep(ANALOGUE_FRAME_TIME_MS / 4UL);
		}

		if (m_radioAudioDelay != NULL && nRadio > 0U)
			m_radioAudioDelay->delay(radioAudio, nRadio);

		if (m_extAudioDelay != NULL && nExternal > 0U)
			m_extAudioDelay->delay(externalAudio, nExternal);

		if (m_extSoundcard != NULL)
			nExternal = m_extInBuffer.getData(externalAudio, ANALOGUE_RADIO_BLOCK_SIZE);

		if (nRadio < ANALOGUE_RADIO_BLOCK_SIZE)
			wxLogError(wxT("No radio audio is being received"));
	}
}

ANALOGUE_SQUELCH CAnalogueRepeaterThread::checkRadioSquelch(const wxFloat32* audio, unsigned int length, ANALOGUE_SQUELCH squelch)
{
	wxASSERT(audio != NULL);

	bool squelch1 = m_controller->getRadioSquelch1();	// Lower squelch for CTCSS/External CTCSS input
	bool squelch2 = m_controller->getRadioSquelch2();	// Higher squelch for plain carrier

	if (m_squelch1Delay != NULL) {
		squelch1 = m_squelch1Delay->delay(squelch1);
		squelch2 = m_squelch2Delay->delay(squelch2);
	}

#if defined(__WXDEBUG__)
	if (m_reader != NULL) {
		squelch1 = true;
		squelch2 = true;
	}
#endif

	if (!squelch1 && !squelch2)
		return AS_CLOSED;

	if (m_state == ARS_LISTENING) {
		// 1750Hz access
		if (m_1750In != NULL) {
			m_1750In->process(audio, length);
			if (squelch2 && m_1750In->getDetected() == STATE_TRUE)
				return AS_TONEBURST;
		}
	} else {
		// Hardware squelch open
		if (squelch2)
			return AS_CARRIER;
	}

	// CTCSS access
	if (m_ctcssInternal) {
		if (m_ctcssIn != NULL) {
			m_ctcssIn->process(audio, length);
			TRISTATE state = m_ctcssIn->getDetected();

			if (squelch1 && state == STATE_TRUE) {
				m_ctcssHangTimer.start();
				return AS_CTCSS;
			}

			if (squelch1 && state == STATE_UNKNOWN && squelch == AS_CTCSS) {
				m_ctcssHangTimer.start();
				return AS_CTCSS;
			}

			if (squelch1 && m_ctcssHangTimer.isRunning() && !m_ctcssHangTimer.hasExpired())
				return AS_CTCSS;

			if (m_ctcssHangTimer.isRunning() && m_ctcssHangTimer.hasExpired())
				m_ctcssHangTimer.stop();
		}
	} else {
		if (squelch1)
			return AS_CTCSS;
	}

	return AS_CLOSED;
}

ANALOGUE_SQUELCH CAnalogueRepeaterThread::checkExternalSquelch()
{
	// Only process the squelch if the external port is enabled
	if (m_extMode != AEM_ENABLED)
		return AS_CLOSED;

	// We have no sound anyway!
	if (m_extSoundcard == NULL)
		return AS_CLOSED;

	bool squelch = m_controller->getExternalSquelch();
	if (squelch)
		return AS_EXTERNAL;

	if (m_extController != NULL) {
		if (m_extController->getSquelch())
			return AS_EXTERNAL;
	}

	return AS_CLOSED;
}

void CAnalogueRepeaterThread::sendTones(wxFloat32* audio, unsigned int length)
{
	wxASSERT(audio != NULL);

	if (length == 0U)
		return;

	// Transmitting over the ack, abort it
	if (m_sendAck != ACK_NONE && m_squelch != AS_CLOSED) {
		m_sendAck = ACK_NONE;

		if (m_extAck != NULL)
			m_extAck->reset();
		if (m_radioAck != NULL)
			m_radioAck->reset();
		if (m_batteryAck != NULL)
			m_batteryAck->reset();

		return;
	}

	// Do callsign here so that it pre-empts the ack, unless ack is already sending
	if (m_sendBeacon1 && m_sendAck != ACK_RADIO_SENDING && m_sendAck != ACK_EXTERNAL_SENDING && m_sendAck != ACK_BATTERY_SENDING) {
		unsigned int len;
		if (m_state == ARS_LISTENING || m_state == ARS_TIMEOUT_RADIO || m_state == ARS_LOCKOUT_RADIO || m_state == ARS_TIMEOUT_EXTERNAL || m_state == ARS_LOCKOUT_EXTERNAL)
			len = m_beacon1->getAudio(audio, length, m_idLevel1);
		else
			len = m_beacon1->getAudio(audio, length, m_idLevel2);

		if (len < length) {
			m_sendBeacon1 = false;
			m_beacon1->reset();
			m_callsignTimer.start();
		}

		return;
	}

	if (m_sendOpen && m_sendAck != ACK_RADIO_SENDING && m_sendAck != ACK_EXTERNAL_SENDING && m_sendAck != ACK_BATTERY_SENDING) {
		unsigned int len;
		if (m_state == ARS_LISTENING || m_state == ARS_TIMEOUT_RADIO || m_state == ARS_LOCKOUT_RADIO || m_state == ARS_TIMEOUT_EXTERNAL || m_state == ARS_LOCKOUT_EXTERNAL)
			len = m_openId->getAudio(audio, length, m_idLevel1);
		else
			len = m_openId->getAudio(audio, length, m_idLevel2);

		if (len < length) {
			m_sendOpen = false;
			m_openId->reset();
			m_callsignTimer.start();
		}

		return;
	}

	if (m_sendClose && m_sendAck != ACK_RADIO_SENDING && m_sendAck != ACK_EXTERNAL_SENDING && m_sendAck != ACK_BATTERY_SENDING) {
		unsigned int len;
		if (m_state == ARS_LISTENING || m_state == ARS_TIMEOUT_RADIO || m_state == ARS_LOCKOUT_RADIO || m_state == ARS_TIMEOUT_EXTERNAL || m_state == ARS_LOCKOUT_EXTERNAL)
			len = m_closeId->getAudio(audio, length, m_idLevel1);
		else
			len = m_closeId->getAudio(audio, length, m_idLevel2);

		if (len < length) {
			m_sendClose = false;
			m_closeId->reset();
			m_callsignTimer.start();
		}

		return;
	}

	if (m_sendBeacon2) {
		unsigned int len;
		if (m_state == ARS_LISTENING)
			len = m_beacon2->getAudio(audio, length, m_idLevel1);
		else
			len = m_beacon2->getAudio(audio, length, m_idLevel2);

		if (len < length) {
			m_beacon2->reset();
    		m_sendBeacon2 = false;
			m_callsignTimer.start();
			return;
		}
	}

	if (m_sendAck == ACK_RADIO_WAITING || m_sendAck == ACK_RADIO_SENDING) {
		if (m_radioAck == NULL) {
			m_sendAck = ACK_NONE;
			return;
		}

		unsigned int len = m_radioAck->getAudio(audio, length, m_ackLevel);

		if (len < length) {
			m_sendAck = ACK_NONE;
			m_radioAck->reset();
			return;
		}

		m_sendAck = ACK_RADIO_SENDING;
		return;
	}

	if (m_sendAck == ACK_EXTERNAL_WAITING || m_sendAck == ACK_EXTERNAL_SENDING) {
		if (m_extAck == NULL) {
			m_sendAck = ACK_NONE;
			return;
		}

		unsigned int len = m_extAck->getAudio(audio, length, m_ackLevel);

		if (len < length) {
			m_sendAck = ACK_NONE;
			m_extAck->reset();
			return;
		}

		m_sendAck = ACK_EXTERNAL_SENDING;
		return;
	}

	if (m_sendAck == ACK_BATTERY_WAITING || m_sendAck == ACK_BATTERY_SENDING) {
		if (m_batteryAck == NULL) {
			m_sendAck = ACK_NONE;
			return;
		}

		unsigned int len = m_batteryAck->getAudio(audio, length, m_ackLevel);

		if (len < length) {
			m_sendAck = ACK_NONE;
			m_batteryAck->reset();
			return;
		}

		m_sendAck = ACK_BATTERY_SENDING;
		return;
	}

	// The timeout tone, here so that a callsign can pre-empt it
	if (m_state == ARS_TIMEOUT_RADIO || m_state == ARS_TIMEOUT_EXTERNAL)
		m_timeoutTones->getAudio(audio, length, m_idLevel1);
}

CAnalogueRepeaterStatusData* CAnalogueRepeaterThread::getStatus()
{
	if (m_stopped)
		return NULL;

	wxString radioDTMF;
	if (m_radioDTMF != NULL)
		radioDTMF = m_radioDTMF->getBuffer();

	wxString extDTMF;
	if (m_extDTMF != NULL)
		extDTMF = m_extDTMF->getBuffer();

	CAnalogueRepeaterStatusData* status = new CAnalogueRepeaterStatusData(m_radioTransmit, m_squelch, m_audioLevel,
		m_state, m_radioAudioVOGAD.getGain(), m_extAudioVOGAD.getGain(), radioDTMF, extDTMF,
		m_timeoutTimer.getTimer(), m_timeoutTimer.getTimeout(), m_lockoutTimer.getTimer(),
		m_lockoutTimer.getTimeout(), m_hangTimer.getTimer(), m_hangTimer.getTimeout(),
		m_latchTimer.getTimer(), m_latchTimer.getTimeout(), m_ackTimer.getTimer(), m_ackTimer.getTimeout(),
		m_minimumTimer.getTimer(), m_minimumTimer.getTimeout(), m_callsignTimer.getTimer(),
		m_callsignTimer.getTimeout(), m_activeHangTimer.getTimer(), m_activeHangTimer.getTimeout(), m_extTransmit,
		m_battery);

	m_audioLevel = 0.0F;

	return status;
}

void CAnalogueRepeaterThread::clock(unsigned int ms)
{
	m_callsignTimer.clock(ms);
	if (m_callsignTimer.hasExpired()) {
		if (m_state == ARS_LISTENING) {
			if (!m_beacon2->isEmpty() && !m_sendBeacon2)
				m_sendBeacon2 = true;
		} else {
			if (!m_beacon1->isEmpty() && !m_sendBeacon1)
				m_sendBeacon1 = true;
		}
	}

	m_callsignStartTimer.clock(ms);
	if (m_callsignStartTimer.isRunning() && m_callsignStartTimer.hasExpired()) {
		m_sendOpen = true;
		m_callsignStartTimer.stop();
	}

	m_hangTimer.clock(ms);
	m_ackTimer.clock(ms);
	m_activeHangTimer.clock(ms);
	m_ctcssHangTimer.clock(ms);

	if (m_squelch != AS_CLOSED) {
		m_latchTimer.clock(ms);
		m_minimumTimer.clock(ms);
		m_timeoutTimer.clock(ms);
		m_lockoutTimer.clock(ms);
	}
}

void CAnalogueRepeaterThread::stateMachine()
{
	switch (m_state) {
		case ARS_SHUTDOWN:
			break;

		case ARS_LISTENING:
			if (m_squelch != AS_CLOSED) {
				// Squelch open
				if (m_callsignAtStart == ACS_OPEN)
					sendOpen();

				if (m_squelch == AS_EXTERNAL)
					setState(ARS_RELAYING_EXTERNAL);
				else
					setState(ARS_RELAYING_RADIO);
			}
			break;

		case ARS_RELAYING_RADIO:
		case ARS_RELAYING_EXTERNAL:
			if (m_squelch != AS_CLOSED) {
				// Squelch open
				if (m_timeoutTimer.isRunning() && m_timeoutTimer.hasExpired()) {
					// The input has timed out
					if (m_state == ARS_RELAYING_RADIO)
						setState(ARS_TIMEOUT_RADIO);
					else
						setState(ARS_TIMEOUT_EXTERNAL);
				} else {
					// Is it a change from one input to the other?
					if (m_state == ARS_RELAYING_RADIO && m_squelch == AS_EXTERNAL)
						setState(ARS_RELAYING_EXTERNAL);
					else if (m_state == ARS_RELAYING_EXTERNAL && (m_squelch == AS_CTCSS || m_squelch == AS_TONEBURST || m_squelch == AS_CARRIER))
						setState(ARS_RELAYING_RADIO);
				}
			} else {
				// Squelch closed
				if (m_firstTime && m_latchTimer.isRunning() && !m_latchTimer.hasExpired()) {
					// Transmission not long enough, closedown
					setState(ARS_LISTENING);
				} else {
					// If first time and the latch was running, send a callsign if needed
					if (m_firstTime && m_latchTimer.isRunning() && m_latchTimer.hasExpired()) {
						if (m_callsignAtStart == ACS_LATCH)
							sendOpen();
					}

					// Signal long enough, or not first time
					if (m_state == ARS_RELAYING_RADIO)
						setState(ARS_WAITING_RADIO);
					else
						setState(ARS_WAITING_EXTERNAL);
				}
			}
			break;

		case ARS_WAITING_RADIO:
		case ARS_WAITING_EXTERNAL:
			if (m_squelch != AS_CLOSED) {
				// Squelch open
				if (m_squelch == AS_EXTERNAL)
					setState(ARS_RELAYING_EXTERNAL);
				else
					setState(ARS_RELAYING_RADIO);
			} else {
				// Squelch closed, is it time for the ack?
				if (m_ackTimer.hasExpired()) {
					// Only send the audible ack after the minimum time
					if (!m_minimumTimer.isRunning() || (m_minimumTimer.isRunning() && m_minimumTimer.hasExpired())) {
						if (m_state == ARS_WAITING_EXTERNAL) {
							if (m_battery)
								m_sendAck = ACK_BATTERY_WAITING;
							else
								m_sendAck = ACK_EXTERNAL_WAITING;
						} else {
							if (m_battery)
								m_sendAck = ACK_BATTERY_WAITING;
							else
								m_sendAck = ACK_RADIO_WAITING;
						}
					}

					m_ackTimer.stop();
					m_timeoutTimer.stop();
					m_lockoutTimer.stop();
					m_hangTimer.start();
					m_minimumTimer.stop();
				}
				if (m_hangTimer.hasExpired()) {
					// Shutdown
					if (m_callsignAtEnd)
						sendClose();
					m_activeHangTimer.start();
					setState(ARS_LISTENING);
				}
			}
			break;

		case ARS_TIMEOUT_RADIO:
			if (m_squelch == AS_CLOSED) {
				setState(ARS_WAITING_RADIO);
			} else {
				if (m_lockoutTimer.hasExpired()) {
					sendCallsign();
					setState(ARS_LOCKOUT_RADIO);
				}
			}
			break;

		case ARS_TIMEOUT_EXTERNAL:
			if (m_squelch == AS_CLOSED) {
				setState(ARS_WAITING_EXTERNAL);
			} else {
				if (m_lockoutTimer.hasExpired()) {
					sendCallsign();
					setState(ARS_LOCKOUT_EXTERNAL);
				}
			}
			break;

		case ARS_LOCKOUT_RADIO:
			if (m_squelch == AS_CLOSED)
				setState(ARS_WAITING_RADIO);
			break;

		case ARS_LOCKOUT_EXTERNAL:
			if (m_squelch == AS_CLOSED)
				setState(ARS_WAITING_EXTERNAL);
			break;
	}
}

void CAnalogueRepeaterThread::setState(ANALOGUE_RPT_STATE state)
{
	// The 'from' state
	switch (m_state) {
		case ARS_SHUTDOWN:
			m_ackTimer.stop();
			m_hangTimer.stop();
			m_latchTimer.stop();
			m_minimumTimer.stop();
			m_timeoutTimer.stop();
			m_lockoutTimer.stop();
			m_callsignTimer.start();
			m_callsignStartTimer.stop();
			m_radioTransmit = false;
			m_extTransmit   = false;
			m_sendBeacon1   = false;
			m_sendBeacon2   = false;
			m_sendOpen      = false;
			m_sendClose     = false;
			m_sendAck       = ACK_NONE;
			break;

		case ARS_LISTENING:
			m_latchTimer.start();
			m_firstTime = true;
			break;

		case ARS_TIMEOUT_RADIO:
		case ARS_TIMEOUT_EXTERNAL:
			m_timeoutTimer.stop();
			m_lockoutTimer.stop();
			break;

		case ARS_LOCKOUT_RADIO:
		case ARS_LOCKOUT_EXTERNAL:
			m_lockoutTimer.stop();
			break;

		default:
			break;
	}

	// The 'to' state
	switch (state) {
		case ARS_SHUTDOWN:
			m_ackTimer.stop();
			m_hangTimer.stop();
			m_latchTimer.stop();
			m_minimumTimer.stop();
			m_timeoutTimer.stop();
			m_lockoutTimer.stop();
			m_callsignTimer.stop();
			m_activeHangTimer.stop();
			m_callsignStartTimer.stop();
			m_radioTransmit = false;
			m_extTransmit   = false;
			m_sendBeacon1   = false;
			m_sendBeacon2   = false;
			m_sendOpen      = false;
			m_sendClose     = false;
			m_sendAck       = ACK_NONE;
			m_state         = ARS_SHUTDOWN;
			m_controller->setExternalTransmit(false);
			m_controller->setRadioTransmit(false);
			m_controller->setActive(false);
			break;

		case ARS_LISTENING:
			m_ackTimer.stop();
			m_hangTimer.stop();
			m_latchTimer.stop();
			m_minimumTimer.stop();
			m_timeoutTimer.stop();
			m_lockoutTimer.stop();
			m_state = ARS_LISTENING;
			break;

		case ARS_RELAYING_RADIO:
		case ARS_RELAYING_EXTERNAL:
			m_radioAudioVOGAD.reset();
			m_extAudioVOGAD.reset();
			m_ackTimer.stop();
			m_hangTimer.stop();
			if (!m_minimumTimer.isRunning())
				m_minimumTimer.start();
			if (!m_timeoutTimer.isRunning())
				m_timeoutTimer.start();
			m_state = state;
			break;

		case ARS_WAITING_RADIO:
		case ARS_WAITING_EXTERNAL:
			m_ackTimer.start();
			m_latchTimer.stop();
			m_firstTime = false;
			m_state = state;
			break;

		case ARS_TIMEOUT_RADIO:
		case ARS_TIMEOUT_EXTERNAL:
			m_lockoutTimer.start();
			m_state = state;
			break;

		case ARS_LOCKOUT_RADIO:
		case ARS_LOCKOUT_EXTERNAL:
			m_lockoutTimer.stop();
			m_state = state;
			break;
	}
}

void CAnalogueRepeaterThread::feedRadio(const wxFloat32* audio, unsigned int length)
{
	wxASSERT(audio != NULL);

	if (m_radioTransmit)
		m_radioOutBuffer.addData(audio, length);

	if (m_pttDelay != NULL)
		m_controller->setRadioTransmit(m_pttDelay->delay(m_radioTransmit));
	else
		m_controller->setRadioTransmit(m_radioTransmit);
}

void CAnalogueRepeaterThread::feedExternal(const wxFloat32* audio, unsigned int length, bool transmit)
{
	wxASSERT(audio != NULL);

	if (m_extSoundcard == NULL)
		return;

	if (transmit) {
		// Scale up the through audio to the maximum
		wxFloat32 extAudio[ANALOGUE_RADIO_BLOCK_SIZE];
		for (unsigned int i = 0U; i < length; i++)
			extAudio[i] = audio[i] / MAX_AUDIO_LEVEL;
		m_extOutBuffer.addData(extAudio, length);

		m_controller->setExternalTransmit(true);
		if (m_extController != NULL)
			m_extController->setTransmit(true);
	} else {
		m_controller->setExternalTransmit(false);
		if (m_extController != NULL)
			m_extController->setTransmit(false);
	}
}

void CAnalogueRepeaterThread::calculateAudioLevel(const wxFloat32* audio, unsigned int length)
{
	wxASSERT(audio != NULL);

	for (unsigned int i = 0U; i < length; i++) {
		wxFloat32 level = ::fabs(audio[i]) / MAX_AUDIO_LEVEL;

		if (level > m_audioLevel)
			m_audioLevel = level;
	}
}

void CAnalogueRepeaterThread::insertCTCSS(wxFloat32* audio, unsigned int length)
{
	wxASSERT(audio != NULL);

	// Add the CTCSS to the output audio only when relaying user audio, not beacons or locked out

	if (length == 0U || m_ctcssOut == NULL || m_ctcssLevel == 0.0F)
		return;

	switch (m_ctcssOutput) {
		case ACO_WHEN_OPEN:
			switch (m_state) {
				// Always put CTCSS on if relaying audio
				case ARS_RELAYING_RADIO:
				case ARS_RELAYING_EXTERNAL:
					m_ctcssOut->getAudio(audio, length, m_ctcssLevel);
					return;

				case ARS_TIMEOUT_RADIO:
				case ARS_TIMEOUT_EXTERNAL:
					// If the callsign at the end is disabled.... put the CTCSS on until the tx closedown
					if (!m_callsignAtEnd) {
						m_ctcssOut->getAudio(audio, length, m_ctcssLevel);
						return;
					}

					// If the callsign at the end is enabled then shut off the CTCSS a second before it's sent
					if (!m_lockoutTimer.isRunning() || m_lockoutTimer.getRemaining() > 0U)
						m_ctcssOut->getAudio(audio, length, m_ctcssLevel);

					return;

				case ARS_WAITING_RADIO:
				case ARS_WAITING_EXTERNAL:
					// If the callsign at the end is disabled.... put the CTCSS on until the tx closedown
					if (!m_callsignAtEnd) {
						m_ctcssOut->getAudio(audio, length, m_ctcssLevel);
						return;
					}

					// If the callsign at the end is enabled then shut off the CTCSS a second before it's sent
					if (!m_hangTimer.isRunning() || m_hangTimer.getRemaining() > 0U)
						m_ctcssOut->getAudio(audio, length, m_ctcssLevel);

					return;

				default:
					// No CTCSS otherwise
					return;
			}
			break;

		case ACO_ON_AUDIO:
			if (m_state == ARS_RELAYING_RADIO || m_state == ARS_RELAYING_EXTERNAL)
				m_ctcssOut->getAudio(audio, length, m_ctcssLevel);
			break;

		case ACO_ALWAYS:
			if (m_radioTransmit)
				m_ctcssOut->getAudio(audio, length, m_ctcssLevel);
			break;

		default:
			break;
	}
}

void CAnalogueRepeaterThread::filterAudio(wxFloat32* audio, unsigned int length)
{
	wxASSERT(audio != NULL);

	if (length == 0U)
		return;

	if (m_radioTransmit)
		m_audioFilter.process(audio, length);
	else
		m_audioFilter.reset();
}

void CAnalogueRepeaterThread::processDTMF(const wxFloat32* radioAudio, const wxFloat32* extAudio, unsigned int nRadio, unsigned int nExt)
{
	wxASSERT(radioAudio != NULL);
	wxASSERT(extAudio != NULL);

	DTMF_COMMAND command = DTMF_NONE;

	if (m_radioDTMF != NULL) {
		if (m_controller->getRadioSquelch1() || m_controller->getRadioSquelch2() || m_reader != NULL)
			command = m_radioDTMF->process(radioAudio, nRadio);
		else
			m_radioDTMF->reset();
	}

	if (command == DTMF_NONE && m_extDTMF != NULL) {
		if (m_controller->getExternalSquelch())
			command = m_extDTMF->process(extAudio, nExt);
		else
			m_extDTMF->reset();
	}

	switch (command) {
		case DTMF_NONE:
			break;
		case DTMF_SHUTDOWN:
			wxLogMessage(wxT("DTMF Shutdown command received"));
			shutdown();
			break;
		case DTMF_STARTUP:
			wxLogMessage(wxT("DTMF Startup command received"));
			startup();
			break;
		case DTMF_TIMEOUT:
			wxLogMessage(wxT("DTMF Timeout command received"));
			timeout();
			break;
		case DTMF_TIMERESET:
			wxLogMessage(wxT("DTMF Time Reset command received"));
			timeReset();
			break;
		case DTMF_COMMAND1:
			wxLogMessage(wxT("DTMF Command 1 command received"));
			command1();
			break;
		case DTMF_COMMAND2:
			wxLogMessage(wxT("DTMF Command 2 command received"));
			command2();
			break;
		case DTMF_OUTPUT1:
			wxLogMessage(wxT("DTMF Output 1 command received"));
			m_output1 = !m_output1;
			m_controller->setOutput1(m_output1);
			break;
		case DTMF_OUTPUT2:
			wxLogMessage(wxT("DTMF Output 2 command received"));
			m_output2 = !m_output2;
			m_controller->setOutput2(m_output2);
			break;
		case DTMF_OUTPUT3:
			wxLogMessage(wxT("DTMF Output 3 command received"));
			m_output3 = !m_output3;
			m_controller->setOutput3(m_output3);
			break;
		case DTMF_OUTPUT4:
			wxLogMessage(wxT("DTMF Output 4 command received"));
			m_output4 = !m_output4;
			m_controller->setOutput4(m_output4);
			break;
	}
}

void CAnalogueRepeaterThread::shutdown()
{
	m_shutdown = true;
}

void CAnalogueRepeaterThread::startup()
{
	m_shutdown = false;
}

void CAnalogueRepeaterThread::timeout()
{
	if (m_state == ARS_RELAYING_RADIO)
		setState(ARS_TIMEOUT_RADIO);
	else if (m_state == ARS_RELAYING_EXTERNAL)
		setState(ARS_TIMEOUT_EXTERNAL);
}

void CAnalogueRepeaterThread::timeReset()
{
	if (m_timeoutTimer.isRunning())
		m_timeoutTimer.start();
	if (m_state == ARS_TIMEOUT_RADIO || m_state == ARS_LOCKOUT_RADIO)
		setState(ARS_RELAYING_RADIO);
	else if (m_state == ARS_TIMEOUT_EXTERNAL || m_state == ARS_LOCKOUT_EXTERNAL)
		setState(ARS_RELAYING_EXTERNAL);
}

void CAnalogueRepeaterThread::command1()
{
	if (!m_command1Line.IsEmpty())
		::wxShell(m_command1Line);
}

void CAnalogueRepeaterThread::command2()
{
	if (!m_command2Line.IsEmpty())
		::wxShell(m_command2Line);
}

void CAnalogueRepeaterThread::sendOpen()
{
	// Already sending, or waiting to send, ignore request
	if (m_sendBeacon1 || m_sendBeacon2 || m_sendOpen || m_sendClose || m_callsignStartTimer.isRunning())
		return;

	unsigned int t1, t2;

	switch (m_callsignHoldoff) {
		case ACH_NONE:
			if (m_callsignStartTimer.getTimeout() > 0U)
				m_callsignStartTimer.start();
			else
				m_sendOpen = true;
			break;

		case ACH_14:
			t1 = m_callsignTimer.getTimeout() / 4U;
			t2 = m_callsignTimer.getTimer();
			if (t2 >= t1) {
				if (m_callsignStartTimer.getTimeout() > 0U)
					m_callsignStartTimer.start();
				else
					m_sendOpen = true;
			}
			break;

		case ACH_12:
			t1 = m_callsignTimer.getTimeout() / 2U;
			t2 = m_callsignTimer.getTimer();
			if (t2 >= t1) {
				if (m_callsignStartTimer.getTimeout() > 0U)
					m_callsignStartTimer.start();
				else
					m_sendOpen = true;
			}
			break;

		case ACH_34:
			t1 = 3U * m_callsignTimer.getTimeout() / 4U;
			t2 = m_callsignTimer.getTimer();
			if (t2 >= t1) {
				if (m_callsignStartTimer.getTimeout() > 0U)
					m_callsignStartTimer.start();
				else
					m_sendOpen = true;
			}
			break;
	}
}

void CAnalogueRepeaterThread::sendClose()
{
	// Already sending, or waiting to send, ignore request
	if (m_sendBeacon1 || m_sendBeacon2 || m_sendOpen || m_sendClose || m_callsignStartTimer.isRunning())
		return;

	unsigned int t1, t2;

	switch (m_callsignHoldoff) {
		case ACH_NONE:
			m_sendClose = true;
			break;

		case ACH_14:
			t1 = m_callsignTimer.getTimeout() / 4U;
			t2 = m_callsignTimer.getTimer();
			if (t2 >= t1)
				m_sendClose = true;
			break;

		case ACH_12:
			t1 = m_callsignTimer.getTimeout() / 2U;
			t2 = m_callsignTimer.getTimer();
			if (t2 >= t1)
				m_sendClose = true;
			break;

		case ACH_34:
			t1 = 3U * m_callsignTimer.getTimeout() / 4U;
			t2 = m_callsignTimer.getTimer();
			if (t2 >= t1)
				m_sendClose = true;
			break;
	}
}

void CAnalogueRepeaterThread::sendCallsign()
{
	// Already sending, or waiting to send, ignore request
	if (m_sendBeacon1 || m_sendBeacon2 || m_sendOpen || m_sendClose || m_callsignStartTimer.isRunning())
		return;

	unsigned int t1, t2;

	switch (m_callsignHoldoff) {
		case ACH_NONE:
			if (!m_beacon1->isEmpty())
				m_sendBeacon1 = true;
			break;

		case ACH_14:
			t1 = m_callsignTimer.getTimeout() / 4U;
			t2 = m_callsignTimer.getTimer();
			if (t2 >= t1 && !m_beacon1->isEmpty())
				m_sendBeacon1 = true;
			break;

		case ACH_12:
			t1 = m_callsignTimer.getTimeout() / 2U;
			t2 = m_callsignTimer.getTimer();
			if (t2 >= t1 && !m_beacon1->isEmpty())
				m_sendBeacon1 = true;
			break;

		case ACH_34:
			t1 = 3U * m_callsignTimer.getTimeout() / 4U;
			t2 = m_callsignTimer.getTimer();
			if (t2 >= t1 && !m_beacon1->isEmpty())
				m_sendBeacon1 = true;
			break;
	}
}

void CAnalogueRepeaterThread::getStatistics()
{
	switch (m_squelch) {
		case AS_CLOSED:
			break;
		case AS_CARRIER:
		case AS_CTCSS:
		case AS_TONEBURST:
			m_radioCount++;
			break;
		case AS_EXTERNAL:
			m_externalCount++;
			break;
	}

	if (m_state == ARS_RELAYING_RADIO || m_state == ARS_RELAYING_EXTERNAL ||
		m_state == ARS_WAITING_RADIO  || m_state == ARS_WAITING_EXTERNAL  ||
		m_state == ARS_TIMEOUT_RADIO  || m_state == ARS_TIMEOUT_EXTERNAL)
		m_relayCount++;

	if (m_radioTransmit)
		m_transmitCount++;

	m_timeCount++;
	if ((m_timeCount % ANALOGUE_TICKS_PER_SEC) == 0U) {
		wxDateTime dateTime = wxDateTime::Now();
		unsigned int hour = dateTime.GetHour();

		if (hour != m_lastHour) {
			writeStatistics();
			m_lastHour = hour;
		}
	}
}

void CAnalogueRepeaterThread::writeStatistics()
{
	wxLogInfo(wxT("STATS: Radio input: %u%%"),    (m_radioCount * 100U) / m_timeCount);
	wxLogInfo(wxT("STATS: External input: %u%%"), (m_externalCount * 100U) / m_timeCount);
	wxLogInfo(wxT("STATS: Relay transmit: %u%%"), (m_relayCount * 100U) / m_timeCount);
	wxLogInfo(wxT("STATS: Total transmit: %u%%"), (m_transmitCount * 100U) / m_timeCount);

	m_radioCount    = 0U;
	m_externalCount = 0U;
	m_relayCount    = 0U;
	m_transmitCount = 0U;
	m_timeCount     = 0U;
}

#if defined(__WXDEBUG__)
void CAnalogueRepeaterThread::getRadioAudio(const wxFloat32* input, unsigned int nSamples)
{
	wxASSERT(input != NULL);

	if (nSamples == 0U)
		return;

	if (m_reader != NULL) {		// Are we reading from a WAV file?
		wxFloat32 buffer[ANALOGUE_RADIO_BLOCK_SIZE * 2U];
		unsigned int n = m_reader->read(buffer, nSamples);
		if (n == 0U) {				// EOF on the input WAV file?
			CWAVFileReader* reader = m_reader;
			m_reader = NULL;

			reader->close();
			delete reader;

			wxLogInfo(wxT("End of the input file"));

			// Send data from the sound card as the file has ended
			m_radioInBuffer.addData(input, nSamples);
			return;
		}

		// Send data from the sound file
		m_radioInBuffer.addData(buffer, n);
	} else {
		// Send data from the sound card
		m_radioInBuffer.addData(input, nSamples);
	}
}
#endif
