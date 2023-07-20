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

int getClientType(std::vector<std::string> tokens, int index, int &clientType)
{
	if (tokens[index] == "message")
	{
		clientType = 0;
		return 0;
	}
	else if (tokens[index] == "ping")
	{
		clientType = 1;
		return 0;
	}
	else if (tokens[index] == "counter")
	{
		clientType = 2;
		return 0;
	}
	else if (tokens[index] == "repeat")
	{
		clientType = 3;
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

int getInteger(std::vector<std::string> tokens, int index, int &integer, std::string intName)
{
	if (tokens[index].empty())
	{
		std::cout << "No " << intName << "integer entered.\n";
		return 1;
	}
	else
	{
		try {
			integer = abs(stoi(tokens[index]));
			return 0;
		}
		catch (std::invalid_argument)
		{
			std::cout << "Invalid integer for " << intName << ".\n";
			return 1;
		}
		catch (std::out_of_range)
		{
			std::cout << "Invalid integer for " << intName << ": out of range.\n";
			return 1;
		}
	}
}

int getMsg(std::vector<std::string> tokens, int index, std::string &msg, bool isRepeat)
{
	if (tokens[index].empty())
	{
		std::cout << "No message entered.\n";
		return 1;
	}
	else
	{
		int msgEnd;
		if (isRepeat == true)
		{
			msgEnd = tokens.size() - 1;
		}
		else
		{
			msgEnd = tokens.size();
		}

		msg = tokens[index];
		for (int i = index + 1; i < msgEnd; i++)
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

		result = getInteger(tokens, 2, serverCmds.portNum, "port number");
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
		int result = getClientType(tokens, 1, clientCmds.clientType);
		if (result != 0)
		{
			return 1;
		}
	}

	// get server IP address.
	int result = getServerIP(tokens, 2, clientCmds.serverIP);
	if (result != 0)
	{
		return 1;
	}

	// get server port number.
	result = getInteger(tokens, 3, clientCmds.serverPortNum, "server port number");
	if (result != 0)
	{
		return 1;
	}

	// sort by clientType and finish populating clientCmds struct.
	switch (clientCmds.clientType) {
	case 0: // message.
		result = getMsg(tokens, 4, clientCmds.msg, false);
		if (result != 0)
		{
			return 1;
		}
		else
		{
			return 0;
		}
		//break;
	case 1: // ping.
		result = getMsg(tokens, 4, clientCmds.msg, false);
		if (result != 0)
		{
			return 1;
		}
		else
		{
			return 0;
		}
		//break;
	case 2: // counter.
		return 0;
	case 3: // repeat.
		result = getMsg(tokens, 4, clientCmds.msg, true);
		if (result != 0)
		{
			return 1;
		}

		result = getInteger(tokens, tokens.size() - 1, clientCmds.delay, "delay");
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