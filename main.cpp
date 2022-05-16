
#include <game.h>
void draw(SDL_Renderer* renderer);
void update();


bool host;
Game* g ;
int main(int argc, char* argv[]) {
    
    
    /*
     *Change this depending on whether you're host or not
     */

    host = true;
    
    
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
    /*
    auto netcallback = [](const NIRelayPacket& packet,NITransferSize size,bool& availabledata,bool& error){
        if(availabledata){
            printf("we got data!\n");
        }else if(error){
            printf("we got an error!\n");
        }
    };
    netcode->poll(netcallback);
     */
}
void draw(SDL_Renderer* renderer){
    g->draw(renderer);
}
