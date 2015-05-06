/*
 *   Copyright (C) 2014 by Rick Schnicker KD0OSS
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

#include <stdio.h>
#include <unistd.h>
#include "ftd2xx.h"

#define MAX_DEVICES	5


FT_HANDLE sdv_open(int iport, int mode, int baud, int reset)
{
    FT_HANDLE	ftHandle;
    FT_STATUS	ftStatus;
    DWORD dwBytesReceived;
    char        cBytes[10];
    int	iNumDevs = 0;
    int	i;
    char * 	pcBufLD[MAX_DEVICES + 1];
    char 	cBufLD[MAX_DEVICES][64];
    unsigned char Lat;
    
    for(i = 0; i < MAX_DEVICES; i++) 
    {
        pcBufLD[i] = cBufLD[i];
    }
    pcBufLD[MAX_DEVICES] = NULL;
    
    ftStatus = FT_ListDevices(pcBufLD, &iNumDevs, FT_LIST_ALL | FT_OPEN_BY_SERIAL_NUMBER);	
    if(ftStatus != FT_OK) 
    {
        printf("Error: FT_ListDevices returned %d\n", (int)ftStatus);
        return 0;
    }
    
    for(i = 0; ( (i <MAX_DEVICES) && (i < iNumDevs) ); i++) 
    {
        printf("Device %d Serial Number - %s\n", i, cBufLD[i]);
    }
    
    ftStatus = FT_SetVIDPID(0x0403, 0x7b38);
    if (ftStatus != FT_OK)
    {
        printf("FT_SetVIDPID() failed, with error %d.\n", (int)ftStatus);
    }
    
    ftStatus = FT_Open(iport, &ftHandle);
    if (ftStatus != FT_OK)
    {
        /*
            This can fail if the ftdi_sio driver is loaded
            use lsmod to check this and rmmod ftdi_sio to remove
            also rmmod usbserial
         */
        printf("FT_OpenEx(%d) failed, with error %d.\n", iport, (int)ftStatus);
        printf("Use lsmod to check if ftdi_sio (and usbserial) are present.\n");
        printf("If so, unload them using rmmod, as they conflict with ftd2xx.\n");
        return 0;
    }
    
    if (reset)
    {
        ftStatus = FT_SetBitMode(ftHandle, 0x80, mode);
        if (ftStatus != FT_OK)
        {
            printf("Failed to set bit mode\n");
            return 0;
        }
        
        usleep(100);
        
        ftStatus = FT_SetBitMode(ftHandle, 0x88, mode);
        if (ftStatus != FT_OK)
        {
            printf("Failed to set bit mode\n");
            return 0;
        }
        
        usleep(10000);
    }
    
    ftStatus = FT_SetBaudRate(ftHandle, baud);
    if (ftStatus != FT_OK)
    {
        printf("Failed to set baud rate (error %d)\n", (int)ftStatus);
        return 0;
    }
    
    // Set 8 data bits, 1 stop bit and no parity
    ftStatus = FT_SetDataCharacteristics(ftHandle, FT_BITS_8, FT_STOP_BITS_1, FT_PARITY_NONE);
    if (ftStatus != FT_OK)
    {
        // FT_SetDataCharacteristics Failed
        printf("Failed to set data characteristics (error %d)\n", (int)ftStatus);
        return 0;
    }
    
    FT_SetTimeouts(ftHandle, 500, 0);
    ftStatus = FT_Read(ftHandle, cBytes, 5, &dwBytesReceived);
    if (ftStatus != FT_OK)
    {
        // FT_Read Timeout
        printf("Read failed.\n");
        return 0;
    }
    
    FT_SetTimeouts(ftHandle, 20, 0);
    
    Lat = 5;
    ftStatus = FT_SetLatencyTimer(ftHandle, Lat);
    ftStatus = FT_GetLatencyTimer(ftHandle, &Lat);
    if (ftStatus == FT_OK) 
    {
        // LatencyTimer contains current value
  //      printf("Latency: %d\n", Lat);
    }
    fflush(stdout);
    return ftHandle;
} // end sdv_open


void sdv_close(FT_HANDLE ftHandle)
{
    FT_Close(ftHandle);
} // end sdv_close


int sdv_write(FT_HANDLE ftHandle, unsigned char *ucBytes, int length, DWORD *dwBytesInQueue)
{
    FT_STATUS	ftStatus;
    
    ftStatus = FT_Write(ftHandle, ucBytes, length, dwBytesInQueue);
    if (ftStatus != FT_OK)
    {
        printf("Failed to write (error %d)\n", (int)ftStatus);
        return -1;;
    }
    return 0;
} // end sdv_write


int sdv_read(FT_HANDLE ftHandle, char *cBytes, int length, int timeout, DWORD *dwBytesReceived)
{
    FT_STATUS	ftStatus;
    
    FT_SetTimeouts(ftHandle, timeout, 0);
    ftStatus = FT_Read(ftHandle, cBytes, length, dwBytesReceived);
    if (ftStatus != FT_OK)
    {
        // FT_Read Timeout
        printf("Read failed.\n");
        return -1;
    }
    return 0;
} // end sdv_read


unsigned char sdv_get_bitmode(FT_HANDLE ftHandle)
{
    FT_STATUS	ftStatus;
    unsigned char ucMode;
    
    ftStatus = FT_GetBitMode(ftHandle, &ucMode);
    if (ftStatus != FT_OK)
    {
        printf("Failed to get bit mode\n");
        return 0;
    }
    return ucMode;
} // end sdv_get_bitmode


short int is_ptt(FT_HANDLE ftHandle)
{
    FT_STATUS	ftStatus;
    unsigned char ucMode;
    
    ftStatus = FT_GetBitMode(ftHandle, &ucMode);
    if (ftStatus != FT_OK)
    {
        printf("Failed to get PTT status\n");
        return 0;
    }
    
    return !(ucMode & 0x01);
} // end is_ptt
