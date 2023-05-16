#pragma once
#include <iostream>
#include <vector>

// start.cpp
void getInput(std::vector<std::string> &tokens);
void selectThread(bool& running, std::vector<std::string>& tokens);

// udpFunctionSelect
void startListenThread(std::vector<std::string>& tokens);
void startMessageThread(std::vector<std::string>& tokens);


// udpFunctions.cpp
void listener(int localPortNum); // why no listen?
void message(std::string destIPstr, int destPortNum, std::string msg);