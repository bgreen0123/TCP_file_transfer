/* This is the file that runs the client side of the TCP connection
 * First it will create the client socket and will set its memory address to 0's
 * Next it will connect itself to the server socket
 * It will the send the server the file the user is looking for
 * Finally it will receive all data back from the server and put it into out.txt
 * The client socket will then close itself*/

#include <stdio.h> /*for printf() and fprintf()*/
#include <sys/socket.h> /*for socket(), connect(), send(), and recv()*/
#include <arpa/inet.h> /*for sockaddr_in and inet_addr()*/
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "DieWithError.h"

#define RCVBUFSIZE 32 /*Size of receive buffer*/
#define PACKETBUFSIZE 80 /*Size of the amount of bytes allowed per packet*/

int main(int argc, char *argv[])
{
	int sock; /*Socket descriptor*/
	struct sockaddr_in fileServAddr;
	unsigned short fileServPort;
	char *servIP;
	char fileString[RCVBUFSIZE];
	char fileBuffer[PACKETBUFSIZE];
	unsigned int fileLen;
	int bytesRcvd, totalBytesRcvd, n;
	FILE *f;

	if ((argc!=1))
	{
		fprintf(stderr, "Usage: %s \n", argv[0]);
		exit(1);
	}
	
	/*HardCoded Values*/
	servIP = "8.8.8.8";
	fileServPort = 5678;

	/*Create a socket using TCP*/
	if((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP))<0)
		DieWithError("socket() failed");

	/*Construct the server address structure*/
	memset(&fileServAddr, 0, sizeof(fileServAddr));
	fileServAddr.sin_family = AF_INET;
	fileServAddr.sin_addr.s_addr = inet_addr(servIP);
	fileServAddr.sin_port = htons(fileServPort);

	/*Establish connection to echo server*/
	if (connect(sock, (struct sockaddr*) &fileServAddr, sizeof(fileServAddr))<0)
		DieWithError("connect() failed");
	
	/*Take in input from user*/
	printf("Enter File: ");
	fgets(fileString,RCVBUFSIZE,stdin);
	fileLen = strlen(fileString);

	/*send the string*/
	if (send(sock, fileString, fileLen, 0)!=fileLen)
		DieWithError("send() sent a different number of bytes than expected");

	/*Receive the same string back from the server*/
	totalBytesRcvd=0;
	n = 1;
	f = fopen("out.txt","w");

	/*While still receiving bytes keep looping*/
	while (1)
	{
		/* Receive data until all data is recieved or an error occurs*/

		if((bytesRcvd = recv(sock, fileBuffer, PACKETBUFSIZE, 0))<=0)
			break;
		totalBytesRcvd += bytesRcvd;
		fprintf(f, "%s", fileBuffer);
		printf("Packet %d received with %d data bytes\n",n,bytesRcvd);
		bzero(fileBuffer,PACKETBUFSIZE);/*Zero out the buffer for more data*/
		n++;
	}
	printf("\nEnd of Transmisson Packet with sequence number %d received with %d data bytes\n",n,bytesRcvd);
	
	printf("\nNumber of data packets received: %d\n",n-1);
	printf("Total number of data bytes received: %d",totalBytesRcvd);

	printf("\n");

	close(sock);
	exit(0);
}
