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
		if (result > 0) // message received.
		{
			std::cout << "Server received: " << newMsg.buffer << '\n';
		}
		else if (result == -1) // peer closed connection gracefully.
		{
			std::cout << "Server: listen peer gracefully closed connection.\n";
		}
		else if (result < -1) // error.
		{
			std::cout << "Server: listen.rx failed.\n";
		}
	} while (result > -1);

	// gracefully close connection.
	result = listen.closeConnection(socket, false);
	if (result != 0)
	{
		std::cout << "Server: listen.closeConnection failed.\n";
		return;
	}
	std::cout << "Server: listen session ended.\n";
}

void Session::echo(SOCKET socket)
{
	tcp echo;
	int result;
	do {
		msg newMsg;
		result = echo.rx(socket, newMsg.buffer, newMsg.bufferLen);
		if (result > 0) // message received.
		{
			int sendResult = echo.tx(socket, newMsg.buffer, result);
			if (sendResult > 0) // message echoed to peer.
			{
				std::cout << "Server echoed: " << newMsg.buffer << '\n';
			}
			else if (sendResult == -1) // echo.tx error.
			{
				std::cout << "Server: echo.tx failed.\n";
				closesocket(socket);
				return;
			}
		}
		else if (result == -1) // peer closed connection gracefully.
		{
			std::cout << "Server: echo peer gracefully closed connection.\n";
		}
		else if (result < -1) // error.
		{
			std::cout << "Server: echo.rx failed.\n";
			closesocket(socket);
			return;
		}
	} while (result > -1);

	// gracefully close connection.
	result = echo.closeConnection(socket, false);
	if (result != 0)
	{
		std::cout << "Server: echo.closeConnection failed.\n";
		return;
	}
	std::cout << "Server: echo session ended.\n";
}

void startSession(SOCKET socket, int sessionType)
{
	Session newSession(socket);
	newSession.run(socket, sessionType);
}