#include "client.h"
#include "tcp.h"

void Client::run(clientStruct clientStruct)
{
	if (clientStruct.clientType = 1)
	{
		this->message(clientStruct);
	}
	else
	{
		return;
	}
}

void Client::message(clientStruct clientStruct)
{
	tcp clientMessage;
	int result = clientMessage.openClientSocket(clientStruct.socket, clientStruct.serverIP, clientStruct.serverPort);
	if (result != 0)
	{
		return;
	}

	const char *sendbuf = clientStruct.msg.c_str();
	result = clientMessage.tx(clientStruct.socket, sendbuf, (int)strlen(sendbuf));
	if (result == SOCKET_ERROR)
	{
		std::cout << "send failed with error: " << WSAGetLastError() << '\n';
		clientMessage.closeSocket(clientStruct.socket);
		return;
	}
	else
	{
		std::cout << "Message sent: " << sendbuf << '\n';
	}
}

void startClient(clientStruct clientStruct)
{
	Client newClient;
	newClient.run(clientStruct);
}