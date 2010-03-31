

#include <geometry.h>
#include <rigidbody.h>
#include <iostream>
#include <Movement.h>
#include <BasePair.h>
#include <Parameter.h>
#include <atomselection.h>
using namespace std;


namespace PTools {

Parameter::Parameter()
{
    shift = 0.0;
    slide = 0.0;
    rise = 0.0;
    tilt = 0.0;
    roll = 0.0;
    twist = 0.0;

}


Parameter::Parameter(const BasePair& bp1,const BasePair& bp2)
{
 
}


Parameter::~Parameter()
{

}

double dotProduct( const Coord3D& u, const Coord3D& v) {
   return (u.x * v.x) + (u.y * v.y) + (u.z * v.z);
}


////code "steal" from deformDna.cpp (author: Pierre Poulain), modified to use the geometric center
Rigidbody Parameter::buildAxisCGGeometricCenter(const Rigidbody& bp)const
{
    AtomSelection selSugar = bp.SelectAtomType("GS2");
    assert(selSugar.Size() == 2);
    // build base3D
    Rigidbody base3D;
    // define atoms for base3D construction
    Coord3D center = selSugar.CreateRigid().FindCenter(); // middle of GS2
    Coord3D pointY = selSugar[0].GetCoords(); // point toward Y (first GS2)
    AtomSelection grain = bp.SelectAtomType("GG1"); // GG1 or GA1
    if (grain.Size() == 0) {
            grain = bp.SelectAtomType("GA1");
    }
    assert(grain.Size() == 1);
    Coord3D pointX = grain[0].GetCoords();
    // Y
    Coord3D axeY =  (pointY - center).Normalize();
    // Z
    Coord3D axeZ;
    VectProd(pointX - center, axeY, axeZ);
    axeZ = axeZ.Normalize();
    // X
    Coord3D axeX;
    VectProd(axeY, axeZ, axeX);
    axeX = axeX.Normalize();
    // origin
    Coord3D origin=bp.FindCenter();
    // save base3D
    Atomproperty atp;

    atp.SetType("OO");
    base3D.AddAtom(atp,origin );
    atp.SetType("OX");
    base3D.AddAtom(atp, origin + axeX);
    atp.SetType("OY");
    base3D.AddAtom(atp, origin + axeY);
    atp.SetType("OZ");
    base3D.AddAtom(atp, origin + axeZ);
    return base3D;

}

////code "steal" from deformDna.cpp (author: Pierre Poulain), modified to use the geometric center
///the definition of an axis for basis is following the standard stated in
///Definitions and Nomenclature of Nucleic Acid Structure Parameters, R. E. Dickerson et alJ. Mol. Biol. (1995) 251, 648–664.
Rigidbody Parameter::buildAxisAAGeometricCenter(const Rigidbody& bp)const
{
    AtomSelection selSugar = bp.SelectAtomType("C1'");
    if (selSugar.Size() != 2)selSugar = bp.SelectAtomType("C1*");
    // build base3D
    Rigidbody base3D;
    // define atoms for base3D construction
    Coord3D center = selSugar.CreateRigid().FindCenter(); // middle of GS2
    Coord3D pointY = selSugar[0].GetCoords(); // point toward Y (first GS2)
    AtomSelection grain = bp.SelectAtomType("C5"); // GG1 or GA1
    if (grain.Size() == 0) {
            grain = bp.SelectAtomType("GA1");
    }
    assert(grain.Size() == 1);
    Coord3D pointX = grain[0].GetCoords();
    // Y
    Coord3D axeY =  (pointY - center).Normalize();
    // Z
    Coord3D axeZ;
    VectProd(pointX - center, axeY, axeZ);
    axeZ = axeZ.Normalize();
    // X
    Coord3D axeX;
    VectProd(axeY, axeZ, axeX);
    axeX = axeX.Normalize();
    // origin
    Coord3D origin=bp.FindCenter();
    // save base3D
    Atomproperty atp;

    atp.SetType("OO");
    base3D.AddAtom(atp,origin );
    atp.SetType("OX");
    base3D.AddAtom(atp, origin + axeX);
    atp.SetType("OY");
    base3D.AddAtom(atp, origin + axeY);
    atp.SetType("OZ");
    base3D.AddAtom(atp, origin + axeZ);
    return base3D;

}
///
void Parameter::MeasureParameters(const Rigidbody& oxyz1, const Rigidbody& oxyz2)
{


    //the vector of the first coordinate
    Coord3D X = oxyz1.GetCoords(1) - oxyz1.GetCoords(0);
    Coord3D Y = oxyz1.GetCoords(2) - oxyz1.GetCoords(0);
    Coord3D Z = oxyz1.GetCoords(3) - oxyz1.GetCoords(0);

    //the vector of the second coordinate
    Coord3D I = oxyz2.GetCoords(1) - oxyz2.GetCoords(0);
    Coord3D J = oxyz2.GetCoords(2) - oxyz2.GetCoords(0);
    Coord3D K = oxyz2.GetCoords(3) - oxyz2.GetCoords(0);

    //the vector of the second axis are projeted in the first axis
    Coord3D e1(ScalProd(I, X), ScalProd(I, Y), ScalProd(I, Z));
    Coord3D e2(ScalProd(J, X), ScalProd(J, Y), ScalProd(J, Z));
    Coord3D e3(ScalProd(K, X), ScalProd(K, Y), ScalProd(K, Z));

    //calcul of rotation
    double a = asin(e1.z);
    
    twist = asin(e1.y / cos(a));
    roll = -a;
    tilt = asin(e2.z /cos(a));

//    twist = acos(ScalProd(X,I));
//    roll   = acos(ScalProd(Y,J));
//    tilt    = acos(ScalProd(Z,K));
    //the vector between the two origins
    Coord3D OO = oxyz2.GetCoords(0) - oxyz1.GetCoords(0);
    //the projection of the vector between the two origins on each axis.
    shift = dotProduct(OO,I);
    slide = dotProduct(OO,J);
    rise = dotProduct(OO,K);

}

Movement Parameter::getMov() const
{
    return Twist(twist)+Roll(roll)+Tilt(tilt)+Rise(rise)+Slide(slide)+Shift(shift);
}


double Parameter::getRise() const {
    return rise;
}

void Parameter::setRise(double rise) {
    this->rise = rise;
}

double Parameter::getRoll() const {
    return roll;
}

void Parameter::setRoll(double roll) {
    this->roll = roll;
}

double Parameter::getShift() const {
    return shift;
}

void Parameter::setShift(double shift) {
    this->shift = shift;
}

double Parameter::getSlide() const {
    return slide;
}

void Parameter::setSlide(double slide) {
    this->slide = slide;
}

double Parameter::getTilt() const {
    return tilt;
}

void Parameter::setTilt(double tilt) {
    this->tilt = tilt;
}

double Parameter::getTwist() const {
    return twist;
}

void Parameter::setTwist(double twist) {
    this->twist = twist;
}


}