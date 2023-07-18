#include "udp.h"

int udp::socketReadStatus(SOCKET &socket)
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

	return select(0, &readfds, 0, &exceptfds, &timeout); // Check socket RX status.
}

int udp::socketWriteStatus(SOCKET &socket)
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

	return select(0, 0, &writefds, &exceptfds, &timeout); // Check socket TX status.
}

int udp::openSocket(int localPortNum, SOCKET &newSocket)
{
	// Open Winsock dll.
	WORD wVersionRequested = MAKEWORD(2, 2);
	WSADATA WinSockData;
	int result = WSAStartup(wVersionRequested, &WinSockData);
	if (result != 0)
	{
		std::cout << "WSAStartup failed with error: " << result << '\n';
		return 1;
	}

	// Specify socket type.
	newSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (newSocket == INVALID_SOCKET)
	{
		std::cout << "socket failed with error: " << WSAGetLastError() << '\n';
		return 1;
	}

	// Local socket parameters.
	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = INADDR_ANY;
	local.sin_port = htons(localPortNum);

	// Assign address/port to socket.
	result = bind(newSocket, (SOCKADDR *)&local, sizeof(local));
	if (result == SOCKET_ERROR)
	{
		std::cout << "bind failed with error: " << WSAGetLastError() << std::endl;
		return 1;
	}
	else
	{
		return 0;
	}
}

int udp::rx(SOCKET &socket, datagram &datagram)
{
	struct sockaddr_in src; // Dummy socket struct to hold RX packet fields.
	int srcSize = sizeof(src);

	int rxReady = socketReadStatus(socket); // Check socket RX status.
	if (rxReady > 0)
	{
		int result = recvfrom(socket, datagram.buffer, datagram.bufferLen, 0, (SOCKADDR *)&src, &srcSize);
		if (result > 0)
		{
			datagram.sin_addr = src.sin_addr;
			datagram.sin_port = src.sin_port;
			return result;
		}
		else if (result == 0) // connection closed gracefully.
		{
			return -1;
		}
		else if (result == SOCKET_ERROR)
		{
			std::cout << "rx.recvfrom failed with error: " << WSAGetLastError() << '\n';
			return -2;
		}
	}
	else if (rxReady == 0) // timeout.
	{
		return 0;
	}
	else if(rxReady == SOCKET_ERROR)
	{
		std::cout << "rx.select failed with error : " << WSAGetLastError() << '\n';
		return -2;
	}
}

int udp::tx(SOCKET &socket, const char* destIP, int destPort, const char *buffer, int len)
{
	// Destination socket parameters.
	struct sockaddr_in dest;
	dest.sin_family = AF_INET;
	dest.sin_addr.s_addr = inet_addr(destIP);
	dest.sin_port = destPort;

	int txReady = socketWriteStatus(socket); // Check socket TX status.
	if (txReady > 0) // socket ready.
	{
		int result = sendto(socket, buffer, len, 0, (SOCKADDR *)&dest, sizeof(dest));
		if (result > 0)
		{
			return result;
		}
		else if (result == SOCKET_ERROR)
		{
			std::cout << "tx.sendto failed with error: " << WSAGetLastError() << '\n';
			return -1;
		}
	}
	else if (txReady == 0) // timeout.
	{
		return 0;
	}
	else if (txReady == SOCKET_ERROR)
	{
		std::cout << "tx.select failed with error: " << WSAGetLastError() << '\n';
		return -1;
	}
}

int udp::closeSocket(SOCKET socket)
{
	// Close socket and Winsock dll.
	int result = closesocket(socket);
	if (result != 0)
	{
		std::cout << "closesocket failed with error: " << WSAGetLastError() << '\n';
		WSACleanup();
		return 1;
	}

	result = WSACleanup();
	if (result != 0)
	{
		std::cout << "WSACleanup failed with error: " << WSAGetLastError() << '\n';
		return 1;
	}
	else
	{
		return 0;
	}
}