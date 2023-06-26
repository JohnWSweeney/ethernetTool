#include "tcpThreads.h"
#include "server.h"
#include "session.h"
#include "atomicBool.h"

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

void startServerThread(std::vector<std::string> &tokens)
{
	if (tokens[1] == "listen" || tokens[1] == "echo")
	{
		int sessionType;
		if (tokens[1] == "listen")
		{
			sessionType = 0;
		}
		else if (tokens[1] == "echo")
		{
			sessionType = 1;
		}

		try {
			int portNum = std::stoi(tokens[2]);
			serverStatus = true;
			std::thread serverThread(startServer, portNum, sessionType);
			serverThread.detach();
		}
		catch (std::invalid_argument)
		{
			std::cout << "Invalid port number.\n";
		}
		catch (std::out_of_range)
		{
			std::cout << "Port number is out of range.\n";
		}
	}
	else if (tokens[1] == "stop")
	{
		serverStatus = false;
	}
	else
	{
		std::cout << "Invalid start/stop command.\n";
	}
}