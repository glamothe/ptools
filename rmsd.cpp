#include <iostream>
#include <cassert>

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
        throw "RmsdSizesDiffers";
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



void Mat33xMat33(Matrix left, Matrix right, Matrix out) //matrix multiplication. Works with out==left or out==right
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
        for(uint l=0;l<3; l++)
        {
            out(l,c)=0;
            for (uint k=0;k<ref.Size();k++)
                out(l,c)+=cref(k,c)*cmob(k,l);
        }

}







/*! \brief Returns uniques screw parameters that corresponds to the rotation/translation parameters
*
*Ref: Hexanions: 6D Space for Twists. Alexis Angelidis. Departement of Computer Science, University of Otago. (2004). Technical Report
*/
Screw MatTrans2screw(Matrix & rotmatrix, const Coord3D & trans)
{

    Screw  screw;
    Coord3D eigenvect;

    Coord3D x,y,z;
    x.x = rotmatrix(0,0) ; x.y = rotmatrix(1,0); x.z=rotmatrix(2,0);
    y.x = rotmatrix(0,1) ; y.y = rotmatrix(1,1); y.z=rotmatrix(2,1);
    z.x = rotmatrix(0,2) ; z.y = rotmatrix(1,2); z.z=rotmatrix(2,2);

    Coord3D pt ; // a point of the rotation axis


    dbl a = rotmatrix(0,0) ; // Xx
    dbl b = rotmatrix(1,1) ; // Yy
    dbl c = rotmatrix(2,2) ; // Zz

    dbl normtranslation = 0;

    if (fabs(1+a-b-c) > EPSILON)
    {
        eigenvect.x = x.x + 1 - b - c ;
        eigenvect.y = x.y + y.x ;
        eigenvect.z = x.z + z.x ;

        screw.transVector = eigenvect / Norm(eigenvect);
        normtranslation = ScalProd(screw.transVector,trans);

        Coord3D s = trans - screw.transVector;
        screw.point.x = 0 ;
        screw.point.y = s.z*z.y + s.y*(1-z.z) ;
        screw.point.z = s.y*y.z+s.z*(1-y.y);
        screw.point = screw.point/(1+x.x-y.y-z.z) ;


    }
    else if (fabs(1-a+b-c)> EPSILON)
    {
        eigenvect.x = y.x + x.y;
        eigenvect.y = y.y + 1 - x.x - z.z ;
        eigenvect.z = y.z + z.y ;

        screw.transVector = eigenvect / Norm(eigenvect);
        normtranslation = ScalProd (screw.transVector,trans);

        Coord3D s = trans - screw.transVector ;
        screw.point.x =  s.z*z.x + s.x*(1-z.z);
        screw.point.y =  0 ;
        screw.point.z =  s.x*x.z + s.z*(1-x.x);
        screw.point = screw.point/(1-x.x+y.y-z.z) ;
    }
    else if (fabs(1-a-b+c)>EPSILON)
    {
        eigenvect.x = z.x + x.z ;
        eigenvect.y = z.y + y.z ;
        eigenvect.z = z.z + 1 - x.x - y.y ;

        screw.transVector = eigenvect / Norm(eigenvect);
        normtranslation = ScalProd(screw.transVector,trans);

        Coord3D s = trans - screw.transVector ;
        screw.point.x = s.y*y.x + s.x*(1-y.y) ;
        screw.point.y = s.x*x.y + s.y*(1-x.x) ;
        screw.point.z =  0 ;
        screw.point = screw.point/(1-x.x-y.y+z.z) ;

    }

   else     // angle=0
      { 
            screw.point = Coord3D(0,0,0);
            if(Norm(trans)!=0)
            {
              screw.transVector = trans / Norm(trans);
            }
            else {  screw.transVector =  Coord3D(0,0,1); /*arbitrary axis*/ }

            normtranslation=Norm(trans);
            screw.angle=0;
            screw.transVector = screw.transVector * normtranslation ;
            return screw;

    }




    //creates a non-aligned vector with  screw.transVector:
    Coord3D v (1,0,0);
    if (fabs(Angle(screw.transVector,v)) < 0.1) v = Coord3D(0,0,1); //v et axe colinéaires: on change v


    Coord3D u = v - ScalProd(v,screw.transVector)*screw.transVector;
    u.Normalize();

    Coord3D uprime;
    Mat33xcoord3D(rotmatrix,u,uprime);

    dbl cost = ScalProd(u,uprime);

    Coord3D usec;
    VectProd(screw.transVector,u,usec);
    dbl sint = ScalProd(usec,uprime);

    if (cost < -1 ) cost=-1;
    if (cost >1 ) cost= 1 ;
    screw.angle = acos(cost);
    if (sint < 0) screw.angle = -screw.angle ;
    screw.transVector = screw.transVector * normtranslation;
    return screw ;
}












/*! \brief calculates a screw that optimally superimpose mob (mobile) on ref (reference)
*
* 
*/
Superimpose_t superimpose( AtomSelection selref, AtomSelection selmob, int verbosity)
{

uint refsz = selref.Size();

Rigidbody rref = selref.CreateRigid();
Rigidbody rmob = selmob.CreateRigid();


DtPoint3 ref[refsz];
DtPoint3 mob[refsz];

for (uint i =0; i <refsz; i++)
 {
   ref[i][0]=rref.GetCoords(i).x;
   ref[i][1]=rref.GetCoords(i).y;
   ref[i][2]=rref.GetCoords(i).z;
}


for (uint i =0; i <refsz; i++)
 {
   mob[i][0]=rmob.GetCoords(i).x;
   mob[i][1]=rmob.GetCoords(i).y;
   mob[i][2]=rmob.GetCoords(i).z;
}

DtMatrix4x4 mat;


Matrix outmat(4,4);
Superimpose_t super;

super.rmsd = sqrt(zuker_superpose(ref, mob, refsz, mat)) ; //zuker_superpose returns msd (not rmsd)
if (verbosity==1)
  {
    std::cout << "Rmsd after superposition: " << super.rmsd << std::endl;
    std::cout << "Matrix: " << std::endl;
  }

for(uint i=0; i<4; i++)
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


} //namespace PTools

