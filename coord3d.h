#ifndef COORD3D_H
#define COORD3D_H


#include <math.h>
#include <string>

using namespace std;

namespace PTools{

struct Coord3D
{
    double x,y,z;
 
    Coord3D() {x=0; y=0;z=0;};
    Coord3D(double nx, double ny, double nz){x=nx;y=ny;z=nz;};

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


inline Coord3D operator* (double scal, const Coord3D& A) {
	return A * scal ; 
}

inline string PrintCoord(const Coord3D& A) { ///< print coordinates
	int size=100;
	char info[size];
	snprintf(info, size, "%8.3f %8.3f %8.3f", A.x, A.y, A.z);
	return info;
}


};

#endif //CORRD3D_H

