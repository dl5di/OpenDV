all:			voicetransmit

CFLAGS :=		-I../Common $(CFLAGS)

voicetransmit:	VoiceTransmit.o VoiceStore.o ../Common/Common.a
				$(CC) $(LDFLAGS) -o voicetransmit VoiceTransmit.o VoiceStore.o ../Common/Common.a $(LIBS)

VoiceTransmit.o:		VoiceTransmit.cpp VoiceTransmit.h VoiceStore.h ../Common/UDPReaderWriter.h ../Common/HeaderData.h ../Common/AMBEData.h \
									../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c VoiceTransmit.cpp

VoiceStore.o:		VoiceStore.cpp VoiceStore.h ../Common/HeaderData.h ../Common/AMBEData.h ../Common/DVTOOLFileReader.h ../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c VoiceStore.cpp

install:		voicetransmit
				install -d -g bin -o root -m 0775 $(BINDIR)
				install -g bin -o root -m 0775 voicetransmit $(BINDIR)

clean:
				rm -f core voicetransmit *.o *~ *.bak
