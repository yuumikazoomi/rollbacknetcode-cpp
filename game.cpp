
#include <game.h>
Game::Game(bool host) : state(host),level(&state){
    this->host = host;
    if(host){
        if(!net.makesocketbind()){
            //maybe show an error
        }
    }else{
        if(net.makesocket()){
            printf("made socket\n");
            net.setremoteaddress("127.0.0.1",6789);
            //send handshake
            NIRelayPacket packet = {0};
            packet.signature = NI_SIGNATURE;
            packet.packettype = kNIHandShake;
            auto t = [this](NITransferSize size, bool error){
                if(size == sizeof(NIRelayPacket)){
                    printf("sent handshake\n");
                }else if(error){
                    printf("error\n");
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
    auto netcallback = [this](const NIRelayPacket& packet,NITransferSize size,bool& availabledata,bool& error){
        if(availabledata){
            switch (packet.packettype) {
                case kNIHandShake:{
                    
                    //send them our seed
                    uint32_t seed = state.getrandomseed();
                    
                    NIRelayPacket outgoing = {0};
                    outgoing.packettype = kSeed;
                    outgoing.signature = NI_SIGNATURE;
                    outgoing.extra = seed;
                    
                    net.sendpacket(&outgoing);
                    printf("we sent a seed:%x\n",seed);
                }
                    break;
                case kSeed:{
                    uint32_t seed = (uint32_t)packet.extra;
                    printf("We got a seed:%x\n",seed);
                }
                    break;
                default:
                    break;
            }
        }else if(error){
            
        }
    };
    net.poll(netcallback);
    
    //update
    state.update();
}
void Game::draw(SDL_Renderer* renderer){
    level.draw(renderer);
}
