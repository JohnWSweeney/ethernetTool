#include "ethernetTool.h"
#include "udp.h"
#include <thread>
#include <atomic>

extern std::atomic<bool> listenStatus;
extern std::atomic<bool> messageStatus;

void listener(int localPortNum)
{
	udpSocket udpListen;
	udpListen.openSocket(localPortNum);

	std::cout << "Listening on port " << localPortNum << "...\n" << std::endl;
	datagram rxDatagram;
	int rxPackets = 0;
	while (listenStatus)
	{
		int result = udpListen.rx(rxDatagram);
		if (result > 0)
		{
			++rxPackets;
			std::cout << "Source address: " << inet_ntoa(rxDatagram.sin_addr) << std::endl;
			std::cout << "Source port: " << rxDatagram.sin_port << std::endl;
			std::cout << "Payload size: " << rxDatagram.payloadLen << std::endl;
			std::cout << "Payload: " << rxDatagram.payload << std::endl;
			std::cout << "Packets received: " << rxPackets << "\n" << std::endl;
		}
	}
	udpListen.closeSocket();
}

void message(std::string destIPstr, int destPortNum, std::string msg)
{
	udpSocket udpMessage;
	int localPortNum = 0; // Pick a port for me.
	udpMessage.openSocket(localPortNum);

	//char buf[sizeof(msg)] = { 0 };
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