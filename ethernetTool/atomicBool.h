#pragma once
#include <atomic>

extern std::atomic<bool> listenStatus;
extern std::atomic<bool> messageStatus;
extern std::atomic<bool> echoStatus;
extern std::atomic<bool> counterStatus;
extern std::atomic<bool> serverStatus;
extern std::atomic<bool> sessionStatus;
extern std::atomic<bool> clientStatus;