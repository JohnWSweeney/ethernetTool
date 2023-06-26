#pragma once
#include <iostream>
#include <WinSock2.h>

class Session : public std::enable_shared_from_this<Session>
{
	// sessionType 0: Listen.
	// sessionType 1: Echo.
public:
	Session(SOCKET socket)
	{
		std::cout << "New session.\n";
	}
	~Session()
	{
		std::cout << "Session ended.\n\n";
	}
	void run(SOCKET socket, int sessionType);
	void listen(SOCKET socket);
	void echo(SOCKET socket);
};

void startSession(SOCKET socket, int sessionType);