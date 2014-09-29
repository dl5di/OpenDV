# Makefile-include
#
# Build the repeater software on Linux
#

include settings.mk

all:	DVAPNode/dvapnode DVRPTRRepeater/dvrptrrepeater GMSKRepeater/gmskrepeater SoundCardRepeater/soundcardrepeater SplitRepeater/splitrepeater

DVAPNode/dvapnode:	Common/Common.a
		make -C DVAPNode

DVRPTRRepeater/dvrptrrepeater:	Common/Common.a
		make -C DVRPTRRepeater

GMSKRepeater/gmskrepeater:	Common/Common.a
		make -C GMSKRepeater

SoundCardRepeater/soundcardrepeater:	Common/Common.a
		make -C SoundCardRepeater

SplitRepeater/splitrepeater:	Common/Common.a
		make -C SplitRepeater

Common/Common.a:
		make -C Common

install:	all
		install -d -g bin -o root -m 0775 $(BINDIR)
		make -C Data install
		make -C DVAPNode install
		make -C DVRPTRRepeater install
		make -C GMSKRepeater install
		make -C SoundCardRepeater install
		make -C SplitRepeater install

clean:
		rm -f core *~ *.bak
		make -C Common clean
		make -C DVAPNode clean
		make -C DVRPTRRepeater clean
		make -C GMSKRepeater clean
		make -C SoundCardRepeater clean
		make -C SplitRepeater clean

include package.mk
