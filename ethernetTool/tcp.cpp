#include "tcp.h"

int tcp::socketReadStatus(SOCKET &socket)
{
	FD_SET readfds;
	FD_ZERO(&readfds);
	FD_SET(socket, &readfds);

	FD_SET exceptfds;
	FD_ZERO(&exceptfds);
	FD_SET(socket, &exceptfds);

	timeval timeout;
	timeout.tv_sec = s;
	timeout.tv_usec = us;

	return select(0, &readfds, 0, &exceptfds, &timeout); // check socket RX status.
}

int tcp::socketWriteStatus(SOCKET &socket)
{
	FD_SET writefds;
	FD_ZERO(&writefds);
	FD_SET(socket, &writefds);

	FD_SET exceptfds;
	FD_ZERO(&exceptfds);
	FD_SET(socket, &exceptfds);

	timeval timeout;
	timeout.tv_sec = s;
	timeout.tv_usec = us;

	return select(0, 0, &writefds, &exceptfds, &timeout); // check socket TX status.
}

int tcp::openServerSocket(int localPortNum, SOCKET &listenSocket)
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
	addrinfo *host = NULL;
	struct addrinfo hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	std::ostringstream oss;
	oss << localPortNum;

	result = getaddrinfo(NULL, oss.str().c_str(), &hints, &host);
	if (result != 0)
	{
		std::cout << "getaddrinfo failed with error: " << result << '\n';
		WSACleanup();
		return 1;
	}

	// Initialize socket to listen for client connections.
	listenSocket = socket(host->ai_family, host->ai_socktype, host->ai_protocol);
	if (listenSocket == INVALID_SOCKET)
	{
		std::cout << "Socket failed with error: " << WSAGetLastError() << '\n';
		freeaddrinfo(host);
		WSACleanup();
		return 1;
	}

	// Bind the listening socket to the host.
	result = bind(listenSocket, host->ai_addr, (int)host->ai_addrlen);
	if (result == SOCKET_ERROR)
	{
		std::cout << "Bind failed with error: " << WSAGetLastError() << '\n';
		closeSocket(listenSocket);
		return 1;
	}

	freeaddrinfo(host);

	// Set socket to listen for a client connection.
	result = listen(listenSocket, SOMAXCONN);
	if (result == SOCKET_ERROR)
	{
		std::cout << "listen failed with error: " << WSAGetLastError() << '\n';
		closeSocket(listenSocket);
		return 1;
	}
	else
	{
		return 0;
	}
}

int tcp::openClientSocket(SOCKET &clientSocket, std::string serverIP, int serverPort)
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

	// Initialize socket.
	clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clientSocket == INVALID_SOCKET)
	{
		std::cout << "Client socket failed with error: " << WSAGetLastError() << '\n';
		WSACleanup();
		return 1;
	}

	// Server info.
	sockaddr_in serverInfo;
	serverInfo.sin_family = AF_INET;
	serverInfo.sin_addr.s_addr = inet_addr(serverIP.c_str());
	serverInfo.sin_port = htons(serverPort);

	// Connect to server.
	result = connect(clientSocket, (SOCKADDR *)&serverInfo, sizeof(serverInfo));
	if (result == SOCKET_ERROR)
	{
		std::cout << "Connect to server failed with error: " << WSAGetLastError() << '\n';
		closeSocket(clientSocket);
		return 1;
	}
	else // connected to server.
	{
		return 0;
	}
}

int tcp::acceptConnection(SOCKET &listenSocket, SOCKET &acceptSocket)
{
	acceptSocket = accept(listenSocket, NULL, NULL);
	if (acceptSocket == INVALID_SOCKET)
	{
		std::cout << "accept failed with error: " << WSAGetLastError() << '\n';
		closeSocket(acceptSocket);
		return 1;
	}
	else // client connected.
	{
		// Get client ip address code here.
		return 0;
	}
}

int tcp::rx(SOCKET &socket, char *buffer, int bufferLen)
{
	int rxReady = socketReadStatus(socket);
	if (rxReady > 0) // socket ready.
	{
		int result = recv(socket, buffer, bufferLen, 0);
		if (result >= 0)
		{
			return result;
		}
		else // socket error.
		{
			closeSocket(socket);
			return SOCKET_ERROR;
		}
	}
	else if (rxReady == 0) // timeout.
	{
		return -1;
	}
	else // socket error.
	{
		closeSocket(socket);
		return SOCKET_ERROR;
	}
}

int tcp::tx(SOCKET &socket, const char *buffer, int bufferLen)
{
	int txReady = socketWriteStatus(socket);
	if (txReady > 0) // socket ready.
	{
		int result = send(socket, buffer, bufferLen, 0);
		return result;
	}
	else if (txReady == 0) // timeout.
	{
		return -1;
	}
	else // socket error.
	{
		closeSocket(socket);
		return SOCKET_ERROR;
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