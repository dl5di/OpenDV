/*
 *   Copyright (C) 2014 by Jonathan Naylor G4KLX and John Hays K7VE
 *   Copyright 2016 by Jeremy McDermond (NH6Z)
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
#include <sys/param.h>

#include <netinet/in.h>

#if defined(RASPBERRY_PI)
#include <wiringPi.h>
#endif

#define dv3k_packet_size(a) (1 + sizeof((a).header) + ntohs((a).header.payload_length))

#pragma pack(push, 1)
struct dv3k_packet {
    unsigned char start_byte;
    struct {
        unsigned short payload_length;
        unsigned char packet_type;
    } header;
    char payload[325];
};
#pragma pack(pop)

#define	DV3000_VERSION		"2015-07-11"

#define	AMBE3000_START_BYTE	0x61

#define	DEFAULT_PORT		2460
#define DEFAULT_TTY		"/dev/ttyUSB0"
#define DEFAULT_BAUD		230400

struct sockaddr_in sa = { 0 };
static int debug = 0;

void dump(char *text, struct dv3k_packet *packet, unsigned int length)
{
	unsigned int offset = 0U;
	unsigned int i;
	unsigned char *data = (unsigned char *) packet;

	fprintf(stdout, "Received %s packet with type %d and length %d:\n", text, 
		packet->header.packet_type, ntohs(packet->header.payload_length));
		
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

const char reset[] = { 0x61, 0x00, 0x01, 0x00, 0x33 };
const char prodId[] = { 0x61, 0x00, 0x01, 0x00, 0x33 };

int openSerial(char *ttyname, long baud)
{
	struct termios tty;
	int fd;
    ssize_t bytesWritten;
	
	fd = open(ttyname, O_RDWR | O_NOCTTY | O_SYNC);
	if (fd < 0) {
		fprintf(stderr, "AMBEserver: error when opening %s: %s\n", ttyname, strerror(errno));
		return fd;
	}

	if (tcgetattr(fd, &tty) != 0) {
		fprintf(stderr, "AMBEserver tcgetattr: %s\n", strerror(errno));
		return -1;
	}
	
	if(debug)
		fprintf(stderr, "Baud %ld\n", baud);
		
	//  Input speed = output speed
	cfsetispeed(&tty, B0);
	
	switch(baud) {
		case 230400:
			cfsetospeed(&tty, B230400);
			break;
		case 460800:
			cfsetospeed(&tty, B460800);
			break;
		default:
			fprintf(stderr, "AMBEserver: unsupported baud rate %ld\n", baud);
			return -1;
	}
	
	tty.c_lflag    &= ~(ECHO | ECHOE | ICANON | IEXTEN | ISIG);
    tty.c_iflag    &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON | IXOFF | IXANY);
    tty.c_cflag    &= ~(CSIZE | CSTOPB | PARENB);
    tty.c_cflag    |= CS8 | CRTSCTS;
    tty.c_oflag    &= ~(OPOST);
    tty.c_cc[VMIN] = 0;
    tty.c_cc[VTIME] = 1;
	
	if (tcsetattr(fd, TCSANOW, &tty) != 0) {
		fprintf(stderr, "AMBEserver: tcsetattr: %s\n", strerror(errno));
		return -1;
	}

	if (debug)
		fprintf(stdout, "opened %s %ld\n", ttyname, baud);
		
	bytesWritten = write(fd, reset, sizeof(reset));
	if(bytesWritten != sizeof(reset)) {
		if(bytesWritten == -1)
			fprintf(stderr, "Error writing reset sequence: %s\n", strerror(errno));
		else
			fprintf(stderr, "Short write on reset sequence\n");
		return -1;
	}
	if (debug)
		fprintf(stderr, "Wrote Reset %ld chars\n", bytesWritten);
		
	bytesWritten = write(fd ,prodId, sizeof(prodId));
	if(bytesWritten != sizeof(prodId)) {
		if(bytesWritten == -1)
			fprintf(stderr, "Error writing Product ID command: %s\n", strerror(errno));
		else
			fprintf(stderr, "Short write on Product ID command");
		return -1;
	}
	if (debug)
		fprintf(stderr,"Wrote prodID %ld chars\n", bytesWritten);
		
	return fd; 
}

int openSocket(in_port_t port)
{
	struct sockaddr_in sa = { 0 };
	int fd;

	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd < 0) {
		fprintf(stderr, "AMBEserver: error when creating the socket: %s\n", strerror(errno));
		return fd;
	}

	sa.sin_family = AF_INET;
	sa.sin_port = htons(port);
	sa.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(fd, (struct sockaddr *) &sa, sizeof(sa)) < 0) {
		fprintf(stderr, "AMBEserver: error when binding the socket to port %u: %s\n", port, strerror(errno));
		return -1;
	}

	if (debug)
		fprintf(stdout, "opened the UDP socket on port %u\n", port);

	return fd;
}

int processSerial(int sockFd, int serialFd)
{
	ssize_t bytes, bytesLeft;
	struct dv3k_packet packet;
	ssize_t bytesRead;
	
	for(int i = 0; i < sizeof(struct dv3k_packet); ++i) {
		bytesRead = read(serialFd, &packet, 1);
		if(bytes == -1) {
			fprintf(stderr, "Error reading from serial port: %s\n", strerror(errno));
			return 0;
		}
		if(packet.start_byte == AMBE3000_START_BYTE)
			break;
	}
	if(packet.start_byte != AMBE3000_START_BYTE) {
		fprintf(stderr, "Couldn't find start byte in serial data\n");
		return 0;
	}

	bytesLeft = sizeof(packet.header);
	while(bytesLeft > 0) {
		bytes = read(serialFd, ((uint8_t *) &packet.header) + sizeof(packet.header) - bytesLeft, bytesLeft);
		if(bytes == -1) {
			fprintf(stderr, "Couldn't read serial data header\n");
			return 0;
		}
		bytesLeft -= (size_t) bytes;
	}
	
	bytesLeft = ntohs(packet.header.payload_length);
    if(bytesLeft > sizeof(packet.payload)) {
        fprintf(stderr, "Serial payload exceeds buffer size: %ld\n", bytesLeft);
        return 0;
    }
    
    while(bytesLeft > 0) {
        bytes = read(serialFd, ((uint8_t *) &packet.payload) + (ntohs(packet.header.payload_length) - bytesLeft), bytesLeft);
        if(bytes == -1) {
            fprintf(stderr, "Couldn't read payload: %s\n", strerror(errno));
            return 0;
        }
        
        bytesLeft -= (size_t) bytes;
    }

	if (debug)
		dump("Serial data", &packet, dv3k_packet_size(packet));

	if (sa.sin_port != 0U)
		if (sendto(sockFd, &packet, dv3k_packet_size(packet), 0, (struct sockaddr *)&sa, sizeof(struct sockaddr_in)) != dv3k_packet_size(packet)) {
			fprintf(stderr, "AMBEserver: error when writing to the socket: %s\n", strerror(errno));
			return 0;
	}

	return 1;
}

int processSocket(int sockFd, int serialFd)
{
	socklen_t addrLen = sizeof(struct sockaddr_in);
	ssize_t bytesRead;
	struct dv3k_packet packet;
	
	bytesRead = recvfrom(sockFd, &packet, sizeof(packet), 0, (struct sockaddr *) &sa, &addrLen);
	if (bytesRead < 0) {
		fprintf(stderr, "AMBEserver: error when reading from the socket: %s\n", strerror(errno));
		return 0;
	}

	if (debug)
		dump("Socket data", &packet, bytesRead);

	if (packet.start_byte != AMBE3000_START_BYTE) {
		fprintf(stderr, "AMBEserver: invalid start byte when reading from the socket, 0x%02X", packet.start_byte);
		return 1;
	}

	if (bytesRead != dv3k_packet_size(packet)) {
		fprintf(stderr, "AMBEserver: invalid length when reading from the socket, %ld=%zd", bytesRead, dv3k_packet_size(packet));
		return 1;
	}

	if (write(serialFd, &packet, dv3k_packet_size(packet)) != dv3k_packet_size(packet)) {
		fprintf(stderr, "AMBEserver: error when writing to the serial port: %s\n", strerror(errno));
		return 0;
	}

	return 1;
}

void usage() {
#if defined(RASPBERRY_PI)
	fprintf(stderr, "Usage: AMBEserver [-d] [-r] [-s speed] [-p port] [-i tty] [-v] [-x]\n");
#else
	fprintf(stderr, "Usage: AMBEserver [-d] [-s speed] [-p port] [-i tty] [-v] [-x]\n");
#endif
	exit(1);
}

int main(int argc, char **argv)
{
	in_port_t port = DEFAULT_PORT;
	long baud = DEFAULT_BAUD;
	char dv3000tty[MAXPATHLEN] = DEFAULT_TTY;
	
	fd_set fds;
	int serialFd;
	int sockFd;
	int topFd;
	
	int c;
	
#if defined(RASPBERRY_PI)
	char reset = 0;
#endif
	char daemon = 0;

	while ((c = getopt(argc, argv, "s:dp:i:vxh")) != -1) {
		switch (c) {
			case 'd':
				daemon = 1;
				break;
			case 'p':
				errno = 0;
				port = strtol(optarg, NULL, 10);
				if(errno != 0 || port == 0) {
					fprintf(stderr, "Invalid port number: %s\n", optarg);
					usage();
				}
				break;
			case 's':
				errno = 0;
				baud = strtol(optarg, NULL, 10);
				if(errno != 0 || baud == 0) {
					fprintf(stderr, "Invalid baud rate: %s\n", optarg);
					usage();
				}
				break;
			case 'i':
				strncpy(dv3000tty, optarg, sizeof(dv3000tty));
				break;
			case 'v':
				fprintf(stdout, "AMBEserver: version " DV3000_VERSION "\n");
				return 0;
			case 'x':
				debug = 1;
				break;
#if defined(RASPBERRY_PI)
			case 'r':
				reset = 1;
				break;
#endif
			case 'h':
			default:
				usage();
				break;
		}
	}
	
	if (strlen(dv3000tty) < 1) {
		fprintf(stderr, "An input tty filename (-i /dev/ttyXXX) must be set.\n");
		return 1;
	}

	if (daemon) {
		pid_t pid = fork();

		if (pid < 0) {
			fprintf(stderr, "AMBEserver: error in fork(), exiting\n");
			exit(1);
		}

		// If this is the parent, exit
		if (pid > 0)
			exit(0);

		// We are the child from here onwards
		setsid();

		umask(0);
	}

#if defined(RASPBERRY_PI)
	if(reset) {
		if (!openWiringPi()) {
			fprintf(stderr,"Unable to open WiringPi, exiting\n");
			exit(1);
		} else {
			fprintf(stderr,"Reset DV3000\n");
		}
	}
#endif

	sockFd = openSocket(port);
	if (sockFd < 0)
		exit(1);
		
	serialFd = openSerial(dv3000tty, baud);
	if (serialFd < 0)
		exit(1);

	topFd = (sockFd > serialFd ? sockFd : serialFd ) + 1;

	for (;;) {
		FD_ZERO(&fds);
		FD_SET(sockFd, &fds);
		FD_SET(serialFd, &fds);

		if (select(topFd, &fds, NULL, NULL, NULL) < 0) {
			fprintf(stderr, "AMBEserver: error from select: %s\n", strerror(errno));
			exit(1);
		}
		
		if(FD_ISSET(sockFd, &fds))
			if (!processSocket(sockFd, serialFd))
				exit(1);

		if(FD_ISSET(serialFd, &fds))
			if (!processSerial(sockFd, serialFd))
				exit(1);
	}

	exit(0);
}
