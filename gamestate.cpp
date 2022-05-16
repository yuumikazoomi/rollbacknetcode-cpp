#include <gamestate.h>

GameState::GameState(bool ishost){
    this->ishost = ishost;
    randomseed = 0;
    if (ishost){
        randomseed = std::random_device()();
    }
    rbound = 0;
    bbound = 0;
    objective.x = 0;
    objective.y = 0;
    memset(&lif, 0, sizeof(LastInputFrame));

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

void GameState::entityupdate(Entity *e)
{
    e->update(timestep);
    Vector2 epos = e->getposition();
    if (iscollidingwithobjective(epos)){
        e->incrementscore();
        spawnobjective();
    }

    if (epos.x < 0){
        e->updatedirection(kDirectionRight);
    }
    else if (epos.x > rbound){
        e->updatedirection(kDirectionLeft);
    }
    else if (epos.y < 0){
        e->updatedirection(kDirectionDown);
    }
    else if (epos.y > bbound){
        e->updatedirection(kDirectionUp);
    }
}

void GameState::update(){
    entityupdate(&me);
    entityupdate(&apponent);

    //create a state packet for the frame
    std::shared_ptr<GameStateAbstract> state = std::make_shared<GameStateAbstract>();
    state->apponent.direction = apponent.getdirection();
    state->apponent.position = apponent.getposition();
    state->apponent.score = apponent.getscore();

    state->self.direction = me.getdirection();
    state->self.position = me.getposition();
    state->self.score = me.getscore();

    state->randomseed = randomseed;

    statestack.push_back(state);

    if (statestack.size() > 10){ //max states{
        statestack.pop_front();
    }
}
void GameState::rollback(uint16_t direction, uint16_t localframe, uint16_t targetframe)
{
    std::deque<std::shared_ptr<GameStateAbstract> >::iterator it = statestack.end();
    uint16_t diff = localframe - targetframe;
    if (diff > 10){
        //not sure what to do here
    }
    else{
        diff *= -1;             //just to go back in the stack (-diff)
        std::advance(it, diff); //go back x frames


        /*
        * 
        *Not sure what to do here 
        */
        //now perform rollback?
        uint16_t predicteddirection = direction;
        while (it != statestack.end()){
            //assign the random seed from that state
            randomseed = it->get()->randomseed;

            //set the objective position
            objective = it->get()->objective;

            //overwrite myself
            EntityState mystate = it->get()->apponent;
            me.updatedirection(mystate.direction);
            me.setscore(mystate.score);
            me.forcesetposition(mystate.position);

            //overwrite apponent state
            EntityState apponentstate = it->get()->apponent;


            //force set the predicted direction for the next x frames

            apponent.updatedirection(predicteddirection);
            apponent.setscore(apponentstate.score);
            apponent.forcesetposition(apponentstate.position);

            //perform game update with these states
            
            entityupdate(&me);
            entityupdate(&apponent);

            //save the states again once the update has been done
            it->get()->randomseed = randomseed;
            it->get()->objective = objective;


            it->get()->self.direction = me.getdirection();
            it->get()->self.position = me.getposition();
            it->get()->self.score = me.getscore();

            it->get()->apponent.direction = apponent.getdirection();
            it->get()->apponent.position = apponent.getposition();
            it->get()->apponent.score = apponent.getscore();



            std::shared_ptr<GameStateAbstract> sgsa = *it;
            ++it;

        }
    }
}

//this function gets called either from the network or our own gamelogic
//if its called from ourself then e==me
//else e==apponent
/*
*localframe == our frame
*targetframe == frame of peer when they hit input
*/
void GameState::updatedirection(Entity *e, uint16_t direction, uint16_t localframe, uint16_t targetframe){

    //if arriving frame doesn't  match our frame perform rollback

    e->updatedirection(direction);

    //we don't need to rollback our own input
    if (e == &apponent){

        //used for displaying on the level
        lif.input = direction;
        lif.targetframe = targetframe;
        lif.localframe = localframe;

        if (localframe > targetframe){
            rollback(direction, localframe, targetframe);
            //perform rollback
        }
    }
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

const LastInputFrame &GameState::getlastinputframe(){
    return lif;
}
const GameStateAbstract &GameState::getlastsyncstate(){
    return lastsync;
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
