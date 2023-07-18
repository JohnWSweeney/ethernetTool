#include "tcpCommands.h"

int getSessionType(std::vector<std::string> tokens, int index, int &sessionType)
{
	if (tokens[index] == "listen")
	{
		sessionType = 0;
		return 0;
	}
	else if (tokens[index] == "echo")
	{
		sessionType = 1;
		return 0;
	}
	else if (tokens[index] == "pong")
	{
		sessionType = 2;
		return 0;
	}
	else
	{
		std::cout << "Invalid server session command.\n";
		return 1;
	}
}

int getClientType(std::vector<std::string> tokens, int index, int &clientType, int &tokenCount)
{
	if (tokens[index] == "message")
	{
		clientType = 0;
		tokenCount = 5;
		return 0;
	}
	else if (tokens[index] == "ping")
	{
		clientType = 1;
		tokenCount = 5;
		return 0;
	}
	else if (tokens[index] == "counter")
	{
		clientType = 2;
		tokenCount = 4;
		return 0;
	}
	else if (tokens[index] == "repeat")
	{
		clientType = 3;
		tokenCount = 5;
		return 0;
	}
	else
	{
		std::cout << "Invalid client command.\n";
		return 1;
	}
}

int getServerIP(std::vector<std::string> tokens, int index, std::string &serverIP)
{
	if (tokens[index] == "host")
	{
		serverIP = "127.0.0.1";
	}
	else
	{
		serverIP = tokens[index];
	}
	return 0;
}

int getTcpPortNum(std::vector<std::string> tokens, int index, int &portNum)
{
	if (tokens[index].empty())
	{
		std::cout << "No port number entered.\n";
		return 1;
	}
	else
	{
		try {
			portNum = stoi(tokens[index]);
			return 0;
		}
		catch (std::invalid_argument)
		{
			std::cout << "Invalid port number.\n";
			return 1;
		}
		catch (std::out_of_range)
		{
			std::cout << "Port number is out of range.\n";
			return 1;
		}
	}
}

int getMsg(std::vector<std::string> tokens, int index, std::string &msg)
{
	if (tokens[index].empty())
	{
		std::cout << "No message entered.\n";
		return 1;
	}
	else
	{
		msg = tokens[index];
		for (int i = index + 1; i < tokens.size(); i++)
		{
			msg = msg + " " + tokens[i];
		}
		return 0;
	}
}

int populateServerCmds(std::vector<std::string> tokens, serverCmds &serverCmds)
{
	if (tokens.size() < 3)
	{
		std::cout << "Too few server commands.\n";
		return 1;
	}
	else
	{
		int result = getSessionType(tokens, 1, serverCmds.sessionType);
		if (result != 0)
		{
			return 1;
		}

		result = getTcpPortNum(tokens, 2, serverCmds.portNum);
		if (result != 0)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
}

int populateClientCmds(std::vector<std::string> tokens, clientCmds &clientCmds)
{
	if (tokens.size() < 2)
	{
		std::cout << "Too few client commands.\n";
		return 1;
	}
	else
	{
		int result = getClientType(tokens, 1, clientCmds.clientType, clientCmds.tokenCount);
		if (result != 0)
		{
			return 1;
		}
	}

	if (tokens.size() < clientCmds.tokenCount)
	{
		std::cout << "Too few client commands.\n";
		return 1;
	}
	else
	{
		int result = getServerIP(tokens, 2, clientCmds.serverIP);
		if (result != 0)
		{
			return 1;
		}

		result = getTcpPortNum(tokens, 3, clientCmds.serverPortNum);
		if (result != 0)
		{
			return 1;
		}

		if (clientCmds.tokenCount < 5)
		{
			return 0;
		}
		else
		{
			int result = getMsg(tokens, 4, clientCmds.msg);
			if (result != 0)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
	}
}