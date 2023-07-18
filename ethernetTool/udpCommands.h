#pragma once
#include <iostream>
#include <string>
#include <vector>

struct udpCmds
{
	int udpFuncType;
	int localPortNum;
	std::string destIPstr;
	int destPortNum;
	std::string msg;
	int startInt;
	int endInt;
	unsigned int delay;
	bool loop;
	
};

int getUdpFuncType(std::vector<std::string> tokens, int index, int &udpFuncType);
int getIP(std::vector < std::string> tokens, int index, std::string &destIPstr);
int getUdpPortNum(std::vector<std::string> tokens, int index, int &portNum);
int getUdpMsg(std::vector<std::string> tokens, int index, std::string &msg);
int getInt(std::vector<std::string> tokens, int index, int &integer);
int getDelay(std::vector<std::string> tokens, int index, unsigned int &delay);
int populateUdpCmds(std::vector<std::string> tokens, udpCmds &udpCmds);