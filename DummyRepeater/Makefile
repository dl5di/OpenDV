# Makefile-include
#
# Build the repeater software on Linux
#

include settings.mk

all:	DummyRepeater/dummyrepeater

DummyRepeater/dummyrepeater:	Common/Common.a
		make -C DummyRepeater

Common/Common.a:
		make -C Common

install:	all
		install -d -g bin -o root -m 0775 $(BINDIR)
		make -C DummyRepeater install

clean:
		rm -f core *~ *.bak
		make -C Common clean
		make -C DummyRepeater clean

include package.mk
