#pragma once
#include <iostream>
#include <WinSock2.h>

struct clientStruct
{
	int clientType;
	// clientType 0: Listen
	// clientType 1: Message
	// clientType 2: Echo
	SOCKET socket = INVALID_SOCKET;
	std::string serverIP;
	int serverPort;
	std::string msg;
};

struct serverStruct
{
	SOCKET listenSocket = INVALID_SOCKET;
	SOCKET acceptSocket = INVALID_SOCKET;
};
