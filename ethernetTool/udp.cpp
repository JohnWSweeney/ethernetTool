#include "udp.h"

int udp::socketReadStatus(SOCKET &udpSocket)
{
	FD_SET readfds;
	FD_ZERO(&readfds);
	FD_SET(udpSocket, &readfds);

	timeval timeout;
	timeout.tv_sec = s;
	timeout.tv_usec = us;

	result = select(0, &readfds, 0, 0, &timeout); // Check socket RX status.
	if (result == SOCKET_ERROR)
	{
		std::cout << "select failed with error: " << WSAGetLastError() << '\n';
		return 0;
	}
	else
	{
		return result;
	}
}

int udp::socketWriteStatus(SOCKET &udpSocket)
{
	FD_SET writefds;
	FD_ZERO(&writefds);
	FD_SET(udpSocket, &writefds);

	timeval timeout;
	timeout.tv_sec = s;
	timeout.tv_usec = us;

	result = select(0, 0, &writefds, 0, &timeout); // Check socket TX status.
	if (result == SOCKET_ERROR)
	{
		std::cout << "select failed with error: " << WSAGetLastError() << '\n';
		return 0;
	}
	else
	{
		return result;
	}
}

int udp::openSocket(int localPortNum)
{
	// Open Winsock dll.
	WORD wVersionRequested = MAKEWORD(2, 2);
	WSADATA WinSockData;
	result = WSAStartup(wVersionRequested, &WinSockData);
	if (result != 0)
	{
		std::cout << "WSAStartup failed with error: " << result << '\n';
		return 1;
	}

	// Update socket.
	udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (udpSocket == INVALID_SOCKET)
	{
		std::cout << "socket failed with error: " << WSAGetLastError() << '\n';
		closeSocket();
		return 1;
	}

	// Local socket parameters.
	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = INADDR_ANY;
	local.sin_port = htons(localPortNum);

	// Assign address/port to socket.
	result = bind(udpSocket, (SOCKADDR *)&local, sizeof(local));
	if (result == SOCKET_ERROR)
	{
		std::cout << "bind failed with error: " << WSAGetLastError() << std::endl;
		closeSocket();
		return 1;
	}
	else
	{
		return 0;
	}
}

int udp::rx(datagram &rxDatagram)
{
	struct sockaddr_in src; // Dummy socket struct to hold RX packet fields.
	int srcSize = sizeof(src);

	char rxbuf[65527] = { 0 }; // Payload buffer.
	int rxbuflen = sizeof(rxbuf);

	int rxReady = socketReadStatus(udpSocket); // Check socket RX status.
	if (rxReady > 0)
	{
		int rxBytes = recvfrom(udpSocket, rxbuf, rxbuflen, 0, (SOCKADDR *)&src, &srcSize);
		if (rxBytes == SOCKET_ERROR)
		{
			std::cout << "recvfrom failed with error: " << WSAGetLastError() << '\n';
		}
		if (rxBytes > 0)
		{
			rxDatagram.sin_addr = src.sin_addr;
			rxDatagram.sin_port = src.sin_port;
			rxDatagram.payloadLen = rxBytes;
			strcpy_s(rxDatagram.payload, rxbuf);
			return rxBytes;
		}
		else
		{
			return 0;
		}
	}
	else // Necessary to prevent looping.
	{
		return 0;
	}
}

int udp::tx(const char* destIP, int destPortNum, const char *buf, int len)
{
	// Destination socket parameters.
	struct sockaddr_in dest;
	dest.sin_family = AF_INET;
	dest.sin_addr.s_addr = inet_addr(destIP);
	dest.sin_port = destPortNum;

	int txReady = socketWriteStatus(udpSocket);// Check socket TX status.
	if (txReady > 0)
	{
		result = sendto(udpSocket, buf, len, 0, (SOCKADDR *)&dest, sizeof(dest));
		if (result == SOCKET_ERROR)
		{
			std::cout << "sendto failed with error: " << WSAGetLastError() << std::endl;
			return 1;
		}
		else
		{
			return 0;
		}
	}
}

int udp::closeSocket()
{
	// Close socket and Winsock dll.
	result = closesocket(udpSocket);
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