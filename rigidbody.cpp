#include "rigidbody.h"
#include "atomselection.h"

namespace PTools{


void Rigidbody::AddAtom(const Atomproperty& at, Coord3D co)
{
Atom atom(at,co);
mAtoms.push_back(atom);
}



Atom Rigidbody::CopyAtom(uint i) const 
{
    return mAtoms[i];
}


void Rigidbody::AddAtom(const Atom& at)
{
Atomproperty atp(at);
Coord3D co = at.GetCoords();

AddAtom(atp,co);

}


Coord3D Rigidbody::FindCenter()
{
  Coord3D center(0.0,0.0,0.0);
  for(uint i=0; i< this->Size() ; i++)
  {
     center =  center + GetCoords(i);
  }

  return ( (1.0/this->Size())*center);

}

Coord3D Rigidbody::GetCoordsFromType(std::string searchType) {
	Coord3D coord(0.0, 0.0, 0.0);
	for(uint i=0; i< this->Size(); i++) {
			if (mAtoms[i].GetType() == searchType) {
			coord = mAtoms[i].GetCoords();
			break;
			}
	}
	return coord;
}


void Rigidbody::Translate(const Coord3D& tr)
{
for (uint i=0; i<this->Size(); i++)
        this->GetAtom(i).Translate(tr);
}


}


