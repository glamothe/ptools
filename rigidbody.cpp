#include "rigidbody.h"


namespace PTools{


void Rigidbody::AddAtom(const Atom& at)
{
Atom at2(at);

mCoords.push_back(at.GetCoords());
at2.SetOwner(this,size());
mArrayAtoms.push_back(at2);

}




}



