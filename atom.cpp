#include <sstream>
#include <stdio.h>

#include "atom.h"
#include "coord3d.h"

using namespace std;

namespace PTools{

Coord3D Atom::GetCoords() const {return mCoords;};

std::string Atom::ToString() const {
    std::stringstream info;
    info<<GetAtomId()<<" "<<GetType()<<" ";
    info<<GetResidType()<<" "<<GetChainId()<<" "<<GetResidId();
    Coord3D coord = GetCoords();
    info<<PrintCoord(coord);
    return info.str();
};



std::string Atom::ToPdbString() const
{
    char output[80];
    const char* atomname = GetType().c_str();
    const char* residName = GetResidType().c_str();
    int residnumber = GetResidId();
    const char* chainID = GetChainId().c_str();

    int atomnumber = GetAtomId();

    Coord3D coord = GetCoords();
    double x = coord.x;
    double y = coord.y;
    double z = coord.z ;

    snprintf(output,80,"ATOM  %5d  %-4s%3s %1s%4d    %8.3f%8.3f%8.3f\n",atomnumber,atomname,residName,chainID,residnumber,x,y,z);
    return (std::string) output;
}


void Atom::Translate(const Coord3D& tr)
{
    mCoords=mCoords+tr;
}


} // namespace PTools

