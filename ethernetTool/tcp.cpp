#include "tcp.h"

int tcp::socketReadStatus(SOCKET &socket)
{
	FD_SET readfds;
	FD_ZERO(&readfds);
	FD_SET(socket, &readfds);

	timeval timeout;
	timeout.tv_sec = s;
	timeout.tv_usec = us;

	return select(0, &readfds, 0, 0, &timeout); //Check socket RX status.
}

int tcp::openSocket(int localPortNum, sockStruct &sockStruct)
{
	// Start Winsock dll.
	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD(2, 2);
	int result = WSAStartup(wVersionRequested, &wsaData);
	if (result != 0)
	{
		std::cout << "WSAStartup failed with error: " << result << '\n';
		return 1;
	}

	// Get local host info.
	struct addrinfo hints;			 // what is this?
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	std::ostringstream oss;
	oss << localPortNum;

	result = getaddrinfo(NULL, oss.str().c_str(), &hints, &sockStruct.host);
	if (result != 0)
	{
		std::cout << "getaddrinfo failed with error: " << result << '\n';
		WSACleanup();
		return 1;
	}

	// Initialize socket to listen for client connections.
	sockStruct.listenSocket = socket(sockStruct.host->ai_family, sockStruct.host->ai_socktype, sockStruct.host->ai_protocol);
	if (sockStruct.listenSocket == INVALID_SOCKET)
	{
		std::cout << "Socket failed with error: " << WSAGetLastError() << '\n';
		freeaddrinfo(sockStruct.host);
		WSACleanup();
		return 1;
	}
	else
	{
		return 0;
	}
}

int tcp::bindListen(sockStruct &sockStruct)
{
	// Bind the listening socket to the host.
	int result = bind(sockStruct.listenSocket, sockStruct.host->ai_addr, (int)sockStruct.host->ai_addrlen);
	if (result == SOCKET_ERROR)
	{
		std::cout << "Bind failed with error: " << WSAGetLastError() << '\n';
		closeSocket(sockStruct.listenSocket);
		return 1;
	}

	freeaddrinfo(sockStruct.host);

	// Set socket to listen for a client connection.
	result = listen(sockStruct.listenSocket, SOMAXCONN);
	if (result == SOCKET_ERROR)
	{
		std::cout << "listen failed with error: " << WSAGetLastError() << '\n';
		closeSocket(sockStruct.listenSocket);
		return 1;
	}
	else
	{
		return 0;
	}
}

int tcp::acceptConnection(sockStruct &sockStruct)
{
	sockStruct.acceptSocket = accept(sockStruct.listenSocket, NULL, NULL);
	if (sockStruct.acceptSocket == INVALID_SOCKET)
	{
		std::cout << "accept failed with error: " << WSAGetLastError() << '\n';
		closeSocket(sockStruct.acceptSocket);
		return 1;
	}
	else
	{
		std::cout << "Client connected.\n";
		// Get client ip address code here.
		return 0;
	}
}

int tcp::makeConnection()
{

}

int tcp::rx(SOCKET &clientSocket, char *buffer, int bufferLen)
{
	int ready = socketReadStatus(clientSocket);
	if (ready > 0)
	{
		int result = recv(clientSocket, buffer, bufferLen, 0);
		if (result > 0)
		{
			return result;
		}
		else if (result == 0)
		{
			std::cout << "Closing connection.\n";
			return 0;
		}
		else
		{
			std::cout << "recv failed with error: " << WSAGetLastError() << '\n';
			closeSocket(clientSocket);
			return 1;
		}
	}
	else
	{
		return 1;
	}
}

int tcp::tx(SOCKET &clientSocket, char *buffer, int bufferLen)
{
	int result = send(clientSocket, buffer, bufferLen, 0);
	if (result == SOCKET_ERROR)
	{
		std::cout << "send failed with error: " << WSAGetLastError() << '\n';
		closesocket(clientSocket);
		WSACleanup();
		return 1;
	}
	else
	{
		return result;
		std::cout << "Message sent: " << buffer << '\n';
		std::cout << "Bytes sent: " << result << '\n';
	}
}

int tcp::shutdownSocket(SOCKET &socket)
{
	int result = shutdown(socket, SD_BOTH);
	if (result == SOCKET_ERROR)
	{
		std::cout << "shutdown failed with error: " << WSAGetLastError() << '\n';
		closeSocket(socket);
		return 1;
	}
	else
	{
		std::cout << "shutdown sucessful.\n";
		return 0;
	}
}

int tcp::closeSocket(SOCKET &socket)
{
	// Clean up.
	int result = closesocket(socket);
	if (result == SOCKET_ERROR)
	{
		std::cout << "socketclose failed with error: " << WSAGetLastError() << '\n';
		return 1;
	}

	result = WSACleanup();
	if (result == SOCKET_ERROR)
	{
		std::cout << "WSACleanup failed with error: " << WSAGetLastError() << '\n';
		return 1;
	}
	else
	{
		return 0;
	}
}