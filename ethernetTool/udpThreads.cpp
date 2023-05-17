#include "ethernetTool.h"
#include <string>
#include <thread>
#include <atomic>

std::atomic<bool> listenStatus;
std::atomic<bool> messageStatus;
std::atomic<bool> echoStatus;

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
		catch (...) {
			std::cout << "Error starting listen." << '\n';
		}
	}
	else if (tokens[1] == "stop")
	{
		listenStatus = false;
	}
	else
	{
		std::cout << "Invalid command. Try again." << '\n';
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
	catch (...) {
		std::cout << "Error sending message." << '\n';
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
		catch (...)
		{
			std::cout << "Error starting echo." << '\n';
		}
	}
	else if (tokens[1] == "stop")
	{
		
		echoStatus = false;
	}
	else
	{
		std::cout << "Invalid command. Try again." << '\n';
	}
}