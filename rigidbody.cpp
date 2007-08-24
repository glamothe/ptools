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

//     this->mAtoms = model.mAtoms;
    this->mForces = model.mForces;
    this->mCoords = model.mCoords;
    this->mAtomProp = model.mAtomProp;


//copy of the matrix:
//Note: very naive way. May be slower than memcpy() or copy using pointers (?)
     for (uint i=0; i<4; i++)
         for (uint j=0; j<4; j++)
             this->mat44[i][j]=model.mat44[i][j];

}


void Rigidbody::AddAtom(const Atomproperty& at, Coord3D co)
{
    mAtomProp.push_back(at);
    mCoords.push_back(co);
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
    return ( (1.0/(double)this->Size())*center);
}


void Rigidbody::CenterToOrigin()
{
    Coord3D c = FindCenter();
    Translate(Coord3D()-c);
}




void Rigidbody::Translate(const Coord3D& tr)
{
//     for (uint i=0; i<this->Size(); i++)
//         this->mCoords[i] += tr ;

    //updates rotation/translation matrix:
    this->mat44[0][3]+=tr.x;
    this->mat44[1][3]+=tr.y;
    this->mat44[2][3]+=tr.z;
}



/*!  \brief this function makes an euler rotation with the Attract convention.
*
*   Note that for this new implementation only the 4x4 rotational/translational
*   matrix is updated. This may allow a big speedup (to be tested) and a 
*   higher flexibility  (  rig.Translate(a); rig.Translate(minus(a)); may now be delayed
*   until the coordinates are really needed. 
*   If coordinates are never asked (why?), then no costly calculation is performed !
*/
void Rigidbody::AttractEulerRotate(double phi, double ssi, double rot)
{

    double  cp, cs, ss, sp, cscp, sscp, sssp, crot, srot, cssp ;

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


      double eulermat[4][4];

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
      mat44xmat44( eulermat , this->mat44, this->mat44);

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
        rigFinal.mCoords.push_back(rig.mCoords[i]);
        rigFinal.mAtomProp.push_back(rig.mAtomProp[i]);
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


void Rigidbody::m_hookCoords(uint i, Coord3D & co) const {};  //(virtual)

///////////// AttractRigidbody implementation:

AttractRigidbody::AttractRigidbody(const Rigidbody & rig)
        : Rigidbody(rig)
{
    // extracts the "extra" field of Atoms to the m_atomTypeNumber array:
    uint   atcategory  = 0;
    double  atcharge   = 0.0;

    for (uint i = 0; i < Size() ; ++i)
    {
        Atomproperty & at (mAtomProp[i]);
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

    setRotation(true);
    setTranslation(true);


}






/*!  \brief normalize a normal mode vector
*
*    this function normalizes a mode vector assuming it to be a n*3 linear vector.
*
*/
void AttractRigidbody::normalizeMode(VCoord3D & mode)
{

double sum = 0.0 ;
for (uint i=0; i<mode.size(); i++)
{
sum+=Norm2(mode[i]); // we normalize a n*3 linear like vector, so Norm2 is the correct function to use.
}

double norm = sqrt(sum);

for (uint i=0; i<mode.size(); i++)
{
  mode[i]=mode[i]/norm;  //divide each component by the vector norm
}

}









/////////////////////////////


// ///apply a normal mode to an AttractRigidbody
// void applyMode(AttractRigidbody & src, AttractRigidbody& dest, const std::vector<Coord3D> & mode, double scalar){
// assert(src.Size() == dest.Size());
// assert(mode.size() == src.Size());
// for(uint i=0; i<src.Size(); i++)
// {
//     Coord3D co = src.GetCoords(i);
//     Coord3D displacment = mode[i];
//     co +=displacment*scalar;
//     dest.SetCoords(i, co);
// }
// 
// 
// }


} //namespace PTools
