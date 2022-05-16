
#ifndef _netinterface_h
#define _netinterface_h


#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define NETINTERFACE_USING_WINDOWS
#else
#define NETINTERFACE_USING_BSD
#endif


#ifdef NETINTERFACE_USING_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <ws2tcpip.h>
typedef SOCKET NISocket;
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
typedef int NISocket;
#endif
#include <string.h>
#include <stdbool.h>
#define NI_SIGNATURE 0xDEAD
enum NIPacketId {
    kNIHandShake = 1,
};
typedef struct sockaddr_in  NISockAddrIn;
typedef struct sockaddr     NISockAddr;
typedef struct SRelayPacket{
    uint16_t header;
    uint16_t signature;
    uint16_t frame;
    uint16_t input;
}RelayPacket;
class NetInterface{
private:
    NISocket connection;
    bool server;
    NISockAddrIn peeraddress;
public:
    NetInterface();
    bool makesocket();
    bool makesocketbind();
    void destroysocket();
    void poll(RelayPacket* packet, bool* availabledata, bool* error);
    ~NetInterface();
    
};
#endif
 
