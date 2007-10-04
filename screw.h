#ifndef SCREW_H
#define SCREW_H

#include <string>
#include <sstream>

#include "coord3d.h"

namespace PTools{

struct Screw
{
    Coord3D transVector;  ///< the translation vector (because of the screw, this is also a vector director of the rotation axis).
    Coord3D point; ///< a point of the rotation axis (to generate the rotation axis)
    dbl angle; ///< angle of rotation

    /// returns a summary of the screw:
    std::string print()
        {
            std::stringstream result;
            result << "rotation axis: " << transVector.toString() ;
            result << "angle: " << angle << "\n" ;
            result << "translation norm: " <<  Norm(transVector) << "\n" ;
            result << "axis point: "  <<  point.toString();
            return result.str();
        } 
};


} //namespace PTools

#endif //SCREW_H


