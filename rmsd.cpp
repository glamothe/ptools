#include <iostream>
#include <cassert>

#include "rmsd.h"
#include "atom.h"
#include "atomselection.h"
#include "rigidbody.h"

#include "geometry.h" //for scalar product


#define EPSILON 1e-3



namespace PTools{

double Rmsd(const AtomSelection& atsel1, const AtomSelection& atsel2)
{
    if (atsel1.Size() != atsel2.Size())
    {
        std::cerr << "Error: trying to superpose two rigidbody of different sizes" << std::endl ;
        throw "RmsdSizesDiffers";
    }

    double sum = 0.0;


    for (uint i=0; i<atsel1.Size(); ++i)
    {
        Atom atom1=atsel1.CopyAtom(i);
        Atom atom2=atsel2.CopyAtom(i);

        sum+=Dist2(atom1,atom2);
    }

    return sqrt(sum/(double) atsel1.Size()) ;


}




typedef Array2D<double> Matrix;


void PrintMat(Array2D<double> & mat)
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


void XRotMatrix(double theta, Matrix & out)
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


void YRotMatrix(double theta, Matrix & out)
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



void ZRotMatrix(double theta, Matrix & out)
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


void Rotate(Rigidbody& rigid, Matrix & mat)
{
    double x,y,z, X, Y, Z;
    for (uint i=0; i<rigid.Size(); i++)
    {
        Coord3D co = rigid.GetCoords(i);
        x= co.x;
        y= co.y;
        z= co.z;

        X = x*mat(0,0) + y*mat(0,1) + z*mat(0,2) ;
        Y = x*mat(1,0) + y*mat(1,1) + z*mat(1,2) ;
        Z = x*mat(2,0) + y*mat(2,1) + z*mat(2,2) ;

        rigid.SetCoords(i, Coord3D(X,Y,Z));

    }


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
void rigidToMatrix(const Rigidbody & rig , Array2D<double> & out) 
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
void MakeTensor(const Rigidbody & ref, const Rigidbody &  mob, Array2D<double> & out)
{

    assert(ref.Size() == mob.Size() );

    //Get the coordinates of ref and mob inside a matrix to then calculates the tensor
    Array2D<double> cref(ref.Size(), 3) ;
    Array2D<double> cmob(mob.Size(), 3) ;

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


    double a = rotmatrix(0,0) ; // Xx
    double b = rotmatrix(1,1) ; // Yy
    double c = rotmatrix(2,2) ; // Zz

    double normtranslation = 0;

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

    double cost = ScalProd(u,uprime);

    Coord3D usec;
    VectProd(screw.transVector,u,usec);
    double sint = ScalProd(usec,uprime);

    if (cost < -1 ) cost=-1;
    if (cost >1 ) cost= 1 ;
    screw.angle = acos(cost);
    if (sint < 0) screw.angle = -screw.angle ;
    screw.transVector = screw.transVector * normtranslation;
    return screw ;
}












/*! \brief calculates a screw that optimally superimpose mob (mobile) on ref (reference)
*
* Algorithm from Sippl et Stegbuchner. Computers Chem. Vol 15, No. 1, p 73-78, 1991.
*/
Screw superpose( AtomSelection selref, AtomSelection selmob, int verbosity)
{

    if (selref.Size() != selmob.Size()) {
                                       std::string error = "Error in superpose.cpp: \
                                       the two AtomSelection objects must have\
                                       the same size !\n";  throw error;  };


    Screw screw; // screw that must be applied to mobile to superimpose on ref


    //creates a copy of the reference and mobile Rigidbodies selections because we need to modify them
    Rigidbody reference = selref.CreateRigid();
    Rigidbody mobile = selmob.CreateRigid();



    Array2D<double> rot(3,3);  //rotation matrix
    Array2D<double> ident(3,3); // identity matrix (only in the beginning!) 
    for (uint i=0; i<3; i++)
        for(uint j=0; j<3; j++)
        if (i!=j) {ident(i,j)=0;} else {ident(i,j)=1;} ;


    //get the translation component:
    Coord3D t0 = reference.FindCenter();
    Coord3D t1 = mobile.FindCenter();


    //center the two objects
    reference.CenterToOrigin();
    mobile.CenterToOrigin();


    Array2D<double> U(3,3); //mixed tensor
    MakeTensor(reference, mobile, U);



    //superimposition algorithm:
    for(uint i=0; i<30; i++)  // we may want to change the number of iterations at some point...
    {

        double arg1,arg2;

        arg1 = U(2,1) - U(1,2) ;
        arg2 = U(1,1) + U(2,2);
        double alpha = atan2(arg1 , arg2 );

        XRotMatrix(-alpha, rot);
        Mat33xMat33(rot,ident,ident);

        //------------------------------------


        Mat33xMat33(rot,U,U);

        arg1 = U(2,0)-U(0,2);
        arg2 = U(0,0)+U(2,2);

        double beta = atan2(arg1,arg2);
        YRotMatrix(beta,rot);
        Mat33xMat33(rot,ident,ident);

        //--------------------------------------


        Mat33xMat33(rot,U,U);

        arg1 = U(1,0) - U(0,1);
        arg2 = U(0,0) + U(1,1);
        double gamma = atan2(arg1,arg2);

        ZRotMatrix(-gamma,rot);
        Mat33xMat33(rot,ident,ident);


        Mat33xMat33(rot,U,U);

    }


    screw = MatTrans2screw(ident, t0-t1);
    screw.point = screw.point + t1 ;

//     if (verbosity==1)
//     {
// 
// 
//         rigidbody newmob(mob);
//         rigidbody newref(ref);
//         selref.setRigid(newref);
//         selmob.setRigid(newmob);
//         newmob.transform(screw);
// 
//         std::cout << "verif screw, rmsdca = " << rmsd(selmob && CA(newmob),selref && CA(newref)) << std::endl ;
// 
//     }

    return screw;
}











} //namespace PTools

