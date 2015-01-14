/*
 *   Copyright (C) 2014 by Jonathan Naylor G4KLX and John Hays K7VE
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
 *
 *   Adapted by K7VE from G4KLX dv3000d
 */

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <termios.h>
#include <string.h>
#include <fcntl.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>

#include <netinet/in.h>
#if defined(RASPBERRY_PI)
#include <wiringPi.h>
#else
int  wiringPiSetup();
void pinMode(int pin, int mode);
void digitalWrite(int pin, int value);
void delay(unsigned int delay);
#define OUTPUT  1
#define HIGH    1
#define LOW     0
#endif

void delay(unsigned int delay) {
	struct timespec tim, tim2;
	tim.tv_sec = 0;
	tim.tv_nsec = delay * 1000UL;
	nanosleep(&tim, &tim2);
};

#define	DV3000_VERSION		"2014-11-12"

#define	AMBE3000_HEADER_LEN	4U
#define	AMBE3000_START_BYTE	0x61U

#define	DEFAULT_PORT		2460U
#define	BUFFER_LENGTH		400U

struct sockaddr_in sa;
int serialFd;
int sockFd;
int debug = 0;
char dv3000tty[50] = "";

void dump(char *text, unsigned char *data, unsigned int length)
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
	
}

#if defined(RASPBERRY_PI)

int openWiringPi(void)
{
        int ret = wiringPiSetup();
        if (ret == -1) {
                fprintf(stderr, "dv3000d: error when initialising wiringPi\n");
                return 0;
        }

        pinMode(7, OUTPUT);             // Power

        // Reset the hardware
        digitalWrite(7, LOW);

        delay(20UL);

        digitalWrite(7, HIGH);

        delay(750UL);

        if (debug)
                fprintf(stdout, "opened the Wiring Pi library\n");

        return 1;
}

#endif


int openSerial()
{
	struct termios tty;
	int fd;
        int n1;
        char reset[5];
        reset[0] = 0x61;
        reset[1] = 0x00;
        reset[2] = 0x01;
        reset[3] = 0x00;
        reset[4] = 0x33;


	
	fd = open(dv3000tty, O_RDWR | O_NOCTTY | O_SYNC);
	if (fd < 0) {
		fprintf(stderr, "AMBEserver: error when opening %s, errno=%d\n", dv3000tty,errno);
		return fd;
	}

	if (tcgetattr(fd, &tty) != 0) {
		fprintf(stderr, "AMBEserver: error %d from tcgetattr\n", errno);
		return -1;
	}

	cfsetospeed(&tty, B230400);
	cfsetispeed(&tty, B230400);

	tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;
	tty.c_iflag &= ~IGNBRK;
	tty.c_lflag = 0;

	tty.c_oflag = 0;
	tty.c_cc[VMIN]  = 0;
	tty.c_cc[VTIME] = 5;

	tty.c_iflag &= ~(IXON | IXOFF | IXANY);

	tty.c_cflag |= (CLOCAL | CREAD);

	tty.c_cflag &= ~(PARENB | PARODD);
	tty.c_cflag &= ~CSTOPB;
	tty.c_cflag &= ~CRTSCTS;

	if (tcsetattr(fd, TCSANOW, &tty) != 0) {
		fprintf(stderr, "AMBEserver: error %d from tcsetattr\n", errno);
		return -1;
	}

	n1 = write(fd,reset,5);
	if (debug)
		fprintf(stdout, "opened %s\n",dv3000tty);
		fprintf(stderr,"Wrote Reset %d chars\n",n1);
	return fd;
}

int openSocket(uint16_t port)
{
	struct sockaddr_in sa;
	int ret;
	int fd;

	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd < 0) {
		fprintf(stderr, "AMBEserver: error when creating the socket, errno=%d", errno);
		return fd;
	}

	memset(&sa, 0x00, sizeof(struct sockaddr_in));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(port);
	sa.sin_addr.s_addr = htonl(INADDR_ANY);

	ret = bind(fd, (struct sockaddr *)&sa, sizeof(struct sockaddr_in));
	if (ret < 0) {
		fprintf(stderr, "AMBEserver: error when binding the socket to port %u, errno=%d", port, errno);
		return fd;
	}

	if (debug)
		fprintf(stdout, "opened the UDP socket on port %u\n", port);

	return fd;
}

int processSerial(void)
{
	unsigned char buffer[BUFFER_LENGTH];
	unsigned int respLen;
	unsigned int offset;
	ssize_t len;

	len = read(serialFd, buffer, 1);
	if (len != 1) {
		fprintf(stderr, "AMBEserver: error when reading from the serial port, errno=%d", errno);
		return 0;
	}

	if (buffer[0U] != AMBE3000_START_BYTE) {
		fprintf(stderr, "AMBEserver: unknown byte from the DV3000, 0x%02X", buffer[0U]);
		return 1;
	}

	offset = 0U;
	while (offset < (AMBE3000_HEADER_LEN - 1U)) {
		len = read(serialFd, buffer + 1U + offset, AMBE3000_HEADER_LEN - 1 - offset);

		if (len == 0)
			delay(5UL);

		offset += len;
	}

	respLen = buffer[1U] * 256U + buffer[2U];

	offset = 0U;
	while (offset < respLen) {
		len = read(serialFd, buffer + AMBE3000_HEADER_LEN + offset, respLen - offset);

		if (len == 0)
			delay(5UL);

		offset += len;
	}

	respLen += AMBE3000_HEADER_LEN;

	if (debug)
		dump("Serial data", buffer, respLen);

	if (sa.sin_port != 0U) {
		len = sendto(sockFd, buffer, respLen, 0, (struct sockaddr *)&sa, sizeof(struct sockaddr_in));
		if (respLen != len) {
			fprintf(stderr, "AMBEserver: error when writing to the socket, len=%zd, errno=%d", len, errno);
			return 0;
		}
	}

	return 1;
}

int processSocket(void)
{
	unsigned char buffer[BUFFER_LENGTH];
	socklen_t sl = sizeof(struct sockaddr_in);
	unsigned int len;
	ssize_t n;
	
	n = recvfrom(sockFd, buffer, BUFFER_LENGTH, 0, (struct sockaddr *)&sa, &sl);
	if (n < 0) {
		fprintf(stderr, "AMBEserver: error when reading from the socket, errno=%d", errno);
		return 0;
	}

	if (debug)
		dump("Socket data", buffer, n);

	if (buffer[0U] != AMBE3000_START_BYTE) {
		fprintf(stderr, "AMBEserver: invalid start byte when reading from the socket, 0x%02X", buffer[0U]);
		return 1;
	}

	len = buffer[1U] * 256U + buffer[2U] + AMBE3000_HEADER_LEN;
	if (n != len) {
		fprintf(stderr, "AMBEserver: invalid length when reading from the socket, %u=%zd", len, n);
		return 1;
	}

	n = write(serialFd, buffer, len);
	if (n != len) {
		fprintf(stderr, "AMBEserver: error when writing to the serial port, errno=%d", errno);
		return 0;
	}

	return 1;
}

int main(int argc, char **argv)
{
	uint16_t port = DEFAULT_PORT;
	int daemon = 0;
	int topFd;
	int commnum;
	fd_set fds;
	int ret;
	int c;

	while ((c = getopt(argc, argv, "dc:p:i:vx")) != -1) {
		switch (c) {
			case 'd':
				daemon = 1;
				break;
			case 'p':
				port = (uint16_t)atoi(optarg);
				break;
			case 'c':
				commnum = (uint16_t)atoi(optarg);
				sprintf(dv3000tty,"/dev/ttyS%d",commnum - 1);
				break;
			case 'i':
				strcpy(dv3000tty,optarg);
				break;
			case 'v':
				fprintf(stdout, "AMBEserver: version " DV3000_VERSION "\n");
				return 0;
			case 'x':
				debug = 1;
				break;
			default:
				fprintf(stderr, "Usage: AMBEserver [-d] [-p port] [-i tty] [-c commportnum] [-v] [-x]\n");
				return 1;
		}
	}
	if (strlen(dv3000tty) < 1) {
		fprintf(stderr, "An input tty filename (-i /dev/ttyXXX) or a COM port number (-c 3) must be set.\n");
		return 1;
	}

	if (daemon) {
		pid_t pid = fork();

		if (pid < 0) {
			fprintf(stderr, "AMBEserver: error in fork(), exiting\n");
			return 1;
		}

		// If this is the parent, exit
		if (pid > 0)
			return 0;

		// We are the child from here onwards
		setsid();

//		chdir("/");

		umask(0);
	}

#if defined(RASPBERRY_PI)
        ret = openWiringPi();
        if (!ret) {
		fprintf(stderr,"Unable to open WiringPi, exiting\n");
                return 1;
	} else {
		fprintf(stderr,"Reset DV3000\n");
	}

#endif

	sockFd = openSocket(port);
	if (sockFd < 0)
		return 1;
		
	serialFd = openSerial();
	if (serialFd < 0)
		return 1;

	memset(&sa, 0x00, sizeof(struct sockaddr_in));

	topFd = sockFd;
	if (serialFd > topFd)
		topFd = serialFd;
	topFd++;

	for (;;) {
		FD_ZERO(&fds);
		FD_SET(sockFd, &fds);
		FD_SET(serialFd, &fds);
		ret = select(topFd, &fds, NULL, NULL, NULL);
		if (ret < 0) {
			fprintf(stderr, "AMBEserver: error from select, errno=%d\n", errno);
			return 1;
		}
		
		if (FD_ISSET(sockFd, &fds)) {
			ret = processSocket();
			if (!ret)
				return 1;
		}

		if (FD_ISSET(serialFd, &fds)) {
			ret = processSerial();
			if (!ret)
				return 1;
		}
	}

	return 0;
}
