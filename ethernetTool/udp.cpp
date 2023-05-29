#include "udp.h"

int udpSocket::socketReadStatus(SOCKET udpSocketServer)
{
	FD_SET readfds;
	FD_ZERO(&readfds);
	FD_SET(udpSocketServer, &readfds);

	timeval timeout; 
	timeout.tv_sec = s;
	timeout.tv_usec = us;

	return select(0, &readfds, 0, 0, &timeout);//Check socket RX status.
}

int udpSocket::socketWriteStatus(SOCKET udpSocketServer)
{
	FD_SET writefds;
	FD_ZERO(&writefds);
	FD_SET(udpSocketServer, &writefds);

	timeval timeout;
	timeout.tv_sec = s;
	timeout.tv_usec = us;

	return select(0, 0, &writefds, 0, &timeout);//Check socket TX status.
}

void udpSocket::openSocket(int localPortNum)
{
	//Open Winsock dll.
	WSADATA WinSockData;
	WSAStartup(MAKEWORD(2, 2), &WinSockData);
	std::cout << "WSAStarup success" << std::endl;

	//Create UDP socket.
	udpSocketServer = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	std::cout << "Server socket opened." << std::endl;

	//Local socket parameters.
	struct sockaddr_in localSock;//new socket struct
	localSock.sin_family = AF_INET;
	localSock.sin_addr.s_addr = INADDR_ANY;
	localSock.sin_port = htons(localPortNum);

	//Assign address/port to new socket.
	result = bind(udpSocketServer, (SOCKADDR *)&localSock, sizeof(localSock)); 
	if (result == SOCKET_ERROR)
	{
		std::cout << "Bind failed with error: " << WSAGetLastError() << std::endl;
	}
} 

int udpSocket::rx(datagram& rxDatagram)
{
	struct sockaddr_in rxAddr;//Dummy socket struct to hold RX packet fields.
	int rxAddrSize = sizeof(rxAddr);

	char rxbuf[1472] = { 0 }; //Payload buffer.
	int rxbuflen = sizeof(rxbuf);//Payload buffer size in bytes.

	int rxReady = socketReadStatus(udpSocketServer); //Check if socket RX ready.
	if (rxReady > 0)
	{
		int rxbytes = recvfrom(udpSocketServer, rxbuf, rxbuflen, 0, (SOCKADDR *)&rxAddr, &rxAddrSize);
		if (rxbytes > 0)
		{
			rxDatagram.sin_addr = rxAddr.sin_addr;
			rxDatagram.sin_port = rxAddr.sin_port;
			rxDatagram.payloadLen = rxbytes;
			strcpy_s(rxDatagram.payload, rxbuf);
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else // Necessary to prevent looping on listen, echo.
	{
		return 0;
	}		
}

int udpSocket::tx(const char* destIP, int destPortNum, const char *buf, int len)
{
	//Destination socket parameters.
	struct sockaddr_in destSock;
	destSock.sin_family = AF_INET;
	destSock.sin_addr.s_addr = inet_addr(destIP);
	destSock.sin_port = destPortNum;

	int txReady = socketWriteStatus(udpSocketServer); //Check if socket TX ready.
	if (txReady > 0)
	{
		result = sendto(udpSocketServer, buf, len, 0, (SOCKADDR *)& destSock, sizeof(destSock));
		if (result == SOCKET_ERROR)
		{
			std::cout << "sendto failed with error: " << WSAGetLastError() << '\n';
			return 1;
		}
		else
		{
			return 0;
		}
	}
}

void udpSocket::closeSocket()
{
	//Close socket.
	closesocket(udpSocketServer);
	std::cout << "Server socket closed.\n";

	//Terminate Winsock dll.
	WSACleanup();
	std::cout << "Clean up success.\n";
}