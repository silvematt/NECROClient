#ifndef TCP_SOCKET_H
#define TCP_SOCEKT_H

#ifdef _WIN32
#include "WinSock2.h"
#include <WS2tcpip.h>
typedef SOCKET sock_t;
#else
typedef int sock_t;
#endif

#include <memory>
#include <cstdint>
#include <queue>

#include <openssl/ssl.h>

#include "SocketAddress.h"
#include "NetworkMessage.h"

#define READ_BLOCK_SIZE 4096

const int TCP_LISTEN_DEFUALT_BACKLOG = SOMAXCONN;

enum SocketAddressesFamily
{
	INET = AF_INET,
	INET6 = AF_INET6
};

//-------------------------------------------------------
// Defines a TCP Socket object.
//-------------------------------------------------------
class TCPSocket
{
protected:
	friend class SocketAddress;
	sock_t m_socket;

	SocketAddress remoteAddress;
	uint16_t remotePort;

	// Read/Write buffers
	NetworkMessage inBuffer;
	std::queue<NetworkMessage> outQueue;

	bool closed = false;

	// OpenSSL support
	bool usesTLS = false;
	SSL* ssl;
	BIO* bio;

public:
	TCPSocket(SocketAddressesFamily family);
	TCPSocket(sock_t inSocket);

	~TCPSocket();

	virtual void OnConnectedCallback() {};
	virtual void ReadCallback() {};
	virtual void SendCallback() {};

	bool						IsOpen();

	int							Bind(const SocketAddress& addr);
	int							Listen(int backlog = TCP_LISTEN_DEFUALT_BACKLOG);
	std::shared_ptr<TCPSocket>	Accept(SocketAddress& addr);
	int							Connect(const SocketAddress& addr);

	void						QueuePacket(NetworkMessage& pckt);
	int							Send();
	int							Receive();

	int							Shutdown();
	int							Close();

	sock_t						GetSocketFD() { return m_socket; };

	int							SetBlockingEnabled(bool blocking);
	int							SetSocketOption(int lvl, int optName, const char* optVal, int optLen);

	void SetRemoteAddressAndPort(const SocketAddress& s, const uint16_t& p)
	{
		remoteAddress = s;
		remotePort = p;
	}

	// OpenSSL
	void TLSSetup(const char* hostname);
	void TLSPerformHandshake();
};

#endif
