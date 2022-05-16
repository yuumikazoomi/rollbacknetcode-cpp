#include <stage.h>

Stage::Stage(GameState* state){
    this->state = state;
}
void Stage::draw(SDL_Renderer* renderer){
    SDL_SetRenderDrawColor( renderer, 245, 245, 245, 255 );
    SDL_RenderClear(renderer);
    
    
    SDL_Rect r = {50,50,50,50};
    SDL_SetRenderDrawColor( renderer, 255, 161, 0, 255 );
    SDL_RenderFillRect( renderer, &r );
    
    SDL_RenderPresent(renderer);
}
