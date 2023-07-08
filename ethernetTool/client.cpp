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
	tcp asdf;
	SOCKET socket = INVALID_SOCKET;
	int result = asdf.openClientSocket(socket, clientCmds.serverIP, clientCmds.serverPortNum);
	if (result != 0)
	{
		return;
	}

	const char *sendbuf = clientCmds.msg.c_str();
	int len = (int)strlen(sendbuf);
	do {
		result = asdf.tx(socket, sendbuf, len);
		if (result > 0)
		{
			std::cout << "Client sent: " << sendbuf << '\n';
		}
		else if (result == -1)
		{
			std::cout << "Client message.tx failed.\n";
		}
	} while (result < len);
}

void startClient(clientCmds clientCmds)
{
	Client newClient;
	newClient.run(clientCmds);
}