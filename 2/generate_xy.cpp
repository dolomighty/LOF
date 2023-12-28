
#undef NDEBUG
#include <assert.h>

#include <stdlib.h>
#include "main.h"
#include "XY.h" // HEADER
#include <vector> // HEADER


static float unirand(){
    // return [0,1]
    return rand()*1.0/RAND_MAX;
}

static float biporand(){
    // return [-1,1]
    return rand()*2.0/RAND_MAX-1.0;
}


// c++20 ha la lerp
static float lerp( float a, float b, float x ){
    return a+(b-a)*x;
}


static float randgauss(){
    return (biporand()+biporand()+biporand()+biporand()+biporand())/5.0;
}


static XY gaussian_xy( XY center, float radius ){
    return center+XY(randgauss(),randgauss())*radius;
}







std::vector<XY> generate_xy(){  // HEADER

    std::vector<XY> xys;
    const int clusters = 3;

    for( int i=0; i<clusters; i++ ){   

        auto center = XY(
            lerp( WINW*1/10, WINW*9/10, unirand()),
            lerp( WINH*1/10, WINH*9/10, unirand())
        );

        auto radius = lerp( WINH/10, WINH/2, unirand());

        for( int j=0; j<100; j++ ){
            xys.push_back( gaussian_xy( center, radius ));
        }
    }


    const int noise = 50;
    for( int i=0; i<noise; i++ ){
        xys.push_back( XY(
            lerp( 0, WINW, unirand()),
            lerp( 0, WINH, unirand())
        ));
    }

    return xys;
}





