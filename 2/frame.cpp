
#undef NDEBUG
#include <assert.h>

#include <SDL2/SDL.h> // HEADER
#include "main.h"
#include "render.h"


void frame( float dt ){  // HEADER

    SDL_SetRenderDrawColor( renderer, 0,0,0, 255 );
    SDL_RenderClear( renderer );

    render();

//    {
//        const int barh = 3;
//        SDL_Rect r ={
//            .x = 0,
//            .y = WINH-barh,
//            .w = (int)(restart_timeout*WINW/restart_secs),
//            .h = barh,
//        };
//        SDL_SetRenderDrawColor( renderer, 255,255,255, 255 );
//        SDL_RenderFillRect( renderer, &r );
//    }
}


    