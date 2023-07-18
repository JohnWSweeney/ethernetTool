#include "udpFunctions.h"
#include "atomicBool.h"
#include "udp.h"

void listener(int localPortNum)
{
	udp udpListen;
	SOCKET socket = INVALID_SOCKET;
	int result = udpListen.openSocket(localPortNum, socket);
	if (result != 0)
	{
		std::cout << "udpListen.openSocket failed.\n";
		udpListen.closeSocket(socket);
		listenStatus = false;
		return;
	}
	else
	{
		std::cout << "Listening on port " << localPortNum << "...\n\n";
	}
	
	datagram datagram;
	int packetsReceived = 0;
	do {
		result = udpListen.rx(socket, datagram);
		if (result > 0) // message received.
		{
			std::cout << "Received on port: " << localPortNum << '\n';
			std::cout << "Source address: " << inet_ntoa(datagram.sin_addr) << '\n';
			std::cout << "Source port: " << datagram.sin_port << '\n';
			std::cout << "Payload size: " << result << " bytes\n";
			std::cout << "Payload: " << datagram.buffer << '\n';
			++packetsReceived;
			std::cout << "Packets received since runtime: " << packetsReceived << "\n\n";
		}
		else if (result < -1) // error.
		{
			std::cout << "listener.rx failed.\n";
			listenStatus = false;
		}
	} while (listenStatus);
	udpListen.closeSocket(socket);
	std::cout << "Listen on port " << localPortNum << " stopped.\n\n";
}

void echo(int localPortNum)
{
	udp udpEcho;
	SOCKET socket = INVALID_SOCKET;
	int result = udpEcho.openSocket(localPortNum, socket);
	if (result != 0)
	{
		std::cout << "udpEcho.openSocket failed.\n";
		udpEcho.closeSocket(socket);
		echoStatus = false;
		return;
	}
	else
	{
		std::cout << "Echoing on port " << localPortNum << "...\n\n";
	}

	datagram datagram;
	int packetsEchoed = 0;
	do {
		result = udpEcho.rx(socket, datagram);
		if (result > 0) // message received.
		{
			std::cout << "Received on port: " << localPortNum << '\n';
			std::cout << "Source address: " << inet_ntoa(datagram.sin_addr) << '\n';
			std::cout << "Source port: " << datagram.sin_port << '\n';
			std::cout << "Payload size: " << result << " bytes\n";
			std::cout << "Payload: " << datagram.buffer << '\n';
			int sendResult = udpEcho.tx(socket, inet_ntoa(datagram.sin_addr), datagram.sin_port, datagram.buffer, result);
			if (sendResult > 0) // message echoed to peer.
			{
				++packetsEchoed;
				std::cout << "Packets echoed since runtime: " << packetsEchoed << "\n\n";
			}
			else if (sendResult == -1) // echo.tx error.
			{
				std::cout << "echo.tx failed.\n";
				echoStatus = false;
			}
		}
		else if (result < -1) // echo.rx error.
		{
			std::cout << "echo.rx failed.\n";
			echoStatus = false;
		}
	} while (echoStatus);
	udpEcho.closeSocket(socket);
	std::cout << "Echo on port " << localPortNum << " stopped.\n\n";
}

void message(std::string destIPstr, int destPortNum, std::string msg)
{
	udp udpMessage;
	SOCKET socket = INVALID_SOCKET;
	int localPortNum = 0; // Pick a port for me.
	int result = udpMessage.openSocket(localPortNum, socket);
	if (result != 0)
	{
		std::cout << "udpMessage.openSocket failed.\n";
		udpMessage.closeSocket(socket);
		messageStatus = false;
		return;
	}

	const char *destIP = destIPstr.c_str();
	const char *txbufptr = msg.data();
	int len = msg.size();
	while (messageStatus)
	{
		int result = udpMessage.tx(socket, destIP, destPortNum, txbufptr, len);
		if (result > 0) // message sent.
		{
			std::cout << "Message: \"" << msg << "\" sent to: " << destIP << " port " << destPortNum << "\n\n";

		}
		else if (result == -1) // message.tx error.
		{
			std::cout << "message.tx failed.\n";
		}
		messageStatus = false;
	}
	udpMessage.closeSocket(socket);
}

void counter(std::string destIPstr, int destPortNum, int start, int end, unsigned int delay, bool loop)
{
	udp udpCounter;
	SOCKET socket = INVALID_SOCKET;
	int localPortNum = 0; // Pick a port for me.
	int result = udpCounter.openSocket(localPortNum, socket);
	if (result != 0)
	{
		std::cout << "udpCounter.openSocket failed.\n";
		udpCounter.closeSocket(socket);
		counterStatus = false;
		return;
	}
	
	const char *destIP = destIPstr.c_str();
	char buf[4] = { 0 };
	int* iPtr = (int*)buf;
	int len = sizeof(buf);

	while (counterStatus)
	{
		if (end - start > 0)
		{
			for (int i = start; i <= end; i++)
			{
				if (counterStatus == false) { break; }
				*iPtr = i;
				int result = udpCounter.tx(socket, destIP, destPortNum, buf, len);
				if (result > 0)
				{
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
					std::cout << "counter.tx failed.\n";
					counterStatus = false;
				}
			}
		}
		else {
			for (int i = start; i >= end; i--)
			{
				if (counterStatus == false) { break; }
				*iPtr = i;
				int result = udpCounter.tx(socket, destIP, destPortNum, buf, len);
				if (result > 0)
				{
					Sleep(delay);
					if (loop == true)
					{
						if (i == end + 1)
						{
							i = start;
						}
					}
				}
				else if (result == -1)
				{
					std::cout << "counter.tx failed.\n";
					counterStatus = false;
				}
			}
		}
		counterStatus = false;
	}
	udpCounter.closeSocket(socket);
	std::cout << "Counter stopped.\n\n";
}