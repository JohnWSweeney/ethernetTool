#include "server.h"
#include "tcpThreads.h"
#include "tcp.h"
#include "atomicBool.h"

void Server::run(int portNum, int sessionType)
{
	tcp server;
	serverStruct serverStruct;
	int result = server.openServerSocket(portNum, serverStruct.listenSocket);
	if (result != 0)
	{
		return;
	}
	else
	{
		std::cout << "Server started.\n\n";
	}

	do {
		result = server.socketReadStatus(serverStruct.listenSocket);
		if (result > 0)
		{
			int acceptResult = server.acceptConnection(serverStruct.listenSocket, serverStruct.acceptSocket);
			if (acceptResult == 0)
			{
				startSessionThread(std::move(serverStruct.acceptSocket), sessionType);
			}
		}
	} while (serverStatus);
	server.closeSocket(serverStruct.listenSocket);
	std::cout << "Server terminated on port " << portNum << ".\n";
}

void startServer(int portNum, int sessionType)
{
	Server newServer;
	newServer.run(portNum, sessionType);
}