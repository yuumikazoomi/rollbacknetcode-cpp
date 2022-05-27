#ifndef _gamestate_h
#define _gamestate_h

#include <entity.h>



#define TEXTUREWIDTH 32
#define TEXTUREHEIGHT 32



enum InGamePacketId
{
    kNothing = 0,
    kNoInput = 0,
    kSeed,
    kProvidedInput,
    kProvidedNoInput
};


class GameState{

private:
    uint16_t mFrameNumber;
    
    Entity me;
    Entity apponent;
    Vector2 objective;
    uint32_t randomseed;
    
    uint32_t rbound;
    uint32_t bbound;
    float timestep;
public:
    GameState();
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
    
    uint16_t getframenumber();

    void generaterandomseed();
};
#endif
