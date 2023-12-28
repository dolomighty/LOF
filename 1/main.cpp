
#undef NDEBUG
#include <assert.h>

#include <stdlib.h>
#include <unistd.h>
#include <SDL2/SDL.h>   // HEADER
#include "loop.h"
#include <time.h>


#define WINW 600    // HEADER
#define WINH 600    // HEADER


SDL_Renderer* renderer;     // HEADER


int main( int argc, char* argv[] ){ 

    char* basename = strrchr(argv[0],'/')+1;
    srand(time(0));

    SDL_Window* win;
    assert( win = 
        SDL_CreateWindow( basename, 
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
                WINW, WINH, 0 ));

    assert( renderer = SDL_CreateRenderer( win, -1,
        SDL_RENDERER_PRESENTVSYNC  |
        SDL_RENDERER_TARGETTEXTURE
    ));

    loop();
    
    SDL_DestroyRenderer( renderer );
    SDL_DestroyWindow( win );
    
    return 0;
}

