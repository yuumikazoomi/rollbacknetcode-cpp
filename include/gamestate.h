#ifndef _gamestate_h
#define _gamestate_h

#include <entity.h>

enum InGamePacketId
{
    kNothing = 0,
    kWin = 1,
    kLose = 2,
    kPause = 3,
    kResume = 4,
    kReserved = 5,
    kMove = 6,
    kRejection = 7,
    kSeed = 8,
    kNoInput = 9,
};
typedef struct SOutOfOrderInputStorage{
    uint16_t input;
}OutOfOrderInputStorage;
typedef struct SLastInputFrame{
    uint16_t localframe;
    uint16_t targetframe;
    uint16_t input;
}LastInputFrame;

typedef struct SEntityState{
    Vector2 position;
    uint16_t direction;
    uint16_t score;
}EntityState;

typedef struct SGameStateAbstract{
    EntityState self;
    EntityState apponent;
    Vector2 objective;
    uint32_t randomseed;
}GameStateAbstract;

#define TEXTUREWIDTH 32
#define TEXTUREHEIGHT 32
class GameState{
private:
//rollback relevangt
    Entity me;
    Entity apponent;
    Vector2 objective;
    uint32_t randomseed;
    GameStateAbstract lastsync;
    bool ishost;

//not rollback relevent
    uint32_t rbound;
    uint32_t bbound;
    LastInputFrame lif;
    int timestep;
    
    std::deque<std::shared_ptr<GameStateAbstract> > statestack;
public:
    GameState(bool ishost);
    void update();
    void updatedirection(Entity* e, uint16_t direction,uint16_t localframe,uint16_t targetframe);
    Entity* getself();
    Entity* getapponent();
    uint32_t getrandomseed();
    void setrandomseed(uint32_t randomseed);
    const Vector2& getobjective();
    void setbound(uint32_t rbound, uint32_t bbound);
    void spawnobjective(); 
    bool iscollidingwithobjective(Vector2 position);
    const LastInputFrame& getlastinputframe();
    void entityupdate(Entity* e);

    const GameStateAbstract& getlastsyncstate();
    void rollback(uint16_t direction,uint16_t localframe, uint16_t targetframe);
    uint32_t xorshift32(uint32_t *state);
};
#endif
