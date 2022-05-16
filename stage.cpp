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
    
    /*
    uint16_t framecount = state->getframecount();
    uint32_t myscore = state->getself()->getscore();
    uint32_t apponentscore = state->getapponent()->getscore();
     */
    
    SDL_Rect rself = {myposition.x,myposition.y,TEXTUREWIDTH,TEXTUREHEIGHT};
    SDL_SetRenderDrawColor( renderer, 255, 161, 0, 255 );
    SDL_RenderFillRect( renderer, &rself );
    
    
    SDL_Rect rapponent = {apponent.x,apponent.y,TEXTUREWIDTH,TEXTUREHEIGHT};
    SDL_SetRenderDrawColor( renderer, 102, 191, 255, 255);
    SDL_RenderFillRect( renderer, &rapponent );
    
    SDL_Rect robjective = {objective.x,objective.y,TEXTUREWIDTH,TEXTUREHEIGHT};
    SDL_SetRenderDrawColor( renderer, 135, 60, 190, 255 );
    SDL_RenderFillRect( renderer, &robjective );
    
    SDL_RenderPresent(renderer);
}
