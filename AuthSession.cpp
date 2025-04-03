#include "AuthSession.h"
#include "ConsoleLogger.h"
#include "FileLogger.h"
#include "NECROClient.h"
#include "AuthCodes.h"

#pragma pack(push, 1)

struct AuthHandler
{
    AuthStatus status;
    size_t packetSize;
    bool (AuthSession::* handler)();
};

struct PacketAuthLoginGatherInfo
{
    uint8_t		id;
    uint8_t		error;
    uint16_t	size;

    uint8_t		versionMajor;
    uint8_t		versionMinor;
    uint8_t		versionRevision;

    uint8_t		usernameSize;
    uint8_t		username[1];
};
static_assert(sizeof(PacketAuthLoginGatherInfo) == (1 + 1 + 2 + 1 + 1 + 1 + 1 + 1), "PacketAuthLoginGatherInfo size assert failed!");
#define	MAX_USERNAME_LENGTH 16-1
#define MAX_ACCEPTED_GATHER_INFO_SIZE (sizeof(PacketAuthLoginGatherInfo) + MAX_USERNAME_LENGTH) // 16 is username length
#define PACKET_AUTH_LOGIN_GATHER_INFO_INITIAL_SIZE 4 // this represent the fixed portion of this packet, which needs to be read to at least identify the packet

#pragma pack(pop)


std::unordered_map<uint8_t, AuthHandler> AuthSession::InitHandlers()
{
	std::unordered_map<uint8_t, AuthHandler> handlers;

	// fill

	return handlers;
}
std::unordered_map<uint8_t, AuthHandler> const Handlers = AuthSession::InitHandlers();


void AuthSession::OnConnectedCallback()
{
	NECRONetManager& netManager = engine.GetNetManager();
	NECROConsole& c = engine.GetConsole();

	// Get local IP address
	sockaddr_in local_addr{};
	socklen_t addr_len = sizeof(local_addr);
	if (getsockname(GetSocketFD(), (sockaddr*)&local_addr, &addr_len) == 0)
	{
		char local_ip[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &local_addr.sin_addr, local_ip, INET_ADDRSTRLEN);

		std::string ipStr(local_ip);
		c.Log("My Local IP: " + ipStr);
		netManager.SetNetDataIpAddress(ipStr);
	}

	// Send greet packet
	Packet greetPacket;
	uint8_t usernameLenght = static_cast<uint8_t>(netManager.GetData().username.size());;

	greetPacket << uint8_t(AuthPacketIDs::PCKTID_AUTH_LOGIN_GATHER_INFO);
	greetPacket << uint8_t(AuthResults::AUTH_SUCCESS);
	greetPacket << uint16_t(sizeof(PacketAuthLoginGatherInfo) - PACKET_AUTH_LOGIN_GATHER_INFO_INITIAL_SIZE + usernameLenght - 1); // this means that after having read the first PACKET_AUTH_LOGIN_GATHER_INFO_INITIAL_SIZE bytes, the server will have to wait for sizeof(PacketAuthLoginGatherInfo) - PACKET_AUTH_LOGIN_GATHER_INFO_INITIAL_SIZE + usernameLenght-1 bytes in order to correctly read this packet

	greetPacket << CLIENT_VERSION_MAJOR;
	greetPacket << CLIENT_VERSION_MINOR;
	greetPacket << CLIENT_VERSION_REVISION;

	greetPacket << usernameLenght;
	greetPacket << netManager.GetData().username.data();

	NetworkMessage message(greetPacket);
	QueuePacket(message);
	Send();
}

void AuthSession::ReadCallback()
{
	LOG_OK("AuthSession ReadCallback");
}
