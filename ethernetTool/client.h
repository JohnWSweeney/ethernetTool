#pragma once
#include <iostream>
#include "tcpCommands.h"

class Client
{
	// clientType 0: message.
	// clientType 1: ping.
	// clientType 2: counter.
public:
	Client() {}
	~Client() {}
	void run(clientCmds clientCmds);
	void message(clientCmds clientCmds);
	void ping(clientCmds clientCmds);
	void counter(clientCmds clientCmds);
};

void startClient(clientCmds clientCmds);