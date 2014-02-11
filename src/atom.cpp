#include <sstream>
#include <stdio.h>

#include "atom.h"
#include "coord3d.h"

using namespace std;

namespace PTools{


//! Convert an atom to a string
std::string Atom::ToString() const {
    std::stringstream info;
    info << atomId << " " << atomType << " ";
    info << residType << " " << chainId << " " << residId;
    info << PrintCoord(coords);
    return info.str();
}


//! convert an atom to a string in PDB format
std::string Atom::ToPdbString() const
{
    char output[81];
    const char* atomname = this->atomType.c_str();
    const char* residName = this->residType.c_str();
    int residnumber = this->residId;
    const char* chainID = this->chainId.c_str();

    int atomnumber = atomId;

    double x = coords.x;
    double y = coords.y;
    double z = coords.z ;

    snprintf(output,80,"ATOM  %5d %-4s %3s %1s%4d    %8.3f%8.3f%8.3f%s\0",
	     atomnumber,atomname,residName,chainID,residnumber,x,y,z,extra.c_str());
//     output[79]='\n';
//     output[80]='\0';
    return std::string(output);
}

//! translate an atom with a Coord3D vector
void Atom::Translate(const Coord3D& tr)
{
    coords = coords+tr;
}


} // namespace PTools

