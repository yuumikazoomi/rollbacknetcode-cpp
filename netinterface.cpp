
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
    if (setsockopt(connection, SOL_SOCKET, SO_REUSEADDR, (char*)&enable, sizeof(int)) != 0){
        destroysocket();
        return false;
    }
    int timeout = 1;
    if (setsockopt(connection, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(int)) != 0){
        destroysocket();
        return false;
    }

    return true;
    
}
bool NetInterface::makesocketbind(){
    server = true;
    NISockAddrIn selfaddress = {0};
    selfaddress.sin_family = AF_INET;
    selfaddress.sin_port = htons(6789);
    selfaddress.sin_addr.s_addr = INADDR_ANY;
    
    if(!makesocket()){
        return false;
    }

    if(bind(connection,(NISockAddr*)&selfaddress,sizeof(NISockAddrIn))!=0){
        destroysocket();
        return false;
    }
    return true;
}
void NetInterface::destroysocket()
{
#ifdef NETINTERFACE_USING_WINDOWS
    if(connection != INVALID_SOCKET){
        closesocket(connection);
    }
#else
    if(connection>0){
        close(connection);
    }
#endif
}
void NetInterface::poll(std::function<void(const NIRelayPacket&,bool&,bool&)> callback){
    NIRelayPacket packet = {0};
    NISockAddrIn address = {0};
    NISockAddrSize sizeofaddress= sizeof(NISockAddrIn);
    bool error = false;
    bool availabledata = false;
    NITransferSize size = recvfrom(connection,(char*)&packet,sizeof(NIRelayPacket),0,(NISockAddr*)&address,&sizeofaddress);
    if(size == sizeof(NIRelayPacket)){
        availabledata = true;
    }else if(size <= 0){
        
    }
    callback(packet,availabledata,error);
}
bool NetInterface::iserrornonblock(){
    int error = getlasterror();
#ifdef NETINTERFACE_USING_WINDOWS
    if(error == WSAEWOULDBLOCK){
        return true;
    }
#else
    if(error == EWOULDBLOCK){
        return true;
    }
#endif
    return false;
}
int NetInterface::getlasterror(){
#ifdef NETINTERFACE_USING_WINDOWS
    return WSAGetLastError();
#else
    return errno;
#endif
}
NetInterface::~NetInterface(){
    destroysocket();
#ifdef NETINTERFACE_USING_WINDOWS
    WSACleanup();
#endif
}
