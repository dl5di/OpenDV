BINDIR := /usr/local/bin
CFLAGS := -O2 -Wall
LIBS := -lusb

all:	rmk8055drv rmuridrv

rmk8055drv:	rmk8055drv.c
	$(CC) $(CFLAGS) -o rmk8055drv rmk8055drv.c $(LIBS)

rmuridrv:	rmuridrv.c
	$(CC) $(CFLAGS) -o rmuridrv rmuridrv.c $(LIBS)

install:	all
	install -g bin -o root -m 0775 dstarrepeater $(BINDIR)
	install -g bin -o root -m 0775 dstarrepeaterd $(BINDIR)

clean:
	$(RM) -f core *~ *.o *.bak rmk8055drv rmuridrv
