#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "tcpStructs.h"

int sortClientCommands(std::vector<std::string> &tokens, clientStruct &clientStruct);
int sortServerCommands(std::vector<std::string> &tokens, clientStruct &clientStruct);