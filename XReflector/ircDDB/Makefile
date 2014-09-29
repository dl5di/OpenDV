all:				libircDDB.a

libircDDB.a:		IRCClient.o IRCDDB.o IRCDDBApp.o IRCMessage.o IRCMessageQueue.o IRCProtocol.o IRCReceiver.o IRCutils.o
					ar rcs libircDDB.a IRCClient.o IRCDDB.o IRCDDBApp.o IRCMessage.o IRCMessageQueue.o IRCProtocol.o IRCReceiver.o IRCutils.o

IRCClient.o:		IRCClient.cpp IRCClient.h IRCReceiver.h IRCProtocol.h IRCutils.h IRCMessageQueue.h IRCApplication.h
					$(CC) $(CFLAGS) -c IRCClient.cpp

IRCDDB.o:			IRCDDB.cpp IRCDDB.h IRCClient.h IRCDDBApp.h
					$(CC) $(CFLAGS) -c IRCDDB.cpp

IRCDDBApp.o:		IRCDDBApp.cpp IRCDDBApp.h IRCDDB.h IRCApplication.h IRCutils.h
					$(CC) $(CFLAGS) -c IRCDDBApp.cpp

IRCMessage.o:		IRCMessage.cpp IRCMessage.h
					$(CC) $(CFLAGS) -c IRCMessage.cpp

IRCMessageQueue.o:	IRCMessageQueue.cpp IRCMessageQueue.h IRCMessage.h
					$(CC) $(CFLAGS) -c IRCMessageQueue.cpp

IRCProtocol.o:		IRCProtocol.cpp IRCProtocol.h IRCApplication.h IRCMessageQueue.h
					$(CC) $(CFLAGS) -c IRCProtocol.cpp

IRCReceiver.o:		IRCReceiver.cpp IRCReceiver.h IRCMessageQueue.h
					$(CC) $(CFLAGS) -c IRCReceiver.cpp

IRCutils.o:			IRCutils.cpp IRCutils.h
					$(CC) $(CFLAGS) -c IRCutils.cpp

clean:
					rm -f core libircDDB.a *.o *.bak *~
