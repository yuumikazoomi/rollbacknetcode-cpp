
#include <netinterface.h>

NetInterface::NetInterface()
{
#ifdef NETINTERFACE_USING_WINDOWS
    WSAData data = {0};
    WSAStartup(MAKEWORD(2,2),&data);
#endif
    memset(&peeraddress,0,sizeof(NISockAddrIn));
}
bool NetInterface::makesocket()
{
    server = true;
    connection = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    if(connection<=0){
        return false;
    }

    int enable = 1;
    if (setsockopt(connection, SOL_SOCKET, SO_REUSEADDR, (char*)&enable, sizeof(int)) != 0)
    {
        destroysocket();
        return false;
    }
    int timeout = 1;
    if (setsockopt(connection, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(int)) != 0)
    {
        destroysocket();
        return false;
    }

    return true;
    
}
bool NetInterface::makesocketbind()
{
    server = true;
    NISockAddrIn selfaddress = {0};
    selfaddress.sin_family = AF_INET;
    selfaddress.sin_port = htons(6789);
    selfaddress.sin_addr.s_addr = INADDR_ANY;
    
    if(!makesocket())
    {
        return false;
    }

    if(bind(connection,(NISockAddr*)&selfaddress,sizeof(NISockAddrIn))!=0)
    {
        destroysocket();
        return false;
    }
    return true;
}
void NetInterface::destroysocket()
{
#ifdef NETINTERFACE_USING_WINDOWS
    if(connection != INVALID_SOCKET)
    {
        closesocket(connection);
    }
#else
    if(connection>0)
    {
        close(connection);
    }
#endif
}
void NetInterface::poll(RelayPacket* packet, bool* availabledata, bool* error){
    
}
NetInterface::~NetInterface()
{
    destroysocket();
#ifdef NETINTERFACE_USING_WINDOWS
    WSACleanup();
#endif
}
