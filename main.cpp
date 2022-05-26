
#include <game.h>
void draw(SDL_Renderer* renderer);
void update();
/*
SDL_Texture *IMG_LoadTexture(SDL_Renderer *renderer, const char *file)
{
    SDL_Texture *texture = NULL;
    SDL_Surface *surface = IMG_Load(file);
    if (surface) {
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }
    return texture;
}
 */
#ifdef NETINTERFACE_USING_WINDOWS
int __stdcall WinMain(HINSTANCE instance, HINSTANCE prev, LPSTR cmdline, int cmdshow)
#else
int main(int argc, char* argv[])
#endif
{
    
    /*
     *Change this depending on whether you're host or not
     */
    bool host = true;
    
    //if we provide a command line argument we can get assigned client
    //beats recompiling every time with host == true and host == false
#ifdef NETINTERFACE_USING_BSD
    if(argc>1){
        host = false;
    }
#endif
    
    Game g(host);
    
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
        
        g.update();
        g.draw(renderer);
        
    }
    
    SDL_DestroyWindow(window);
    SDL_Quit();
   
    return 0;
}

void update(){
    
}
void draw(SDL_Renderer* renderer){
    
}
