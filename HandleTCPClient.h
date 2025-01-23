/* This is the header file that takes care of acually transfering the file
 * First it receives the file name requested by the client
 * Then it retrieves the data from the requested file 80 bytes at a time
 * When there are no more bytes to transfer it will send a End of Tranmission packet
 * Finally it will close the socket it has open*/

#include <stdio.h> /* for printf() and fprintf() */
#include <string.h> /* Used for strncat() */
#include <unistd.h> /* for close() */
#include "DieWithError.h"

#define RCVBUFSIZE 32 /* Size of receive buffer */
#define SENDBUFSIZE 80 /* Amount of bytes allowed in one packet*/

void HandleTCPClient(int clntSocket)
{
	char fileBuffer[RCVBUFSIZE]; /* Buffer for file name */
	char data[SENDBUFSIZE]; /* Buffer for file data */
	int totalBytesRcvd,bytesRcvd,n;
	FILE *f;

	/* Receive file name from client */
	if ((bytesRcvd = recv(clntSocket, fileBuffer, RCVBUFSIZE, 0)) < 0)
		DieWithError("recv() failed");

	/* Open file */
	fileBuffer[bytesRcvd-1] = '\0';
	f = fopen(fileBuffer,"r");
	if (f == NULL)
		DieWithError("File doesn't exist");

	n = 1;
	bytesRcvd = 0;
	totalBytesRcvd = 0;

	/* Send file data to client and send again until end of transmission */
	while (fgets(data,SENDBUFSIZE,f) != NULL) /* NULL indicates end of transmission */
	{
		/* Send data to client*/
		if ((bytesRcvd = send(clntSocket, data, sizeof(data), 0)) == -1)
			DieWithError("send() failed");

		totalBytesRcvd += bytesRcvd;
		printf("Packet %d transmitted with %d data bytes\n",n,bytesRcvd);
		bzero(data,SENDBUFSIZE);/*Zero out the buffer for room*/
		n++;
	}

	bytesRcvd = 0;
	printf("\nEnd of Transmission Packet with sequence number %d transmitted with %d data bytes\n",n,bytesRcvd);
	
	printf("\nNumber of data packets transmitted: %d\n",n-1);
	printf("Total number of data bytes transmitted: %d\n",totalBytesRcvd);
	close(clntSocket); /* Close client socket */
}
