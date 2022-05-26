
#include <game.h>

Game::Game(bool host) : level(&mCurrentState){
    this->host = host;
    
    //set this to true once handshake and seeds are shared
    connected = false;
    
    if(host){
        mCurrentState.generaterandomseed();
        
        //make a binding socket
        if(!net.makesocketbind()){
            //maybe show an error
        }
    }else{
        if(net.makesocket()){
            //send handshake
            //in return we'll receive a random seed
            net.setremoteaddress("127.0.0.1",6789);
            
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
    
    mFrameNumber = 0;
    
    mLastFrameFromApponent = 0;
}
void Game::handlepacket(const NIRelayPacket& packet, NITransferSize size){
    switch (packet.packettype) {
        case kNIHandShake:{
            //send them our seed
            uint32_t seed = mCurrentState.getrandomseed();
            
            NIRelayPacket outgoing = {0};
            outgoing.packettype = kSeed;
            outgoing.signature = NI_SIGNATURE;
            outgoing.extra = seed;
            
            net.sendpacket(&outgoing);
            
            //spawn the objective
            mCurrentState.spawnobjective();
            //begin gameplay
            

            processing = true;
            connected = true;
        }
            break;
        case kSeed:{
            
            
            uint32_t seed = (uint32_t)packet.extra;
            mCurrentState.setrandomseed(seed);
            
            //spawn the objective
            mCurrentState.spawnobjective();
            

            processing = true;
            connected = true;
        }
            break;
        case kProvidedInput:{
            uint32_t extra = packet.extra;
            InputForFrame apponentInput;
            apponentInput.mInput = gethightwo(extra);
            apponentInput.mFrame = getlowtwo(extra);
            mApponentInputQue.push_front(apponentInput);
            
            //store this for later
            mLastFrameFromApponent = apponentInput.mFrame;
            
            //check if we need to suspend processing in case we're getting too far ahead
            if(mFrameNumber - apponentInput.mFrame > INPUT_RENDER_DELAY){
                printf("suspending until we get a recent frame\n");
                //suspend until we're within render delay?
                processing = false;
            }else{
                if(connected){
                    processing = true;
                }
                
            }
            
        }
            break;
        default:
            break;
    }
}
void Game::update(){
    //check network
    
    //we're going to save this so later we can call update on the input
    
    
    
    auto netcallback = [this](const NIRelayPacket& packet,NITransferSize size) mutable {
        //handle pckets
        handlepacket(packet,size);
    };
    net.poll(netcallback);
    
    
    
    uint16_t myInput = 0;
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
                        myInput = kInputRight;
                    }
                        break;
                    case SDLK_DOWN:{
                        myInput = kInputDown;
                    }
                        break;
                    case SDLK_LEFT:{
                        myInput = kInputLeft;
                    }
                        break;
                    case SDLK_UP:{
                        myInput = kInputUp;
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
        ++mFrameNumber;
        
        //send packet even if no input was provided, use input value == 0
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
        
        sethightwo(&packet.extra,myInput);
        setlowtwo(&packet.extra,mFrameNumber);
        
        //send packet
        net.sendpacket(&packet);
        
        //push our input into the que
        InputForFrame iff ;
        iff.mInput = myInput;
        iff.mFrame = mFrameNumber;
        mMyInputQue.push_front(iff);
        

        
        
        //check if any of our packets need to be dequed
        
        InputForFrame inputToBeRenderedSelf = mMyInputQue.back();
        //time to render our input
        //printf("%d\n",mFrameNumber - inputToBeRenderedSelf.mFrame == INPUT_RENDER_DELAY);
        printf("current frame:%d:frame in deque:%d\n",mFrameNumber,inputToBeRenderedSelf.mFrame);
        if(mFrameNumber - inputToBeRenderedSelf.mFrame >= INPUT_RENDER_DELAY){
            printf("ready to pop our input:%d:%d\n",mFrameNumber,inputToBeRenderedSelf.mFrame);
            
            if(!mApponentInputQue.empty()){
                //check if the apponent's input is ready to be rendered
                
                InputForFrame inputToBeRenderedApp = mApponentInputQue.back();
                
                if(mFrameNumber - inputToBeRenderedApp.mFrame >= INPUT_RENDER_DELAY){
                    //ready to pop it and lock it
                    printf("ready to pop apponent input\n");
                    mMyInputQue.pop_back();
                    
                    mApponentInputQue.pop_back();
                    
                    mCurrentState.update(inputToBeRenderedSelf.mInput,inputToBeRenderedApp.mInput);
                    
                }else{
                    printf("apponent is not ready to pop\n");
                }
            }else{
                printf("apponent input que is empty?\n");
            }
            
            
        }
        
        /*
        //if ques are getting too big discard
        if(mApponentInputQue.size()>20){
            mApponentInputQue.pop_back();
        }
        if(mMyInputQue.size()>20){
            mMyInputQue.pop_back();
        }
         */
        if(mFrameNumber - mLastFrameFromApponent > INPUT_RENDER_DELAY){
            processing = false;
            
        }
    
    }

    
}

void Game::draw(SDL_Renderer* renderer){
    level.draw(renderer);
}
