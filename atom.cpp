#include <sstream>
#include "atom.h"
#include "coord3d.h"
#include "rigidbody.h"

namespace PTools{

Coord3D Atom::GetCoords() const {return mCoords;};

std::string Atom::Print() {
	std::stringstream info;
	info<<GetAtomId()<<" "<<GetType()<<" ";
	info<<GetResidType()<<" "<<GetChainId()<<" "<<GetResidId();
	Coord3D coord = GetCoords();
	info<<" "<<coord.x<<" "<<coord.y<<" "<<coord.z;
	return info.str();
};

}

