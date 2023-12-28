
#undef NDEBUG
#include <assert.h>

#include <SDL2/SDL.h> // HEADER
#include "main.h"
#include "render.h"


void frame( SDL_Event event, float dt ){  // HEADER

    SDL_SetRenderDrawColor( renderer, 0,0,0, 255 );
    SDL_RenderClear( renderer );

    render();
}


