
#include <geometry.h>
#include <rigidbody.h>
#include <iostream>
#include <Movement.h>
#include <BasePair.h>
#include <Stacking.h>
#include <atomselection.h>
using namespace std;


namespace PTools
{

Stacking::Stacking()
{

}

Stacking::~Stacking()
{

}


Rigidbody Stacking::axis(const Rigidbody& bp1, const Rigidbody& bp2)const
{
    //1 determine if the considered basePair is a G-C or a A-T
    bool bp1isGC = bp1.SelectAtomType("GG1").Size()>0;
    bool bp2isGC = bp2.SelectAtomType("GG1").Size()>0;

    //2 build rigidbody for pyrimidine

    Rigidbody Bp1Pyr;
    if (bp1isGC)
    {
        Bp1Pyr = bp1.SelectAtomType("GC1").CreateRigid() + bp1.SelectAtomType("GC2").CreateRigid();
    }
    else
    {
        Bp1Pyr = bp1.SelectAtomType("GT1").CreateRigid() + bp1.SelectAtomType("GT2").CreateRigid();
    }

    Rigidbody Bp2Pyr;
    if (bp2isGC)
    {
        Bp2Pyr = bp2.SelectAtomType("GC1").CreateRigid() + bp2.SelectAtomType("GC2").CreateRigid();
    }
    else
    {
        Bp2Pyr = bp2.SelectAtomType("GT1").CreateRigid() + bp2.SelectAtomType("GT2").CreateRigid();
    }

    //3 build rigidbody for purine
    Rigidbody Bp1Pur;
     if (bp1isGC)
    {
        Bp1Pur = bp1.SelectAtomType("GG1").CreateRigid() + bp1.SelectAtomType("GG2").CreateRigid() + bp1.SelectAtomType("GG3").CreateRigid();
    }
    else
    {
        Bp1Pur = bp1.SelectAtomType("GA1").CreateRigid() + bp1.SelectAtomType("GA2").CreateRigid() + bp1.SelectAtomType("GA3").CreateRigid();
    }

    Rigidbody Bp2Pur;
    if (bp2isGC)
    {
        Bp2Pur = bp2.SelectAtomType("GG1").CreateRigid() + bp2.SelectAtomType("GG2").CreateRigid() + bp2.SelectAtomType("GG3").CreateRigid();
    }
    else
    {
        Bp2Pur = bp2.SelectAtomType("GA1").CreateRigid() + bp2.SelectAtomType("GA2").CreateRigid() + bp2.SelectAtomType("GA3").CreateRigid();
    }

    //4 compute center
    Coord3D pyrBp1center = Bp1Pyr.FindCenter() ;
    Coord3D pyrBp2center = Bp2Pyr.FindCenter() ;
    Coord3D purBp1center = Bp1Pur.FindCenter() ;
    Coord3D purBp2center = Bp2Pur.FindCenter() ;


    //build pyr axis
    //5.1 get the projection of the center of the first base on the line defines
    //by the center of the second base and the vector of the first base.
    Coord3D vectorBp1Bp2 = pyrBp1center - pyrBp2center;
    Coord3D vectorBp1pyr2 = (Bp1Pyr.CopyAtom(1).GetCoords() - pyrBp1center).Normalize();

    double proj=ScalProd(vectorBp1Bp2,vectorBp1pyr2);
    
    //5.2 project the center of the second base to the correct place.
    Coord3D Pyr = pyrBp2center + vectorBp1pyr2 * proj;

    //5.3 build the Axis for Pyrimidines
    Rigidbody pyrAxis;
    pyrAxis.AddAtom(Atomproperty(),pyrBp1center);
    pyrAxis.AddAtom(Atomproperty(),Pyr);


    return pyrAxis;
//    AtomSelection selSugar = bp1.SelectAtomType("GS2");
//    assert(selSugar.Size() == 2);
//    // build base3D
//    Rigidbody base3D;
//    // define atoms for base3D construction
//    Coord3D center = selSugar.CreateRigid().FindCenter(); // middle of GS2
//    Coord3D pointY = selSugar[0].GetCoords(); // point toward Y (first GS2)
//    AtomSelection grain = bp1.SelectAtomType("GG1"); // GG1 or GA1
//    if (grain.Size() == 0) {
//            grain = bp1.SelectAtomType("GA1");
//    }
//    assert(grain.Size() == 1);
//    Coord3D pointX = grain[0].GetCoords();
//    // Y
//    Coord3D axeY =  (pointY - center).Normalize();
//    // Z
//    Coord3D axeZ;
//    VectProd(pointX - center, axeY, axeZ);
//    axeZ = axeZ.Normalize();
//    // X
//    Coord3D axeX;
//    VectProd(axeY, axeZ, axeX);
//    axeX = axeX.Normalize();
//    // origin
//    Coord3D origin=bp1.FindCenter();
//    // save base3D
//    Atomproperty atp;
//
//    atp.SetType("OO");
//    base3D.AddAtom(atp,origin );
//    atp.SetType("OX");
//    base3D.AddAtom(atp, origin + axeX);
//    atp.SetType("OY");
//    base3D.AddAtom(atp, origin + axeY);
//    atp.SetType("OZ");
//    base3D.AddAtom(atp, origin + axeZ);
    
}


}