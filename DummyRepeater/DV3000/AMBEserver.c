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

#define DV3K_TYPE_CONTROL 0x00
#define DV3K_TYPE_AMBE 0x01
#define DV3K_TYPE_AUDIO 0x02

static const unsigned char DV3K_START_BYTE   = 0x61;

static const unsigned char DV3K_CONTROL_RATEP  = 0x0A;
static const unsigned char DV3K_CONTROL_PRODID = 0x30;
static const unsigned char DV3K_CONTROL_VERSTRING = 0x31;
static const unsigned char DV3K_CONTROL_RESET = 0x33;
static const unsigned char DV3K_CONTROL_READY = 0x39;
static const unsigned char DV3K_CONTROL_CHANFMT = 0x15;

#define dv3k_packet_size(a) (1 + sizeof((a).header) + ntohs((a).header.payload_length))

#pragma pack(push, 1)
struct dv3k_packet {
    unsigned char start_byte;
    struct {
        unsigned short payload_length;
        unsigned char packet_type;
    } header;
    union {
        struct {
            unsigned char field_id;
            union {
                char prodid[16];
                char ratep[12];
                char version[48];
                short chanfmt;
            } data;
        } ctrl;
        struct {
            unsigned char field_id;
            unsigned char num_samples;
            short samples[160];
            unsigned char cmode_field_id;
            short cmode_value;
        } audio;
        struct {
            struct {
                unsigned char field_id;
                unsigned char num_bits;
                unsigned char data[9];
            } data;
            struct {
                unsigned char field_id;
                unsigned short value;
            } cmode;
            struct {
                unsigned char field_id;
                unsigned char tone;
                unsigned char amplitude;
            } tone;
        } ambe;
    } payload;
};
#pragma pack(pop)

#define	DV3000_VERSION		"2015-07-11"

#define	DEFAULT_PORT		2460
#define DEFAULT_TTY		"/dev/ttyUSB0"
#define DEFAULT_BAUD		230400

struct sockaddr_in sa = { 0 };
static int debug = 0;

// THIS IS A BCM PIN NUMBER, NOT A WIRINGPI PIN NUMBER!
static const unsigned char RESET_PIN = 4;

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

int hardwareReset(void)
{
	int fd;
	char fileName[MAXPATHLEN];
	char gpioNumber[4];
	
	//  Export the pin for use
	if((fd = open("/sys/class/gpio/export", O_WRONLY)) == -1) {
		fprintf(stderr, "AMBEserver: Unable to open GPIO export interface: %s\n", strerror(errno));
		return 0;
	}
	
	sprintf(gpioNumber, "%d", RESET_PIN);
	if(write(fd, gpioNumber, strlen(gpioNumber)) == -1  && errno != EBUSY) {
		fprintf(stderr, "AMBEserver: Unable to export GPIO interface: %s\n", strerror(errno));
		close(fd);
		return 0;
	}
	close(fd);
	
	// XXX THIS IS UGLY
	// XXX If we are not root, we have to wait for udevd to change the
	// XXX permissions on the gpio interfaces for us, otherwise we'll
	// XXX get a permission denied when trying to write the direction
	// XXX we could start polling the thing, but that would kinda suck
	// XXX too.  I would have to think of a better way to spin around the
	// XXX thing to make sure we can get to it.

	usleep(100000);
	
	//  Put the pin in output mode
	sprintf(fileName, "/sys/class/gpio/gpio%d/direction", RESET_PIN);
	if((fd = open(fileName, O_WRONLY)) == -1) {
		fprintf(stderr, "AMBEserver: Unable to open GPIO direction interface for pin %d: %s\n", RESET_PIN, strerror(errno));
		return 0;
	}
	
	if(write(fd, "out", 3) == -1) {
		fprintf(stderr, "AMBEserver: Unable to set GPIO direction for pint %d: %s\n", RESET_PIN, strerror(errno));
		close(fd);
		return 0;
	}
	close(fd);
	
	//  Toggle the reset line
	sprintf(fileName, "/sys/class/gpio/gpio%d/value", RESET_PIN);	
	if((fd = open(fileName, O_WRONLY)) == -1) {
		fprintf(stderr, "AMBEserver: Unable to open GPIO pin interface for pin %d: %s\n", RESET_PIN, strerror(errno));
		return 0;
	}
	
	if(write(fd, "0", 1) == -1) {
		fprintf(stderr, "AMBEserver: Unable to reset DV3000: %s\n", strerror(errno));
		close(fd);
		return 0;
	}
	usleep(20000);
	if(write(fd, "1", 1) == -1) {
		fprintf(stderr, "AMBEserver: Unable to reset DV3000: %s\n", strerror(errno));
		close(fd);
		return 0;
	}
	close(fd);
	
	return 1;
}

int readSerialPacket(int fd, struct dv3k_packet *packet)
{
	ssize_t bytes, bytesLeft;
	ssize_t bytesRead;
	int i;
	
	for(i = 0; i < sizeof(struct dv3k_packet); ++i) {
		bytesRead = read(fd, packet, 1);
		if(bytes == -1) {
			fprintf(stderr, "AMBEserver: Error reading from serial port: %s\n", strerror(errno));
			return 0;
		}
		if(packet->start_byte == DV3K_START_BYTE)
			break;
	}
	if(packet->start_byte != DV3K_START_BYTE) {
		fprintf(stderr, "AMBEserver: Couldn't find start byte in serial data\n");
		return 0;
	}

	bytesLeft = sizeof(packet->header);
	while(bytesLeft > 0) {
		bytes = read(fd, ((uint8_t *) &packet->header) + sizeof(packet->header) - bytesLeft, bytesLeft);
		if(bytes == -1) {
			fprintf(stderr, "AMBEserver: Couldn't read serial data header\n");
			return 0;
		}
		bytesLeft -= (size_t) bytes;
	}
	
	bytesLeft = ntohs(packet->header.payload_length);
    if(bytesLeft > sizeof(packet->payload)) {
        fprintf(stderr, "AMBEserver: Serial payload exceeds buffer size: %zd\n", bytesLeft);
        return 0;
    }
    
    while(bytesLeft > 0) {
        bytes = read(fd, ((uint8_t *) &packet->payload) + (ntohs(packet->header.payload_length) - bytesLeft), bytesLeft);
        if(bytes == -1) {
            fprintf(stderr, "AMBEserver: Couldn't read payload: %s\n", strerror(errno));
            return 0;
        }
        
        bytesLeft -= (size_t) bytes;
    }
    
    return 1;
}

static inline int checkResponse(struct dv3k_packet *responsePacket, unsigned char response)
{
	if(responsePacket->start_byte != DV3K_START_BYTE ||
	   responsePacket->header.packet_type != DV3K_TYPE_CONTROL ||
	   responsePacket->payload.ctrl.field_id != response)
		return 0;
		
	return 1;
}

int initDV3K(int fd, int hwReset)
{
	struct dv3k_packet responsePacket;
	char prodId[17];
	char version[49];
	
	struct dv3k_packet ctrlPacket = {
        .start_byte = DV3K_START_BYTE,
        .header.packet_type = DV3K_TYPE_CONTROL,
        .header.payload_length = htons(1),
        .payload.ctrl.field_id = DV3K_CONTROL_RESET
    };

	if(hwReset == 1) {
		if(hardwareReset() == 0)
			return 0;
	} else {
		if(write(fd, &ctrlPacket, dv3k_packet_size(ctrlPacket)) == -1) {
			fprintf(stderr, "AMBEserver: error writing reset packet: %s\n", strerror(errno));
			return 0;
		}
	}
	
	if(readSerialPacket(fd, &responsePacket) == 0) {
		fprintf(stderr, "AMBEserver: error receiving response to reset\n");
		return 0;
	}
	
	if(checkResponse(&responsePacket, DV3K_CONTROL_READY) == 0) {
	   fprintf(stderr, "AMBEserver: invalid response to reset\n");
	   return 0;
	}
	
	ctrlPacket.payload.ctrl.field_id = DV3K_CONTROL_PRODID;
	if(write(fd, &ctrlPacket, dv3k_packet_size(ctrlPacket)) == -1) {
		fprintf(stderr, "AMBEserver: error writing product id packet: %s\n", strerror(errno));
		return 0;
	}
	
	if(readSerialPacket(fd, &responsePacket) == 0) {
		fprintf(stderr, "AMBEserver: error receiving response to product id request\n");
		return 0;
	}
	
	if(checkResponse(&responsePacket, DV3K_CONTROL_PRODID) == 0) {
	   fprintf(stderr, "AMBEserver: invalid response to product id query\n");
	   dump("ProdID Response:", &responsePacket, sizeof(responsePacket));
	   return 0;
	}
	strncpy(prodId, responsePacket.payload.ctrl.data.prodid, sizeof(prodId));
	
	ctrlPacket.payload.ctrl.field_id = DV3K_CONTROL_VERSTRING;
	if(write(fd, &ctrlPacket, dv3k_packet_size(ctrlPacket)) == -1) {
		fprintf(stderr, "AMBEserver: error writing version packet: %s\n", strerror(errno));
		return 0;
	}
	
	if(readSerialPacket(fd, &responsePacket) == 0) {
		fprintf(stderr, "AMBEserver: error receiving response to version request\n");
		return 0;
	}
	
	if(checkResponse(&responsePacket, DV3K_CONTROL_VERSTRING) == 0) {
	   fprintf(stderr, "AMBEserver: invalid response to version query\n");
	   return 0;
	}
	strncpy(version, responsePacket.payload.ctrl.data.version, sizeof(version));
	
	fprintf(stdout, "AMBEserver: Initialized %s version %s.\n", prodId, version); 

	return 1;
}

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
	struct dv3k_packet packet;

	if(readSerialPacket(serialFd, &packet) == 0) {
		fprintf(stderr, "AMBEserver: processSerial failed to read packet\n");
		return 0;
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

	if (packet.start_byte != DV3K_START_BYTE) {
		fprintf(stderr, "AMBEserver: invalid start byte when reading from the socket, 0x%02X", packet.start_byte);
		return 1;
	}

	if (bytesRead != dv3k_packet_size(packet)) {
		fprintf(stderr, "AMBEserver: invalid length when reading from the socket, %zd=%zd", bytesRead, dv3k_packet_size(packet));
		return 1;
	}

	if (write(serialFd, &packet, dv3k_packet_size(packet)) != dv3k_packet_size(packet)) {
		fprintf(stderr, "AMBEserver: error when writing to the serial port: %s\n", strerror(errno));
		return 0;
	}

	return 1;
}

void usage() {
#ifdef __CYGWIN__
	fprintf(stderr, "Usage: AMBEserver [-d] [-r] [-s speed] [-p port] [-c comport_number] [-v] [-x]\n");
#else
	fprintf(stderr, "Usage: AMBEserver [-d] [-r] [-s speed] [-p port] [-i tty] [-v] [-x]\n");
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

#ifdef __CYGWIN__
	int commnum;
#endif
	
	char reset = 0;
	char daemon = 0;

	setvbuf(stdout, NULL, _IOLBF, 0);
	setvbuf(stderr, NULL, _IOLBF, 0);

#ifdef __CYGWIN__
	while ((c = getopt(argc, argv, "dp:s:c:vxrh")) != -1) {
#else
	while ((c = getopt(argc, argv, "dp:s:i:vxrh")) != -1) {
#endif
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
#ifdef __CYGWIN__
			case 'c':
				commnum = strtol(optarg, NULL, 10);
				sprintf(dv3000tty,"/dev/ttyS%d",commnum - 1);
				break;
#else
			case 'i':
				strncpy(dv3000tty, optarg, sizeof(dv3000tty));
				break;
#endif
			case 'v':
				fprintf(stdout, "AMBEserver: version " DV3000_VERSION "\n");
				return 0;
			case 'x':
				debug = 1;
				break;
			case 'r':
				reset = 1;
				break;
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

	fprintf(stdout, "AMBEserver: Starting...\n");

	serialFd = openSerial(dv3000tty, baud);
	if (serialFd < 0)
		exit(1);

	fprintf(stdout, "AMBEserver: Opened serial port %s at %ld bps.\n", dv3000tty, baud);
	
	if(initDV3K(serialFd, reset) == 0) {
		fprintf(stderr, "AMBEserver: Could not initialize the DV3K!\n");
		exit(1);
	}
	
	sockFd = openSocket(port);
	if (sockFd < 0)
		exit(1);

	fprintf(stdout, "AMBEserver: Listening for connections on UDP port %d.\n", port);
		
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
