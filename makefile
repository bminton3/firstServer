server: server.c handleTCPClient.c dieWithError.c
	gcc -o server server.c handleTCPClient.c dieWithError.c

client: TCPEchoClient.c dieWithError.c
	gcc -o client TCPEchoCLient.c dieWithError.c
