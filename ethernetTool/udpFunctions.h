#pragma once
#include <iostream>

// udpFuncType 0: Listen.
// udpFuncType 1: Echo.
// udpFuncType 2: Message.
// udpFuncType 3: Counter.

void listener(int localPortNum);
void echo(int localPortNum);
void message(std::string destIPstr, int destPortNum, std::string msg);
void counter(std::string destIPstr, int destPortNum, int start, int end, unsigned int delay, bool loop);