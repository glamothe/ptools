#ifndef RMSD_H
#define RMSD_H

#include "rigidbody.h"
#include "atomselection.h"
#include "coord3d.h"


namespace PTools
{

double Rmsd(const AtomSelection& atsel1, const AtomSelection& atsel2);
/// root mean square deviation without superposition

}

#endif //RSMD_H

