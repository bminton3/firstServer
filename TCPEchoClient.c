/*
 * TCPEchoClient.c
 * 
 * Copyright 2014 Ben <ben@ben-VirtualBox>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define	RCVBUFSIZE	32

void dieWithError(char *errorMessage);

int main(int argc, char **argv)
{
	int sock;
	struct sockaddr_in echoServAddr;
	unsigned short echoServPort;
	char *servIP;
	char *echoString;
	char echoBuffer[RCVBUFSIZE];
	unsigned int echoStringLen;
	int bytesRcvd, totalBytesRcvd;
	
	if ((argc < 3) || (argc > 4))
	{
		fprintf(stderr, "Usage: %s <Server IP> <Echo Word> [<Echo Port>]\n", argv[0]);
		exit(1);
	}
	
	servIP = argv[1];
	echoString = argv[2];
	
	if (argc == 4)
	{
		echoServPort = atoi(argv[3]);
	}
	else
	{
		echoServPort = 7;
	}
	
	/* Create a reliable stream socket with TCP */
	if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
	{
		dieWithError("socket() failed");
	}
	
	/* Construct the server address structure */
	memset(&echoServAddr, 0, sizeof(echoServAddr));
	echoServAddr.sin_family		= AF_INET;
	echoServAddr.sin_addr.s_addr= inet_addr(servIP);
	echoServAddr.sin_port		= htons(echoServPort);
	
	/* Establish the connection to the echo server */
	if (connect(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
	{
		dieWithError("connect() failed");
	}
	
	echoStringLen = strlen(echoString);
	
	/* Send the string to the server */
	if (send(sock, echoString, echoStringLen, 0) != echoStringLen)
	{
		dieWithError("send() sent a different number of bytes than expected");
	}
	
	/* Receive the same string back from the server */
	totalBytesRcvd = 0;
	printf("Received: ");
	while (totalBytesRcvd < echoStringLen)
	{
		/* Receive up to the buffer size (minus 1 to leave space for a null terminator) bytes from the sender */
		if ((bytesRcvd = recv(sock, echoBuffer, RCVBUFSIZE - 1, 0)) <= 0)
		{
			dieWithError("recv() failed or connection closed prematurely");
		}
		totalBytesRcvd += bytesRcvd;
		echoBuffer[bytesRcvd] = '\0';	/* terminate the string! */
		printf(echoBuffer);
	}
	
	printf("\n");
	close(sock);
	return 0;
}

