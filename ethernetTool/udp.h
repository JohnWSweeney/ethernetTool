#pragma once
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<iostream>
#include<WinSock2.h>
#include<WS2tcpip.h>
#pragma comment (lib, "Ws2_32.lib")

struct datagram
{
	IN_ADDR sin_addr;
	USHORT sin_port;
	const char *destIP;
	int destPort;
	char buffer[512] = { 0 };
	int bufferLen = sizeof(buffer);
};

class udp
{
	long s = 0;
	long us = 1;
	int socketReadStatus(SOCKET &socket);
	int socketWriteStatus(SOCKET &socket);
public:
	int openSocket(int localPortNum, SOCKET &newSocket);
	int rx(SOCKET &socket, datagram &datagram);
	int tx(SOCKET &socket, const char* destIP, int destPortNum, const char *buffer, int len);
	int closeSocket(SOCKET socket);
};