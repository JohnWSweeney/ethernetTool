#pragma once
#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <sstream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment (lib, "Ws2_32.lib")

struct msg
{
	char buffer[512] = { 0 };
	int bufferLen = sizeof(buffer);
	int msgLen;
};

struct sockStruct
{
	SOCKET listenSocket = INVALID_SOCKET;
	SOCKET acceptSocket = INVALID_SOCKET;
	addrinfo *host = NULL;
};

class tcp
{
private:
	long s = 0;
	long us = 1;
public:
	int socketReadStatus(SOCKET &socket);
	int openSocket(int localPortNum, sockStruct &sockStruct);
	int bindListen(sockStruct &sockStruct);
	int acceptConnection(sockStruct &sockStruct);
	int makeConnection();
	int rx(SOCKET &clientSocket, char *buffer, int bufferLen);
	int tx(SOCKET &clientSocket, char *buffer, int bufferLen);
	int shutdownSocket(SOCKET &socket);
	int closeSocket(SOCKET &socket);
};