#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "tcpStructs.h"

int copyIP(std::vector<std::string> &tokens, clientStruct &clientStruct);
int convertPortNum(std::vector <std::string> &tokens, clientStruct &clientStruct);
int copyMsg(std::vector<std::string> &tokens, clientStruct &clientStruct);
int populateClientStruct(std::vector<std::string> &tokens, clientStruct &clientStruct);
int sortClientCommands(std::vector<std::string> &tokens, clientStruct &clientStruct);