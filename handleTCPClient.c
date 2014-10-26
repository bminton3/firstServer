/*
 * handleTCPClient.c
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
#include <unistd.h>

#define	RCVBUFSIZE	32

void dieWithError(char *errorMessage);

void handleTCPClient(int clntSocket)
{
	char echoBuffer[RCVBUFSIZE];
	int recvMsgSize;
	
	/* Receive message from client */
	if ((recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0)) < 0)
	{
		dieWithError("recv() failed");
	}
	
	/* send received string and receive again until end of transmission */
	while (recvMsgSize > 0)
	{
		/* Echo message back to the client */
		if (send(clntSocket, echoBuffer, recvMsgSize, 0) != recvMsgSize)
		{
			dieWithError("send() failed");
		}
		
		/* See if there is more data to receive */
		if ((recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0)) < 0)
		{
			dieWithError("recv() failed");
		}
	}
	
	close(clntSocket);
}

