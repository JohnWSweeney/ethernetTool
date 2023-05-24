#include "start.h"
#include "udpThreads.h"

extern std::atomic<bool> listenStatus;
extern std::atomic<bool> messageStatus;
extern std::atomic<bool> echoStatus;
extern std::atomic<bool> counterStatus;

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

void selectThread(bool& running, std::vector<std::string>& tokens)
{
	// start new thread running listen function.
	if (tokens[0] == "listen")
	{
		startListenThread(tokens);
	}
	// Send a single packet.
	else if (tokens[0] == "message")
	{
		startMessageThread(tokens);
	}
	// start new thread running echo function.
	else if (tokens[0] == "echo")
	{
		startEchoThread(tokens);
	}
	//
	else if (tokens[0] == "counter")
	{
		startCounterThread(tokens);
	}
	// stop all active threads.
	else if (tokens[0] == "stop")
	{
		if (tokens[1] == "all")
		{
			listenStatus = false;
			messageStatus = false;
			echoStatus = false;
			counterStatus = false;
			std::cout << "All threads stopped." << '\n';
		}
		else
		{
			std::cout << "Invalid command. Try again." << '\n';
		}
	}
	// Terminate active threads, program.
	else if (tokens[0] == "exit")
	{
		listenStatus = false;
		messageStatus = false;
		echoStatus = false;
		counterStatus = false;
		running = false;
		Sleep(750);
	}
	// Reject all other inputs.
	else
	{
		std::cout << "Invalid command. Try again." << '\n';
	}
	tokens.clear();
}