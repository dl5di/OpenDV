all:	dv3000d

GCC := gcc

CFLAGS := -O2 -DRASPBERRY_PI -Wall
LIBS := -lwiringPi

dv3000d:	dv3000d.o
	$(GCC) $(LDFLAGS) -o dv3000d dv3000d.o $(LIBS)
	
dv3000d.o:	dv3000d.c
	$(GCC) $(CFLAGS) -c dv3000d.c

clean:
	$(RM) -f dv3000d *~ *.bak *.o
