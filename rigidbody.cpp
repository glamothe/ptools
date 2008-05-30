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
   : CoordsArray(model)
 {
//this copy constructor is needed because dbl[4][4] is not
// automatically copied with the default copy constructor

//     this->mAtoms = model.mAtoms;
    this->mForces = model.mForces;
//     this->_coords = model._coords;
    this->mAtomProp = model.mAtomProp;
    this->mAtoms = model.mAtoms;

}


void Rigidbody::AddAtom(const Atomproperty& at, Coord3D co)
{
    mAtomProp.push_back(at);
    AddCoord(co);
}


Atom Rigidbody::CopyAtom(uint i) const
{
   Atom at(mAtomProp[i],GetCoords(i));
   return at;
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
    return ( (1.0/(dbl)this->Size())*center);
}


void Rigidbody::CenterToOrigin()
{
    Coord3D c = FindCenter();
    Translate(Coord3D()-c);
}

dbl Rigidbody::RadiusGyration()
{
    Coord3D c = this->FindCenter();
    dbl r=0.0;
    for (uint i=0; i< this->Size(); i++)
    {
        r += Norm2( c - this->GetCoords(i) );
    }
    return ( sqrt( r/ this->Size() ));
}

dbl Rigidbody::Radius()
{
    Coord3D center = this->FindCenter();
    uint size = this->Size();
    dbl radius = 0.0;
    for (uint i=0; i < size; i++)
    {
        dbl rad=Norm(center - this->GetCoords(i));
        if (radius < rad) {radius=rad;}
    }
    return radius;
}


void Rigidbody::Translate(const Coord3D& tr)
{
   CoordsArray::Translate(tr);
}



/*!  \brief this function makes an euler rotation with the Attract convention.
*
*   Note that for this new implementation only the 4x4 rotational/translational
*   matrix is updated. This may allow a big speedup (to be tested) and a 
*   higher flexibility  (  rig.Translate(a); rig.Translate(minus(a)); may now be delayed
*   until the coordinates are really needed. 
*   If coordinates are never asked (why?), then no costly calculation is performed !
*/
void Rigidbody::AttractEulerRotate(dbl phi, dbl ssi, dbl rot)
{

    dbl  cp, cs, ss, sp, cscp, sscp, sssp, crot, srot, cssp ;

    cs=cos(ssi);
    cp=cos(phi);
    ss=sin(ssi);
    sp=sin(phi);
    cscp=cs*cp;
    cssp=cs*sp;
    sscp=ss*cp;
    sssp=ss*sp;
    crot=cos(rot);
    srot=sin(rot);


      dbl eulermat[4][4];

      eulermat[0][0] = crot*cscp + srot*sp;
      eulermat[0][1] = srot*cscp - crot*sp;
      eulermat[0][2] = sscp;
      eulermat[0][3] = 0.0;

      eulermat[1][0] = crot*cssp-srot*cp ;
      eulermat[1][1] = srot*cssp+crot*cp;
      eulermat[1][2] = sssp;
      eulermat[1][3] = 0.0;

      eulermat[2][0] = -crot*ss;
      eulermat[2][1] = -srot*ss;
      eulermat[2][2] = cs;
      eulermat[2][3] = 0.0;

      eulermat[3][0] = 0.0;
      eulermat[3][1] = 0.0;
      eulermat[3][2] = 0.0;
      eulermat[3][3] = 1.0;

      //matrix multiplication
      this->MatrixMultiply(eulermat);
//       mat44xmat44( eulermat , this->mat44, this->mat44);

}




AtomSelection Rigidbody::SelectAllAtoms()
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
        if ( mAtomProp[i].GetType()==atomtype)
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
        if (mAtomProp[i].GetResidType()==residtype)
            newsel.AddAtomIndex(i);
    }
    return newsel;
}


AtomSelection Rigidbody::SelectChainId(std::string chainId) {
    AtomSelection newsel;
    newsel.SetRigid(*this);
    for (uint i=0; i<Size(); i++)
    {
        if (mAtomProp[i].GetChainId()==chainId)
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
        Atomproperty& atp ( mAtomProp[i] );
        if (atp.GetResidId() >=start && atp.GetResidId() <= stop) newsel.AddAtomIndex(i);
    }
    return newsel;
}


AtomSelection Rigidbody::CA() {
    return SelectAtomType("CA");
}


/// operator +
Rigidbody Rigidbody::operator+(const Rigidbody& rig) {
    Rigidbody rigFinal(*this);
    for (uint i=0; i< rig.Size() ; i++) {
        rigFinal.AddCoord(rig.GetCoords(i));
        rigFinal.mAtomProp.push_back(rig.mAtomProp[i]);
    }
    return rigFinal;
}


void Rigidbody::ABrotate(const Coord3D& A, const Coord3D& B, dbl theta)
{
    PTools::ABrotate(A,B, *this, theta);
}



std::string Rigidbody::PrintPDB() const
{
 uint size=this->Size();
 std::string output;
  for (uint i=0; i < size ; i++)
	{
	output = output + mAtoms[i].ToPdbString();
	}
 return(std::string) output;
} 

void Rigidbody::m_hookCoords(uint i, Coord3D & co) const {};  //(virtual)




} //namespace PTools
