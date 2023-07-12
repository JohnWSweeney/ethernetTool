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
	else if (sessionType == 2)
	{
		this->pong(socket);
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

void Session::pong(SOCKET socket)
{
	tcp pong;
	int result;

	// ping-pong message with client until client or server closes connection or error.
	do {
		msg newMsg;
		result = pong.rx(socket, newMsg.buffer, newMsg.bufferLen);
		if (result > 0) // ping received.
		{
			Sleep(1000);
			int sendResult = pong.tx(socket, newMsg.buffer, result);
			if (sendResult > 0) // pong sent.
			{
				std::cout << "Server: pong\n";
			}
			else if (sendResult == -1) // Server pong.tx error.
			{
				std::cout << "Server: pong.tx failed.\n";
				closesocket(socket);
				WSACleanup();
				sessionStatus = false;
				return;
			}
		}
		else if (result == -1) // peer gracefully closed connection.
		{
			std::cout << "Server: peer gracefully closed connection.\n";
			sessionStatus = false;
			break;
		}
		else if (result < -1) // pong.rx error.
		{
			std::cout << "Server: pong.rx failed.\n";
			closesocket(socket);
			WSACleanup();
			sessionStatus = false;
			return;
		}
	} while (sessionStatus);

	result = pong.closeConnection(socket, false);
	if (result != 0)
	{
		std::cout << "Server: pong.closeConnection failed.\n";
		return;
	}
	std::cout << "Server: pong session ended.\n";
}


void startSession(SOCKET socket, int sessionType)
{
	Session newSession(socket);
	newSession.run(socket, sessionType);
}