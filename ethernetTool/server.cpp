#include "server.h"
#include "tcpThreads.h"
#include "tcp.h"
#include "atomicBool.h"

void Server::run(int portNum, int sessionType)
{
	tcp server;
	SOCKET listenSocket = INVALID_SOCKET;
	int result = server.openServerSocket(portNum, listenSocket);
	if (result != 0)
	{
		return;
	}
	else
	{
		std::cout << "Server started.\n";
	}

	do {
		result = server.socketReadStatus(listenSocket);
		if (result > 0)
		{
			SOCKET acceptSocket = INVALID_SOCKET;
			int acceptResult = server.acceptConnection(listenSocket, acceptSocket);
			if (acceptResult == 0)
			{
				startSessionThread(std::move(acceptSocket), sessionType);
			}
		}
	} while (serverStatus);
	server.closeSocket(listenSocket);
	std::cout << "Server terminated on port " << portNum << ".\n";
}

void startServer(int portNum, int sessionType)
{
	Server newServer;
	newServer.run(portNum, sessionType);
}