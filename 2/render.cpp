
#undef NDEBUG
#include <assert.h>

#include "XY.h"
#include "generate_xy.h"
#include <stdlib.h>
#include <vector>
#include <SDL2/SDL.h>
#include "main.h"
#include <thread>
#include <chrono>
#include <cstdio>
#include "LOF.h"
#include "loop.h"
#include <mutex>




static std::vector<XY> XYs;
static auto lof = LOF( XYs );
static std::mutex mutex;


static void thread_loop(){
    while(true){
        mutex.lock();
            XYs = generate_xy();
            lof.run( XYs );
        mutex.unlock();
        std::this_thread::sleep_for(std::chrono::seconds( 10 ));
//        std::this_thread::sleep_for(std::chrono::milliseconds( 10 ));
    }
}




static void run(){
    static bool started;
    if(started)return;
    static std::thread helper(thread_loop);
    started=true;
}




static void dot( XY xy, int radius=3 ){
    SDL_Rect r ={
        .x = (int)(xy.x-(radius/2)),
        .y = (int)(xy.y-(radius/2)),
        .w = radius,
        .h = radius,
    };
    SDL_RenderFillRect( renderer, &r );
}




static bool mouse_inside( const XY& xy, float r ){
    auto scarto = xy-XY(mouse_x,mouse_y);
    return (scarto*scarto < r*r);
}




static double lerp( double a, double b, double t ){
    return a+(b-a)*t;
}

double lerp( const std::vector<int>& vals, double t ){
    // t = [0,1] interpola, altrimenti estrapola
    t *= vals.size();
    int a = floor(t); // t=5.3 → a=5
    t -= a; // 5.3 - 5 = 0.3
    int b = a+1;
    a = std::max( 0, std::min( (int)vals.size()-1, a ));
    b = std::max( 0, std::min( (int)vals.size()-1, b ));
    return lerp( vals[a], vals[b], t );
}





void render(){ // HEADER

    run();

    const int mac_palette[][3]={
        { 255, 255, 255 }, // 0
        { 252, 244, 4   }, // 1
        { 255, 100, 4   }, // 2
        { 220, 8,   8   }, // 3
        { 240, 8,   132 }, // 4
        { 72,  0,   164 }, // 5
        { 0,   0,   212 }, // 6
        { 0,   172, 232 }, // 7
        { 32,  184, 20  }, // 8
        { 0,   100, 16  }, // 9
        { 88,  44,  4   }, // 10
        { 144, 112, 56  }, // 11
        { 192, 192, 192 }, // 12
        { 128, 128, 128 }, // 13
        { 64,  64,  64  }, // 14
        { 0,   0,   0   }, // 15
    };



    
    // snapshot, se il thread non la sta aggiornando

    static auto local_XYs  = XYs;
    static auto dens_grid  = lof.grid();
    static auto dens_max   = lof.dens_max();
    static auto grid_granu = lof.grid_granu();
    static auto lof_knn    = lof.knn();
    static auto lof_lof    = lof.lof();
    static auto lof_max    = lof.lof_max();

    if(mutex.try_lock()){
        local_XYs  = XYs;
        dens_grid  = lof.grid();
        dens_max   = lof.dens_max();
        grid_granu = lof.grid_granu();
        lof_knn    = lof.knn();
        lof_lof    = lof.lof();
        lof_max    = lof.lof_max();
        mutex.unlock();
    }





//    // heatmap
//    for( auto [key,val]: dens_grid ){
//        // key = "123 456"
//        int x,y;
//        std::sscanf(key.c_str(),"%d %d",&x,&y);
//
//        float heat = (float)val.size()/dens_max;
//        if(heat<0)heat=0; else
//        if(heat>1)heat=1;
//
//        int rgb[3] = {
//            (int)lerp( 0, 255, heat ),
//            (int)lerp( 0, 128, heat ),
//            (int)lerp( 0, 64,  heat ),
//        };
//        SDL_SetRenderDrawColor( renderer, rgb[0], rgb[1], rgb[2], 255 );
//
//        SDL_Rect r ={
//            .x = x,
//            .y = y,
//            .w = grid_granu,
//            .h = grid_granu,
//        };
//        SDL_RenderFillRect( renderer, &r );
//    }
//
//
//
//
//    int over = -1;
//
//    for( int i=0; i<local_XYs.size(); i++ ){
//        auto& xy = local_XYs[i];
//        auto rgb = mac_palette[0];
//        SDL_SetRenderDrawColor( renderer, rgb[0], rgb[1], rgb[2], 255 );
//        dot(xy);
//        if(mouse_inside(xy,10))over=i;
//    }
//
//    if(over>=0){
//        auto xy = local_XYs[over];
//        auto rgb = mac_palette[3];
////        SDL_SetRenderDrawColor( renderer, rgb[0], rgb[1], rgb[2], 255 );
//        dot(xy,6);
//
//        rgb = mac_palette[5];
////        SDL_SetRenderDrawColor( renderer, rgb[0], rgb[1], rgb[2], 255 );
//        auto ids = lof_knn[over];
//        for( auto i: ids ){
//            auto xy = local_XYs[i];
//            dot(xy,6);
//        }
//    }


    int over = -1;

    for( int i=0; i<local_XYs.size(); i++ ){
        auto& xy = local_XYs[i];

        if(lof_lof[i]<1){
            // range [0,1)
            double t = lof_lof[i];
//            // per il .svg vedi mk-rgb-const.sh
//            #include "dyn/rgb-inlier.h" // SHELL ./mk-rgb-const.sh colors-inlier.svg
            const auto R = std::vector<int>({ 0,   255 });
            const auto G = std::vector<int>({ 0,   255 });
            const auto B = std::vector<int>({ 255, 255 });
            SDL_SetRenderDrawColor( renderer, lerp(R,t), lerp(G,t), lerp(B,t), 255 );
        } else {
            // range [1,lof_max)
            double t = lof_lof[i]/lof_max;
//            // per il .svg vedi mk-rgb-const.sh
//            #include "dyn/rgb-outlier.h" // SHELL ./mk-rgb-const.sh colors-outlier.svg
            const auto R = std::vector<int>({ 255, 255 });
            const auto G = std::vector<int>({ 255, 0   });
            const auto B = std::vector<int>({ 255, 0   });
            SDL_SetRenderDrawColor( renderer, lerp(R,t), lerp(G,t), lerp(B,t), 255 );
        }


        dot(xy);
        if(mouse_inside(xy,10))over=i;
    }

    if(over>=0){
        SDL_SetRenderDrawColor( renderer, 255,255,255, 255 );
        auto xy = local_XYs[over];
        dot(xy,6);
        auto ids = lof_knn[over];
        for( auto i: ids ){
            auto xy = local_XYs[i];
            dot(xy,6);
        }
    }
}


