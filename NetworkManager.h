#ifndef NECRO_NETWORK_MANAGER
#define NECRO_NETWORK_MANAGER

#include "TCPSocket.h"
#include "SocketUtility.h"

class NECRONetManager
{
private:
	std::unique_ptr<TCPSocket> authSocket;

public:

	int Init()
	{
		SocketUtility::Initialize();

		authSocket = std::make_unique<TCPSocket>(SocketAddressesFamily::INET);

		uint16_t outPort = 61531;
		SocketAddress localAddr(AF_INET, INADDR_ANY, outPort);
		int flag = 1;

		authSocket->SetSocketOption(IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(int));

		return 0;
	}
};


#endif
