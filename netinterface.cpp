
#include <netinterface.h>

NetInterface::NetInterface()
{
#ifdef NETINTERFACE_USING_WINDOWS
    WSAData data = {0};
    WSAStartup(MAKEWORD(2,2),&data);
#endif
    memset(&peeraddress,0,sizeof(NISockAddrIn));
}
bool NetInterface::makeblock(){
#ifdef NETINTERFACE_USING_WINDOWS
    int timeout = 1000;
    if (setsockopt(connection, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(int)) != 0){
        destroysocket();
        return false;
    }
#else
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 1000000;
    if (setsockopt(connection, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv)) != 0) {
        destroysocket();
        return false;
    }
#endif
}
bool NetInterface::nonblock(){
#ifdef NETINTERFACE_USING_WINDOWS
    int timeout = 1;
    if (setsockopt(connection, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(int)) != 0){
        destroysocket();
        return false;
    }
#else
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 1000;
    if (setsockopt(connection, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv)) != 0) {
        destroysocket();
        return false;
    }
#endif
}
bool NetInterface::makesocket()
{
    connection = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    if(connection<=0){
        return false;
    }

    int enable = 1;
    if (setsockopt(connection, SOL_SOCKET, SO_REUSEADDR, (char*)&enable, sizeof(int)) != 0){
        destroysocket();
        return false;
    }

    if(!makeblock()){
        destroysocket();
        return false;
    }
    
    return true;
    
}
bool NetInterface::makesocketbind(){
    host = true;
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
void NetInterface::setremoteaddress(const char* hostname, uint16_t port){
#ifdef NETINTERFACE_USING_WINDOWS
    InetPtonA(AF_INET,hostname,&peeraddress.sin_addr)
#else
    inet_aton(hostname,&peeraddress.sin_addr);
#endif
    peeraddress.sin_port = htons(port);
}
void NetInterface::poll(std::function<void(const NIRelayPacket&,NITransferSize,bool&,bool&)> callback){
    NIRelayPacket packet = {0};
    NISockAddrIn address = {0};
    NISockAddrSize sizeofaddress = sizeof(NISockAddrIn);
    bool error = false;
    bool availabledata = false;
    NITransferSize size = recvfrom(connection,(char*)&packet,sizeof(NIRelayPacket),0,(NISockAddr*)&address,&sizeofaddress);
    if(size == sizeof(NIRelayPacket)){
        availabledata = true;
        if(packet.signature == NI_SIGNATURE
           && packet.packettype == kNIHandShake){//simple handshake
            if(host){
                //client sent a handshake
                //save the address
                sizeofpeeraddress = sizeofaddress;
                memcpy(&peeraddress,&address,sizeofaddress);
            }
        }
    }else if(size <= 0){
        if(!iserrornonblock()){
            error = true;
        }
    }
    callback(packet,size,availabledata,error);
}
void NetInterface::sendpacket(NIRelayPacket* packet,std::function<void(NITransferSize,bool&)> callback){
    NITransferSize size = sendto(connection,(char*)&packet,sizeof(NIRelayPacket),0,(NISockAddr*)&peeraddress,sizeofpeeraddress);
    bool error = false;
    if(size <= 0){
        if(!iserrornonblock()){
            error = true;
        }
    }
    if(callback!=nullptr){
        callback(size,error);
    }
    
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
