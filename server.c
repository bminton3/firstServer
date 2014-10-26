/*
 * server.c
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

#define	MAXPENDING	5

void dieWithError(char *errorMessage);
void handleTCPClient(int clntSocket);

int main(int argc, char **argv)
{
	int serverSocket;
	int clientSocket;
	struct sockaddr_in echoServAddr;	/* local address */
	struct sockaddr_in echoClntAddr;	/* remote address */
	unsigned short echoServPort;		/* server port */
	unsigned int clntLen;				/* length of the client address data structure */
	
	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s <Server Port>\n", argv[0]);
		exit(1);
	}
	
	echoServPort = atoi(argv[1]);
	
	/* Create socket for incoming connections */
	if ((serverSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
	{
		dieWithError("socket() failed");
	}
	
	/* Construct local address structure */
	memset(&echoServAddr, 0, sizeof(echoServAddr));		/* Zero out structure */
	echoServAddr.sin_family = AF_INET;					/* Internet address family */
	echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY); 	/* Any incoming interface */
	echoServAddr.sin_port = htons(echoServPort);		/* Local port */
	
	/* Bind to the local address */
	if (bind(serverSocket, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
	{
		dieWithError("bind() failed");
	}
	
	if (listen(serverSocket, MAXPENDING) < 0)
	{
		dieWithError("listen() failed");
	}
	
	for (;;) /* Run forever */
	{
		/* Set the size of the in-out parameter */
		clntLen = sizeof(echoClntAddr);
		
		/* Wait for a client to connect */
		if ((clientSocket = accept(serverSocket, (struct sockaddr *) &echoClntAddr, &clntLen)) < 0)
		{
			dieWithError("accept() failed");
		}
		
		/* clientSocket is connected to a client ! */
		
		printf("Handling client %s\n", inet_ntoa(echoClntAddr.sin_addr));
		
		handleTCPClient(clientSocket);
	}
	return 0;
}

