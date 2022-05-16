#ifndef _game_h
#define _game_h
#include <stage.h>
class Game{
private:
    Stage level;
    GameState state;
    NetInterface net;
    bool host;
    bool processing;
public:
    Game(bool host);
    void update();
    void draw(SDL_Renderer* renderer);
private:
    void handlepacket(const NIRelayPacket& ppacket, NITransferSize size);
};
#endif
