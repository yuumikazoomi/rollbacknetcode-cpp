#ifndef _game_h
#define _game_h
#include <stage.h>

#define MAX_SYNC_DRIFT 10

typedef struct SLastSyncInfo
{
    GameState mState;
    //mApponentDirection is ambiguous use mApponentInput instead
    //uint16_t mOpponentDirection;
    
    uint16_t mOpponentInput;
}LastSyncInfo;

class Game{
private:
    Stage level;
    
    NetInterface net;
    
    
    bool host;
    
    
    bool processing;
    
    //last packed arrived by the peer just so we can update it in our update function
    NIRelayPacket peerinput;
    
    
    //double ended queue for previous local inputs
    std::deque<uint16_t> mPrevLocalInputs;
    
    //last synced state
    LastSyncInfo mLastSyncInfo;
    
    
    GameState mCurrentState;

    std::set<NIRelayPacket> packetlist;
public:
        
    
    Game(bool host);
    void update();
    void draw(SDL_Renderer* renderer);
    
    
private:
    void handlepacket(const NIRelayPacket& ppacket, NITransferSize size);
    
    void rollback(uint16_t direction, uint16_t targetframe);
};
#endif
