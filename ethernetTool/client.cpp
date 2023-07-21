#include "client.h"
#include "tcp.h"
#include "atomicBool.h"

void Client::run(clientCmds clientCmds)
{
	if (clientCmds.clientType == 0)
	{
		this->message(clientCmds);
	}
	else if (clientCmds.clientType == 1)
	{
		this->ping(clientCmds);
	}
	else if (clientCmds.clientType == 2)
	{
		this->counter(clientCmds);
	}
	else if (clientCmds.clientType == 3)
	{
		this->repeat(clientCmds);
	}
	else
	{
		return;
	}
}

void Client::message(clientCmds clientCmds)
{
	tcp message;
	SOCKET socket = INVALID_SOCKET;
	int result = message.openClientSocket(socket, clientCmds.serverIP, clientCmds.serverPortNum);
	if (result != 0)
	{
		std::cout << "Client: message.openClientSocket failed.\n";
		closesocket(socket);
		WSACleanup();
		return;
	}

	// send message to server.
	const char *sendbuf = clientCmds.msg.c_str();
	int len = (int)strlen(sendbuf);
	result = message.tx(socket, sendbuf, len);
	if (result > 0)
	{
		std::cout << "Client sent: " << sendbuf << '\n';
	}
	else if (result == -1)
	{
		std::cout << "Client: message.tx failed.\n";
		closesocket(socket);
		WSACleanup();
		return;
	}

	// gracefully close connection.
	result = message.closeConnection(socket, true);
	if (result != 0)
	{
		std::cout << "Client: message.closeConnection failed.\n";
		return;
	}
	std::cout << "Client: message ended.\n";
}

void Client::ping(clientCmds clientCmds)
{
	tcp ping;
	SOCKET socket = INVALID_SOCKET;
	int result = ping.openClientSocket(socket, clientCmds.serverIP, clientCmds.serverPortNum);
	if (result != 0)
	{
		std::cout << "Client: ping.openClientSocket failed.\n";
		closesocket(socket);
		WSACleanup();
		return;
	}

	// send initial message volley to server.
	const char *sendbuf = clientCmds.msg.c_str();
	int len = (int)strlen(sendbuf);
	result = ping.tx(socket, sendbuf, len);
	if (result > 0)
	{
		std::cout << "Client: ping\n";
	}
	else if (result == -1)
	{
		std::cout << "Client: ping.tx initial volley failed.\n";
		closesocket(socket);
		WSACleanup();
		clientStatus = false;
		return;
	}

	// ping-pong message with server until client or server closes connection or error.
	do {
		msg newMsg;
		result = ping.rx(socket, newMsg.buffer, newMsg.bufferLen);
		if (result > 0) // pong received.
		{
			Sleep(1000);
			int sendResult = ping.tx(socket, newMsg.buffer, result);
			if (sendResult > 0) // ping sent.
			{
				std::cout << "Client: ping\n";
			}
			else if (sendResult == -1) // ping.tx error.
			{
				std::cout << "Client: ping.tx failed.\n";
				closesocket(socket);
				WSACleanup();
				clientStatus = false;
				return;
			}
		}
		else if (result == -1) // peer gracefully closed connection.
		{
			std::cout << "Client: peer gracefully closed connection.\n";
			clientStatus = false;
			break;
		}
		else if (result < -1) // error.
		{
			std::cout << "Client: ping.rx failed.\n";
			closesocket(socket);
			WSACleanup();
			clientStatus = false;
			return;
		}
	} while (clientStatus);

	// gracefully close connection.
	result = ping.closeConnection(socket, true);
	if (result != 0)
	{
		std::cout << "Client: ping.closeConnection failed.\n";
		return;
	}
	std::cout << "Client: ping session ended.\n";
}

void Client::counter(clientCmds clientCmds)
{
	tcp counter;
	SOCKET socket = INVALID_SOCKET;
	int result;
	result = counter.openClientSocket(socket, clientCmds.serverIP, clientCmds.serverPortNum);
	if (result != 0)
	{
		std::cout << "Client: counter.openClientSocket failed.\n";
		closesocket(socket);
		WSACleanup();
		return;
	}

	int start = clientCmds.startInt;
	int end = clientCmds.endInt;
	int delay = clientCmds.delay;
	bool loop = clientCmds.loop;
	std::string bufStr;
	const char *buf;
	int len;
	do {
		// check if peer has gracefully closed connection.
		msg sdMsg;
		result = counter.rx(socket, sdMsg.buffer, sdMsg.bufferLen);
		if (result == -1) // peer closed connection gracefully.
		{
			std::cout << "Client.repeat: peer closed connection gracefully.\n";
			clientStatus = false;
			break;
		}

		if (end - start > 0)
		{
			for (int i = start; i <= end; i++)
			{
				if (clientStatus == false) { break; }
				bufStr = std::to_string(i);
				buf = bufStr.c_str();
				len = sizeof(buf);
				result = counter.tx(socket, buf, len);
				if (result > 0) // counter sent.
				{
					std::cout << "Client sent: " << buf << '\n';
					Sleep(delay);
					if (loop == true)
					{
						if (i == end)
						{
							i = start;
						}
					}
				}
				else if (result == -1) // counter.tx error.
				{
					std::cout << "Client: counter.tx failed.\n";
					closesocket(socket);
					WSACleanup();
					return;
				}
			}
		}
		else
		{
			for (int i = start; i >= end; i--)
			{
				if (clientStatus == false) { break; }
				bufStr = std::to_string(i);
				buf = bufStr.c_str();
				len = sizeof(buf);
				result = counter.tx(socket, buf, len);
				if (result > 0)
				{
					std::cout << "Client sent: " << buf << '\n';
					Sleep(delay);
					if (loop == true)
					{
						if (i == end + 1)
						{
							i = start;
						}
					}
				}
				else if (result == -1) // counter.tx error.
				{
					std::cout << "Client: counter.tx failed.\n";
					closesocket(socket);
					WSACleanup();
					return;
				}
			}
		}
		clientStatus = false;
	} while (clientStatus);

	// gracefully close connection.
	result = counter.closeConnection(socket, true);
	if (result != 0)
	{
		std::cout << "Client: counter.closeConnection failed.\n";
		return;
	}
	std::cout << "Client: counter ended.\n";
}

void Client::repeat(clientCmds clientCmds)
{
	tcp repeat;
	SOCKET socket = INVALID_SOCKET;
	int result = repeat.openClientSocket(socket, clientCmds.serverIP, clientCmds.serverPortNum);
	if (result != 0)
	{
		std::cout << "Client: repeat.openClientSocket failed.\n";
		closesocket(socket);
		WSACleanup();
		return;
	}

	// send message every second until user ends function, peer closes connection, or error.
	const char *sendbuf = clientCmds.msg.c_str();
	int len = (int)strlen(sendbuf);
	do {
		// check if peer has gracefully closed connection.
		msg sdMsg;
		result = repeat.rx(socket, sdMsg.buffer, sdMsg.bufferLen);
		if (result == -1) // peer closed connection gracefully.
		{
			std::cout << "Client.repeat: peer closed connection gracefully.\n";
			clientStatus = false;
			break;
		}

		// send message.
		result = repeat.tx(socket, sendbuf, len);
		if (result > 0)
		{
			std::cout << "Client sent: " << sendbuf << '\n';
		}
		else if (result == -1)
		{
			std::cout << "Client repeat.tx failed.\n";
			closesocket(socket);
			WSACleanup();
			return;
		}
		Sleep(clientCmds.delay);
	} while (clientStatus);

	// gracefully close connection.
	result = repeat.closeConnection(socket, true);
	if (result != 0)
	{
		std::cout << "Client repeat.asdf failed.\n";
	}
	std::cout << "Client repeat stopped.\n";
}

void startClient(clientCmds clientCmds)
{
	Client newClient;
	newClient.run(clientCmds);
}