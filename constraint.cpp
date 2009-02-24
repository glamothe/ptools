#include "constraint.h"

namespace PTools{

Constraint CreateDefaultConstraint(const AttractRigidbody& rec,const AttractRigidbody& lig, uint recnb, uint lignb)
{
    Constraint constr;
    constr.lig1 = recnb;
    constr.lig1 = lignb;
    constr.at1 = -1 ; //center of mass for ligand 1
    double mindist2 = 1e15;
    int pluspres = -1;
    Coord3D centrerec = rec.FindCenter();

    uint ligsize = lig.Size();

    for (uint i=0; i<ligsize; i++)
    {
        Coord3D co = lig.GetCoords(i);
        double distance =  Norm2(co-centrerec);
        if (distance  < mindist2)
           {
            pluspres = i;
            mindist2 = distance;
           }
    }
    constr.at2 = (int) pluspres;
    return constr;
}

}