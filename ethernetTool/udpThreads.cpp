#include "udpThreads.h"
#include "udpFunctions.h"
#include "atomicBool.h"

void startListenThread(std::vector<std::string>& tokens)
{
	if (tokens[1] == "start")
	{
		try {
			int portNum = std::stoi(tokens[2]);
			listenStatus = true;
			std::thread listenThread(listener, portNum);
			listenThread.detach();
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
		listenStatus = false;
	}
	else
	{
		std::cout << "Invalid start/stop command.\n";
	}
}

void startMessageThread(std::vector<std::string>& tokens)
{
	try {
		std::string destIPstr = tokens[1];
		int portNum = std::stoi(tokens[2]);
		std::string msg = tokens[3];
		for (int i = 4; i < tokens.size(); i++)
		{
			msg = msg + " " + tokens[i];
		}
		messageStatus = true;
		std::thread messageThread(message, destIPstr, portNum, msg);
		messageThread.detach();
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

void startEchoThread(std::vector<std::string>& tokens)
{
	if (tokens[1] == "start")
	{
		try {
			int portNum = std::stoi(tokens[2]);
			echoStatus = true;
			std::thread echoThread(echo, portNum);
			echoThread.detach();
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
		
		echoStatus = false;
	}
	else
	{
		std::cout << "Invalid start/stop command.\n";
	}
}

void startCounterThread(std::vector<std::string>& tokens)
{
	if (tokens[1] == "stop")
	{
		counterStatus = false;
	}
	else
	{
		try {
			std::string destIPstr = tokens[1];
			int portNum = std::stoi(tokens[2]);
			int start = std::stoi(tokens[3]);
			int end = std::stoi(tokens[4]);
			unsigned int delay = std::stoi(tokens[5]); // detect negative integer.

			bool loop;
			if (tokens.size() < 7)
			{
				loop = false;
			}
			else if (tokens[6] == "loop")
			{
				loop = true;
			}
			else // invalid input detect
			{
				loop = false;
				std::cout << "Invalid loop command. Loop disabled.\n";
			}

			counterStatus = true;
			std::thread counterThread(counter, destIPstr, portNum, start, end, delay, loop);
			counterThread.detach();
		}
		catch (std::invalid_argument)
		{
			std::cout << "Invalid port number and/or start, end, and/or delay integer(s).\n";
		}
		catch (std::out_of_range)
		{
			std::cout << "Port number and/or start, end, and/or delay integer(s) is/are out of range.\n";
		}
	}
}