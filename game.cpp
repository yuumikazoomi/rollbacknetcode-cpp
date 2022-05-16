
#include <game.h>

Game::Game(bool host) : state(host),level(&state){
    this->host = host;
    currentframe = 0;
    
    if(host){
        if(!net.makesocketbind()){
            //maybe show an error
        }
    }else{
        if(net.makesocket()){
            net.setremoteaddress("127.0.0.1",6789);
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
        
        default:
            break;
    }
}
void Game::update(){
    //check network
    
    //we're going to save this so later we can call update on the input
    NIRelayPacket peerinput = {0};
    
    
    auto netcallback = [this,peerinput](const NIRelayPacket& packet,NITransferSize size) mutable {
        //handle pckets
        handlepacket(packet,size);
        
        
        if(packet.packettype == kProvidedInput){
            //save the peers input and later call update on it
            
            memcpy(&peerinput,&packet,sizeof(NIRelayPacket));
        }
    };
    net.poll(netcallback);
    
    
    
    uint16_t myinput = 0;
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
                        myinput = kInputRight;
                    }
                        break;
                    case SDLK_DOWN:{
                        myinput = kInputDown;
                    }
                        break;
                    case SDLK_LEFT:{
                        myinput = kInputLeft;
                    }
                        break;
                    case SDLK_UP:{
                        myinput = kInputUp;
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
        
        NIRelayPacket packet = {0};
        packet.packettype = kProvidedInput;
        packet.signature = NI_SIGNATURE;
        
        //make packet with our input
        /*
         *            four byte packet
         *
         *        high              low
         *      two bytes         two bytes
         *        input             frame
         * | - - - - - - - - | - - - - - - - - |
         */
        sethightwo(&packet.extra,myinput);
        setlowtwo(&packet.extra,currentframe);
        
        
        
        //send packet
        net.sendpacket(&packet);
        
        //update
        /*
         *            four byte packet
         *
         *        high              low
         *      two bytes         two bytes
         *        input             frame
         * | - - - - - - - - | - - - - - - - - |
         */
        uint16_t apponentframe = getlowtwo(peerinput.extra);
        uint16_t apponentinput = gethightwo(peerinput.extra);
        if(apponentinput!=0){
            printf("%d\n",apponentinput);
        }
        //update gamestate with both our input and peer's input
        state.update(myinput,apponentinput);
        
    }
    
    
    
}
void Game::draw(SDL_Renderer* renderer){
    level.draw(renderer);
}
