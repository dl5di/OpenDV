/*
 *   Copyright (C) 2010,2011,2012,2013 by Jonathan Naylor G4KLX
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

#ifndef	StarNetServerPreferences_H
#define	StarNetServerPreferences_H

#include <wx/wx.h>
#include <wx/notebook.h>

#include "StarNetServerMiscellaneousSet.h"
#include "StarNetServerCallsignSet.h"
#include "StarNetServerIrcDDBSet.h"
#include "StarNetSet.h"
#include "RemoteSet.h"

class CStarNetServerPreferences : public wxDialog {
public:
	CStarNetServerPreferences(wxWindow* parent, int id,
				 const wxString& callsign, const wxString& address,
				 const wxString& hostname, const wxString& username, const wxString& password,
				 bool logEnabled,
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
				 const wxString& starNetBand1,  const wxString& callsign1,  const wxString& logoff1,  const wxString& info1,  const wxString& permanent1,  unsigned int userTimeout1,  unsigned int groupTimeout1,  STARNET_CALLSIGN_SWITCH callsignSwitch1,  bool txMsgSwitch1,  const wxString& link1,
				 const wxString& starNetBand2,  const wxString& callsign2,  const wxString& logoff2,  const wxString& info2,  const wxString& permanent2,  unsigned int userTimeout2,  unsigned int groupTimeout2,  STARNET_CALLSIGN_SWITCH callsignSwitch2,  bool txMsgSwitch2,  const wxString& link2,
				 const wxString& starNetBand3,  const wxString& callsign3,  const wxString& logoff3,  const wxString& info3,  const wxString& permanent3,  unsigned int userTimeout3,  unsigned int groupTimeout3,  STARNET_CALLSIGN_SWITCH callsignSwitch3,  bool txMsgSwitch3,  const wxString& link3,
				 const wxString& starNetBand4,  const wxString& callsign4,  const wxString& logoff4,  const wxString& info4,  const wxString& permanent4,  unsigned int userTimeout4,  unsigned int groupTimeout4,  STARNET_CALLSIGN_SWITCH callsignSwitch4,  bool txMsgSwitch4,  const wxString& link4,
				 const wxString& starNetBand5,  const wxString& callsign5,  const wxString& logoff5,  const wxString& info5,  const wxString& permanent5,  unsigned int userTimeout5,  unsigned int groupTimeout5,  STARNET_CALLSIGN_SWITCH callsignSwitch5,  bool txMsgSwitch5,  const wxString& link5,
				 const wxString& starNetBand6,  const wxString& callsign6,  const wxString& logoff6,  const wxString& info6,  const wxString& permanent6,  unsigned int userTimeout6,  unsigned int groupTimeout6,  STARNET_CALLSIGN_SWITCH callsignSwitch6,  bool txMsgSwitch6,  const wxString& link6,
				 const wxString& starNetBand7,  const wxString& callsign7,  const wxString& logoff7,  const wxString& info7,  const wxString& permanent7,  unsigned int userTimeout7,  unsigned int groupTimeout7,  STARNET_CALLSIGN_SWITCH callsignSwitch7,  bool txMsgSwitch7,  const wxString& link7,
				 const wxString& starNetBand8,  const wxString& callsign8,  const wxString& logoff8,  const wxString& info8,  const wxString& permanent8,  unsigned int userTimeout8,  unsigned int groupTimeout8,  STARNET_CALLSIGN_SWITCH callsignSwitch8,  bool txMsgSwitch8,  const wxString& link8,
				 const wxString& starNetBand9,  const wxString& callsign9,  const wxString& logoff9,  const wxString& info9,  const wxString& permanent9, unsigned int userTimeout9,  unsigned int groupTimeout9,  STARNET_CALLSIGN_SWITCH callsignSwitch9,  bool txMsgSwitch9,  const wxString& link9,
				 const wxString& starNetBand10, const wxString& callsign10, const wxString& logoff10, const wxString& info10, const wxString& permanent10, unsigned int userTimeout10, unsigned int groupTimeout10, STARNET_CALLSIGN_SWITCH callsignSwitch10, bool txMsgSwitch10, const wxString& link10,
				 const wxString& starNetBand11, const wxString& callsign11, const wxString& logoff11, const wxString& info11, const wxString& permanent11, unsigned int userTimeout11, unsigned int groupTimeout11, STARNET_CALLSIGN_SWITCH callsignSwitch11, bool txMsgSwitch11, const wxString& link11,
				 const wxString& starNetBand12, const wxString& callsign12, const wxString& logoff12, const wxString& info12, const wxString& permanent12, unsigned int userTimeout12, unsigned int groupTimeout12, STARNET_CALLSIGN_SWITCH callsignSwitch12, bool txMsgSwitch12, const wxString& link12,
				 const wxString& starNetBand13, const wxString& callsign13, const wxString& logoff13, const wxString& info13, const wxString& permanent13, unsigned int userTimeout13, unsigned int groupTimeout13, STARNET_CALLSIGN_SWITCH callsignSwitch13, bool txMsgSwitch13, const wxString& link13,
				 const wxString& starNetBand14, const wxString& callsign14, const wxString& logoff14, const wxString& info14, const wxString& permanent14, unsigned int userTimeout14, unsigned int groupTimeout14, STARNET_CALLSIGN_SWITCH callsignSwitch14, bool txMsgSwitch14, const wxString& link14,
				 const wxString& starNetBand15, const wxString& callsign15, const wxString& logoff15, const wxString& info15, const wxString& permanent15, unsigned int userTimeout15, unsigned int groupTimeout15, STARNET_CALLSIGN_SWITCH callsignSwitch15, bool txMsgSwitch15, const wxString& link15,
#else
				 const wxString& starNetBand1,  const wxString& callsign1,  const wxString& logoff1,  const wxString& info1,  const wxString& permanent1,  unsigned int userTimeout1,  unsigned int groupTimeout1, STARNET_CALLSIGN_SWITCH callsignSwitch1, bool txMsgSwitch1,
				 const wxString& starNetBand2,  const wxString& callsign2,  const wxString& logoff2,  const wxString& info2,  const wxString& permanent2,  unsigned int userTimeout2,  unsigned int groupTimeout2, STARNET_CALLSIGN_SWITCH callsignSwitch2, bool txMsgSwitch2,
				 const wxString& starNetBand3,  const wxString& callsign3,  const wxString& logoff3,  const wxString& info3,  const wxString& permanent3,  unsigned int userTimeout3,  unsigned int groupTimeout3, STARNET_CALLSIGN_SWITCH callsignSwitch3, bool txMsgSwitch3,
				 const wxString& starNetBand4,  const wxString& callsign4,  const wxString& logoff4,  const wxString& info4,  const wxString& permanent4,  unsigned int userTimeout4,  unsigned int groupTimeout4, STARNET_CALLSIGN_SWITCH callsignSwitch4, bool txMsgSwitch4,
				 const wxString& starNetBand5,  const wxString& callsign5,  const wxString& logoff5,  const wxString& info5,  const wxString& permanent5,  unsigned int userTimeout5,  unsigned int groupTimeout5, STARNET_CALLSIGN_SWITCH callsignSwitch5, bool txMsgSwitch5,
				 const wxString& starNetBand6,  const wxString& callsign6,  const wxString& logoff6,  const wxString& info6,  const wxString& permanent6,  unsigned int userTimeout6,  unsigned int groupTimeout6, STARNET_CALLSIGN_SWITCH callsignSwitch6, bool txMsgSwitch6,
				 const wxString& starNetBand7,  const wxString& callsign7,  const wxString& logoff7,  const wxString& info7,  const wxString& permanent7,  unsigned int userTimeout7,  unsigned int groupTimeout7, STARNET_CALLSIGN_SWITCH callsignSwitch7, bool txMsgSwitch7,
				 const wxString& starNetBand8,  const wxString& callsign8,  const wxString& logoff8,  const wxString& info8,  const wxString& permanent8,  unsigned int userTimeout8,  unsigned int groupTimeout8, STARNET_CALLSIGN_SWITCH callsignSwitch8, bool txMsgSwitch8,
				 const wxString& starNetBand9,  const wxString& callsign9,  const wxString& logoff9,  const wxString& info9,  const wxString& permanent9,  unsigned int userTimeout9,  unsigned int groupTimeout9, STARNET_CALLSIGN_SWITCH callsignSwitch9, bool txMsgSwitch9,
				 const wxString& starNetBand10, const wxString& callsign10, const wxString& logoff10, const wxString& info10, const wxString& permanent10, unsigned int userTimeout10,  unsigned int groupTimeout10, STARNET_CALLSIGN_SWITCH callsignSwitch10, bool txMsgSwitch10,
				 const wxString& starNetBand11, const wxString& callsign11, const wxString& logoff11, const wxString& info11, const wxString& permanent11, unsigned int userTimeout11,  unsigned int groupTimeout11, STARNET_CALLSIGN_SWITCH callsignSwitch11, bool txMsgSwitch11,
				 const wxString& starNetBand12, const wxString& callsign12, const wxString& logoff12, const wxString& info12, const wxString& permanent12, unsigned int userTimeout12,  unsigned int groupTimeout12, STARNET_CALLSIGN_SWITCH callsignSwitch12, bool txMsgSwitch12,
				 const wxString& starNetBand13, const wxString& callsign13, const wxString& logoff13, const wxString& info13, const wxString& permanent13, unsigned int userTimeout13,  unsigned int groupTimeout13, STARNET_CALLSIGN_SWITCH callsignSwitch13, bool txMsgSwitch13,
				 const wxString& starNetBand14, const wxString& callsign14, const wxString& logoff14, const wxString& info14, const wxString& permanent14, unsigned int userTimeout14,  unsigned int groupTimeout14, STARNET_CALLSIGN_SWITCH callsignSwitch14, bool txMsgSwitch14,
				 const wxString& starNetBand15, const wxString& callsign15, const wxString& logoff15, const wxString& info15, const wxString& permanent15, unsigned int userTimeout15,  unsigned int groupTimeout15, STARNET_CALLSIGN_SWITCH callsignSwitch15, bool txMsgSwitch15,
#endif
				 bool remoteEnabled, const wxString& remotePassword, unsigned int remotePort);
	virtual ~CStarNetServerPreferences();

	virtual bool Validate();

	virtual wxString     getCallsign() const;
	virtual wxString     getAddress() const;

	virtual wxString     getHostname() const;
	virtual wxString     getUsername() const;
	virtual wxString     getPassword() const;

	virtual bool         getLogEnabled() const;

	virtual wxString     getStarNetBand1() const;
	virtual wxString     getStarNetCallsign1() const;
	virtual wxString     getStarNetLogoff1() const;
	virtual wxString     getStarNetInfo1() const;
	virtual wxString     getStarNetPermanent1() const;
	virtual unsigned int getStarNetUserTimeout1() const;
	virtual unsigned int getStarNetGroupTimeout1() const;
	virtual STARNET_CALLSIGN_SWITCH getStarNetCallsignSwitch1() const;
	virtual bool         getStarNetTXMsgSwitch1() const;
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	virtual wxString     getStarNetLink1() const;
#endif

	virtual wxString     getStarNetBand2() const;
	virtual wxString     getStarNetCallsign2() const;
	virtual wxString     getStarNetLogoff2() const;
	virtual wxString     getStarNetInfo2() const;
	virtual wxString     getStarNetPermanent2() const;
	virtual unsigned int getStarNetUserTimeout2() const;
	virtual unsigned int getStarNetGroupTimeout2() const;
	virtual STARNET_CALLSIGN_SWITCH getStarNetCallsignSwitch2() const;
	virtual bool         getStarNetTXMsgSwitch2() const;
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	virtual wxString     getStarNetLink2() const;
#endif

	virtual wxString     getStarNetBand3() const;
	virtual wxString     getStarNetCallsign3() const;
	virtual wxString     getStarNetLogoff3() const;
	virtual wxString     getStarNetInfo3() const;
	virtual wxString     getStarNetPermanent3() const;
	virtual unsigned int getStarNetUserTimeout3() const;
	virtual unsigned int getStarNetGroupTimeout3() const;
	virtual STARNET_CALLSIGN_SWITCH getStarNetCallsignSwitch3() const;
	virtual bool         getStarNetTXMsgSwitch3() const;
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	virtual wxString     getStarNetLink3() const;
#endif

	virtual wxString     getStarNetBand4() const;
	virtual wxString     getStarNetCallsign4() const;
	virtual wxString     getStarNetLogoff4() const;
	virtual wxString     getStarNetInfo4() const;
	virtual wxString     getStarNetPermanent4() const;
	virtual unsigned int getStarNetUserTimeout4() const;
	virtual unsigned int getStarNetGroupTimeout4() const;
	virtual STARNET_CALLSIGN_SWITCH getStarNetCallsignSwitch4() const;
	virtual bool         getStarNetTXMsgSwitch4() const;
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	virtual wxString     getStarNetLink4() const;
#endif

	virtual wxString     getStarNetBand5() const;
	virtual wxString     getStarNetCallsign5() const;
	virtual wxString     getStarNetLogoff5() const;
	virtual wxString     getStarNetInfo5() const;
	virtual wxString     getStarNetPermanent5() const;
	virtual unsigned int getStarNetUserTimeout5() const;
	virtual unsigned int getStarNetGroupTimeout5() const;
	virtual STARNET_CALLSIGN_SWITCH getStarNetCallsignSwitch5() const;
	virtual bool         getStarNetTXMsgSwitch5() const;
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	virtual wxString     getStarNetLink5() const;
#endif

	virtual wxString     getStarNetBand6() const;
	virtual wxString     getStarNetCallsign6() const;
	virtual wxString     getStarNetLogoff6() const;
	virtual wxString     getStarNetInfo6() const;
	virtual wxString     getStarNetPermanent6() const;
	virtual unsigned int getStarNetUserTimeout6() const;
	virtual unsigned int getStarNetGroupTimeout6() const;
	virtual STARNET_CALLSIGN_SWITCH getStarNetCallsignSwitch6() const;
	virtual bool         getStarNetTXMsgSwitch6() const;
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	virtual wxString     getStarNetLink6() const;
#endif

	virtual wxString     getStarNetBand7() const;
	virtual wxString     getStarNetCallsign7() const;
	virtual wxString     getStarNetLogoff7() const;
	virtual wxString     getStarNetInfo7() const;
	virtual wxString     getStarNetPermanent7() const;
	virtual unsigned int getStarNetUserTimeout7() const;
	virtual unsigned int getStarNetGroupTimeout7() const;
	virtual STARNET_CALLSIGN_SWITCH getStarNetCallsignSwitch7() const;
	virtual bool         getStarNetTXMsgSwitch7() const;
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	virtual wxString     getStarNetLink7() const;
#endif

	virtual wxString     getStarNetBand8() const;
	virtual wxString     getStarNetCallsign8() const;
	virtual wxString     getStarNetLogoff8() const;
	virtual wxString     getStarNetInfo8() const;
	virtual wxString     getStarNetPermanent8() const;
	virtual unsigned int getStarNetUserTimeout8() const;
	virtual unsigned int getStarNetGroupTimeout8() const;
	virtual STARNET_CALLSIGN_SWITCH getStarNetCallsignSwitch8() const;
	virtual bool         getStarNetTXMsgSwitch8() const;
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	virtual wxString     getStarNetLink8() const;
#endif

	virtual wxString     getStarNetBand9() const;
	virtual wxString     getStarNetCallsign9() const;
	virtual wxString     getStarNetLogoff9() const;
	virtual wxString     getStarNetInfo9() const;
	virtual wxString     getStarNetPermanent9() const;
	virtual unsigned int getStarNetUserTimeout9() const;
	virtual unsigned int getStarNetGroupTimeout9() const;
	virtual STARNET_CALLSIGN_SWITCH getStarNetCallsignSwitch9() const;
	virtual bool         getStarNetTXMsgSwitch9() const;
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	virtual wxString     getStarNetLink9() const;
#endif

	virtual wxString     getStarNetBand10() const;
	virtual wxString     getStarNetCallsign10() const;
	virtual wxString     getStarNetLogoff10() const;
	virtual wxString     getStarNetInfo10() const;
	virtual wxString     getStarNetPermanent10() const;
	virtual unsigned int getStarNetUserTimeout10() const;
	virtual unsigned int getStarNetGroupTimeout10() const;
	virtual STARNET_CALLSIGN_SWITCH getStarNetCallsignSwitch10() const;
	virtual bool         getStarNetTXMsgSwitch10() const;
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	virtual wxString     getStarNetLink10() const;
#endif

	virtual wxString     getStarNetBand11() const;
	virtual wxString     getStarNetCallsign11() const;
	virtual wxString     getStarNetLogoff11() const;
	virtual wxString     getStarNetInfo11() const;
	virtual wxString     getStarNetPermanent11() const;
	virtual unsigned int getStarNetUserTimeout11() const;
	virtual unsigned int getStarNetGroupTimeout11() const;
	virtual STARNET_CALLSIGN_SWITCH getStarNetCallsignSwitch11() const;
	virtual bool         getStarNetTXMsgSwitch11() const;
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	virtual wxString     getStarNetLink11() const;
#endif

	virtual wxString     getStarNetBand12() const;
	virtual wxString     getStarNetCallsign12() const;
	virtual wxString     getStarNetLogoff12() const;
	virtual wxString     getStarNetInfo12() const;
	virtual wxString     getStarNetPermanent12() const;
	virtual unsigned int getStarNetUserTimeout12() const;
	virtual unsigned int getStarNetGroupTimeout12() const;
	virtual STARNET_CALLSIGN_SWITCH getStarNetCallsignSwitch12() const;
	virtual bool         getStarNetTXMsgSwitch12() const;
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	virtual wxString     getStarNetLink12() const;
#endif

	virtual wxString     getStarNetBand13() const;
	virtual wxString     getStarNetCallsign13() const;
	virtual wxString     getStarNetLogoff13() const;
	virtual wxString     getStarNetInfo13() const;
	virtual wxString     getStarNetPermanent13() const;
	virtual unsigned int getStarNetUserTimeout13() const;
	virtual unsigned int getStarNetGroupTimeout13() const;
	virtual STARNET_CALLSIGN_SWITCH getStarNetCallsignSwitch13() const;
	virtual bool         getStarNetTXMsgSwitch13() const;
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	virtual wxString     getStarNetLink13() const;
#endif

	virtual wxString     getStarNetBand14() const;
	virtual wxString     getStarNetCallsign14() const;
	virtual wxString     getStarNetLogoff14() const;
	virtual wxString     getStarNetInfo14() const;
	virtual wxString     getStarNetPermanent14() const;
	virtual unsigned int getStarNetUserTimeout14() const;
	virtual unsigned int getStarNetGroupTimeout14() const;
	virtual STARNET_CALLSIGN_SWITCH getStarNetCallsignSwitch14() const;
	virtual bool         getStarNetTXMsgSwitch14() const;
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	virtual wxString     getStarNetLink14() const;
#endif

	virtual wxString     getStarNetBand15() const;
	virtual wxString     getStarNetCallsign15() const;
	virtual wxString     getStarNetLogoff15() const;
	virtual wxString     getStarNetInfo15() const;
	virtual wxString     getStarNetPermanent15() const;
	virtual unsigned int getStarNetUserTimeout15() const;
	virtual unsigned int getStarNetGroupTimeout15() const;
	virtual STARNET_CALLSIGN_SWITCH getStarNetCallsignSwitch15() const;
	virtual bool         getStarNetTXMsgSwitch15() const;
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	virtual wxString     getStarNetLink15() const;
#endif

	virtual bool         getRemoteEnabled() const;
	virtual wxString     getRemotePassword() const;
	virtual unsigned int getRemotePort() const;

private:
	CStarNetServerCallsignSet*      m_callsign;
	CStarNetServerIrcDDBSet*        m_ircDDB;
	CStarNetSet*                    m_starNet1;
	CStarNetSet*                    m_starNet2;
	CStarNetSet*                    m_starNet3;
	CStarNetSet*                    m_starNet4;
	CStarNetSet*                    m_starNet5;
	CStarNetSet*                    m_starNet6;
	CStarNetSet*                    m_starNet7;
	CStarNetSet*                    m_starNet8;
	CStarNetSet*                    m_starNet9;
	CStarNetSet*                    m_starNet10;
	CStarNetSet*                    m_starNet11;
	CStarNetSet*                    m_starNet12;
	CStarNetSet*                    m_starNet13;
	CStarNetSet*                    m_starNet14;
	CStarNetSet*                    m_starNet15;
	CRemoteSet*                     m_remote;
	CStarNetServerMiscellaneousSet* m_miscellaneous;
};

#endif
