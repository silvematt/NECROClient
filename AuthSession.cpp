#include "AuthSession.h"
#include "ConsoleLogger.h"
#include "FileLogger.h"
#include "NECROClient.h"

#include "AuthCodes.h"

#include <sstream>
#include <iomanip>

#include "AES.h"


#pragma pack(push, 1)

// -------------------------------------------------------------------------------------------------------
// When defining packets: 
// 1) S prefix means (for)Server, so it's a packet that the server will receive and the client will send
// 2) C prefix means (for)Client, so it's a packet that the client will receive and the server will send
// -------------------------------------------------------------------------------------------------------

struct SPacketAuthLoginGatherInfo
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
static_assert(sizeof(SPacketAuthLoginGatherInfo) == (1 + 1 + 2 + 1 + 1 + 1 + 1 + 1), "SPacketAuthLoginGatherInfo size assert failed!");
#define	MAX_USERNAME_LENGTH 16-1
#define S_MAX_ACCEPTED_GATHER_INFO_SIZE (sizeof(SPacketAuthLoginGatherInfo) + MAX_USERNAME_LENGTH) // 16 is username length
#define S_PACKET_AUTH_LOGIN_GATHER_INFO_INITIAL_SIZE 4 // this represent the fixed portion of this packet, which needs to be read to at least identify the packet


struct CPacketAuthLoginGatherInfo
{
	uint8_t		id;
	uint8_t		error;
};
static_assert(sizeof(CPacketAuthLoginGatherInfo) == (1 + 1), "CPacketAuthLoginGatherInfo size assert failed!");


// Login Proof doesn't exist for now, this is just a dull packet
struct SPacketAuthLoginProof
{
    uint8_t		id;
    uint8_t		error;

    uint32_t    clientsIVRandomPrefix;
};
static_assert(sizeof(SPacketAuthLoginProof) == (1 + 1 + 4), "SPacketAuthLoginProof size assert failed!");


// Login Proof doesn't exist for now, this is just a dull packet
struct CPacketAuthLoginProof
{
    uint8_t		id;
    uint8_t		error;
    uint16_t    size;

    uint8_t     sessionKey[AES_128_KEY_SIZE];
};
static_assert(sizeof(CPacketAuthLoginProof) == (1 + 1 + 2 + AES_128_KEY_SIZE), "CPacketAuthLoginProof size assert failed!");
#define C_PACKET_AUTH_LOGIN_PROOF_INITIAL_SIZE 4 // this represent the fixed portion of this packet, which needs to be read to at least identify the packet


#pragma pack(pop)


std::unordered_map<uint8_t, AuthHandler> AuthSession::InitHandlers()
{
	std::unordered_map<uint8_t, AuthHandler> handlers;

	// fill
	handlers[PCKTID_AUTH_LOGIN_GATHER_INFO] = { AuthStatus::STATUS_GATHER_INFO, sizeof(CPacketAuthLoginGatherInfo) , &HandlePacketAuthLoginGatherInfoResponse};
    handlers[PCKTID_AUTH_LOGIN_ATTEMPT]     = { AuthStatus::STATUS_LOGIN_ATTEMPT, C_PACKET_AUTH_LOGIN_PROOF_INITIAL_SIZE , &HandlePacketAuthLoginProofResponse };

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
	greetPacket << uint16_t(sizeof(SPacketAuthLoginGatherInfo) - S_PACKET_AUTH_LOGIN_GATHER_INFO_INITIAL_SIZE + usernameLenght - 1); // this means that after having read the first PACKET_AUTH_LOGIN_GATHER_INFO_INITIAL_SIZE bytes, the server will have to wait for sizeof(PacketAuthLoginGatherInfo) - PACKET_AUTH_LOGIN_GATHER_INFO_INITIAL_SIZE + usernameLenght-1 bytes in order to correctly read this packet

	greetPacket << CLIENT_VERSION_MAJOR;
	greetPacket << CLIENT_VERSION_MINOR;
	greetPacket << CLIENT_VERSION_REVISION;

	greetPacket << usernameLenght;
	greetPacket << netManager.GetData().username; // string is and should be without null terminator!

	NetworkMessage message(greetPacket);
	QueuePacket(message);
    //Send(); packets are sent by checking POLLOUT events in the socket, and we check for POLLOUT events only if there are packets written in the outQueue
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
            LOG_WARNING("Discarding packet.");

            // Discard packet, nothing we should handle
            packet.Clear();
            break;
        }

        // Check if the current cmd matches our state
        if (status != it->second.status)
        {
            LOG_WARNING("Status mismatch. Status is: '" + std::to_string(status) + "' but should have been '" + std::to_string(it->second.status) + "'. Closing the connection.");

            Shutdown();
            Close();
            return;
        }

        // Check if the passed packet sizes matches the handler's one, otherwise we're not ready to process this yet
        uint16_t size = uint16_t(it->second.packetSize);
        if (packet.GetActiveSize() < size)
            break;

        // If it's a variable-sized packet, we need to ensure size
        if (cmd == PCKTID_AUTH_LOGIN_ATTEMPT)
        {
            CPacketAuthLoginProof* pcktData = reinterpret_cast<CPacketAuthLoginProof*>(packet.GetReadPointer());
            size += pcktData->size; // we've read the handler's defined packetSize, so this is safe. Attempt to read the remainder of the packet

            // Check for size
            if (size > sizeof(CPacketAuthLoginProof))
            {
                Shutdown();
                Close();
                return;
            }
        }
        

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
    CPacketAuthLoginGatherInfo* pckData = reinterpret_cast<CPacketAuthLoginGatherInfo*>(inBuffer.GetBasePointer());
    NECRONetManager& net = engine.GetNetManager();

    if (pckData->error == AuthResults::AUTH_SUCCESS)
    {
        // Continue authentication
        c.Log("Gather info succeded...");
        status = STATUS_LOGIN_ATTEMPT;

        // Here you would send login proof to the server, after having received hashes in the CPacketAuthLoginGatherInfo packet above
        // Send the random IV prefix so the server can make sure it's not the same as the client
        Packet packet;

        packet << uint8_t(AuthPacketIDs::PCKTID_AUTH_LOGIN_ATTEMPT);
        packet << uint8_t(LoginProofResults::LOGIN_SUCCESS);
        
        // Randomize and send the prefix
        net.GetData().iv.RandomizePrefix();
        net.GetData().iv.ResetCounter();

        packet << uint32_t(net.GetData().iv.prefix);

        std::cout << "My IV Prefix: " << net.GetData().iv.prefix << std::endl;

        NetworkMessage m(packet);
        QueuePacket(m);
        //Send(); packets are sent by checking POLLOUT events in the socket, and we check for POLLOUT events only if there are packets written in the outQueue

    }
    else if (pckData->error == AuthResults::AUTH_FAILED_USERNAME_IN_USE)
    {
        LOG_ERROR("Authentication failed, username is already in use.");
        c.Log("Authentication failed. Username is already in use.");
        return false;
    }
    else if (pckData->error == AuthResults::AUTH_FAILED_UNKNOWN_ACCOUNT)
    {
        LOG_ERROR("Authentication failed, username does not exist.");
        c.Log("Authentication failed, username does not exist.");
        return false;
    }
    else if (pckData->error == AuthResults::AUTH_FAILED_WRONG_CLIENT_VERSION)
    {
        LOG_ERROR("Authentication failed, invalid client version.");
        c.Log("Authentication failed, invalid client version.");
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

bool AuthSession::HandlePacketAuthLoginProofResponse()
{
    NECRONetManager& netManager = engine.GetNetManager();

    NECROConsole& c = engine.GetConsole();
    CPacketAuthLoginProof* pckData = reinterpret_cast<CPacketAuthLoginProof*>(inBuffer.GetBasePointer());

    if (pckData->error == LoginProofResults::LOGIN_SUCCESS)
    {
        // Continue authentication
        c.Log("Authentication succeeded.");
        status = STATUS_AUTHED;

        // Save the session key in the netManager data
        std::copy(std::begin(pckData->sessionKey), std::end(pckData->sessionKey), std::begin(netManager.GetData().sessionKey));

        // Convert sessionKey to hex string in order to print it
        std::ostringstream sessionStrStream;
        for (int i = 0; i < AES_128_KEY_SIZE; ++i)
        {
            sessionStrStream << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(netManager.GetData().sessionKey[i]);
        }
        std::string sessionStr = sessionStrStream.str();

        LOG_DEBUG("My session key is: " + sessionStr);
    }
    else //  (pckData->error == LoginProofResults::LOGIN_FAILED)
    {
        LOG_ERROR("Authentication failed. Server returned LoginProofResults::LOGIN_FAILED..");
        c.Log("Authentication failed.");
        return false;
    }

    return true;
}
