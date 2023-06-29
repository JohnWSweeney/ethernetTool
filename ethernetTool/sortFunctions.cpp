#include "sortFunctions.h"
#include "atomicBool.h"

int copyIP(std::vector<std::string> &tokens, clientStruct &clientStruct)
{
	if (tokens[2] == "host")
	{
		clientStruct.serverIP = "127.0.0.1";
	}
	else
	{
		clientStruct.serverIP = tokens[2];
	}
	return 0;
}

int convertPortNum(std::vector <std::string> &tokens, clientStruct &clientStruct)
{
	try {
		clientStruct.serverPort = stoi(tokens[3]);
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

int copyMsg(std::vector<std::string> &tokens, clientStruct &clientStruct)
{
	if (tokens[4].empty())
	{
		return 1;
	}
	else
	{
		clientStruct.msg = tokens[4];
		for (int i = 5; i < tokens.size(); i++)
		{
			clientStruct.msg = clientStruct.msg + " " + tokens[i];
		}
		return 0;
	}
}

int populateClientStruct(std::vector<std::string> &tokens, clientStruct &clientStruct)
{
	int result = copyIP(tokens, clientStruct);
	if (result != 0)
	{
		return 1;
	}
	result = convertPortNum(tokens, clientStruct);
	if (result != 0)
	{
		return 1;
	}
	result = copyMsg(tokens, clientStruct);
	if (result != 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int sortClientCommands(std::vector<std::string> &tokens, clientStruct &clientStruct)
{
	if (tokens[1] == "message")
	{
		clientStruct.clientType = 1; // Message client.
		int result = populateClientStruct(tokens, clientStruct);
		if (result != 0)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		std::cout << "Invalid client command.\n";
		return 1;
	}
}