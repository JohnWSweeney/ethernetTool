#include "udpFunctions.h"
#include "atomicBool.h"
#include "udp.h"

void listener(int localPortNum)
{
	udp udpListen;
	int result = udpListen.openSocket(localPortNum);
	if (result != 0)
	{
		std::cout << "openSocket failed.\n";
		listenStatus = false;
	}
	else
	{
		std::cout << "Listening on port " << localPortNum << "...\n\n";
	}

	datagram rxDatagram;
	int packetsReceived = 0;
	while (listenStatus)
	{
		int result = udpListen.rx(rxDatagram);
		if (result > 0)
		{
			std::cout << "Received on port: " << localPortNum << '\n';
			std::cout << "Source address: " << inet_ntoa(rxDatagram.sin_addr) << '\n';
			std::cout << "Source port: " << rxDatagram.sin_port << '\n';
			std::cout << "Payload size: " << rxDatagram.payloadLen << " bytes\n";
			std::cout << "Payload: " << rxDatagram.payload << '\n';
			++packetsReceived;
			std::cout << "Packets received since runtime: " << packetsReceived << "\n\n";
		}
	}
	udpListen.closeSocket();
	std::cout << "Listen terminated on port " << localPortNum << ".\n\n";
}

void message(std::string destIPstr, int destPortNum, std::string msg)
{
	udp udpMessage;
	int localPortNum = 0; // Pick a port for me.
	int result = udpMessage.openSocket(localPortNum);
	if (result != 0)
	{
		std::cout << "openSocket failed.\n";
		messageStatus = false;
	}

	const char *destIP = destIPstr.c_str();
	const char *txbufptr = msg.data();
	int len = msg.size();
	while(messageStatus)
	{
		int result = udpMessage.tx(destIP, destPortNum, txbufptr, len);
		if (result == 0)
		{
			std::cout << "Message: \"" << msg << "\" sent to: " << destIP << " port " << destPortNum << "\n\n";
			
		}
		messageStatus = false;
	}
	udpMessage.closeSocket();
}

void echo(int localPortNum)
{
	udp udpEcho;
	int result = udpEcho.openSocket(localPortNum);
	if (result != 0)
	{
		std::cout << "openSocket failed.\n";
		echoStatus = false;
	}
	else
	{
		std::cout << "Echoing on port " << localPortNum << "...\n\n";
	}

	datagram rxDatagram;
	int packetsEchoed = 0;
	while (echoStatus)
	{
		result = udpEcho.rx(rxDatagram);
		if (result > 0)
		{
			std::cout << "Echoed on port: " << localPortNum << '\n';
			std::cout << "Source address: " << inet_ntoa(rxDatagram.sin_addr) << '\n';
			std::cout << "Source port: " << rxDatagram.sin_port << '\n';
			std::cout << "Payload size: " << rxDatagram.payloadLen << '\n';
			std::cout << "Payload: " << rxDatagram.payload << '\n';
			udpEcho.tx(inet_ntoa(rxDatagram.sin_addr), rxDatagram.sin_port, rxDatagram.payload, rxDatagram.payloadLen);
			++packetsEchoed;
			std::cout << "Packets echoed since runtime: " << packetsEchoed << "\n\n";
		}
	}
	udpEcho.closeSocket();
	std::cout << "Echo terminated on port " << localPortNum << ".\n\n";
}

void counter(std::string destIPstr, int destPortNum, int start, int end, unsigned int delay, bool loop)
{
	udp udpCounter;
	int localPortNum = 0; // Pick a port for me.
	int result = udpCounter.openSocket(localPortNum);
	if (result != 0)
	{
		std::cout << "openSocket failed.\n";
		counterStatus = false;
	}
	
	const char *destIP = destIPstr.c_str();
	char buf[4] = {0};
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
				udpCounter.tx(destIP, destPortNum, buf, len);
				Sleep(delay);
				if (loop == true)
				{
					if (i == end)
					{
						i = start;
					}
				}
			}
		}
		else{
			for (int i = start; i > end; i--)
			{
				if (counterStatus == false) { break; }
				*iPtr = i;
				udpCounter.tx(destIP, destPortNum, buf, len);
				Sleep(delay);
				if (loop == true)
				{
					if (i == end + 1)
					{
						i = start;
					}
				}
			}
		}
		counterStatus = false;
	}
	udpCounter.closeSocket();
}