#include <sstream>
#include "atom.h"
#include "coord3d.h"

using namespace std;

namespace PTools{
		
Coord3D Atom::GetCoords() const {return mCoords;};

string Atom::Print() const { 
	std::stringstream info;
	info<<GetAtomId()<<" "<<GetType()<<" ";
	info<<GetResidType()<<" "<<GetChainId()<<" "<<GetResidId();
	Coord3D coord = GetCoords();
	info<<PrintCoord(coord); 
	return info.str();
};


void Atom::Translate(const Coord3D& tr)
{
   mCoords=mCoords+tr;
}


}

