#pragma once
#include "udpCommands.h"

int getUdpFuncType(std::vector<std::string> tokens, int index, int &udpFuncType)
{
	if (tokens[index] == "listen")
	{
		udpFuncType = 0;
		return 0;
	}
	else if (tokens[index] == "echo")
	{
		udpFuncType = 1;
		return 0;
	}
	else if (tokens[index] == "message")
	{
		udpFuncType = 2;
		return 0;
	}
	else if (tokens[index] == "counter")
	{
		udpFuncType = 3;
		return 0;
	}
	else
	{
		std::cout << "Invalid function command.\n";
		return 1;
	}
}

int getIP(std::vector < std::string> tokens, int index, std::string &destIPstr)
{
	if (tokens[index].empty())
	{
		std::cout << "No destination IP address entered.\n";
		return 1;
	}
	else
	{
		if (tokens[index] == "host")
		{
			destIPstr = "127.0.0.1";
		}
		else
		{
			destIPstr = tokens[index];
		}
		return 0;
	}
}

int getUdpPortNum(std::vector<std::string> tokens, int index, int &portNum)
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

int getUdpMsg(std::vector<std::string> tokens, int index, std::string &msg)
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

int getInt(std::vector<std::string> tokens, int index, int &integer)
{
	if (tokens[index].empty())
	{
		std::cout << "No integer entered.\n";
		return 1;
	}
	else
	{
		try {
			integer = stoi(tokens[index]);
			return 0;
		}
		catch (std::invalid_argument)
		{
			std::cout << "Invalid integer.\n";
			return 1;
		}
		catch (std::out_of_range)
		{
			std::cout << "Integer is out of range.\n";
			return 1;
		}
	}
}

int getDelay(std::vector<std::string> tokens, int index, unsigned int &delay)
{
	if (tokens[index].empty())
	{
		std::cout << "No integer entered.\n";
		return 1;
	}
	else
	{
		try {
			delay = stoul(tokens[index]);
			return 0;
		}
		catch (std::invalid_argument)
		{
			std::cout << "Invalid integer.\n";
			return 1;
		}
		catch (std::out_of_range)
		{
			std::cout << "Integer is out of range.\n";
			return 1;
		}
	}
}

int populateUdpCmds(std::vector<std::string> tokens, udpCmds &udpCmds)
{
	if (tokens.size() < 2) // 
	{
		std::cout << "Too few UDP commands.\n";
		return 1;
	}
	else // get function type and token count.
	{
		int result = getUdpFuncType(tokens, 0, udpCmds.udpFuncType);
		if (result != 0)
		{
			return 1;
		}
	}

	if (udpCmds.udpFuncType == 0 || udpCmds.udpFuncType == 1) // populate listen, echo functions fields.
	{
		int result = getUdpPortNum(tokens, 1, udpCmds.localPortNum); // get local port.
		if (result != 0)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else if (udpCmds.udpFuncType == 2) // populate message function fields.
	{
		int result = getIP(tokens, 1, udpCmds.destIPstr); // get dest IP addr.
		if (result != 0)
		{
			return 1;
		}
			
		result = getUdpPortNum(tokens, 2, udpCmds.destPortNum); // get dest port.
		if (result != 0)
		{
			return 1;
		}

		result = getUdpMsg(tokens, 3, udpCmds.msg); // get message.
		if (result != 0)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else if (udpCmds.udpFuncType == 3) // populate counter function fields.
	{
		int result = getIP(tokens, 1, udpCmds.destIPstr); // get dest IP addr.
		if (result != 0)
		{
			return 1;
		}

		result = getUdpPortNum(tokens, 2, udpCmds.destPortNum); // get dest port.
		if (result != 0)
		{
			return 1;
		}

		result = getInt(tokens, 3, udpCmds.startInt); // get start integer.
		if (result != 0)
		{
			std::cout << "Invalid counter start integer.\n";
			return 1;
		}

		result = getInt(tokens, 4, udpCmds.endInt); // get end integer.
		if (result != 0)
		{
			std::cout << "Invalid counter end integer.\n";
			return 1;
		}

		result = getDelay(tokens, 5, udpCmds.delay); // get delay integer.
		if (result != 0)
		{
			std::cout << "Invalid counter delay integer.\n";
			return 1;
		}

		if (tokens.size() < 7)
		{
			udpCmds.loop = false;
			return 0;
		}
		else
		{
			if (tokens[6] == "loop")
			{
				udpCmds.loop = true;
				return 0;
			}
			else
			{
				std::cout << "Invalid counter loop argument.\n";
				return 1;
			}
		}
	}
}