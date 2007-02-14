#include "coord3d.h"


namespace PTools{

Coord3D& Coord3D::Normalize(){
    double norm = Norm(*this);
    x = x / norm;
    y = y / norm;
    z = z / norm; 
    return *this;    
    }

}

