/*
 *   Copyright (C) 2009,2013,2015 by Jonathan Naylor G4KLX
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

#ifndef	HeaderData_H
#define	HeaderData_H

#include <string>

class CHeaderData {
public:
	CHeaderData();
	CHeaderData(const CHeaderData& header);
	CHeaderData(const unsigned char* data, unsigned int length, bool check);
	CHeaderData(const std::string& myCall1,  const std::string& myCall2, const std::string& yourCall,
				const std::string& rptCall1, const std::string& rptCall2, unsigned char flag1 = 0x00,
				unsigned char flag2 = 0x00, unsigned char flag3 = 0x00);
	~CHeaderData();

	std::string getMyCall1() const;
	std::string getMyCall2() const;
	std::string getYourCall() const;
	std::string getRptCall1() const;
	std::string getRptCall2() const;

	unsigned char getFlag1() const;
	unsigned char getFlag2() const;
	unsigned char getFlag3() const;

	bool isAck() const;
	bool isNoResponse() const;
	bool isRelayUnavailable() const;
	bool isRepeaterMode() const;
	bool isDataPacket() const;
	bool isInterrupted() const;
	bool isControlSignal() const;
	bool isUrgent() const;
	unsigned char getRepeaterFlags() const;

	void setFlag1(unsigned char flag);
	void setFlag2(unsigned char flag);
	void setFlag3(unsigned char flag);

	void setMyCall1(const std::string& callsign);
	void setMyCall2(const std::string& callsign);
	void setYourCall(const std::string& callsign);
	void setRptCall1(const std::string& callsign);
	void setRptCall2(const std::string& callsign);

	void setRepeaterMode(bool set);
	void setDataPacket(bool set);
	void setInterrupted(bool set);
	void setControlSignal(bool set);
	void setUrgent(bool set);
	void setRepeaterFlags(unsigned char set);

	bool isValid() const;

	void reset();

	CHeaderData& operator=(const CHeaderData& header);

private:
	std::string   m_myCall1;
	std::string   m_myCall2;
	std::string   m_yourCall;
	std::string   m_rptCall1;
	std::string   m_rptCall2;
	unsigned char m_flag1;
	unsigned char m_flag2;
	unsigned char m_flag3;
	bool          m_valid;
};

#endif
