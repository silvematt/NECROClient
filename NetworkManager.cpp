#include "NetworkManager.h"
#include "NECROClient.h"
#include "AuthCodes.h"

int NECRONetManager::Init()
{
	SocketUtility::Initialize();

	CreateAuthSocket();

	return 0;
}

void NECRONetManager::CreateAuthSocket()
{
	isConnecting = false;

	authSocket = std::make_unique<AuthSession>(SocketAddressesFamily::INET);

	int flag = 1;
	authSocket->SetSocketOption(IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(int));
	authSocket->SetBlockingEnabled(false);
}

int NECRONetManager::ConnectToAuthServer()
{
	if (isConnecting)
		return 0;

	uint16_t outPort = 61531;
	struct in_addr addr;
	inet_pton(AF_INET, "127.0.0.1", &addr);

	SocketAddress authAddr(AF_INET, addr.s_addr, outPort);

	authSocket->SetRemoteAddressAndPort(authAddr, outPort);

	authSocketConnected = false;
	isConnecting = true;

	if (authSocket->Connect(authAddr) != 0)
	{
		// Manage error
		NECROConsole& c = engine.GetConsole();

		LOG_ERROR("Error while attempting to connect.");
		c.Log("Connection failed! Server is down or not accepting connections.");
		OnDisconnect();
		return -1;
	}

	LOG_DEBUG("Attempting to connect to Auth Server...");

	// Initialize the pollfds vector
	pollfd pfd;
	pfd.fd = authSocket->GetSocketFD();
	pfd.events = POLLOUT;
	pfd.revents = 0;
	poll_fds.push_back(pfd);

	return 0;
}

int NECRONetManager::NetworkUpdate()
{
	if (poll_fds.size() <= 0)
		return 0;

	static int timeout = 0;

	int res = WSAPoll(poll_fds.data(), poll_fds.size(), timeout);

	// Check for errors
	if (res < 0)
	{
		LOG_ERROR("Could not Poll()");
		OnDisconnect();
		return -1;
	}

	// Check for timeout
	if (res == 0)
	{
		return 0;
	}

	// Wait for connection
	if (!authSocketConnected)
	{
		int res = CheckIfAuthConnected();
		if (res == -1)
		{
			OnDisconnect();
			return res;
		}
	}
	else // Connection has been estabilished
	{
		int res = CheckForIncomingData();
		if (res == -1)
		{
			OnDisconnect();
			return res;
		}
	}

	return 0;
}

void NECRONetManager::OnDisconnect()
{
	// Delete and recreate socket for next try
	authSocket->Close();
	authSocket.reset();
	poll_fds.clear();

	CreateAuthSocket();

	authSocketConnected = false;
	isConnecting = false;
}

int NECRONetManager::CheckIfAuthConnected()
{
	NECROConsole& c = engine.GetConsole();

	// Check for errors
	if (poll_fds[0].revents & (POLLERR | POLLHUP | POLLNVAL))
	{
		int error = 0;
		int len = sizeof(error);
		if (getsockopt(poll_fds[0].fd, SOL_SOCKET, SO_ERROR, (char*)&error, &len) == 0)
		{
			// Get error type and print a message
			if (error == WSAECONNREFUSED)
			{
				LOG_ERROR("Connection refused: server is down or not accepting connections.");
				c.Log("Connection refused: server is down or not accepting connections.");
			}
			else
			{
				LOG_ERROR("AuthSocket encountered an error!");
				c.Log("Connection lost! Server may have crashed or kicked you.");
			}
		}
		else
		{
			LOG_ERROR("getsockopt failed! [%d]", SocketUtility::GetLastError());
			c.Log("Connection lost! Server may have crashed or kicked you.");
		}

		return -1;
	}

	// Check if the socket is ready for writing, means we either connected successfuly or failed to connect
	if (poll_fds[0].revents & POLLOUT)
	{
		// If we get here, it indicates that a non-blocking connect has completed
		int error = 0;
		int len = sizeof(error);
		if (getsockopt(poll_fds[0].fd, SOL_SOCKET, SO_ERROR, (char*)&error, &len) < 0)
		{
			LOG_ERROR("getsockopt failed! [%d]", SocketUtility::GetLastError());
			return -1;
		}
		else if (error != 0)
		{
			LOG_ERROR("Socket error after connect! [%d]", error);

			// Handle connection error
			if (error == WSAECONNREFUSED)
				LOG_ERROR("Server refused the connection!");

			engine.GetConsole().Log("Server refused the connection!");

			return -1;
		}
		else
		{
			LOG_OK("Connection established successfully!");
			engine.GetConsole().Log("Connection established successfully!");

			// Switch from POLLOUT to POLLIN to get incoming data
			poll_fds[0].events = POLLIN;

			OnConnectedToAuthServer();
			return 0;
		}
	}

	return 0;
}

void NECRONetManager::OnConnectedToAuthServer()
{
	NECROConsole& c = engine.GetConsole();

	isConnecting = false;
	authSocketConnected = true;

	// Get local IP address
	sockaddr_in local_addr{};
	socklen_t addr_len = sizeof(local_addr);
	if (getsockname(authSocket->GetSocketFD(), (sockaddr*)&local_addr, &addr_len) == 0)
	{
		char local_ip[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &local_addr.sin_addr, local_ip, INET_ADDRSTRLEN);
		
		std::string ipStr(local_ip);
		c.Log("My Local IP: " + ipStr);
		SetNetDataIpAddress(ipStr);
	}

	// Send greet packet
	Packet greetPacket;
	uint8_t usernameLenght = static_cast<uint8_t>(data.username.size());;

	greetPacket << uint8_t(AuthPacketIDs::PCKTID_AUTH_LOGIN_GATHER_INFO);
	greetPacket << uint8_t(AuthResults::AUTH_SUCCESS);
	greetPacket << uint16_t(sizeof(PacketAuthLoginGatherInfo) - PACKET_AUTH_LOGIN_GATHER_INFO_INITIAL_SIZE + usernameLenght - 1); // this means that after having read the first PACKET_AUTH_LOGIN_GATHER_INFO_INITIAL_SIZE bytes, the server will have to wait for sizeof(PacketAuthLoginGatherInfo) - PACKET_AUTH_LOGIN_GATHER_INFO_INITIAL_SIZE + usernameLenght-1 bytes in order to correctly read this packet

	greetPacket << CLIENT_VERSION_MAJOR;
	greetPacket << CLIENT_VERSION_MINOR;
	greetPacket << CLIENT_VERSION_REVISION;

	greetPacket << usernameLenght;
	greetPacket << data.username.data();

	NetworkMessage message(greetPacket);
	authSocket->QueuePacket(message);
	authSocket->Send();
}

int NECRONetManager::CheckForIncomingData()
{
	NECROConsole& c = engine.GetConsole();

	if (poll_fds[0].revents & (POLLERR | POLLHUP | POLLNVAL))
	{
		int error = 0;
		int len = sizeof(error);
		if (getsockopt(poll_fds[0].fd, SOL_SOCKET, SO_ERROR, (char*)&error, &len) == 0)
		{
			LOG_ERROR("AuthSocket encountered an error!");
			c.Log("Connection lost! Server may have crashed or kicked you.");
		}
		else
		{
			LOG_ERROR("getsockopt failed! [%d]", SocketUtility::GetLastError());
			c.Log("Connection lost! Server may have crashed or kicked you.");
		}

		return -1;
	}

	else if (poll_fds[0].revents & POLLIN)
	{
		int r = authSocket->Receive();

		// If receive failed
		if (r == -1)
		{
			LOG_ERROR("Client socket error/disconnection detected.");
			return -1;
		}
	}

	return 0;
}
