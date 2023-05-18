#pragma once
#include <iostream>
#include <vector>

// start.cpp
void getInput(std::vector<std::string> &tokens);
void selectThread(bool& running, std::vector<std::string>& tokens);

// udpFunctionSelect
void startListenThread(std::vector<std::string>& tokens);
void startMessageThread(std::vector<std::string>& tokens);
void startEchoThread(std::vector<std::string>& tokens);
void startCounterThread(std::vector<std::string>& tokens);


// udpFunctions.cpp
void listener(int localPortNum);
void message(std::string destIPstr, int destPortNum, std::string msg);
void echo(int localPortNum);
void counter(std::string destIPstr, int destPortNum, int start, int end, int delay, bool loop);