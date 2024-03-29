#include "start.h"
#include "tcpThreads.h"
#include "udpThreads.h"
#include "atomicBool.h"

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

void startMenu(bool& running)
{
	std::vector<std::string> tokens;
	getInput(tokens);

	// start thread running UDP listen function.
	if (tokens[0] == "listen")
	{
		if (tokens[1] == "stop")
		{
			listenStatus = false;
		}
		else
		{
			startListenThread(tokens);
		}
	}
	// Send a UDP message.
	else if (tokens[0] == "message")
	{
		startMessageThread(tokens);
	}
	// start thread running UDP echo function.
	else if (tokens[0] == "echo")
	{
		if (tokens[1] == "stop")
		{
			echoStatus = false;
		}
		else
		{
			startEchoThread(tokens);
		}	
	}
	// start thread running UDP counter function.
	else if (tokens[0] == "counter")
	{
		if (tokens[1] == "stop")
		{
			counterStatus = false;
		}
		else
		{
			startCounterThread(tokens);
		}
	}
	// start TCP servwr thread.
	else if (tokens[0] == "server")
	{
		if (tokens[1] == "stop")
		{
			serverStatus = false;
		}
		else
		{
			startServerThread(tokens);
		}
	}
	// stop TCP session thread(s).
	else if (tokens[0] == "session")
	{
		if (tokens[1] == "stop")
		{
			sessionStatus = false;
		}
	}
	// start TCP client thread.
	else if (tokens[0] == "client")
	{
		if (tokens[1] == "stop")
		{
			clientStatus = false;
		}
		else
		{
			startClientThread(tokens);
		}
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
			serverStatus = false;
			sessionStatus = false;
			clientStatus = false;
			std::cout << "All threads stopped.\n";
		}
		else
		{
			std::cout << "Invalid stop command.\n";
		}
	}
	// Terminate active threads, program.
	else if (tokens[0] == "exit")
	{
		listenStatus = false;
		messageStatus = false;
		echoStatus = false;
		counterStatus = false;
		serverStatus = false;
		sessionStatus = false;
		clientStatus = false;
		running = false;
	}
	// Reject all other inputs.
	else
	{
		std::cout << "Invalid function command.\n";
	}
}