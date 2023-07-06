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
	tcp clientMessage;
	SOCKET socket = INVALID_SOCKET;
	int result = clientMessage.openClientSocket(socket, clientCmds.serverIP, clientCmds.serverPortNum);
	if (result != 0)
	{
		return;
	}

	const char *sendbuf = clientCmds.msg.c_str();
	do {
		result = clientMessage.tx(socket, sendbuf, (int)strlen(sendbuf));
		if (result == SOCKET_ERROR)
		{
			std::cout << "Client send failed with error: " << WSAGetLastError() << '\n';
			break;
		}
		else if (result >= 0)
		{
			std::cout << "Client sent: " << sendbuf << '\n';
		}
	} while (result < 0);
}

void startClient(clientCmds clientCmds)
{
	Client newClient;
	newClient.run(clientCmds);
}