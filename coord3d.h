#ifndef COORD3D_H
#define COORD3D_H


#include <math.h>

namespace PTools{

struct Coord3D
{
    double x,y,z;
    
    inline Coord3D & operator= (const Coord3D &);

};


inline Coord3D & Coord3D::operator= (const Coord3D & C)
{
    x = C.x;
    y = C.y;
    z = C.z;
    return *this;
}


inline Coord3D operator+ (const Coord3D& A,const Coord3D& B)
{
    Coord3D P;
    P.x = A.x + B.x ;
    P.y = A.y +  B.y ;
    P.z = A.z + B.z ;
    return P;
}

inline Coord3D operator- (const Coord3D& A,const Coord3D& B)
{
    Coord3D P;
    P.x = A.x - B.x ;
    P.y = A.y -  B.y ;
    P.z = A.z - B.z ;
    return P;
}

inline double Norm(const Coord3D & A)
{
    return sqrt (A.x*A.x + A.y*A.y + A.z*A.z) ;
}

inline double Norm2(const Coord3D & A)
{
    return  (A.x*A.x + A.y*A.y + A.z*A.z);
}

inline Coord3D operator* (const Coord3D& A, double scal)
{
    Coord3D P;
    P.x = A.x * scal ;
    P.y = A.y * scal ;
    P.z = A.z * scal ;
    return P;
}


inline Coord3D operator* (double scal, const Coord3D& A)
{
      return A * scal ; 
}



};

#endif //CORRD3D_H

