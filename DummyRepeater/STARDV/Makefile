all:	dvemu

GCC := gcc

CFLAGS := -O2 -Wno-write-strings
LIBS := -lftd2xx

dvemu:	dvemu.o
	$(GCC) $(LDFLAGS) $(CFLAGS) -o dvemu dvemu.cpp sdvtools.cpp $(LIBS)
	
clean:
	$(RM) -f dvemu *~ *.bak *.o

install:
	install dvemu /usr/bin
