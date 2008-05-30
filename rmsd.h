#ifndef RMSD_H
#define RMSD_H

#include "rigidbody.h"
#include "atomselection.h"
#include "coord3d.h"
#include "screw.h"
#include "basetypes.h"





namespace PTools
{

struct Superimpose_t
{
dbl rmsd;
Matrix matrix;
};



/// root mean square deviation without superposition
dbl Rmsd(const AtomSelection& atsel1, const AtomSelection& atsel2);


// Vissage MatTrans2screw(Mat33 rotmatrix, coord3D trans); // transforme t(r(X)) en un vissage d'axe de rotation colinéaire au vecteur translation.

Superimpose_t superimpose( AtomSelection selref, AtomSelection selmob, int verbosity=0);




}

#endif //RSMD_H

