#pragma once
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
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

class tcp
{
private:
	long s = 0;
	long us = 1;
public:
	int socketReadStatus(SOCKET &socket);
	int socketWriteStatus(SOCKET &socket);
	int openServerSocket(int localPortNum, SOCKET &listenSocket);
	int openClientSocket(SOCKET &clientSocket, std::string serverIP, int serverPort);
	int acceptConnection(SOCKET &listenSocket, SOCKET &acceptSocket);
	int rx(SOCKET &socket, char *buffer, int bufferLen);
	int tx(SOCKET &socket, const char *buffer, int bufferLen);
	int shutdownSocket(SOCKET &socket);
	int closeSocket(SOCKET &socket);
};