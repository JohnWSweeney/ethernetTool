#pragma once
#include <iostream>
#include <string>
#include <vector>

struct serverCmds
{
	int sessionType;
	int portNum;
};

struct clientCmds
{
	int clientType;
	std::string serverIP;
	int serverPortNum;
	std::string msg;
	int delay;
};

int getSessionType(std::vector<std::string> tokens, int index, int &sessionType);
int getClientType(std::vector<std::string> tokens, int index, int &clientType);
int getServerIP(std::vector<std::string> tokens, int index, std::string &serverIP);
int getInteger(std::vector<std::string> tokens, int index, int &integer, std::string intName);
int getMsg(std::vector<std::string> tokens, int index, std::string &msg);
int populateServerCmds(std::vector<std::string> tokens, serverCmds &serverCmds);
int populateClientCmds(std::vector<std::string> tokens, clientCmds &clientCmds);