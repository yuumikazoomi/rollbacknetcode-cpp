
#include <game.h>

Game::Game(bool host) : state(host),level(&state){
    this->host = host;
    if(host){
        if(!net.makesocketbind()){
            //maybe show an error
        }
    }else{
        if(net.makesocket()){
            net.setremoteaddress("0.0.0.0",6789);
            //send handshake
            NIRelayPacket packet = {0};
            packet.signature = NI_SIGNATURE;
            packet.packettype = kNIHandShake;
            
            
            auto t = [this,packet](NITransferSize size){
                if(size == sizeof(NIRelayPacket)){
                    printpacket(packet);
                }else{
                }
                
            };
            
            
            net.sendpacket(&packet,t);
        }else{
            //maybe show an error
        }
    }
    
}

void Game::update(){
    //check input
    SDL_Event event;
    while (SDL_PollEvent(&event)){
        switch (event.type){
            case SDL_QUIT:
                exit(1);
                //quit
                break;
        }
    }
    
    //check network
    auto netcallback = [this](const NIRelayPacket& packet,NITransferSize size){
        printpacket(packet);
        switch (packet.packettype) {
            case kNIHandShake:{
                //send them our seed
                uint32_t seed = state.getrandomseed();
                
                NIRelayPacket outgoing = {0};
                outgoing.packettype = kSeed;
                outgoing.signature = NI_SIGNATURE;
                outgoing.extra = seed;
                
                net.sendpacket(&outgoing);
            }
                break;
            case kSeed:{
                uint32_t seed = (uint32_t)packet.extra;
                state.setrandomseed(seed);
            }
                break;
            default:
                break;
        }
    };
    net.poll(netcallback);
    
    //update
    state.update();
}
void Game::draw(SDL_Renderer* renderer){
    level.draw(renderer);
}
