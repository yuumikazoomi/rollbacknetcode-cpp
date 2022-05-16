
#include <game.h>
void draw(SDL_Renderer* renderer);
void update();


bool host;
Game* g ;
#ifdef NETINTERFACE_USING_WINDOWS
int __stdcall WinMain(HINSTANCE instance, HINSTANCE prev, LPSTR cmdline, int cmdshow)
#else
int main()
#endif
{
    
    /*
     *Change this depending on whether you're host or not
     */

    host = false;
    
    
    g = new Game(host);
    
    SDL_Init(SDL_INIT_VIDEO);
    
    
    SDL_Window *window = SDL_CreateWindow(
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
        
        update();
        draw(renderer);
    }
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

void update(){
    g->update();
}
void draw(SDL_Renderer* renderer){
    g->draw(renderer);
}
