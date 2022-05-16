#ifndef _sage_h
#define _sage_h
#include <gamestate.h>
class Stage{
private:
    GameState* state;
public:
    Stage(GameState* state);
    void draw(SDL_Renderer* renderer);
};
#endif
