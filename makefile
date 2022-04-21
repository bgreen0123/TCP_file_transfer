CC=gcc
CFLAGS=-Wall

all: TCPMsgClient TCPMsgServer

TCPMsgClient: TCPMsgClient.o

TCPMsgServer: TCPMsgServer.o

TCPMsgClient.o: TCPMsgClient.c
	$(CC) -c TCPMsgClient.c

TCPMsgServer.o: TCPMsgServer.c 
	$(CC) -c TCPMsgServer.c

clean:
	rm -f TCPMsgClient.o TCPMsgServer.o TCPMsgClient TCPMsgServer
