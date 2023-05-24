#pragma once
#include <iostream>
#include <vector>
#include <sstream>
#include <Windows.h>

void getInput(std::vector<std::string> &tokens);
void selectThread(bool& running, std::vector<std::string>& tokens);