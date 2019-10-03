#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>

using namespace std;

void createClient(int SocketFD, string IP, int PORT) {
  struct sockaddr_in stSockAddr;
	int Res;

	if (-1 == SocketFD) 
	{
		perror("cannot create socket");
		exit(EXIT_FAILURE);
	}

	memset(&stSockAddr, 0, sizeof(struct sockaddr_in));

	stSockAddr.sin_family = AF_INET;
	stSockAddr.sin_port = htons(PORT);
	Res = inet_pton(AF_INET, IP.c_str(), &stSockAddr.sin_addr);

	if (0 > Res) 
	{
		perror("error: first parameter is not a valid address family");
		close(SocketFD);
		exit(EXIT_FAILURE);
	}
	else if (0 == Res) 
	{
		perror("char string (second parameter does not contain valid ipaddress");
		close(SocketFD);
		exit(EXIT_FAILURE);
	}
	if (-1 == connect(SocketFD, (const struct sockaddr *)&stSockAddr, sizeof(struct sockaddr_in))) 
	{
		perror("connect failed");
		close(SocketFD);
		exit(EXIT_FAILURE);
	}
}

void createServer(int SocketFD, int PORT) {
  struct sockaddr_in stSockAddr;

	if (-1 == SocketFD) {
		perror("cannot create socket");
		exit(EXIT_FAILURE);
	}

	memset(&stSockAddr, 0, sizeof(struct sockaddr_in));

	stSockAddr.sin_family = AF_INET;
	stSockAddr.sin_port = htons(PORT);
	stSockAddr.sin_addr.s_addr = INADDR_ANY;

	if (-1 == bind(SocketFD,(const struct sockaddr *)&stSockAddr,sizeof(struct sockaddr_in))) {
		perror("error bind failed");
		close(SocketFD);
		exit(EXIT_FAILURE);
	}

	if (-1 == listen(SocketFD, 10)) {
		perror("error listen failed");
		close(SocketFD);
		exit(EXIT_FAILURE);
	}
}

