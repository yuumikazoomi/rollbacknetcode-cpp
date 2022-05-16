
#include <gamestate.h>

int main(int argc, char* argv[]) {

    SDL_Window *window;

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(
        "An SDL2 window",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        1080,
        720,
        SDL_WINDOW_OPENGL
    );

    // Check that the window was successfully created
    if (window == NULL) {
        return 1;
    }
    
    SDL_Event event;
    int quit = 0;
    while( !quit ){

        /* Poll for events */
        while( SDL_PollEvent( &event ) ){
            
            switch( event.type ){
                /* Keyboard event */
                /* Pass the event data onto PrintKeyInfo() */
                case SDL_KEYDOWN:
                case SDL_KEYUP:

                    break;

                /* SDL_QUIT event (window close) */
                case SDL_QUIT:
                    quit = 1;
                    break;

                default:
                    break;
            }

        }

    }
    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
}
