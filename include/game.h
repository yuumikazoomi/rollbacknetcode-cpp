#ifndef _game_h
#define _game_h
#include <stage.h>

#define INPUT_RENDER_DELAY 3

typedef struct SLastSyncInfo
{
    GameState mState;
    //mApponentDirection is ambiguous use mApponentInput instead
    //uint16_t mOpponentDirection;
    
    uint16_t mOpponentInput;
}LastSyncInfo;

typedef struct SInputForFrame{
    uint16_t mInput;
    uint16_t mFrame;
}InputForFrame;


class Game{
private:
    Stage level;
    
    NetInterface net;
    
    
    bool host;
    
    
    bool processing;
    
    //once handshake and seeds are shared we set this to true
    bool connected;
    
    GameState mCurrentState;
    
    std::deque<InputForFrame> mMyInputQue;
    std::deque<InputForFrame> mApponentInputQue;
    
    
    uint16_t mFrameNumber;
    
    uint16_t mLastFrameFromApponent;
public:
        
    
    Game(bool host);
    void update();
    void draw(SDL_Renderer* renderer);
    
    
private:
    void handlepacket(const NIRelayPacket& ppacket, NITransferSize size);

};
#endif
