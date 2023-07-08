#include "server.h"
#include "tcpThreads.h"
#include "tcp.h"
#include "atomicBool.h"

std::string serverTypeStr(int sessionType)
{
	std::string serverType;
	if (sessionType == 0)
	{
		return serverType = "Listen";
	}
	else if (sessionType == 1)
	{
		return serverType = "Echo";
	}
}

void Server::run(int portNum, int sessionType)
{
	tcp server;
	SOCKET listenSocket = INVALID_SOCKET;
	std::string serverType = serverTypeStr(sessionType);
	int result = server.openServerSocket(portNum, listenSocket);
	if (result != 0)
	{
		return;
	}
	else
	{
		std::cout << serverType << " server started on port " << portNum << ".\n";
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
	std::cout << serverType << " server terminated on port " << portNum << ".\n";
}

void startServer(int portNum, int sessionType)
{
	Server newServer;
	newServer.run(portNum, sessionType);
}