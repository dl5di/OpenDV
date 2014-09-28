# Makefile
# make Digital Voice on Linux (Ubuntu)
# Jonathan Naylor, March 2009

include settings.mk

all:			DVToolReader/dvtoolreader DVToolWriter/dvtoolwriter ambe2wav/ambe2wav dvtool2wav/dvtool2wav wav2ambe/wav2ambe wav2dvtool/wav2dvtool

DVToolReader/dvtoolreader:	Common/Common.a
				make -C DVToolReader

DVToolWriter/dvtoolwriter:	Common/Common.a
				make -C DVToolWriter

ambe2wav/ambe2wav:	Common/Common.a
				make -C ambe2wav

dvtool2wav/dvtool2wav:	Common/Common.a
				make -C dvtool2wav

wav2ambe/wav2ambe:	Common/Common.a
				make -C wav2ambe

wav2dvtool/wav2dvtool:	Common/Common.a
				make -C wav2dvtool

Common/Common.a:
				make -C Common

install:		all
				make -C DVToolReader install
				make -C DVToolWriter install
				make -C ambe2wav install
				make -C dvtool2wav install
				make -C wav2ambe install
				make -C wav2dvtool install

clean:
				rm -f core *~ *.bak
				make -C Common clean
				make -C DVToolReader clean
				make -C DVToolWriter clean
				make -C ambe2wav clean
				make -C dvtool2wav clean
				make -C wav2ambe clean
				make -C wav2dvtool clean
