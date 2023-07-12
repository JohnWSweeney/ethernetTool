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
	else if (sessionType == 2)
	{
		return serverType = "Pong";
	}
}

void Server::run(int portNum, int sessionType)
{
	tcp server;
	SOCKET listenSocket = INVALID_SOCKET;
	SOCKET acceptSocket = INVALID_SOCKET;
	std::string serverType = serverTypeStr(sessionType);
	int result = server.openServerSocket(portNum, listenSocket);
	if (result != 0)
	{
		closesocket(listenSocket);
		WSACleanup();
		return;
	}
	else
	{
		std::cout << serverType << " server started on port " << portNum << ".\n";
	}

	// listen for client connections, start new session thread on connection acceptance.
	do {
		result = server.socketReadStatus(listenSocket);
		if (result > 0)
		{
			int acceptResult = server.acceptConnection(listenSocket, acceptSocket);
			if (acceptResult != 0)
			{
				closesocket(listenSocket);
				closesocket(acceptSocket);
				WSACleanup();
				return;
			}
			else
			{
				startSessionThread(std::move(acceptSocket), sessionType);
			}
		}
	} while (serverStatus);
	closesocket(listenSocket);
	closesocket(acceptSocket);
	WSACleanup();
	std::cout << serverType << " server terminated on port " << portNum << ".\n";
}
void startServer(int portNum, int sessionType)
{
	Server newServer;
	newServer.run(portNum, sessionType);
}