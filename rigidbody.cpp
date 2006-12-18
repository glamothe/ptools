#include "rigidbody.h"


namespace PTools{


void Rigidbody::AddAtom(const Atomproperty& at, Coord3D co)
{
mCoords.push_back(co);
mArrayAtomProp.push_back(at);
}



Atom Rigidbody::CopyAtom(uint i) const 
{
    Atom at(mArrayAtomProp[i], mCoords[i]);
    return at;

}

}


