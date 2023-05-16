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
	char payload[1472] = { 0 };
};

class udpSocket
{
public:
	long s = 0;
	long us = 1;
	int result = 0;
	SOCKET udpSocketServer;
	int socketReadStatus(SOCKET udpSocketServer);
	int socketWriteStatus(SOCKET udpSocketServer);
	void openSocket(int localPortNum);
	int rx(datagram& rxDatagram);
	int tx(const char* destIP, int destPortNum, const char *buf, int len);
	void closeSocket();
};