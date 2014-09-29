BINDIR := /usr/local/bin
CFLAGS := -O2 -Wall
LIBS := -lusb-1.0

all:	rmk8055drv rmuridrv

rmk8055drv:	rmk8055drv.c
	$(CC) $(CFLAGS) -o rmk8055drv rmk8055drv.c $(LIBS)

rmuridrv:	rmuridrv.c
	$(CC) $(CFLAGS) -o rmuridrv rmuridrv.c $(LIBS)

install:	all
	install -g bin -o root -m 0775 rmk8055drv $(BINDIR)
	install -g bin -o root -m 0775 rmuridrv $(BINDIR)

clean:
	$(RM) -f core *~ *.o *.bak rmk8055drv rmuridrv
