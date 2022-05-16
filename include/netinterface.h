
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
typedef SOCKET  NISocket;
typedef int     NITransferSize;
typedef int     NISockAddrSize;
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
typedef int         NISocket;
typedef size_t      NITransferSize;
typedef socklen_t   NISockAddrSize;
#endif

#include <string.h>
#include <functional>
#define NI_SIGNATURE 0xDEAD
enum NIPacketId {
    kNIHandShake = 1,
};
typedef struct sockaddr_in  NISockAddrIn;
typedef struct sockaddr     NISockAddr;
typedef struct SNIRelayPacket{
    uint16_t signature;
    uint16_t packettype;
    uint16_t frame;
    uint16_t input;
}NIRelayPacket;
class NetInterface{
private:
    NISocket connection;
    bool host;
    NISockAddrIn peeraddress;
public:
    NetInterface();
    bool makesocket();
    bool makesocketbind();
    void destroysocket();
    void poll(std::function<void(const NIRelayPacket&,bool&,bool&)> callback);
    void sendinput(uint16_t input,std::function<void(bool&)> callback);
    
    bool iserrornonblock();
    int getlasterror();

    
    ~NetInterface();
};
#endif
 
