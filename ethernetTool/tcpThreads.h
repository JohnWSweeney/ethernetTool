#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <WinSock2.h>

void startServerThread(std::vector<std::string> tokens);
void startSessionThread(SOCKET socket, int sessionType);
void startClientThread(std::vector<std::string> tokens);