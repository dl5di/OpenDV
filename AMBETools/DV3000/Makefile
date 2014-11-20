all:	dv3000d AMBEserver AMBEserverGPIO

GCC := gcc

CFLAGS := -O2 -DRASPBERRY_PI -Wall
CFLAGS2 := -O2 -Wall
LIBS := -lwiringPi

dv3000d:	dv3000d.o
	$(GCC) $(LDFLAGS) -o dv3000d dv3000d.o $(LIBS)
	
dv3000d.o:	dv3000d.c
	$(GCC) $(CFLAGS) -c dv3000d.c

AMBEserver:	AMBEserver.o
	$(GCC) $(LDFLAGS) -o AMBEserver AMBEserver.o 
	
AMBEserver.o:	AMBEserver.c
	$(GCC) $(CFLAGS2) -c AMBEserver.c

AMBEserverGPIO:	AMBEserverGPIO.o
	$(GCC) $(LDFLAGS) -o AMBEserverGPIO AMBEserverGPIO.o $(LIBS)
	
AMBEserverGPIO.o:	AMBEserver.c
	$(GCC) $(CFLAGS) -o AMBEserverGPIO.o -c AMBEserver.c

clean:
	$(RM) -f dv3000d AMBEserver AMBEserverGPIO *~ *.bak *.o

install:
	install AMBEserver /usr/bin
	install AMBEserverGPIO /usr/bin

init-install:
	install init.d/AMBEserver /etc/init.d
	install init.d/AMBEserverGPIO /etc/init.d
