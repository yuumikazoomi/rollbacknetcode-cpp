 #include <gamestate.h>

GameState::GameState(bool ishost){
    this->ishost = ishost;
    randomseed = 0;
    if (ishost){
        randomseed = std::random_device()();
    }
    rbound = 1080;
    bbound = 720;
    objective.x = 0;
    objective.y = 0;

    timestep = 1; //fixed timestep
}
//simple 2d box collision check
bool GameState::iscollidingwithobjective(Vector2 position){
    Vector2 rect1 = position;
    Vector2 rect2 = objective;
    if (rect1.x < rect2.x + TEXTUREWIDTH &&
        rect1.x + TEXTUREWIDTH > rect2.x &&
        rect1.y < rect2.y + TEXTUREHEIGHT &&
        TEXTUREHEIGHT + rect1.y > rect2.y)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void GameState::spawnobjective()
{
    uint16_t x = xorshift32(&randomseed);
    uint16_t y = xorshift32(&randomseed);
    x %= rbound - TEXTUREWIDTH;
    y %= bbound - TEXTUREHEIGHT;
    objective.x = x;
    objective.y = y;
}

void GameState::update(uint16_t myinput,uint16_t apponentinput){
    
    //update our position based on input provided
    me.update(myinput,timestep);
    apponent.update(apponentinput,timestep);
    
    //get our position and check if we're touching objective
    Vector2 epos = me.getposition();
    if (iscollidingwithobjective(epos)){
        me.incrementscore();
        spawnobjective();
    }
    
    //get apponent position and check if we're touching objective
    epos = apponent.getposition();
    if (iscollidingwithobjective(epos)){
        apponent.incrementscore();
        spawnobjective();
    }
}

void GameState::rollback(uint16_t direction, uint16_t targetframe)
{
    
}

Entity *GameState::getapponent(){
    return &apponent;
}
Entity *GameState::getself(){
    return &me;
}
uint32_t GameState::getrandomseed(){
    return randomseed;
}
void GameState::setrandomseed(uint32_t randomseed){
    this->randomseed = randomseed;
}
const Vector2 &GameState::getobjective(){
    return objective;
}
void GameState::setbound(uint32_t rbound, uint32_t bbound){
    this->rbound = rbound;
    this->bbound = bbound;
}

uint32_t GameState::xorshift32(uint32_t *state){
    /* Algorithm "xor" from p. 4 of Marsaglia, "Xorshift RNGs" */
    uint32_t x = *state;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    *state = x;
    return x;
}
bool GameState::gethost(){
    return ishost;
}
