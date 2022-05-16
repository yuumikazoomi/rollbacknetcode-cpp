
#include <gamestate.h>
void draw(SDL_Renderer* renderer);
void update();

int main(int argc, char* argv[]) {
    
    SDL_Window *window;
    
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow(
                              "An SDL2 window",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              1080,
                              720,
                              0
                              );
    
    if (window == NULL) {
        return 1;
    }
    
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    
    bool alive = true;
    
    while (alive){
        SDL_Event event;
        while (SDL_PollEvent(&event)){
            switch (event.type){
                case SDL_QUIT:
                    alive = SDL_FALSE;
                    break;
            }
        }
        update();
        draw(renderer);
    }
    SDL_DestroyWindow(window);
    
    SDL_Quit();
    return 0;
}
void update(){
    
}
void draw(SDL_Renderer* renderer){
    SDL_SetRenderDrawColor( renderer, 245, 245, 245, 255 );
    SDL_RenderClear(renderer);
    
    
    SDL_Rect r = {50,50,50,50};
    SDL_SetRenderDrawColor( renderer, 255, 161, 0, 255 );
    SDL_RenderFillRect( renderer, &r );
    
    
    
    SDL_RenderPresent(renderer);
}
