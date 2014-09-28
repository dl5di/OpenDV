# Makefile-include
#
# Build the repeater software on Linux
#

include settings.mk

all:	DStarRepeater/dstarrepeater DStarRepeaterConfig/dstarrepeaterconfig

DStarRepeater/dstarrepeater:	Common/Common.a
		make -C DStarRepeater

DStarRepeaterConfig/dstarrepeaterconfig:	Common/Common.a
		make -C DStarRepeaterConfig

Common/Common.a:
		make -C Common

install:	all
		install -d -g bin -o root -m 0775 $(BINDIR)
		make -C Data install
		make -C DStarRepeater install
		make -C DStarRepeaterConfig install

clean:
		rm -f core *~ *.bak
		make -C Common clean
		make -C DStarRepeater clean
		make -C DStarRepeaterConfig clean

include package.mk
