
#include <game.h>
bool operator<(const NIRelayPacket &a, const NIRelayPacket &b)
{
    uint16_t aframe = getlowtwo(a.extra);
    uint16_t bframe = getlowtwo(b.extra);
    return aframe < bframe;
}
Game::Game(bool host) : level(&mCurrentState){
    this->host = host;
    
    //set this to true once handshake and seeds are shared
    connected = false;
    
    memset(&mLastSyncInfo,0,sizeof(LastSyncInfo));
    memset(&peerinput,0,sizeof(NIRelayPacket));
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
            
            //initialise mLastSyncInfo to the current game state as host
            memcpy(&mLastSyncInfo.mState,&mCurrentState,sizeof(GameState));
            
            processing = true;
            connected = true;
        }
            break;
        case kSeed:{
            
            
            uint32_t seed = (uint32_t)packet.extra;
            mCurrentState.setrandomseed(seed);
            
            //spawn the objective
            mCurrentState.spawnobjective();
            
            //initialise mLastSyncInfo to the current game state as client
            memcpy(&mLastSyncInfo.mState,&mCurrentState,sizeof(GameState));
            
            
            processing = true;
            connected = true;
        }
            break;
        case kProvidedInput:{
            //ave the peers packet so later in the frame we can grab their input and framenumber
            memcpy(&this->peerinput,&packet,sizeof(NIRelayPacket));
            
            uint16_t tframe = getlowtwo(peerinput.extra);
            uint16_t gframe = mLastSyncInfo.mState.getframenumber();
            if(tframe<gframe){
                //ignore this packet, it probably arrived late
            }else if (tframe == gframe){
                //we got a frame match, do a rollback
                uint16_t tinput = gethightwo(peerinput.extra);
                
                rollback(tinput,tframe);
            }else{
                
                //check if the list contains a frame that matches gframe
                for(const NIRelayPacket& inlist : packetlist){
                    uint16_t inlistframe = getlowtwo(inlist.extra);
                    uint16_t inlistinput = gethightwo(inlist.extra);
                    if(inlistframe == gframe){
                        rollback(inlistframe,inlistinput);
                        //remove this packet from the list
                        packetlist.erase(inlist);
                        break;
                    }
                }
                
                //add our current arrived packet to the list
                packetlist.insert(peerinput);
               // printf("packetlist size:%d\n",packetlist.size());
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
        sethightwo(&packet.extra,myinput);
        setlowtwo(&packet.extra,mCurrentState.getframenumber());
        
        //send packet
        net.sendpacket(&packet);
        
        //put our own input into double ended que
        mPrevLocalInputs.push_front(myinput);
        
        if(mPrevLocalInputs.size() > 30){//30 local inputs is a safe bet
            
            mPrevLocalInputs.pop_back();
        }

        
        
        /*
         *            four byte packet
         *
         *        high              low
         *      two bytes         two bytes
         *        input             frame
         * | - - - - - - - - | - - - - - - - - |
         */
        //get the peer's packet we took earlier from network callback and
        //grab input and frame
        uint16_t apponentframe = getlowtwo(peerinput.extra);
        uint16_t apponentinput = gethightwo(peerinput.extra);
        
        //update gamestate with both our input and peer's input
        mCurrentState.update(myinput,apponentinput);
        
        
        /*
        //perform rollback?
        if(peerinput.packettype == kProvidedInput){
            //we don't need to process a frame if apponent frame is behind the sync frame
            if(apponentframe==mLastSyncInfo.mState.getframenumber()){
            
                rollback(apponentinput,apponentframe);
            }
            
        }
         */
        

    }
    if (mCurrentState.getframenumber() - mLastSyncInfo.mState.getframenumber() > MAX_SYNC_DRIFT){
        processing = false;
    }else if(connected){
        processing = true;
    }
}
void Game::rollback(uint16_t input, uint16_t targetframe)
{
    //This input must be for the last sync state. E.g. if the last known enemy input was on 26
    // then the last sync state is frame 27. So we need the input for frame 27 to continue.
    
    //corrected
    //C_ASSERT(targetframe == mLastSyncInfo.mState.getframenumber());
    
    assert(targetframe == mLastSyncInfo.mState.getframenumber());

    //Calculate diff
    
    
    //size_t currentFrame = mCurrentState.mCurrentFrame;
    
    //corrected
    uint16_t currentFrame = mCurrentState.getframenumber();
    
    //Rollback our state
    
    //mCurrentState = mLastSyncInfo.mState;
    
    //corrected
    memcpy(&mCurrentState,&mLastSyncInfo.mState,sizeof(GameState));
    
    
    bool firstUpdateInLoop = true;

    //Rollforward with the new info
    while (mCurrentState.getframenumber() < currentFrame)
    {
        
        
        //I believe you meant to write "input" from the argument above
        //let me make an adjustment
        
        //mCurrentState.update(mPrevLocalInputs[currentFrame - mCurrentState.mCurrentFrame], direction);
        //corrected
        
        //this line is broken and causes a crash
        mCurrentState.update(mPrevLocalInputs.at(currentFrame - mCurrentState.getframenumber()-1), input);
        
        
        if (firstUpdateInLoop)
        {
            //After the first roll forward, update sync state
            //memcpy instead
            
            //mLastSyncInfo.mState = mCurrentState;
            //corrected
            memcpy(&mLastSyncInfo.mState,&mCurrentState,sizeof(GameState));
            
            //ambiguity use Input instead of Direction
            
            //mLastSyncInfo.mOpponentDirection = direction;
            //corrected
            mLastSyncInfo.mOpponentInput = input;
            
            firstUpdateInLoop = false;
        }
    }
}
void Game::draw(SDL_Renderer* renderer){
    level.draw(renderer);
}
