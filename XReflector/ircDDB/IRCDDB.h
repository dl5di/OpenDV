/*

CIRCDDB - ircDDB client library in C++

Copyright (C) 2010-2011   Michael Dirska, DL1BFF (dl1bff@mdx.de)
Copyright (C) 2011,2012   Jonathan Naylor, G4KLX

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/


#if !defined(_IRCDDB_H)
#define _IRCDDB_H

#include <wx/wx.h>

enum IRCDDB_RESPONSE_TYPE {
	IDRT_NONE,
	IDRT_USER,
	IDRT_GATEWAY,
	IDRT_REPEATER
};

struct CIRCDDBPrivate;

class CIRCDDB {
public:
	CIRCDDB(const wxString& hostName, unsigned int port, const wxString& callsign, const wxString& password,
	    const wxString& versionInfo, const wxString& localAddr = wxEmptyString );
	~CIRCDDB();

	// A false return implies a network error, or unable to log in
	bool open();


	// rptrQTH can be called multiple times if necessary
	//   callsign     The callsign of the repeater
	//   latitude     WGS84 position of antenna in degrees, positive value -> NORTH
	//   longitude    WGS84 position of antenna in degrees, positive value -> EAST
	//   desc1, desc2   20-character description of QTH
	//   infoURL      URL of a web page with information about the repeater

	void rptrQTH( const wxString& callsign, double latitude, double longitude, const wxString& desc1,
	    const wxString& desc2, const wxString& infoURL );



	// rptrQRG can be called multiple times if necessary
	//  callsign      callsign of the repeater
	//  txFrequency   repeater TX frequency in MHz
	//  duplexShift   duplex shift in MHz (positive or negative value):  RX_freq = txFrequency + duplexShift
	//  range       range of the repeater in meters (meters = miles * 1609.344)
	//  agl         height of the antenna above ground in meters (meters = feet * 0.3048)

	void rptrQRG( const wxString& callsign, double txFrequency, double duplexShift, double range, double agl );


	// If you call this method once, watchdog messages will be sent to the
	// to the ircDDB network every 15 minutes. Invoke this method every 1-2 minutes to indicate
	// that the gateway is working properly. After activating the watchdog, a red LED will be displayed
	// on the ircDDB web page if this method is not called within a period of about 30 minutes.
	// The string wdInfo should contain information about the source of the alive messages, e.g.,
	// version of the RF decoding software. For example, the ircDDB java software sets this
	// to "rpm_ircddbmhd-x.z-z".  The string wdInfo must contain at least one non-space character.

	void kickWatchdog(const wxString& callsign, const wxString& wdInfo);



	// get internal network status
	int getConnectionState();
	// one of these values is returned:
	//  0  = not (yet) connected to the IRC server
	//  1-6  = a new connection was established, download of repeater info etc. is
	//         in progress
	//  7 = the ircDDB connection is fully operational
	//  10 = some network error occured, next state is "0" (new connection attempt)

	// Send heard data, a false return implies a network error
	bool sendHeard(const wxString& myCall, const wxString& myCallExt,
	          const wxString& yourCall, const wxString& rpt1,
		  const wxString& rpt2, unsigned char flag1,
		  unsigned char flag2, unsigned char flag3 );


	// same as sendHeard with two new fields:
	//   network_destination:  empty string or 8-char call sign of the repeater
	//	    or reflector, where this transmission is relayed to.
	//   tx_message:  20-char TX message or empty string, if the user did not
	//       send a TX message
	bool sendHeardWithTXMsg(const wxString& myCall, const wxString& myCallExt,
	          const wxString& yourCall, const wxString& rpt1,
		  const wxString& rpt2, unsigned char flag1,
		  unsigned char flag2, unsigned char flag3,
		  const wxString& network_destination,
		  const wxString& tx_message );

	// this method should be called at the end of a transmission
	//  num_dv_frames: number of DV frames sent out (96 bit frames, 20ms)
	//  num_dv_silent_frames: number of DV silence frames sent out in the
	//	last transmission, or -1 if the information is not available
	//  num_bit_errors: number of bit errors of the received data. This should
	//      be the derived from the first Golay block of the voice data. This
	//      error correction code only looks at 24 bits of the 96 bit frame.
	//      So, the overall bit error rate is calculated like this:
	//      BER = num_bit_errors / (num_dv_frames * 24)
	//      Set num_bit_errors = -1, if the error information is not available.
	bool sendHeardWithTXStats(const wxString& myCall, const wxString& myCallExt,
	          const wxString& yourCall, const wxString& rpt1,
		  const wxString& rpt2, unsigned char flag1,
		  unsigned char flag2, unsigned char flag3,
		  int num_dv_frames,
		  int num_dv_silent_frames,
		  int num_bit_errors );

	// The following three functions don't block waiting for a reply, they just send the data

	// Send query for a gateway/reflector, a false return implies a network error
	bool findGateway(const wxString& gatewayCallsign);

	// Send query for a repeater module, a false return implies a network error
	bool findRepeater(const wxString& repeaterCallsign);

	// Send query for a user, a false return implies a network error
	bool findUser(const wxString& userCallsign);

	// The following functions are for processing received messages
	
	// Get the waiting message type
	IRCDDB_RESPONSE_TYPE getMessageType();

	// Get a gateway message, as a result of IDRT_REPEATER returned from getMessageType()
	// A false return implies a network error
	bool receiveRepeater(wxString& repeaterCallsign, wxString& gatewayCallsign, wxString& address);

	// Get a gateway message, as a result of IDRT_GATEWAY returned from getMessageType()
	// A false return implies a network error
	bool receiveGateway(wxString& gatewayCallsign, wxString& address);

	// Get a user message, as a result of IDRT_USER returned from getMessageType()
	// A false return implies a network error
	bool receiveUser(wxString& userCallsign, wxString& repeaterCallsign, wxString& gatewayCallsign, wxString& address);

	bool receiveUser(wxString& userCallsign, wxString& repeaterCallsign, wxString& gatewayCallsign, wxString& address,
	    wxString& timeStamp );

	void close();		// Implictely kills any threads in the IRC code


private:
	struct CIRCDDBPrivate * const d;

};

#endif  // _IRCDDB_H

