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
	if (tokens[index].empty())
	{
		std::cout << "No server IP address entered.\n";
		return 1;
	}
	else
	{
		if (tokens[index] == "host")
		{
			serverIP = "127.0.0.1";
		}
		else
		{
			serverIP = tokens[2];
		}
		return 0;
	}
}

int getInteger(std::vector<std::string> tokens, int index, bool isSigned, std::string intName, int &integer)
{
	if (tokens[index].empty())
	{
		std::cout << "No " << intName << "integer entered.\n";
		return 1;
	}
	else
	{
		try {
			integer = stoi(tokens[index]);
			if (isSigned == false)
			{
				integer = abs(integer);
			}
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

		result = getInteger(tokens, 2, false, "port number", serverCmds.portNum);
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
	if (tokens.size() < 4)
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

		// get server IP address.
		result = getServerIP(tokens, 2, clientCmds.serverIP);
		if (result != 0)
		{
			return 1;
		}

		// get server port number.
		result = getInteger(tokens, 3, false, "server port number", clientCmds.serverPortNum);
		if (result != 0)
		{
			return 1;
		}
	}

	// sort by clientType and finish populating clientCmds struct.
	if (clientCmds.clientType == 0) // message.
	{
		if (tokens.size() < 5)
		{
			std::cout << "Too few client message commands.\n";
			return 1;
		}
		else
		{
			// get message.
			int result = getMsg(tokens, 4, clientCmds.msg, false);
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
	else if (clientCmds.clientType == 1) // ping.
	{
		if (tokens.size() < 5)
		{
			std::cout << "Too few client ping commands.\n";
			return 1;
		}
		else
		{
			// get message.
			int result = getMsg(tokens, 4, clientCmds.msg, false);
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
	else if (clientCmds.clientType == 2) // counter.
	{
		if (tokens.size() < 5) // basic counter.
		{
			clientCmds.startInt = 0;
			clientCmds.endInt = INT_MAX;
			clientCmds.delay = 1000;
			clientCmds.loop = false;
			return 0;
		}
		else // custom counter.
		{
			if (tokens.size() < 7)
			{
				std::cout << "Too few client custom counter commands.\n";
				return 1;
			}
			else
			{
				// get counter start integer.
				int result = getInteger(tokens, 4, true, "startInt", clientCmds.startInt);
				if (result != 0)
				{
					return 1;
				}

				// get counter end integer.
				result = getInteger(tokens, 5, true, "endInt", clientCmds.endInt);
				if (result != 0)
				{
					return 1;
				}

				// get delay.
				result = getInteger(tokens, 6, false, "delay", clientCmds.delay);
				if (result != 0)
				{
					return 1;
				}

				// get loop status.
				if (tokens.size() >= 7)
				{
					if (tokens[7] == "loop")
					{
						clientCmds.loop = true;
					}
					else
					{
						clientCmds.loop = false;
					}
					return 0;
				}
			}
		}
	}
	else if (clientCmds.clientType == 3) // message repeater
	{
		if (tokens.size() < 6)
		{
			std::cout << "Too few client message repeat commands.\n";
			return 1;
		}
		else
		{
			// get message.
			int result = getMsg(tokens, 4, clientCmds.msg, true);
			if (result != 0)
			{
				return 1;
			}

			// get delay.
			result = getInteger(tokens, tokens.size() - 1, false, "delay", clientCmds.delay);
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