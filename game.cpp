
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
                    
                }else{
                }
                
            };
            
            
            net.sendpacket(&packet,t);
        }else{
            //maybe show an error
        }
    }
    processing = false;
    
}
void Game::handlepacket(const NIRelayPacket& packet, NITransferSize size){
    switch (packet.packettype) {
        case kNIHandShake:{
            //send them our seed
            uint32_t seed = state.getrandomseed();
            
            NIRelayPacket outgoing = {0};
            outgoing.packettype = kSeed;
            outgoing.signature = NI_SIGNATURE;
            outgoing.extra = seed;
            
            net.sendpacket(&outgoing);
            
            //begin gameplay
            processing = true;
        }
            break;
        case kSeed:{
            processing = true;
            uint32_t seed = (uint32_t)packet.extra;
            state.setrandomseed(seed);
        }
            break;
        case kProvidedInput:{
            /*
             *            four byte packet
             *
             *        high              low
             *      two bytes         two bytes
             *        input             frame
             * | - - - - - - - - | - - - - - - - - |
             */
            uint16_t frame = getlowtwo(packet.extra);
            uint16_t input = gethightwo(packet.extra);
            state.updatedirection(state.getapponent(),input,frame);
        }
            break;
        case kProvidedNoInput:{
            uint16_t frame = getlowtwo(packet.extra);
            
            //user sent no input on frame x
        }
            break;
        default:
            break;
    }
}
void Game::update(){
    //check network
    auto netcallback = [this](const NIRelayPacket& packet,NITransferSize size){
        //handle pckets
        handlepacket(packet,size);
    };
    net.poll(netcallback);
    uint16_t direction = 0;
    bool directionchanged = false;
    SDL_Event event;
    
    while(SDL_PollEvent(&event)){
        switch (event.type){
            case SDL_QUIT:
                exit(1);
                //quit
                break;
            case SDL_KEYDOWN:{
                switch (event.key.keysym.sym) {
                    case SDLK_RIGHT:{
                        directionchanged = true;
                        direction = kDirectionRight;
                    }
                        break;
                    case SDLK_DOWN:{
                        directionchanged = true;
                        direction = kDirectionDown;
                    }
                        break;
                    case SDLK_LEFT:{
                        directionchanged = true;
                        direction = kDirectionLeft;
                    }
                        break;
                    case SDLK_UP:{
                        directionchanged = true;
                        direction = kDirectionUp;
                    }
                        break;
                }
            }
                break;
            default:
                break;
        }
    }
    if(processing){
        //send input to peer
        if(directionchanged){
            state.updatedirection(direction);
            
            NIRelayPacket packet = {0};
            packet.packettype = kProvidedInput;
            packet.signature = NI_SIGNATURE;
            
            //make the packet
            sethightwo(&packet.extra,state.getself()->getdirection());
            setlowtwo(&packet.extra,state.getframecount());
            
            //send the packet
            net.sendpacket(&packet);
        }else{
            //send no input packet
            NIRelayPacket packet = {0};
            packet.packettype = kProvidedNoInput;
            packet.signature = NI_SIGNATURE;
            setlowtwo(&packet.extra,state.getframecount());
            
            //send the packet
            net.sendpacket(&packet);
        }
        //update
        state.update();
    }
    
    
    
}
void Game::draw(SDL_Renderer* renderer){
    level.draw(renderer);
}
