
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

    Rigidbody bp1Pyr;
    if (bp1isGC)
    {
        bp1Pyr = bp1.SelectAtomType("GC1").CreateRigid() + bp1.SelectAtomType("GC2").CreateRigid();
    }
    else
    {
        bp1Pyr = bp1.SelectAtomType("GT1").CreateRigid() + bp1.SelectAtomType("GT2").CreateRigid();
    }

    Rigidbody bp2Pyr;
    if (bp2isGC)
    {
        bp2Pyr = bp2.SelectAtomType("GC1").CreateRigid() + bp2.SelectAtomType("GC2").CreateRigid();
    }
    else
    {
        bp2Pyr = bp2.SelectAtomType("GT1").CreateRigid() + bp2.SelectAtomType("GT2").CreateRigid();
    }

    //3 build rigidbody for purine
    Rigidbody bp1Pur;
     if (bp1isGC)
    {
        bp1Pur = bp1.SelectAtomType("GG1").CreateRigid() + bp1.SelectAtomType("GG2").CreateRigid() + bp1.SelectAtomType("GG3").CreateRigid();
    }
    else
    {
        bp1Pur = bp1.SelectAtomType("GA1").CreateRigid() + bp1.SelectAtomType("GA2").CreateRigid() + bp1.SelectAtomType("GA3").CreateRigid();
    }

    Rigidbody bp2Pur;
    if (bp2isGC)
    {
        bp2Pur = bp2.SelectAtomType("GG1").CreateRigid() + bp2.SelectAtomType("GG2").CreateRigid() + bp2.SelectAtomType("GG3").CreateRigid();
    }
    else
    {
        bp2Pur = bp2.SelectAtomType("GA1").CreateRigid() + bp2.SelectAtomType("GA2").CreateRigid() + bp2.SelectAtomType("GA3").CreateRigid();
    }

    //4 compute center
    Coord3D pyrBp1center = bp1Pyr.FindCenter() ;
    Coord3D pyrBp2center = bp2Pyr.FindCenter() ;
    Coord3D purBp1center = bp1Pur.FindCenter() ;
    Coord3D purBp2center = bp2Pur.FindCenter() ;

    Rigidbody axis;
    
    if(  Norm(pyrBp1center-pyrBp2center) < Norm(pyrBp1center-purBp2center) )
    {
       axis = axisPyrimidines( bp1Pyr, pyrBp1center, pyrBp2center );
    }
    else
    {
       axis = axisPyrimidines( bp1Pyr, pyrBp1center, purBp2center );
    }

   return axis;

}

Rigidbody Stacking::axisPyrimidines(const Rigidbody& pyr, const Coord3D& centerBase1, const Coord3D&  centerBase2)const
{
    //build pyr axis
    //5.1 get the projection of the center of the first base on the line defines
    //by the center of the second base and the vector of the first base.
    Coord3D vectorBp1Bp2 = centerBase1 - centerBase2;
    Coord3D vectorBp1pyr2 = (pyr.CopyAtom(1).GetCoords() - centerBase1).Normalize();

    double proj=ScalProd(vectorBp1Bp2,vectorBp1pyr2);

    //5.2 project the center of the second base to the correct place.
    Coord3D Pyr = centerBase2 + vectorBp1pyr2 * proj;

    //5.3 build the Axis for Pyrimidines
    Rigidbody pyrAxis;
    pyrAxis.AddAtom(Atomproperty(),centerBase1);
    pyrAxis.AddAtom(Atomproperty(),Pyr);


    return pyrAxis;
}

}