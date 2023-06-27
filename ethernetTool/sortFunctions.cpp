#include "sortFunctions.h"

int sortClientCommands(std::vector<std::string> &tokens, clientStruct &clientStruct)
{
	// message serverIP serverPort msg 
	if (tokens[1] == "message")
	{
		// Set client type.
		clientStruct.clientType = 1;
		// Set server IP address.
		if (tokens[2] == "host")
		{
			clientStruct.serverIP = "127.0.0.1";
		}
		else
		{
			clientStruct.serverIP = tokens[2];
		}
		// Set server port number.
		try {
			clientStruct.serverPort = stoi(tokens[3]);
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
		// Set message.
		clientStruct.msg = tokens[4];
		for (int i = 5; i < tokens.size(); i++)
		{
			clientStruct.msg = clientStruct.msg + " " + tokens[i];
		}
		return 0;
	}
	else
	{
		std::cout << "Invalid client command.\n";
		return 1;
	}
}