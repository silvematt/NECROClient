#include "AuthSession.h"
#include "ConsoleLogger.h"
#include "FileLogger.h"
#include "NECROClient.h"
#include "AuthCodes.h"

#pragma pack(push, 1)

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


struct PacketAuthLoginGatherInfoResponse
{
	uint8_t		id;
	uint8_t		error;
	uint16_t	size;
};
static_assert(sizeof(PacketAuthLoginGatherInfoResponse) == (1 + 1 + 2), "PacketAuthLoginGatherInfoResponse size assert failed!");
#define MAX_ACCEPTED_GATHER_INFO_RESPONSE_SIZE (sizeof(PacketAuthLoginGatherInfoResponse)) // 16 is username length
#define PACKET_AUTH_LOGIN_GATHER_INFO_RESPONSE_INITIAL_SIZE 4 // this represent the fixed portion of this packet, which needs to be read to at least identify the packet

#pragma pack(pop)


std::unordered_map<uint8_t, AuthHandler> AuthSession::InitHandlers()
{
	std::unordered_map<uint8_t, AuthHandler> handlers;

	// fill
	handlers[PCKTID_AUTH_LOGIN_GATHER_INFO] = { AuthStatus::STATUS_GATHER_INFO, PACKET_AUTH_LOGIN_GATHER_INFO_RESPONSE_INITIAL_SIZE , &HandlePacketAuthLoginGatherInfoResponse };

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

    NetworkMessage& packet = inBuffer;

    while (packet.GetActiveSize())
    {
        uint8_t cmd = packet.GetReadPointer()[0]; // read first byte

        auto it = Handlers.find(cmd);
        if (it == Handlers.end())
        {
            // Discard packet, nothing we should handle
            packet.Clear();
            break;
        }

        // Check if the current cmd matches our state
        if (status != it->second.status)
        {
            Shutdown();
            Close();
            return;
        }

        // Check if the passed packet sizes matches the handler's one, otherwise we're not ready to process this yet
        uint16_t size = uint16_t(it->second.packetSize);
        if (packet.GetActiveSize() < size)
            break;

        // If it's a variable-sized packet, we need to ensure size
        

        // At this point, ensure the read size matches the whole packet size
        if (packet.GetActiveSize() < size)
            break;  // probably a short receive

        // Call the Handler's function and ensure it returns true
        if (!(*this.*it->second.handler)())
        {
            Close();
            return;
        }

        packet.ReadCompleted(size); // Flag the read as completed, the while will look for remaining packets
    }
}

bool AuthSession::HandlePacketAuthLoginGatherInfoResponse()
{
    NECROConsole& c = engine.GetConsole();
    PacketAuthLoginGatherInfoResponse* pckData = reinterpret_cast<PacketAuthLoginGatherInfoResponse*>(inBuffer.GetBasePointer());

    if (pckData->error == AuthResults::AUTH_SUCCESS)
    {
        // Continue authentication
        c.Log("Authentication succeeded...");
    }
    else if (pckData->error == AuthResults::AUTH_FAILED_USERNAME_IN_USE)
    {
        LOG_ERROR("Authentication failed, username is already in use.");
        c.Log("Authentication failed. Username is already in use.");
        return false;
    }
    else
    {
        LOG_ERROR("Authentication failed, server hasn't returned AuthResults::AUTH_SUCCESS.");
        c.Log("Authentication failed.");
        return false;
    }

	return true;
}
