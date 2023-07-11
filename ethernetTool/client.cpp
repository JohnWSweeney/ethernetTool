#include "client.h"
#include "tcp.h"
#include "atomicBool.h"

void Client::run(clientCmds clientCmds)
{
	if (clientCmds.clientType == 0)
	{
		this->message(clientCmds);
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


void startClient(clientCmds clientCmds)
{
	Client newClient;
	newClient.run(clientCmds);
}