#ifndef NECRO_NETWORK_MANAGER
#define NECRO_NETWORK_MANAGER

#include "AuthSession.h"
#include "SocketUtility.h"

struct NetData
{
	std::string username;
	std::string ipAddress;
};

class NECRONetManager
{
private:
	NetData data;

	std::unique_ptr<AuthSession> authSocket;
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


	// NetData Setters
	void SetNetDataUsername(const std::string& u)
	{
		data.username = u;
	}

	void SetNetDataIpAddress(const std::string& i)
	{
		data.ipAddress = i;
	}

	NetData& GetData()
	{
		return data;
	}
};


#endif
