#pragma once
#include <iostream>
#include "tcpStructs.h"

class Client
{
public:
	Client()
	{
		std::cout << "Client started.\n";
	}
	~Client()
	{
		std::cout << "Client closed.\n\n";
	}
	void run(clientStruct clientStruct);
	void message(clientStruct clientStruct);
};

void startClient(clientStruct clientStruct);