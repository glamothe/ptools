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

Coord3D Rigidbody::GetCoordsFromType(string searchType) {
	Coord3D coord(0.0, 0.0, 0.0);
	for(uint i=0; i< this->Size(); i++) {
			if (mArrayAtomProp[i].GetType() == searchType) {
			coord = mCoords[i];
			break;
			}
	}
	return coord;
}



}


