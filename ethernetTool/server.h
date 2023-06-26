#pragma once
#include <iostream>

class Server
{
public:
	Server(){};
	~Server(){}
	void run(int portNum, int sessionType);
};

void startServer(int portNum, int sessionType);