#ifndef RMSD_H
#define RMSD_H

#include "rigidbody.h"
#include "atomselection.h"
#include "coord3d.h"
#include "screw.h"
#include "basetypes.h"





namespace PTools
{

struct Superpose_t
{
    dbl rmsd;
    Matrix matrix;
};



/// root mean square deviation without superposition
dbl Rmsd(const AtomSelection& atsel1, const AtomSelection& atsel2);

Superpose_t superpose( AtomSelection selref, AtomSelection selmob, int verbosity=0);




}

#endif //RSMD_H

