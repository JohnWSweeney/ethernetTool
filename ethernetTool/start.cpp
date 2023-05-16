#include "ethernetTool.h"
#include "udp.h"
#include <sstream>
#include <Windows.h>

extern std::atomic<bool> listenStatus;
extern std::atomic<bool> messageStatus;

void getInput(std::vector<std::string> &tokens)
{
	// Tokenize user input.
	do {
		std::string input, tempStr;
		std::getline(std::cin, input, '\n');
		std::stringstream strStream(input);
		while (getline(strStream, tempStr, ' '))
		{
			tokens.push_back(tempStr);
		}
	} while (tokens.empty());
}

void selectThread(bool &running, std::vector<std::string> &tokens)
{
	// start new thread running echo function.
	if (tokens[0] == "echo")
	{
		//startEchoThread(tokens);
	}
	// start new thread running listen function.
	else if (tokens[0] == "listen")
	{
		startListenThread(tokens);
	}
	// Send a single packet.
	else if (tokens[0] == "message")
	{
		startMessageThread(tokens);
	}
	// counter
	else if (tokens[0] == "counter")
	{
		startMessageThread(tokens);
	}
	// Terminate active threads, program.
	else if (tokens[0] == "exit")
	{
		listenStatus = false;
		messageStatus = false;
		running = false;
		Sleep(750);
	}
	// Reject all other inputs.
	else
	{
		std::cout << "Invalid command. Try again." << std::endl;
	}
	tokens.clear();
}