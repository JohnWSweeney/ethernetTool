#pragma once
#include <iostream>
#include "tcpCommands.h"

class Client
{
public:
	Client() {}
	~Client() {}
	void run(clientCmds clientCmds);
	void message(clientCmds clientCmds);
};

void startClient(clientCmds clientCmds);