#pragma once
#include <iostream>
#include <atomic>

void listener(int localPortNum);
void message(std::string destIPstr, int destPortNum, std::string msg);
void echo(int localPortNum);
void counter(std::string destIPstr, int destPortNum, int start, int end, unsigned int delay, bool loop);