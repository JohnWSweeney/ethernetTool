#include "ethernetTool.h"
#include "udp.h"
#include <thread>
#include <atomic>

extern std::atomic<bool> listenStatus;
extern std::atomic<bool> messageStatus;
extern std::atomic<bool> echoStatus;

void listener(int localPortNum)
{
	udpSocket udpListen;
	udpListen.openSocket(localPortNum);

	std::cout << "Listening on port " << localPortNum << "...\n" << '\n';
	datagram rxDatagram;
	int rxPackets = 0;
	while (listenStatus)
	{
		int result = udpListen.rx(rxDatagram);
		if (result > 0)
		{
			++rxPackets;
			std::cout << "Source address: " << inet_ntoa(rxDatagram.sin_addr) << '\n';
			std::cout << "Source port: " << rxDatagram.sin_port << '\n';
			std::cout << "Payload size: " << rxDatagram.payloadLen << " bytes" << '\n';
			std::cout << "Payload: " << rxDatagram.payload << '\n';
			std::cout << "Packets received: " << rxPackets << "\n" << '\n';
		}
	}
	udpListen.closeSocket();
}

void message(std::string destIPstr, int destPortNum, std::string msg)
{
	udpSocket udpMessage;
	int localPortNum = 0; // Pick a port for me.
	udpMessage.openSocket(localPortNum);

	const char *destIP = destIPstr.c_str();
	const char *txbufptr = msg.data();
	int len = msg.size();
	while(messageStatus)
	{
		int result = udpMessage.tx(destIP, destPortNum, txbufptr, len);
		if (result == 0)
		{
			std::cout << "Message: \"" << msg << "\" sent to: " << destIP << ":" << destPortNum << "\n";
			
		}
		messageStatus = false;
	}
	udpMessage.closeSocket();
}

void echo(int localPortNum)
{
	udpSocket udpEcho;
	udpEcho.openSocket(localPortNum);

	std::cout << "Echoing on port " << localPortNum << "...\n" << '\n';
	datagram rxDatagram;
	while (echoStatus)
	{
		int result = udpEcho.rx(rxDatagram);
		if (result > 0)
		{
			std::cout << "Echo:" << '\n';
			std::cout << "Source address: " << inet_ntoa(rxDatagram.sin_addr) << '\n';
			std::cout << "Source port: " << rxDatagram.sin_port << '\n';
			std::cout << "Payload size: " << rxDatagram.payloadLen << " bytes" << '\n';
			std::cout << "Payload: " << rxDatagram.payload << "\n" << '\n';
			udpEcho.tx(inet_ntoa(rxDatagram.sin_addr), rxDatagram.sin_port, rxDatagram.payload, rxDatagram.payloadLen);
		}
	}
	udpEcho.closeSocket();
}