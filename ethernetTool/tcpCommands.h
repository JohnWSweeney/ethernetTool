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
	int tokenCount;
	std::string serverIP;
	int serverPortNum;
	std::string msg;
	int startInt;
	int endInt;
	int delay;
	bool loop;
};

int getSessionType(std::vector<std::string> tokens, int index, int &sessionType);
int getClientType(std::vector<std::string> tokens, int index, int &clientType);
int getServerIP(std::vector<std::string> tokens, int index, std::string &serverIP);
int getInteger(std::vector<std::string> tokens, int index, bool isSigned, std::string intName, int &integer);
int getMsg(std::vector<std::string> tokens, int index, std::string &msg, bool isRepeat);
int populateServerCmds(std::vector<std::string> tokens, serverCmds &serverCmds);
int populateClientCmds(std::vector<std::string> tokens, clientCmds &clientCmds);