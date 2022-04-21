/*This is the file that runs the server side of the TCP connection
 * First it will create the server socket and will set its memory address to 0's
 * Next it will bind itself to the client socket
 * Then it will be marked as listening for client request
 * Then is will wait to fufil them anad accept the connection
 * Once connected it will send the clients socket to HandleTCPClient to take care of transferring data
 * Finally it will close the server socket*/

#include <stdio.h> /*for printf() and fprintf()*/
#include <sys/socket.h> /*for socket(), connect(), send(), and recv()*/
#include <arpa/inet.h> /*for sockaddr_in and inet_addr()*/
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "HandleTCPClient.h"

#define MAXPENDING 5 /* Maximum outstanding connection requests */


int main(int argc, char *argv[])
{
	int servSock; /* Socket descriptor for server */
	int clntSock; /* Socket descriptor for client */
	struct sockaddr_in fileServAddr; /* Local address */
	struct sockaddr_in fileClntAddr; /* Client address */
	unsigned short fileServPort; /* Server port */
	unsigned int clntLen; /* Length of client address data structure */

	if (argc != 1) /* Test for correct number of arguments */
	{
		fprintf(stderr, "Usage: %s\n", argv[0]);
		exit(1);
	}

	fileServPort = 5678; /* Hard coded local port */

	/* Create socket for incoming connections */
	if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		DieWithError( "socket() failed");

	/* Construct local address structure */
	memset(&fileServAddr, 0, sizeof(fileServAddr)); /* Zero out structure */
	fileServAddr.sin_family = AF_INET; /* Internet address family */
	fileServAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
	fileServAddr.sin_port = htons(fileServPort); /* Hard coded local port */

	/* Bind to the local address */
	if (bind(servSock, (struct sockaddr*)&fileServAddr, sizeof(fileServAddr)) < 0)
		DieWithError ("bind() failed");

	/* Mark the socket so it will listen for incoming connections */
	if (listen(servSock, MAXPENDING) < 0)
		DieWithError("listen() failed");

	for (;;) /* Run forever */
	{
		/* Set the size of the in-out parameter */
		clntLen = sizeof(fileClntAddr);

		/* Wait for a client to connect */
		if ((clntSock = accept(servSock, (struct sockaddr *)&fileClntAddr, &clntLen)) < 0)
			DieWithError("accept() failed");

		/* clntSock is connected to a client! */
		printf("Handling client %s\n", inet_ntoa(fileClntAddr.sin_addr));
		HandleTCPClient(clntSock);

		close(servSock);
		return 0;
	}
/* NOT REACHED BECAUSE SERVER ALWAYS WAITS FOR CONNECTION
 * UNLESS THERE IS AN ERROR */
}
