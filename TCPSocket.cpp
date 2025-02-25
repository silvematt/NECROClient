#include "TCPSocket.h"
#include "SocketUtility.h"

#ifndef _WIN32
#include <sys/socket.h>
#endif

#include <iostream>
#include <string>

#include "ConsoleLogger.h"
#include "FileLogger.h"


TCPSocket::TCPSocket(SocketAddressesFamily family)
{
	closed = false;

	m_socket = socket(family, SOCK_STREAM, IPPROTO_TCP);

	if (m_socket == INVALID_SOCKET)
	{
		LOG_ERROR(std::string("Error 1 during TCPSocket::Create()"));
		LOG_ERROR(std::to_string(SocketUtility::GetLastError()));
	}
}

TCPSocket::TCPSocket(sock_t inSocket)
{
	closed = false;

	m_socket = inSocket;

	if (m_socket == INVALID_SOCKET)
	{
		LOG_ERROR(std::string("Error 2 during TCPSocket::Create()"));
		LOG_ERROR(std::to_string(SocketUtility::GetLastError()));
	}
}

TCPSocket::~TCPSocket()
{
	Close();
}

bool TCPSocket::IsOpen()
{
	return (!closed);
}

int TCPSocket::Bind(const SocketAddress& addr)
{
	int err = bind(m_socket, &addr.m_addr, addr.GetSize());

	if (err != 0)
	{
		LOG_ERROR(std::string("Error during TCPSocket::Bind() [" + std::to_string(SocketUtility::GetLastError()) + "]"));
		return SocketUtility::GetLastError();
	}

	return SOCKET_UTILITY_NO_ERROR;
}

int TCPSocket::Listen(int backlog)
{
	int err = listen(m_socket, backlog);

	if (err != 0)
	{
		LOG_ERROR(std::string("Error during TCPSocket::Listen() [" + std::to_string(SocketUtility::GetLastError()) + "]"));
		return SocketUtility::GetLastError();
	}

	return SOCKET_UTILITY_NO_ERROR;
}

std::shared_ptr<TCPSocket> TCPSocket::Accept(SocketAddress& addr)
{
	int addrLen = addr.GetSize();
	sock_t inSocket = accept(m_socket, &addr.m_addr, &addrLen);

	if (inSocket != INVALID_SOCKET)
		return std::make_shared<TCPSocket>(inSocket);
	else
	{
		if (!SocketUtility::ErrorIsWouldBlock())
		{
			LOG_ERROR(std::string("Error during TCPSocket::Accept()"));
		}

		return nullptr;
	}
}

int TCPSocket::Connect(const SocketAddress& addr)
{
	int err = connect(m_socket, &addr.m_addr, addr.GetSize());

	if (err != 0)
	{
		LOG_ERROR(std::string("Error during TCPSocket::Connect() [") + std::to_string(SocketUtility::GetLastError()) + "]");
		return SocketUtility::GetLastError();
	}

	return SOCKET_UTILITY_NO_ERROR;
}

void TCPSocket::QueuePacket(NetworkMessage& pckt)
{
	outQueue.push(pckt);
}

int TCPSocket::Send()
{
	if (outQueue.empty())
		return 0;

	NetworkMessage& out = outQueue.front();

	int bytesSent = send(m_socket, reinterpret_cast<const char*>(out.GetReadPointer()), out.GetActiveSize(), 0);

	if (bytesSent < 0)
	{
		if (SocketUtility::ErrorIsWouldBlock())
			return 0;

		Shutdown();

		LOG_ERROR(std::string("Error during TCPSocket::Send() [") + std::to_string(SocketUtility::GetLastError()) + "]");
		return SocketUtility::GetLastError();
	}

	// Mark that 'bytesSent' were sent
	out.ReadCompleted(bytesSent);

	if (out.GetActiveSize() == 0)
		outQueue.pop(); // if whole packet was sent, pop it from the queue, otherwise we had a short send and will come back later

	// SendCallback(); needed?

	return bytesSent;
}

int TCPSocket::Receive()
{
	if (!IsOpen())
		return 0;

	inBuffer.CompactData();
	inBuffer.EnlargeBufferIfNeeded();

	// Manually write on the inBuffer
	int bytesReceived = recv(m_socket, reinterpret_cast<char*>(inBuffer.GetWritePointer()), inBuffer.GetRemainingSpace(), 0);

	if (bytesReceived < 0)
	{
		if (SocketUtility::ErrorIsWouldBlock())
			return 0;

		Shutdown();
		LOG_ERROR(std::string("Error during TCPSocket::Receive() [") + std::to_string(SocketUtility::GetLastError()) + "]");
		return -1;
	}

	// Make sure to update the write pos
	inBuffer.WriteCompleted(bytesReceived);

	LOG_INFO("Received %d bytes of something!", bytesReceived);

	ReadCallback();	// this will handle the data we've received

	return bytesReceived;
}

int TCPSocket::SetBlockingEnabled(bool blocking)
{
#ifdef _WIN32
	u_long mode = blocking ? 0 : 1;
	int result = ioctlsocket(m_socket, FIONBIO, &mode);

	if (result != 0)
	{
		LOG_ERROR(std::string("Error during TCPSocket::SetBlockingEnabled(" + std::to_string(blocking) + ") [") + std::to_string(SocketUtility::GetLastError()) + "]");
		return SocketUtility::GetLastError();
	}

	return SOCKET_UTILITY_NO_ERROR;
#else
	int flags = fcntl(fd, F_GETFL, 0);

	if (flags == -1)
	{
		LOG_ERROR(std::string("Error during TCPSocket::SetBlockingEnabled(" + std::to_string(blocking) + ") [") + std::to_string(SocketUtility::GetLastError()) + "]");
		return SocketUtility::GetLastError();
	}

	flags = blocking ? (flags & ~O_NONBLOCK) : (flags | O_NONBLOCK);

	result = fcntl(fd, F_SETFL, flags);

	if (result != 0)
	{
		LOG_ERROR(std::string("Error during TCPSocket::SetBlockingEnabled(" + std::to_string(blocking) + ") [") + std::to_string(SocketUtility::GetLastError()) + "]");
		return SocketUtility::GetLastError();
	}

	return SOCKET_UTILITY_NO_ERROR;
#endif
}

int TCPSocket::SetSocketOption(int lvl, int optName, const char* optVal, int optLen)
{
	int optResult = setsockopt(m_socket, lvl, optName, optVal, optLen);

	if (optResult != 0)
	{
		LOG_ERROR(std::string("Error during TCPSocket::SetSocketOption(" + std::to_string(optName) + ") [") + std::to_string(SocketUtility::GetLastError()) + "]");
		return SocketUtility::GetLastError();
	}

	return SOCKET_UTILITY_NO_ERROR;
}

int TCPSocket::Shutdown()
{
	if (closed)
		return 0;

	closed = true;

#ifdef _WIN32
	int result = shutdown(m_socket, SD_SEND);

	if (result < 0)
		LOG_ERROR("Error while shutting down the socket");

	return result;
#else
	int result = shutdown(m_socket, SHUT_WR);

	if (result < 0)
		LOG_ERROR("Error while shutting down the socket");

	return result;
#endif
}

int TCPSocket::Close()
{
#ifdef _WIN32

	int result = closesocket(m_socket);
	return result;
#else
	int result = close(m_socket);
	return result;
#endif
}
