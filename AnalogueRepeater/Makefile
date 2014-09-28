# Makefile-include
#
# Build the repeater software on Linux
#

include settings.mk

all:	AnalogueRepeater/analoguerepeater

AnalogueRepeater/analoguerepeater:	Common/Common.a
		make -C AnalogueRepeater

Common/Common.a:
		make -C Common

install:	all
		install -d -g bin -o root -m 0775 $(BINDIR)
		make -C AnalogueRepeater install

clean:
		rm -f core *~ *.bak
		make -C Common clean
		make -C AnalogueRepeater clean

include package.mk
