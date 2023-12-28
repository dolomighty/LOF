
#ifndef _XY_h_
#define _XY_h_

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

    double dot( XY other ){
        return x*other.x + y*other.y;
    }

    double operator * ( XY rhs ){
        return dot(rhs);
    }

    XY operator * ( double rhs ){
        return XY( x*rhs, y*rhs );
    }

    XY operator - ( const XY& rhs ) const {
        return XY( x-rhs.x, y-rhs.y );
    }

    XY operator + ( XY rhs ){
        return XY( x+rhs.x, y+rhs.y );
    }

    XY operator / ( double rhs ){
        return XY(x/rhs,y/rhs);
    }
};



#endif // _XY_h_

