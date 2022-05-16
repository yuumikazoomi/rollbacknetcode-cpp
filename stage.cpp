#include <stage.h>

Stage::Stage(GameState* state){
    this->state = state;
}
void Stage::draw(SDL_Renderer* renderer){
    SDL_SetRenderDrawColor( renderer, 245, 245, 245, 255 );
    SDL_RenderClear(renderer);
    
    Vector2 myposition = state->getself()->getposition();
    Vector2 apponent = state->getapponent()->getposition();
    Vector2 objective = state->getobjective();
    uint16_t framecount = state->getframecount();
    uint32_t myscore = state->getself()->getscore();
    uint32_t apponentscore = state->getapponent()->getscore();

    SDL_Rect r = {myposition.x,myposition.y,TEXTUREWIDTH,TEXTUREHEIGHT};
    SDL_SetRenderDrawColor( renderer, 255, 161, 0, 255 );
    SDL_RenderFillRect( renderer, &r );
    
    SDL_RenderPresent(renderer);
}
