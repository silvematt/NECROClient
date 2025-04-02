#ifndef AUTH_SESSION_H
#define AUTH_SESSION_H

#include "TCPSocket.h"
#include <unordered_map>

// Status of the current socket
enum AuthStatus
{
    STATUS_GATHER_INFO = 0,
    STATUS_LOGIN_ATTEMPT,
    STATUS_AUTHED,
    STATUS_CLOSED
};

class AuthSession;
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

//----------------------------------------------------------------------------------------------------
// AuthSession is the extension of the base TCPSocket class, that defines the methods and
// functionality that defines the exchange of messages with the connected server
//----------------------------------------------------------------------------------------------------
class AuthSession : public TCPSocket
{
public:
    AuthSession(SocketAddressesFamily fam) : TCPSocket(fam), status(STATUS_GATHER_INFO) {}

    AuthSession(sock_t socket) : TCPSocket(socket), status(STATUS_GATHER_INFO) {}

    AuthStatus status;

    static std::unordered_map<uint8_t, AuthHandler> InitHandlers();

    void ReadCallback() override;
};

#endif
