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
	int payloadLen;
	char payload[65527] = { 0 };
};

class udp
{
public:
	long s = 0;
	long us = 1;
	int result = 0;
	SOCKET udpSocket = INVALID_SOCKET;
	int socketReadStatus(SOCKET &udpSocket);
	int socketWriteStatus(SOCKET &udpSocket);
	int openSocket(int localPortNum);
	int rx(datagram &rxDatagram);
	int tx(const char* destIP, int destPortNum, const char *buf, int len);
	int closeSocket();
};