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



void Rigidbody::Translate(const Coord3D& tr)
{
    for (uint i=0; i<this->Size(); i++)
        this->GetAtom(i).Translate(tr);
}


AtomSelection Rigidbody::SelectAllAtoms( Rigidbody& rigid)
{
    AtomSelection newsel;
    newsel.SetRigid(*this);
    for (uint i=0; i < Size(); i++)
    {
        newsel.AddAtomIndex(i);
    }


    return newsel;

}


AtomSelection Rigidbody::SelectAtomType(std::string atomtype)
{
    AtomSelection newsel;
    newsel.SetRigid(*this);

    for (uint i=0; i<Size(); i++)
    {
        if (GetAtom(i).GetType()==atomtype)
            newsel.AddAtomIndex(i);
    }

    return newsel;
}


AtomSelection Rigidbody::SelectResidType(std::string residtype)
{
    AtomSelection newsel;
    newsel.SetRigid(*this);

    for (uint i=0; i<Size(); i++)
    {
        if (GetAtom(i).GetResidType()==residtype)
            newsel.AddAtomIndex(i);
    }
    return newsel;
}


AtomSelection Rigidbody::SelectChainId(std::string chainId) {
    AtomSelection newsel;
    newsel.SetRigid(*this);
    for (uint i=0; i<Size(); i++)
    {
        if (GetAtom(i).GetChainId()==chainId)
            newsel.AddAtomIndex(i);
    }
    return newsel;
}

AtomSelection Rigidbody::SelectResRange(uint start, uint stop)
{
    AtomSelection newsel;
    newsel.SetRigid(*this);

    for (uint i=0; i < Size(); i++)
    {
        Atom at = GetAtom(i);
        if (at.GetResidId() >=start && at.GetResidId() <= stop) newsel.AddAtomIndex(i);
    }
    return newsel;
}


AtomSelection Rigidbody::CA() {return SelectAtomType("CA");}

/// operator =
Rigidbody& Rigidbody::operator=(const Rigidbody& rig) {
    mAtoms = rig.mAtoms;
    return *this;
}

/// operator +
Rigidbody Rigidbody::operator+(const Rigidbody& rig) {
    Rigidbody rigFinal;
    rigFinal.mAtoms = mAtoms;
    for(uint i=0; i< rig.Size() ; i++) {
        rigFinal.mAtoms.push_back(rig.mAtoms[i]);
    }
    return rigFinal;
}


} //namespace PTools
