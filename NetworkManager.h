#ifndef NECRO_NETWORK_MANAGER
#define NECRO_NETWORK_MANAGER

#include "TCPSocket.h"
#include "SocketUtility.h"

class NECRONetManager
{
private:
	std::unique_ptr<TCPSocket> authSocket;
	bool authSocketConnected = false;
	bool isConnecting = false;

	std::vector<pollfd> poll_fds;

public:

	int Init();
	void CreateAuthSocket();

	int ConnectToAuthServer();
	
	int CheckIfAuthConnected();
	int CheckForIncomingData();
	int NetworkUpdate();

	void OnDisconnect();

	void OnConnectedToAuthServer();
};


#endif
