// $Id$
#include <iostream>
#include <cassert>
#include <stdexcept>


#include "rmsd.h"
#include "atom.h"
#include "atomselection.h"
#include "rigidbody.h"

#include "geometry.h" //for scalar product


#define EPSILON 1e-3

typedef dbl   DtFloat;
typedef DtFloat         DtPoint3[3];
typedef DtFloat         DtPoint4[4];
typedef DtFloat         DtMatrix3x3[3][3];
typedef DtFloat         DtMatrix4x4[4][4];

// extern "C"
// {
dbl zuker_superpose(DtPoint3 *c1, DtPoint3 *c2, int len, DtMatrix4x4 M);
// }


namespace PTools{

dbl Rmsd(const AtomSelection& atsel1, const AtomSelection& atsel2)
{
    if (atsel1.Size() != atsel2.Size())
    {
        std::cerr << "Error: trying to superpose two rigidbody of different sizes" << std::endl ;
        throw std::invalid_argument("RmsdSizesDiffers");
    }

    dbl sum = 0.0;


    for (uint i=0; i<atsel1.Size(); ++i)
    {
        Atom atom1=atsel1.CopyAtom(i);
        Atom atom2=atsel2.CopyAtom(i);

        sum+=Dist2(atom1,atom2);
    }

    return sqrt(sum/(dbl) atsel1.Size()) ;


}




// typedef Array2D<dbl> Matrix;


void PrintMat(Array2D<dbl> & mat)
{
    for (uint i=0; i<3;i++)
    {
        for (uint j=0; j<3;j++)
            std::cout << "| " << mat(i,j) << " " ;
        std::cout << " | "<<std::endl;
    }


}


//matrix multiplication. Works with out==left or out==right
void Mat33xMat33(Matrix left, Matrix right, Matrix out) 
{
    Matrix tmp(3,3);
    for (uint k=0; k<3; k++)
        for (uint l=0; l<3; l++)
        {
            tmp(k,l)=0;
            for (uint i=0; i<3;i++)
                tmp(k,l) += left(k,i)*right(i,l);
        }

    //copy the temporary matrix:
    out=tmp;
}


void XRotMatrix(dbl theta, Matrix & out)
{
    out(0,0)=1;
    out(0,1)=0;
    out(0,2)=0;

    out(1,0)=0;
    out(1,1)=cos(theta);
    out(1,2)=-sin(theta);

    out(2,0)=0;
    out(2,1)=sin(theta);
    out(2,2)=cos(theta);
}


void YRotMatrix(dbl theta, Matrix & out)
{
    out(0,0)=cos(theta);
    out(0,1)=0;
    out(0,2)=sin(theta);

    out(1,0)=0;
    out(1,1)=1;
    out(1,2)=0;

    out(2,0)=-sin(theta);
    out(2,1)=0;
    out(2,2)=cos(theta);
}



void ZRotMatrix(dbl theta, Matrix & out)
{
    out(0,0)=cos(theta);
    out(0,1)=-sin(theta);
    out(0,2)=0;

    out(1,0)=sin(theta);
    out(1,1)=cos(theta);
    out(1,2)=0;

    out(2,0)=0;
    out(2,1)=0;
    out(2,2)=1;
}



void Mat33xcoord3D(Matrix & mat, Coord3D& in, Coord3D& out)
{
    Coord3D temp;
    temp.x = in.x*mat(0,0) + in.y*mat(0,1) + in.z*mat(0,2) ;
    temp.y = in.x*mat(1,0) + in.y*mat(1,1) + in.z*mat(1,2) ;
    temp.z = in.x*mat(2,0) + in.y*mat(2,1) + in.z*mat(2,2) ;

    out = temp ;

}




/*! \brief (local function only)  extracts a matrix[n][3] from rigidbody coordinates
*
*/
void rigidToMatrix(const Rigidbody & rig , Array2D<dbl> & out)
{
    for (uint atom=0; atom<rig.Size();atom++)
    {
        out(atom,0) =  rig.GetCoords(atom).x;
        out(atom,1) =  rig.GetCoords(atom).y;
        out(atom,2) =  rig.GetCoords(atom).z;
    }

}






/*! \brief Calculates the tensor between reference and mobile
*
*  (as indicated in the Sippl article)
*/
void MakeTensor(const Rigidbody & ref, const Rigidbody &  mob, Array2D<dbl> & out)
{

    assert(ref.Size() == mob.Size() );

    //Get the coordinates of ref and mob inside a matrix to then calculates the tensor
    Array2D<dbl> cref(ref.Size(), 3) ;
    Array2D<dbl> cmob(mob.Size(), 3) ;

    rigidToMatrix(ref, cref);
    rigidToMatrix(mob, cmob);


    //tensor calculation:
    for (uint c=0; c<3; c++)
        for (uint l=0;l<3; l++)
        {
            out(l,c)=0;
            for (uint k=0;k<ref.Size();k++)
                out(l,c)+=cref(k,c)*cmob(k,l);
        }

}





/*! \brief calculates a screw that optimally superpose mob (mobile) on ref (reference)
*
*
*/
Superpose_t superpose( const CoordsArray& cref, const  CoordsArray& cmob, int verbosity)
{

    uint refsz = cref.Size();


    DtPoint3 ref[refsz];
    DtPoint3 mob[refsz];

    for (uint i =0; i <refsz; i++)
    {
        Coord3D c;
        cref.GetCoords(i,c);
        ref[i][0]=c.x;
        ref[i][1]=c.y;
        ref[i][2]=c.z;
    }


    for (uint i =0; i <refsz; i++)
    {
        Coord3D c;
        cmob.GetCoords(i,c);
        mob[i][0]=c.x;
        mob[i][1]=c.y;
        mob[i][2]=c.z;
    }

    DtMatrix4x4 mat;


    Matrix outmat(4,4);
    Superpose_t super;

    super.rmsd = sqrt(zuker_superpose(ref, mob, refsz, mat)) ; //zuker_superpose returns msd (not rmsd)
    if (verbosity==1)
    {
        std::cout << "Rmsd after superposition: " << super.rmsd << std::endl;
        std::cout << "Matrix: " << std::endl;
    }

    for (uint i=0; i<4; i++)
    {
        for (uint j=0; j<4; j++)
        {
            if (verbosity==1)
                std::cout << mat[j][i] << "  " ;

            outmat(i,j) = mat[j][i];
        }
        if (verbosity==1) std::cout << std::endl;

    }

    super.matrix = outmat;

    return super;
}



Superpose_t superpose( const Rigidbody & ref, const Rigidbody& mob, int verbosity)
{
CoordsArray cref = ref.ToCoordsArray() ;
CoordsArray cmob = mob.ToCoordsArray() ;

return superpose( cref, cmob, verbosity);

};


















} //namespace PTools

