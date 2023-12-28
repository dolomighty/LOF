
#ifndef _XY_h_
#define _XY_h_

#undef NDEBUG
#include <assert.h>

#include <string>
#include <cmath>


struct XY {
    double x;
    double y;

    XY(){
        XY(0,0);
    }

    XY( double _x, double _y ){
        x = _x;
        y = _y;
    }

    double dot( const XY& other ) const {
        return x*other.x + y*other.y;
    }

    double operator * ( const XY& rhs ) const {
        return dot(rhs);
    }

    XY operator * ( double rhs ){
        return XY( x*rhs, y*rhs );
    }

    XY operator - ( const XY& rhs ) const {
        return XY( x-rhs.x, y-rhs.y );
    }

    XY operator + ( const XY& rhs ) const {
        return XY( x+rhs.x, y+rhs.y );
    }

    XY operator / ( double rhs ){
        return XY(x/rhs,y/rhs);
    }

    std::string hashgrid_key( int granularity ) const {
        assert( granularity >= 1 );
        int X = std::floor(x/granularity)*granularity;
        int Y = std::floor(y/granularity)*granularity;
        auto key = std::to_string(X)+" "+std::to_string(Y);
        return key;
    }

//    std::vector<std::string> hashgrid_keys( int granularity ) const {
//        assert( granularity >= 1 );
//
//        int X = std::floor(x/granularity)*granularity;
//        int Y = std::floor(y/granularity)*granularity;
//
//        std::vector<std::string> keys;
//
//        keys.push_back(std::to_string(X-1)+" "+std::to_string(Y-1));
//        keys.push_back(std::to_string(X+0)+" "+std::to_string(Y-1));
//        keys.push_back(std::to_string(X+1)+" "+std::to_string(Y-1));
//
//        keys.push_back(std::to_string(X-1)+" "+std::to_string(Y+0));
//        keys.push_back(std::to_string(X+0)+" "+std::to_string(Y+0));
//        keys.push_back(std::to_string(X+1)+" "+std::to_string(Y+0));
//
//        keys.push_back(std::to_string(X-1)+" "+std::to_string(Y+1));
//        keys.push_back(std::to_string(X+0)+" "+std::to_string(Y+1));
//        keys.push_back(std::to_string(X+1)+" "+std::to_string(Y+1));
//
//        return keys;
//    }
};



#endif // _XY_h_

