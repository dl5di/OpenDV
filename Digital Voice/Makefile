# Makefile
# make Digital Voice on Linux (Ubuntu)
# Jonathan Naylor, March 2009

include settings.mk

all:			DCSClient/DCSClient DExtraClient/DExtraClient DVAPClient/DVAPClient DVRPTRClient/DVRPTRClient DVToolReader/DVToolReader GMSKClient/GMSKClient SoundCardClient/SoundCardClient

DCSClient/DCSClient:	Common/Common.a
				make -C DCSClient

DExtraClient/DExtraClient:	Common/Common.a
				make -C DExtraClient

DVAPClient/DVAPClient:	Common/Common.a
				make -C DVAPClient

DVRPTRClient/DVRPTRClient:	Common/Common.a
				make -C DVRPTRClient

DVToolReader/DVToolReader:	Common/Common.a
				make -C DVToolReader

GMSKClient/GMSKClient:	Common/Common.a
				make -C GMSKClient

SoundCardClient/SoundCardClient:	Common/Common.a
				make -C SoundCardClient

Common/Common.a:
				make -C Common

install:		all
				make -C DCSClient install
				make -C DExtraClient install
				make -C DVAPClient install
				make -C DVRPTRClient install
				make -C DVToolReader install
				make -C GMSKClient install
				make -C SoundCardClient install

clean:
				rm -f core *~ *.bak
				make -C Common clean
				make -C DCSClient clean
				make -C DExtraClient clean
				make -C DVAPClient clean
				make -C DVRPTRClient clean
				make -C DVToolReader clean
				make -C GMSKClient clean
				make -C SoundCardClient clean
