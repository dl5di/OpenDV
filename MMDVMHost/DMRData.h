/*
 *	Copyright (C) 2015 by Jonathan Naylor, G4KLX
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; version 2 of the License.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 */

#ifndef DMRData_H
#define	DMRData_H

#include "DMRDefines.h"

enum DMR_DATA_TYPE {
	DMRDT_VOICE_LC_HEADER,
	DMRDT_VOICE_PI_HEADER,
	DMRDT_VOICE_SYNC,
	DMRDT_VOICE,
	DMRDT_DATA_HEADER,
	DMRDT_CSBK,
	DMRDT_RATE12_DATA,
	DMRDT_RATE34_DATA,
	DMRDT_RATE1_DATA,
	DMRDT_TERMINATOR
};

class CDMRData {
public:
	CDMRData(const CDMRData& data);
	CDMRData();
	~CDMRData();

	CDMRData& operator=(const CDMRData& data);

	unsigned int getSlotNo() const;
	void setSlotNo(unsigned int slotNo);

	unsigned int getSrcId() const;
	void setSrcId(unsigned int id);

	unsigned int getDstId() const;
	void setDstId(unsigned int id);

	FLCO getFLCO() const;
	void setFLCO(FLCO flco);

	unsigned int getN() const;
	void setN(unsigned int n);

	DMR_DATA_TYPE getDataType() const;
	void setDataType(DMR_DATA_TYPE dataType);

	void setData(const unsigned char* buffer);
	unsigned int getData(unsigned char* buffer) const;

private:
	unsigned int   m_slotNo;
	unsigned char* m_data;
	unsigned int   m_srcId;
	unsigned int   m_dstId;
	FLCO           m_flco;
	DMR_DATA_TYPE  m_dataType;
	unsigned int   m_n;
};

#endif
