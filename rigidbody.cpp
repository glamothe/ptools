#include "rigidbody.h"
#include "atomselection.h"
#include "geometry.h"
#include "pdbio.h"

namespace PTools{


Rigidbody::Rigidbody()
{
    ResetMatrix();
}



Rigidbody::Rigidbody(std::string filename)
{
    ReadPDB(filename,*this);
    ResetMatrix();
}


Rigidbody::Rigidbody(const Rigidbody& model)
{
//this copy constructor is needed because double[4][4] is not
// automatically copied with the default copy constructor

    this->mAtoms = model.mAtoms;
    this->mForces = model.mForces;

//copy of the matrix:
    for (uint i=0; i<4; i++)
        for (uint j=0; j<4; j++)
            this->mat44[i][j]=model.mat44[i][j];


}


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


Coord3D Rigidbody::FindCenter() const
{
    Coord3D center(0.0,0.0,0.0);
    for (uint i=0; i< this->Size() ; i++)
    {
        center =  center + GetCoords(i);
    }
    return ( (1.0/(double)this->Size())*center);
}


void Rigidbody::CenterToOrigin()
{
    Coord3D c = FindCenter();
    Translate(Coord3D()-c);
}




void Rigidbody::Translate(const Coord3D& tr)
{
    for (uint i=0; i<this->Size(); i++)
        this->GetAtomReference(i).Translate(tr);

    //updates rotation/translation matrix:
    this->mat44[0][3]+=tr.x;
    this->mat44[1][3]+=tr.y;
    this->mat44[2][3]+=tr.z;
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


AtomSelection Rigidbody::CA() {
    return SelectAtomType("CA");
}

/// operator =
Rigidbody& Rigidbody::operator=(const Rigidbody& rig) {
    mAtoms = rig.mAtoms;
    return *this;
}

/// operator +
Rigidbody Rigidbody::operator+(const Rigidbody& rig) {
    Rigidbody rigFinal;
    rigFinal.mAtoms = mAtoms;
    for (uint i=0; i< rig.Size() ; i++) {
        rigFinal.mAtoms.push_back(rig.mAtoms[i]);
    }
    return rigFinal;
}


void Rigidbody::ABrotate(const Coord3D& A, const Coord3D& B, double theta)
{
    PTools::ABrotate(A,B,*this, *this, theta);
}


void Rigidbody::ResetMatrix()
{
    for (uint i=0; i<4; i++)
        for (uint j=0; j<4; j++)
        {
            if (i!=j) mat44[i][j]=0;
            else mat44[i][j]=1;
        }

}


void Rigidbody::PrintMatrix() const
{
    std::cout << "### MAT BEGIN\n";
    for (uint i=0; i<4; i++)
    {

        std::cout << "MAT   ";
        for (uint j=0; j<4; j++)
        {
            printf("%13.7f", this->mat44[i][j]) ;
        }
        std::cout << std::endl;
    }
    std::cout << "### MAT END\n";
}


void Rigidbody::ApplyMatrix(double mat[4][4])
{
    mat44xrigid(*this, *this, mat);
}


Vdouble Rigidbody::GetMatrix() const
{
    Vdouble result;
    const double * ptr;

    ptr=(double*)mat44;
    for (uint i=0; i<16; i++)
    {
        result.push_back(*ptr++);
    }

    return result;

}




///////////// AttractRigidbody implementation:

AttractRigidbody::AttractRigidbody(const Rigidbody & rig)
        : Rigidbody(rig)
{
    // extracts the "extra" field of Atoms to the m_atomTypeNumber array:
    uint   atcategory  = 0;
    double  atcharge   = 0.0;

    for (uint i = 0; i < Size() ; ++i)
    {
        Atom at = GetAtom(i);
        std::string extra = at.GetExtra();

        std::istringstream iss( extra );
        iss >> atcategory >> atcharge ;
        m_atomTypeNumber.push_back(atcategory-1);  // -1 to directly use the atomTypeNumber into C-array 
        m_charge.push_back(atcharge);

	if (isAtomActive(i))
	{
		m_activeAtoms.push_back(i);
	}

    }

}





} //namespace PTools
