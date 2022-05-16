#ifndef _gamestate_h
#define _gamestate_h

#include <entity.h>

enum InGamePacketId
{
    kNothing = 0,
    kNoInput = 0,
    kSeed,
    kProvidedInput,
    kProvidedNoInput
};
#define TEXTUREWIDTH 32
#define TEXTUREHEIGHT 32
class GameState{
private:
    
    Entity me;
    Entity apponent;
    Vector2 objective;
    uint32_t randomseed;
    bool ishost;

    uint32_t rbound;
    uint32_t bbound;
    int timestep;
public:
    GameState(bool ishost);
    void update(uint16_t myinput,uint16_t apponentinput);
    Entity* getself();
    Entity* getapponent();
    uint32_t getrandomseed();
    void setrandomseed(uint32_t randomseed);
    const Vector2& getobjective();
    void setbound(uint32_t rbound, uint32_t bbound);
    void spawnobjective(); 
    bool iscollidingwithobjective(Vector2 position);
    void entityupdate(Entity* e);

    uint32_t xorshift32(uint32_t *state);
    
    bool gethost();
};
#endif
