#include "session.h"
#include "tcp.h"
#include "atomicBool.h"

void Session::run(SOCKET socket, int sessionType)
{
	if (sessionType == 0)
	{
		this->listen(socket);
	}
	else if (sessionType == 1)
	{
		this->echo(socket);
	}
	else
	{
		return;
	}
}

void Session::listen(SOCKET socket)
{
	tcp listen;
	int result;
	do {
		msg newMsg;
		result = listen.rx(socket, newMsg.buffer, newMsg.bufferLen);
		if (result > 0)
		{
			std::cout << "Server received: " << newMsg.buffer << '\n';
		}
		else if (result < -1)
		{
			std::cout << "Server listen rx failed.\n";
		}
	} while (result > 0);
}

void Session::echo(SOCKET socket)
{
	tcp echo;
	int result;
	do {
		msg newMsg;
		result = echo.rx(socket, newMsg.buffer, newMsg.bufferLen);
		if (result > 0)
		{
			int txResult = echo.tx(socket, newMsg.buffer, result);
			if (txResult > 0)
			{
				std::cout << "Server echoed: " << newMsg.buffer << '\n';
			}
			else if (txResult < -1)
			{
				std::cout << "Server echo.tx failed.\n";
				break;
			}
		}
		else if (result < -1)
		{
			std::cout << "Server echo.rx failed.\n";
		}
	} while (result > 0);
}

void startSession(SOCKET socket, int sessionType)
{
	Session newSession(socket);
	newSession.run(socket, sessionType);
}
