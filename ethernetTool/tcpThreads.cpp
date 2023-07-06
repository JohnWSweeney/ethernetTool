#include "tcpThreads.h"
#include "tcpCommands.h"
#include "server.h"
#include "session.h"
#include "client.h"
#include "atomicBool.h"

void startServerThread(std::vector<std::string> tokens)
{
	serverCmds serverCmds;
	int result = populateServerCmds(tokens, serverCmds);
	if (result != 0)
	{
		return;
	}
	else
	{
		serverStatus = true;
		std::thread serverThread(startServer, serverCmds.portNum, serverCmds.sessionType);
		serverThread.detach();
	}
}

void startSessionThread(SOCKET socket, int sessionType)
{
	try {
		std::thread sessionThread(startSession, socket, sessionType);
		sessionThread.detach();
	}
	catch (...)
	{
		std::cout << "Session start failed.\n";
	}
}

void startClientThread(std::vector<std::string> tokens)
{
	clientCmds clientCmds;
	int result = populateClientCmds(tokens, clientCmds);
	if (result != 0)
	{
		return;
	}
	else
	{
		try {
			std::thread clientThread(startClient, clientCmds);
			clientThread.detach();
		}
		catch (...)
		{
			std::cout << "Client start failed.\n";
		}
	}
}