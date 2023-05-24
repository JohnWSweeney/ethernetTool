#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <thread>

void startListenThread(std::vector<std::string>& tokens);
void startMessageThread(std::vector<std::string>& tokens);
void startEchoThread(std::vector<std::string>& tokens);
void startCounterThread(std::vector<std::string>& tokens);