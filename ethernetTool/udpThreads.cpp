#include "udpThreads.h"
#include "udpFunctions.h"
#include "udpCommands.h"
#include "atomicBool.h"

void startListenThread(std::vector<std::string> tokens)
{
	udpCmds udpCmds;
	int result = populateUdpCmds(tokens, udpCmds);
	if (result != 0)
	{
		return;
	}
	else
	{
		listenStatus = true;
		std::thread listenThread(listener, udpCmds.localPortNum);
		listenThread.detach();
	}
}

void startEchoThread(std::vector<std::string> tokens)
{
	udpCmds udpCmds;
	int result = populateUdpCmds(tokens, udpCmds);
	if (result != 0)
	{
		return;
	}
	else
	{
		echoStatus = true;
		std::thread echoThread(echo, udpCmds.localPortNum);
		echoThread.detach();
	}
}

void startMessageThread(std::vector<std::string> tokens)
{
	udpCmds udpCmds;
	int result = populateUdpCmds(tokens, udpCmds);
	if (result != 0)
	{
		return;
	}
	else
	{
		messageStatus = true;
		std::thread messageThread(message, udpCmds.destIPstr, udpCmds.destPortNum, udpCmds.msg);
		messageThread.detach();
	}
}

void startCounterThread(std::vector<std::string> tokens)
{
	udpCmds udpCmds;
	int result = populateUdpCmds(tokens, udpCmds);
	if (result != 0)
	{
		return;
	}
	else
	{
		counterStatus = true;
		std::thread counterThread(counter, udpCmds.destIPstr, udpCmds.destPortNum, udpCmds.startInt, udpCmds.endInt, udpCmds.delay, udpCmds.loop);
		counterThread.detach();
	}
}