/*
 *   Copyright (C) 2015 by Rick Schnicker KD0OSS
 *   based on code by Jonathan Naylor G4KLX
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

#include <stdlib.h>
#include <getopt.h>
#include <signal.h>
#include "dvemu.h"

struct sockaddr_in sa;
int    sockFd;
bool   bExitRequested = false;

#pragma GCC diagnostic ignored "-Wunused-parameter"

/*
 * sigintHandler --
 *
 *    SIGINT handler, so we can gracefully exit when the user hits ctrl-C.
 */
static void sigintHandler(int signum)
{
    bExitRequested = true;
} // end sigintHandler


int setNonblocking(int fd)
{
    int flags;
    
    /* If they have O_NONBLOCK, use the Posix way to do it */
    #if defined(O_NONBLOCK)
    /* Fixme: O_NONBLOCK is defined but broken on SunOS 4.1.x and AIX 3.2.5. */
    if (-1 == (flags = fcntl(fd, F_GETFL, 0)))
        flags = 0;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    #else
    /* Otherwise, use the old way of doing it */
    flags = 1;
    return ioctl(fd, FIOBIO, &flags);
    #endif
}     


void dump(const char *text, const unsigned char *data, unsigned int length)
{
	unsigned int offset = 0U;
	unsigned int i;

	fputs(text, stdout);
	fputc('\n', stdout);

	while (length > 0U) {
		unsigned int bytes = (length > 16U) ? 16U : length;

		fprintf(stdout, "%04X:  ", offset);

		for (i = 0U; i < bytes; i++)
			fprintf(stdout, "%02X ", data[offset + i]);

		for (i = bytes; i < 16U; i++)
			fputs("   ", stdout);

		fputs("   *", stdout);

		for (i = 0U; i < bytes; i++) {
			unsigned char c = data[offset + i];

			if (isprint(c))
				fputc(c, stdout);
			else
				fputc('.', stdout);
		}

		fputs("*\n", stdout);

		offset += 16U;

		if (length >= 16U)
			length -= 16U;
		else
			length = 0U;
	}
} // end dump


int openSocket(uint16_t port, int debug)
{
	struct sockaddr_in sa;
	int ret;
	int fd;

	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd < 0) {
		fprintf(stderr, "dvemu: error when creating the socket, errno=%d\n", errno);
		return fd;
	}

	memset(&sa, 0x00, sizeof(struct sockaddr_in));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(port);
	sa.sin_addr.s_addr = htonl(INADDR_ANY);

	ret = bind(fd, (struct sockaddr *)&sa, sizeof(struct sockaddr_in));
	if (ret < 0) {
		fprintf(stderr, "dvemu: error when binding the socket to port %u, errno=%d\n", port, errno);
		return fd;
	}

	if (debug)
		fprintf(stdout, "opened the UDP socket on port %u\n", port);

    setNonblocking(fd);
	return fd;
} // end openSocket


int processSerial(FT_HANDLE ftHandle, int debug)
{
	unsigned char buffer[BUFFER_LENGTH];
	unsigned int  respLen;
	unsigned int  offset;
    unsigned int  len;
    DWORD         dwBytesReceived = 0;

    sdv_read(ftHandle, (char*)buffer, 1, 20, &dwBytesReceived);
    if (dwBytesReceived == 0)
        return 1;
	
	if (buffer[0U] != AMBE3000_START_BYTE) {
		fprintf(stderr, "dvemu: unknown byte from the DV3000, 0x%02X\n", buffer[0U]);
		return 1;
	}

	offset = 0U;
	while (offset < (AMBE3000_HEADER_LEN - 1U)) {
		sdv_read(ftHandle, (char*)buffer + 1U + offset, AMBE3000_HEADER_LEN - 1 - offset, 20, &dwBytesReceived);

		//if (dwBytesReceived == 0)
		//	usleep(50UL);

		offset += dwBytesReceived;
	}

	respLen = buffer[1U] * 256U + buffer[2U];

	offset = 0U;
	while (offset < respLen) {
		sdv_read(ftHandle, (char*)buffer + AMBE3000_HEADER_LEN + offset, respLen - offset, 20, &dwBytesReceived);

		//if (dwBytesReceived == 0)
		//	usleep(50UL);

		offset += dwBytesReceived;
	}

	respLen += AMBE3000_HEADER_LEN;

	if (debug)
		dump("Serial data", buffer, respLen);

	if (sa.sin_port != 0U) {
        buffer[1] |= is_ptt(ftHandle) << 4;
		len = sendto(sockFd, buffer, respLen, 0, (struct sockaddr *)&sa, sizeof(struct sockaddr_in));
		if (respLen != len) {
			fprintf(stderr, "dvemu: error when writing to the socket, len=%d, errno=%d\n", len, errno);
			return 0;
		}
	}

	return 1;
} // end processSerial


int processSocket(FT_HANDLE ftHandle, int debug)
{
	unsigned char buffer[BUFFER_LENGTH];
	socklen_t     sl = sizeof(struct sockaddr_in);
	unsigned int  len;
    DWORD         dwBytesInQueue = 0;
	ssize_t       n;
	
	n = recvfrom(sockFd, buffer, BUFFER_LENGTH, 0, (struct sockaddr *)&sa, &sl);
    if (n == 0) return 0;
    
	if (n < 0) {
	//	fprintf(stderr, "dvemu: error when reading from the socket, errno=%d\n", errno);
		return -1;
	}

	if (debug)
		dump("Socket data", buffer, n);

	if (buffer[0U] != AMBE3000_START_BYTE) {
		fprintf(stderr, "dvemu: invalid start byte when reading from the socket, 0x%02X\n", buffer[0U]);
		return 0;
	}

	len = buffer[1U] * 256U + buffer[2U] + AMBE3000_HEADER_LEN;
	if (n != len) {
		fprintf(stderr, "dvemu: invalid length when reading from the socket, %u=%d\n", len, (int)n);
		return 0;
	}

	sdv_write(ftHandle, buffer, len, &dwBytesInQueue);
	//if (dwBytesInQueue != len) {
	//	fprintf(stderr, "dvemu: error when writing to the usb port, bytes sent=%d:%d\n", len, dwBytesInQueue);
	//	return 0;
	//}

	return 0;
} // end processSocket


int dvemu(FT_HANDLE ftHandle, int port, int debug)
{
	int    topFd;
	fd_set fds;
	int    ret;

	sockFd = openSocket(port, debug);
	if (sockFd < 0)
		return 1;
		
	memset(&sa, 0x00, sizeof(struct sockaddr_in));

	topFd = sockFd;
	topFd++;

    FD_ZERO(&fds);
    FD_SET(sockFd, &fds);
    ret = select(topFd, &fds, NULL, NULL, NULL);
    if (ret < 0) {
        fprintf(stderr, "dvemu: error from select, errno=%d\n", errno);
        return 1;
    }
    
    while (!bExitRequested) {
        if (FD_ISSET(sockFd, &fds)) {
			ret = processSocket(ftHandle, debug);
	//		if (ret < 0)
		//		return 1;
		}

        ret = processSerial(ftHandle, debug);
		if (!ret)
			return 1;
        
        usleep(10000);
	}

	return 0;
} // end dvemu


int main(int argc, char *argv[])
{
    FT_HANDLE  ftHandle;
    int        iUdpPort = 2460;
    int        iDevice = 0;
    int        i = 0;
    
    while ((i = getopt(argc, argv, "p:d:")) != -1)
    {
        switch (i)
        {
            case 'p':
                iUdpPort = strtoul(optarg, NULL, 0);
                if (iUdpPort < 1)
                    iUdpPort = 2460;
                break;
                
            case 'd':
                iDevice = strtoul(optarg, NULL, 0);
                if (iDevice < 0)
                    iDevice = 0;
                break;
                
            default:
                fprintf(stderr, "usage: %s [-p udp port], [-d device #]\n", *argv);
                exit(-1);
        }
    }
    

    signal(SIGINT, sigintHandler);
    
    if ((ftHandle = sdv_open(iDevice, BIT_BANG, 460800, 1)) == NULL)
        goto exit;
    
    dvemu(ftHandle, iUdpPort, 0);
    
exit:
    signal(SIGINT, SIG_DFL);
    sdv_close(ftHandle);
    return 0;
} // end main
