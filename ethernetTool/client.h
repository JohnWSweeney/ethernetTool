#pragma once
#include <iostream>
#include "tcpCommands.h"

class Client
{
	// clientType 0: message.
	// clientType 1: ping.
public:
	Client() {}
	~Client() {}
	void run(clientCmds clientCmds);
	void message(clientCmds clientCmds);
	void ping(clientCmds clientCmds);
};

void startClient(clientCmds clientCmds);