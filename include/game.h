#ifndef _game_h
#define _game_h
#include <stage.h>
typedef struct SLastSyncInfo
{
    GameState mState;
    size_t mFrameNumber;
    uint16_t mOpponentDirection;
}LastSyncInfo;

class Game{
private:
    Stage level;
    GameState state;
    NetInterface net;
    bool host;
    bool processing;
    
    //current frame count
    uint16_t currentframe;
    
    //last packed arrived by the peer just so we can update it in our update function
    NIRelayPacket peerinput;
    
    
    
public:
    Game(bool host);
    void update();
    void draw(SDL_Renderer* renderer);
private:
    void handlepacket(const NIRelayPacket& ppacket, NITransferSize size);
};
#endif
