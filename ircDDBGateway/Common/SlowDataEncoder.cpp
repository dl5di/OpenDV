/*
 *	Copyright (C) 2010 by Jonathan Naylor, G4KLX
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

#include "SlowDataEncoder.h"
#include "CCITTChecksum.h"
#include "DStarDefines.h"

const unsigned int  SLOW_DATA_BLOCK_SIZE = 6U;

const unsigned int  SLOW_DATA_FULL_BLOCK_SIZE = SLOW_DATA_BLOCK_SIZE * 10U;

const unsigned char SLOW_DATA_FILLER_BYTES[] = {'f', 'f', 'f' };

const unsigned int HEADER_SIZE = 54U;
const unsigned int TEXT_SIZE = 24U;

CSlowDataEncoder::CSlowDataEncoder() :
m_headerData(NULL),
m_textData(NULL),
m_gpsData(NULL),
m_interleavedData(NULL),
m_headerPtr(0U),
m_textPtr(0U),
m_gpsPtr(0U),
m_interleavedPtr(0U),
m_gpsDataSize(0U),
m_gpsDataFullSize(0U),
m_interleavedDataFullSize(0U)
{
}

CSlowDataEncoder::~CSlowDataEncoder()
{
	reset();
}

void CSlowDataEncoder::clearHeaderData()
{
	if(m_headerData)
	{
		delete[] m_headerData;
		m_headerData = NULL;
		m_headerPtr = 0U;
	}
}

void CSlowDataEncoder::clearTextData()
{
	if(m_textData)
	{
		delete[] m_textData;
		m_textData = NULL;
		m_textPtr = 0U;
	}
}

void CSlowDataEncoder::clearGPSData()
{
	if(m_gpsData)
	{
		delete[] m_gpsData;
		m_gpsData = NULL;
		m_gpsPtr = 0U;
	}
}

void CSlowDataEncoder::clearInterleavedData()
{
	if(m_interleavedData)
	{
		delete[] m_interleavedData;
		m_interleavedData = NULL;
		m_interleavedPtr = 0U;
	}
}

void CSlowDataEncoder::getData(unsigned char* source, unsigned char* data, unsigned int &sourcePtr, unsigned int sourceLength)
{
	wxASSERT(data != NULL);
	if(source != NULL){
		data[0U] = source[sourcePtr++] ^ SCRAMBLER_BYTE1;
		data[1U] = source[sourcePtr++] ^ SCRAMBLER_BYTE2;
		data[2U] = source[sourcePtr++] ^ SCRAMBLER_BYTE3;
	}
	else{
		data[0U] = SLOW_DATA_FILLER_BYTES[sourcePtr++] ^ SCRAMBLER_BYTE1;
		data[1U] = SLOW_DATA_FILLER_BYTES[sourcePtr++] ^ SCRAMBLER_BYTE2;
		data[2U] = SLOW_DATA_FILLER_BYTES[sourcePtr++] ^ SCRAMBLER_BYTE3;
	}

	if (sourcePtr >= sourceLength)
		sourcePtr = 0U;
}

void CSlowDataEncoder::getHeaderData(unsigned char* data)
{
	getData(m_headerData, data, m_headerPtr, SLOW_DATA_FULL_BLOCK_SIZE);
}

void CSlowDataEncoder::getTextData(unsigned char* data)
{
	getData(m_textData, data, m_textPtr, SLOW_DATA_FULL_BLOCK_SIZE);
}

void CSlowDataEncoder::getGPSData(unsigned char* data)
{
	getData(m_gpsData, data, m_gpsPtr, m_gpsDataFullSize);
}

void CSlowDataEncoder::getInterleavedData(unsigned char* data)
{
	if(m_textData && !m_gpsData && !m_headerData)
		getTextData(data);
	else if(!m_textData && m_gpsData && !m_headerData)
		getGPSData(data);
	else if(!m_textData && !m_gpsData && m_headerData)
		getHeaderData(data);
	else {
		buildInterleavedData();
		getData(m_interleavedData, data, m_interleavedPtr, m_gpsDataFullSize);
	}
}

void CSlowDataEncoder::buildInterleavedData()
{
	//first build interleaved data if we do not have it
	if(!m_interleavedData)
	{	
		getInterleavedDataLength();
		m_interleavedData = new unsigned char[m_interleavedDataFullSize];
		::memset(m_interleavedData, 'f', m_interleavedDataFullSize);

		unsigned int textPtr = 0U;
		unsigned int gpsPtr = 0U;
		unsigned int headerPtr = 0U;

		//now proceed with data copying, according to this document http://www.qsl.net/kb9mwr/projects/dv/dstar/Slow%20Data.pdf
		if(m_textData && m_gpsData){
			for(unsigned int interleavedPtr = 0; interleavedPtr < m_interleavedDataFullSize; interleavedPtr += SLOW_DATA_BLOCK_SIZE){
				if(textPtr < TEXT_SIZE
				   && ((interleavedPtr / SLOW_DATA_BLOCK_SIZE) & 0x01U) == 0)
				{
					::memcpy(m_interleavedData + interleavedPtr, m_textData + textPtr, SLOW_DATA_BLOCK_SIZE);
					textPtr += SLOW_DATA_BLOCK_SIZE;
				}
				else if(gpsPtr < m_gpsDataSize){
					::memcpy(m_interleavedData + interleavedPtr, m_gpsData + gpsPtr, SLOW_DATA_BLOCK_SIZE);
					gpsPtr += SLOW_DATA_BLOCK_SIZE;
				}
				else if(m_headerData && headerPtr < HEADER_SIZE){
					::memcpy(m_interleavedData + interleavedPtr, m_headerData + headerPtr, SLOW_DATA_BLOCK_SIZE);
					headerPtr += SLOW_DATA_BLOCK_SIZE;
				}
			}
		}
		else if(m_textData && !m_gpsData && m_headerData){
			//according to above doc, header and text are not interleaved, just on after the other. filler bytes between resync bytes.
			::memcpy(m_interleavedData, m_textData, SLOW_DATA_FULL_BLOCK_SIZE);
			::memcpy(m_interleavedData + SLOW_DATA_FULL_BLOCK_SIZE, m_headerData, SLOW_DATA_FULL_BLOCK_SIZE);
		}
		else if(!m_textData && m_gpsData && m_headerData){
			//could not find any spec about this particular case, let's put the data one after the other
			::memcpy(m_interleavedData, m_gpsData, SLOW_DATA_FULL_BLOCK_SIZE);
			::memcpy(m_interleavedData + SLOW_DATA_FULL_BLOCK_SIZE, m_headerData, SLOW_DATA_FULL_BLOCK_SIZE);
		}
	}
}

unsigned int CSlowDataEncoder::getInterleavedDataLength()
{
	//calculate size (including filler bytes);
	m_interleavedDataFullSize = 0U;
	if(m_textData) m_interleavedDataFullSize += TEXT_SIZE;
	if(m_headerData) m_interleavedDataFullSize += HEADER_SIZE;
	if(m_gpsData) m_interleavedDataFullSize += m_gpsDataSize;
	m_interleavedDataFullSize = SLOW_DATA_FULL_BLOCK_SIZE * (1U + ((m_interleavedDataFullSize - 1U) / SLOW_DATA_FULL_BLOCK_SIZE));
	return m_interleavedDataFullSize;
}

void CSlowDataEncoder::sync()
{
	m_headerPtr = 0U;
	m_textPtr   = 0U;
	m_gpsPtr    = 0U;
	m_interleavedPtr = 0U;
}

void CSlowDataEncoder::reset()
{
	clearHeaderData();
	clearTextData();
	clearGPSData();
	clearInterleavedData();
}

void CSlowDataEncoder::setHeaderData(const CHeaderData& header)
{
	clearInterleavedData();
	if(!m_headerData) m_headerData = new unsigned char[SLOW_DATA_FULL_BLOCK_SIZE];
	::memset(m_headerData, 'f', SLOW_DATA_FULL_BLOCK_SIZE);

	m_headerData[0U]  = SLOW_DATA_TYPE_HEADER | 5U;
	m_headerData[1U]  = header.getFlag1();
	m_headerData[2U]  = header.getFlag2();
	m_headerData[3U]  = header.getFlag3();
	m_headerData[4U]  = header.getRptCall2().GetChar(0);
	m_headerData[5U]  = header.getRptCall2().GetChar(1);

	m_headerData[6U]  = SLOW_DATA_TYPE_HEADER | 5U;
	m_headerData[7U]  = header.getRptCall2().GetChar(2);
	m_headerData[8U]  = header.getRptCall2().GetChar(3);
	m_headerData[9U]  = header.getRptCall2().GetChar(4);
	m_headerData[10U] = header.getRptCall2().GetChar(5);
	m_headerData[11U] = header.getRptCall2().GetChar(6);

	m_headerData[12U] = SLOW_DATA_TYPE_HEADER | 5U;
	m_headerData[13U] = header.getRptCall2().GetChar(7);
	m_headerData[14U] = header.getRptCall1().GetChar(0);
	m_headerData[15U] = header.getRptCall1().GetChar(1);
	m_headerData[16U] = header.getRptCall1().GetChar(2);
	m_headerData[17U] = header.getRptCall1().GetChar(3);

	m_headerData[18U] = SLOW_DATA_TYPE_HEADER | 5U;
	m_headerData[19U] = header.getRptCall1().GetChar(4);
	m_headerData[20U] = header.getRptCall1().GetChar(5);
	m_headerData[21U] = header.getRptCall1().GetChar(6);
	m_headerData[22U] = header.getRptCall1().GetChar(7);
	m_headerData[23U] = header.getYourCall().GetChar(0);

	m_headerData[24U] = SLOW_DATA_TYPE_HEADER | 5U;
	m_headerData[25U] = header.getYourCall().GetChar(1);
	m_headerData[26U] = header.getYourCall().GetChar(2);
	m_headerData[27U] = header.getYourCall().GetChar(3);
	m_headerData[28U] = header.getYourCall().GetChar(4);
	m_headerData[29U] = header.getYourCall().GetChar(5);

	m_headerData[30U] = SLOW_DATA_TYPE_HEADER | 5U;
	m_headerData[31U] = header.getYourCall().GetChar(6);
	m_headerData[32U] = header.getYourCall().GetChar(7);
	m_headerData[33U] = header.getMyCall1().GetChar(0);
	m_headerData[34U] = header.getMyCall1().GetChar(1);
	m_headerData[35U] = header.getMyCall1().GetChar(2);

	m_headerData[36U] = SLOW_DATA_TYPE_HEADER | 5U;
	m_headerData[37U] = header.getMyCall1().GetChar(3);
	m_headerData[38U] = header.getMyCall1().GetChar(4);
	m_headerData[39U] = header.getMyCall1().GetChar(5);
	m_headerData[40U] = header.getMyCall1().GetChar(6);
	m_headerData[41U] = header.getMyCall1().GetChar(7);

	m_headerData[42U] = SLOW_DATA_TYPE_HEADER | 5U;
	m_headerData[43U] = header.getMyCall2().GetChar(0);
	m_headerData[44U] = header.getMyCall2().GetChar(1);
	m_headerData[45U] = header.getMyCall2().GetChar(2);
	m_headerData[46U] = header.getMyCall2().GetChar(3);

	CCCITTChecksum cksum;
	cksum.update(m_headerData + 1U, 5U);
	cksum.update(m_headerData + 7U, 5U);
	cksum.update(m_headerData + 13U, 5U);
	cksum.update(m_headerData + 19U, 5U);
	cksum.update(m_headerData + 25U, 5U);
	cksum.update(m_headerData + 31U, 5U);
	cksum.update(m_headerData + 37U, 5U);
	cksum.update(m_headerData + 43U, 4U);

	unsigned char checkSum[2U];
	cksum.result(checkSum);

	m_headerData[47U] = checkSum[0];

	m_headerData[48U] = SLOW_DATA_TYPE_HEADER | 1U;
	m_headerData[49U] = checkSum[1];

	m_headerPtr = 0U;
}

void CSlowDataEncoder::setTextData(const wxString& text)
{
	clearInterleavedData();
	if(!m_textData) m_textData = new unsigned char[SLOW_DATA_FULL_BLOCK_SIZE];
	::memset(m_textData, 'f', SLOW_DATA_FULL_BLOCK_SIZE);

	wxString paddedText = text;
	paddedText.Append(wxT("                      "));
	paddedText.Truncate(20U);

	m_textData[0U]  = SLOW_DATA_TYPE_TEXT | 0U;
	m_textData[1U]  = paddedText.GetChar(0);
	m_textData[2U]  = paddedText.GetChar(1);
	m_textData[3U]  = paddedText.GetChar(2);
	m_textData[4U]  = paddedText.GetChar(3);
	m_textData[5U]  = paddedText.GetChar(4);

	m_textData[6U]  = SLOW_DATA_TYPE_TEXT | 1U;
	m_textData[7U]  = paddedText.GetChar(5);
	m_textData[8U]  = paddedText.GetChar(6);
	m_textData[9U]  = paddedText.GetChar(7);
	m_textData[10U] = paddedText.GetChar(8);
	m_textData[11U] = paddedText.GetChar(9);

	m_textData[12U] = SLOW_DATA_TYPE_TEXT | 2U;
	m_textData[13U] = paddedText.GetChar(10);
	m_textData[14U] = paddedText.GetChar(11);
	m_textData[15U] = paddedText.GetChar(12);
	m_textData[16U] = paddedText.GetChar(13);
	m_textData[17U] = paddedText.GetChar(14);

	m_textData[18U] = SLOW_DATA_TYPE_TEXT | 3U;
	m_textData[19U] = paddedText.GetChar(15);
	m_textData[20U] = paddedText.GetChar(16);
	m_textData[21U] = paddedText.GetChar(17);
	m_textData[22U] = paddedText.GetChar(18);
	m_textData[23U] = paddedText.GetChar(19);

	m_textPtr = 0U;
}

void CSlowDataEncoder::setGPSData(const wxString& gpsData)
{
	size_t gpsDataStrLen;
	clearInterleavedData();

	if(m_gpsData) delete[] m_gpsData;
	m_gpsDataSize = 0U;
	m_gpsPtr = 0U;

	if((gpsDataStrLen = gpsData.length()) > 0){
		unsigned int gpsDataPos;
		unsigned int strPos = 0;
		m_gpsDataSize = 1U + ((gpsDataStrLen - 1U) / 6U);//to make room for the type bytes
		m_gpsDataSize += gpsDataStrLen;
		m_gpsDataFullSize = SLOW_DATA_FULL_BLOCK_SIZE * (1U + ((m_gpsDataSize - 1U) / SLOW_DATA_FULL_BLOCK_SIZE));

		m_gpsData = new unsigned char[m_gpsDataFullSize];
		::memset(m_gpsData, 'f', m_gpsDataFullSize);

		for(gpsDataPos = 0; gpsDataPos < m_gpsDataFullSize;){
			unsigned int dataLen = gpsDataStrLen - strPos < 5U ? gpsDataStrLen - strPos : 5U;
			m_gpsData[gpsDataPos++] = SLOW_DATA_TYPE_GPS | dataLen;

			for(unsigned int i = 0U; i < dataLen; i++){
				m_gpsData[gpsDataPos++] = gpsData.GetChar(strPos++);
			}
		}
	}
}

