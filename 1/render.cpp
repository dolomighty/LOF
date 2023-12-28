
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




static std::vector<XY> XYs;
static std::vector<float> dens;





//static std::vector<float> calc_dens( std::vector<XY>& XYs ){
//    std::vector<float> den;
//    float max = -1E9;
//    for( int t=0; t<XYs.size(); t++ ){
//        float sum=0;
//        for( int o=0; o<XYs.size(); o++ ){
//            if(t==o)continue;
//            auto ot = t-o;
////            sum += (ot*ot)/(1+ot*ot); // [0,1)
////            sum += 1/(ot*ot);
//            sum += std::sqrt(ot*ot);
////            sum += ot*ot;
//        }
//        sum = sum/XYs.size();
////        sum = std::sqrt(sum)/XYs.size();
//        if(max<sum)max=sum;
////        den.push_back(sum/(1+sum)); // quasi-log
//        den.push_back(sum/max); // [0,1]
////        den.push_back(sum*1000); // [0,1]
//    }
//    return den;
//}



static std::vector<float> calc_dens( std::vector<XY>& XYs, float range ){
    const float rr = range*range;
    std::vector<float> den;
    float max = -1E9;
    for( int t=0; t<XYs.size(); t++ ){
        float sum=0;
        int n=1;
        for( int o=0; o<XYs.size(); o++ ){
            if(t==o)continue;
            auto ot = t-o;
            auto dd = ot*ot;
            if(dd>rr)continue;
            sum += dd;
            n++;
        }
//        sum = std::sqrt(sum);
//        sum /= n;
        if(max<sum)max=sum;
        den.push_back(sum); // [0,1]
    }
    // rescale
    for( int t=0; t<den.size(); t++ ){
        den[t]=den[t]/(1+den[t]);
    }
    return den;
}

static void thread_loop(){
    while(true){
        XYs = generate_xy();
        dens = calc_dens( XYs, 50 );
        std::this_thread::sleep_for(std::chrono::seconds( 2 ));
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



static float lerp( float a, float b, float t ){
    return a+(b-a)*t;
}

static float lerp_from_stops( const std::vector<float>& stops, float t ){
    // t = [0,1]
    t = std::max( 0.0f, std::min(t, 1.0f ));
    t *= stops.size();
    int a = (int)floor(t);
    int b = std::min((int)a+1, (int)stops.size()-1);
    return lerp( stops[a], stops[b], t-a );
}



void render(){ // HEADER

    run();

//    const int mac_palette[][3]={
//        { 255, 255, 255 },
//        { 252, 244, 4   },
//        { 255, 100, 4   },
//        { 220, 8,   8   },
//        { 240, 8,   132 },
//        { 72,  0,   164 },
//        { 0,   0,   212 },
//        { 0,   172, 232 },
//        { 32,  184, 20  },
//        { 0,   100, 16  },
//        { 88,  44,  4   },
//        { 144, 112, 56  },
//        { 192, 192, 192 },
//        { 128, 128, 128 },
//        { 64,  64,  64  },
//        { 0,   0,   0   },
//    };

    for( int i=0; i<dens.size(); i++ ){
        auto xy = XYs[i];
        auto d = dens[i]; // [0,1)
        const std::vector<float> r={ 0,   255, 255 };
        const std::vector<float> g={ 0,   255, 0   };
        const std::vector<float> b={ 255, 255, 0   };
//        const std::vector<float> r={ 255 };
//        const std::vector<float> g={ 255 };
//        const std::vector<float> b={ 255 };
        float rgb[3] = {
            lerp_from_stops( r, d ),
            lerp_from_stops( g, d ),
            lerp_from_stops( b, d ),
        };
        SDL_SetRenderDrawColor( renderer, rgb[0], rgb[1], rgb[2], 255 );
        dot(xy);
    }
}



